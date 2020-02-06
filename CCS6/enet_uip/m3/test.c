//###########################################################################
// FILE:   enet_uip.c
// TITLE:  Ethernet GPIO control using uIP TCP/IP stack
//###########################################################################
// $TI Release: F28M35x Support Library v207 $
// $Release Date: Mon Sep 21 16:49:54 CDT 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "inc/hw_gpio.h"
#include "inc/hw_udma.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ethernet.h"

#include "board_drivers/set_pinout_f28m35x.h"

#include "driverlib/udma.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/ethernet.h"
#include "driverlib/interrupt.h"
#include "driverlib/flash.h"
#include "driverlib/cpu.h"
#include "driverlib/ipc.h"
#include "driverlib/ram.h"

#include <string.h>
#include <stdio.h>

#include "utils/ustdlib.h"
#include "utils/uartstdio.h"

#include "uip/uip.h"
#include "uip/uip_arp.h"
#include "httpd.h"
#include "dhcpc/dhcpc.h"

//*****************************************************************************
//! \addtogroup master_example_list
//! <h1>Ethernet GPIO control using uIP stack (enet_uip)</h1>
//!
//! This example application demonstrates controlling a GPIO pin using Ethernet
//! communication. It makes use of the uIP TCP/IP stack.
//! A basic web site is served over the Ethernet port. The web site displays
//! a few lines of text, a Toggle LED button, an input to select an LED (6 or 7)
//! and a get status button. The user specifies which LED needs to be toggled
//! and presses the Toggle button. The user gets the status of the LED(ON or
//! OFF), by pressing the Get status button.
//! This code uses a static IP address. Depending on the client machine,the user
//! may need to manually set a fixed static IP address.
//! Follow the following steps to assign a static IP address to the client
//! machine
//!  - disable any wireless connection that may be active.
//!  - start-> control Panel->right click on "Network connection"->open.
//!  - Identify the connection to use "Local Area connection" or "Wired
//!    Network Connection" or some other name depending on the client machine.
//!  - Right click on the connection -> click on properties. In the "Local Area
//!    Connection" or "Wired Network connection" properties window under
//!    "general" tab in the "This connection uses the following items:" list
//!    scroll down and click on "Internet Protocol (TCP/IP)" item(! don't
//!    uncheck it). click on properties. In the "Internet Protocol(TCP/IP)
//!    properties" window select "Use the following IP address" and enter the
//!    following:
//!         IP address:   "169.254.42.86"
//!         Subnet mask:  "255.255.0.0"
//!  - click "ok" twice to close the open dialog boxes.
//!  - Remember to revert back the above settings to their default values once
//!    the use of this server is complete.
//!  - The IP address for the server is "169.254.254.169"
//!
//!
//! \b Note
//! - Connect the MAC with the PHY using the provided jumpers.
//!	  Connect the first 15 pins of Row C to Row B using the jumpers.
//!
//! - All EMAC examples use a generic TI??MAC address A8-63-F2-00-00-80.
//!   User defined MAC address can be programmed in a fixed non-volatile memory
//!   location. Refer to device data sheet and reference guides for details.
//!
//! - When using a silicon that is REV 0 remove the comment from line
//!   #define REV_0_SILICON
//!
//! \b Caution
//! - If you have two concerto CCARDS communicating with each other, they
//!   need to have different EMAC addresses.
//!
//! For additional details on uIP, refer to the uIP web page at:
//! http://www.sics.se/~adam/old-uip/
//
//*****************************************************************************


//Shared RAM definitions
#define M3_MASTER 0
#define C28_MASTER 1
void Shared_Ram_dataRead_m3(void);
void Shared_Ram_dataWrite_m3(int,float);

int m3_r_array[2048]; // this array is mapped to S0
int m3_r_w_array[2048]; // this array is mapped to S2

#pragma DATA_SECTION(m3_r_array,"SHARERAMS0");
#pragma DATA_SECTION(m3_r_w_array,"SHARERAMS2");


#ifdef _FLASH
// These are defined by the linker (see device linker command file)
extern unsigned long RamfuncsLoadStart;
extern unsigned long RamfuncsRunStart;
extern unsigned long RamfuncsLoadSize;
#endif

//*****************************************************************************
// Defines for Silicon revision.
//*****************************************************************************
// #define REV_0_SILICON

//*****************************************************************************
// Defines for setting up the system clock.
//*****************************************************************************
#define SYSTICKHZ               CLOCK_CONF_SECOND
#define SYSTICKMS               (1000 / SYSTICKHZ)
#define SYSTICKUS               (1000000 / SYSTICKHZ)
#define SYSTICKNS               (1000000000 / SYSTICKHZ)

//*****************************************************************************
// Macro for accessing the Ethernet header information in the buffer.
//*****************************************************************************
u8_t ucUIPBuffer[UIP_BUFSIZE + 2];
u8_t *uip_buf;

#define BUF                     ((struct uip_eth_hdr *)uip_buf)

//*****************************************************************************
// A set of flags.  The flag bits are defined as follows:
//     0 -> An indicator that a SysTick interrupt has occurred.
//     1 -> An RX Packet has been received.
//     2 -> A TX packet DMA transfer is pending.
//     3 -> A RX packet DMA transfer is pending.
//*****************************************************************************
#define FLAG_SYSTICK            0
#define FLAG_RXPKT              1
#define FLAG_TXPKT              2
#define FLAG_RXPKTPEND          3
static volatile unsigned long g_ulFlags;

//*****************************************************************************
// A system tick counter, incremented every SYSTICKMS.
//*****************************************************************************
volatile unsigned long g_ulTickCounter = 0;

//*****************************************************************************
// The control table used by the uDMA controller.  This table must be aligned
// to a 1024 byte boundary.  In this application uDMA is only used for USB,
// so only the first 8 channels are needed.
//*****************************************************************************
#pragma DATA_ALIGN(g_sDMAControlTable, 1024)
tDMAControlTable g_sDMAControlTable[8];

//*****************************************************************************
// Default TCP/IP Settings for this application.
// Default to Link Local address ... (169.254.1.0 to 169.254.254.255).  Note:
// This application does not implement the Zeroconf protocol.  No ARP query is
// issued to determine if this static IP address is already in use.
// TODO:  Uncomment the following #define statement to enable STATIC IP
// instead of DHCP.
//*****************************************************************************
#define USE_STATIC_IP

#ifndef DEFAULT_IPADDR0
#define DEFAULT_IPADDR0         169
#endif

#ifndef DEFAULT_IPADDR1
#define DEFAULT_IPADDR1         254
#endif

#ifndef DEFAULT_IPADDR2
#define DEFAULT_IPADDR2         254
#endif

#ifndef DEFAULT_IPADDR3
#define DEFAULT_IPADDR3         169
#endif

#ifndef DEFAULT_NETMASK0
#define DEFAULT_NETMASK0        255
#endif

#ifndef DEFAULT_NETMASK1
#define DEFAULT_NETMASK1        255
#endif

#ifndef DEFAULT_NETMASK2
#define DEFAULT_NETMASK2        0
#endif

#ifndef DEFAULT_NETMASK3
#define DEFAULT_NETMASK3        0
#endif

//*****************************************************************************
// UIP Timers (in MS)
//*****************************************************************************
#define UIP_PERIODIC_TIMER_MS   500
#define UIP_ARP_TIMER_MS        10000

//*****************************************************************************
// Defines for commands
//*****************************************************************************
#define velocity            0xA1
#define omega               0xA2
#define left                0xA3
#define right               0xA4
#define get_veloctiy        4
#define get_omega           5
#define get_left            6
#define get_right           7
#define INVALID_CMD      0xA5
#define INVALID_INPUT    0xA6
#define NO_CMD           0xA6

int x = 0;
int y = 0;
//*****************************************************************************
// functions added in httpd.c
//*****************************************************************************
extern void httpd_clear_command(void);
extern void httpd_clear_command(void);
extern int httpd_get_command(int *command_word);
extern void httpd_insert_response(int data_length,char *data);
extern float velocity_cmd;
extern float omega_cmd;
extern float left_cmd;
extern float right_cmd;


while(true)
{
if(httpd_get_command(&command_word))
{
    httpd_clear_command();
  printf("%d",command_word);
  x = command_word;
  EthernetProcessCMD(command_word);

                }
 
void EthernetProcessCMD(int command)
{
	//snprintf(buffer,sizeof(buffer),"%f", m3_r_w_array[1]);
    switch(command)
    {
    case velocity:
        Shared_Ram_dataWrite_m3(velocity, velocity_cmd);
        break;

    case omega:
        Shared_Ram_dataWrite_m3(omega, omega_cmd);
        break;

    case left:
        Shared_Ram_dataWrite_m3(left, left_cmd);
        break;

    case right:
        Shared_Ram_dataWrite_m3(right, right_cmd);
        break;

    default:
        break;
    }
}

void Shared_Ram_dataWrite_m3(int motor,float value)
{
    switch(motor)
    {
    case velocity:
        m3_r_w_array[0] = value;
        y = value;
        break;
    case omega:
        m3_r_w_array[1] = value;
        break;
    case left:
        m3_r_w_array[2] = value;
        break;
    case right:
        m3_r_w_array[3] = value;
        break;
    }
}

void Shared_Ram_dataRead_m3(void)
{
   // int index = 0;
       // m3_r_w_array[index] = m3_r_array[index];
}
