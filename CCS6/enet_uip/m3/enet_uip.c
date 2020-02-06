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
//! - All EMAC examples use a generic TI MAC address A8-63-F2-00-00-80.
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

float m3_r_array[2048]; // this array is mapped to S0
float m3_r_w_array[2048]; // this array is mapped to S2

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
#define grip             0xA7
#define magnitude             0xA9
#define degree              0xAA
#define inverse_x           0xAB
#define inverse_y           0xAC
#define inverse_the         0xAD
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
extern float grip_cmd;
extern float magnitude_cmd;
extern float degree_cmd;
extern float inverse_x_cmd;
extern float inverse_y_cmd;
extern float inverse_the_cmd;


int i = 0;
char buffer[6];
char *buffer_pointer = &buffer[0];


//*****************************************************************************
// The error routine that is called if the driver library encounters an error.
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}

#endif

//*****************************************************************************
// The interrupt handler for the SysTick interrupt.
//*****************************************************************************
void
SysTickIntHandler(void)
{
    // Increment the system tick count.
    g_ulTickCounter++;

    // Indicate that a SysTick interrupt has occurred.
    HWREGBITW(&g_ulFlags, FLAG_SYSTICK) = 1;
}

//*****************************************************************************
// Display the current IP address on the screen and transmit it via the UART.
//*****************************************************************************
void
ShowIPAddress(const uip_ipaddr_t sIPAddr)
{
    char pcBuffer[24];

    usprintf(pcBuffer, "IP: %d.%d.%d.%d", sIPAddr[0] & 0xff,
             sIPAddr[0] >> 8, sIPAddr[1] & 0xff, sIPAddr[1] >> 8);
}

//*****************************************************************************
//! When using the timer module in UIP, this function is required to return
//! the number of ticks.  Note that the file "clock-arch.h" must be provided
//! by the application, and define CLOCK_CONF_SECONDS as the number of ticks
//! per second, and must also define the typedef "clock_time_t".
//*****************************************************************************
clock_time_t
clock_time(void)
{
    return((clock_time_t)g_ulTickCounter);
}

//*****************************************************************************
// The interrupt handler for the Ethernet interrupt.
//*****************************************************************************
void
EthernetIntHandler(void)
{
    unsigned long ulTemp;

    // Read and Clear the interrupt.
    ulTemp = EthernetIntStatus(ETH_BASE, false);
    EthernetIntClear(ETH_BASE, ulTemp);

    // Check to see if an RX Interrupt has occurred.
    if(ulTemp & ETH_INT_RX)
    {
        // Indicate that a packet has been received.
        HWREGBITW(&g_ulFlags, FLAG_RXPKT) = 1;

        // Disable Ethernet RX Interrupt.
        EthernetIntDisable(ETH_BASE, ETH_INT_RX);
    }

    // Check to see if waiting on a DMA to complete.
    if(HWREGBITW(&g_ulFlags, FLAG_RXPKTPEND) == 1)
    {
        // Verify the channel transfer is done
        if(uDMAChannelModeGet(UDMA_CHANNEL_ETH0RX) == UDMA_MODE_STOP)
        {
            // Indicate that a data has been read in.
            HWREGBITW(&g_ulFlags, FLAG_RXPKTPEND) = 0;
        }
    }

    // Check to see if the Ethernet TX uDMA channel was pending.
    if(HWREGBITW(&g_ulFlags, FLAG_TXPKT) == 1)
    {
        // Verify the channel transfer is done
        if(uDMAChannelModeGet(UDMA_CHANNEL_ETH0TX) == UDMA_MODE_STOP)
        {
            // Trigger the transmission of the data.
            HWREG(ETH_BASE + MAC_O_TR) = MAC_TR_NEWTX;

            // Indicate that a packet has been sent.
            HWREGBITW(&g_ulFlags, FLAG_TXPKT) = 0;
        }
    }
}

//*****************************************************************************
// Callback for when DHCP client has been configured.
//*****************************************************************************
void
dhcpc_configured(const struct dhcpc_state *s)
{
    uip_sethostaddr(&s->ipaddr);
    uip_setnetmask(&s->netmask);
    uip_setdraddr(&s->default_router);
    ShowIPAddress(s->ipaddr);
}

//*****************************************************************************
// Read a packet using DMA instead of directly reading the FIFO if the
// alignment will allow it.
//*****************************************************************************
long
EthernetPacketGetDMA(unsigned long ulBase, unsigned char *pucBuf, long lBufLen)
{
    unsigned long ulTemp;
    unsigned char pucData[4];
    unsigned char *pucBuffer;
    long lTempLen, lFrameLen;
    long lRemainder;
    int iIdx;

    // Check the arguments.
    ASSERT(ulBase == ETH_BASE);
    ASSERT(pucBuf != 0);
    ASSERT(lBufLen > 0);

    // If the buffer is not aligned on an odd half-word then it cannot use DMA.
    // This is because the two packet length bytes are written in front of the
    // packet, and the packet data must have two bytes that can be pulled off
    // to become a word and leave the remainder of the buffer word aligned.
    if(((unsigned long)pucBuf & 3) != 2)
    {
        // If there is not proper alignment the packet must be sent without
        // using DMA.
        return(EthernetPacketGetNonBlocking(ulBase, pucBuf, lBufLen));
    }

    // Read WORD 0 from the FIFO, set the receive Frame Length and store the
    // first two bytes of the destination address in the receive buffer.
    ulTemp = HWREG(ulBase + MAC_O_DATA);
    lFrameLen = (long)(ulTemp & 0xffff);
    pucBuf[0] = (unsigned char)((ulTemp >> 16) & 0xff);
    pucBuf[1] = (unsigned char)((ulTemp >> 24) & 0xff);

    // The maximum DMA size is the frame size - the two bytes already read and
    // truncated to the nearest word size.
    lTempLen = (lFrameLen - 2) & 0xfffffffc;
    lRemainder = (lFrameLen - 2) & 3;

    // Don't allow writing beyond the end of the buffer.
    if(lBufLen < lTempLen)
    {
        lRemainder = lTempLen - lBufLen;
        lTempLen =  lBufLen;
    }
    else if(lBufLen >= (lFrameLen - 2 + 3))
    {
        // If there is room, just DMA the last word as well so that the
        // special copy after DMA is not required.
        lRemainder = 0;
        lTempLen = lFrameLen - 2 + 3;
    }

    // Mark the receive as pending.
    HWREGBITW(&g_ulFlags, FLAG_RXPKTPEND) = 1;

    // Set up the DMA to transfer the Ethernet header when a
    // packet is received
    uDMAChannelTransferSet(UDMA_CHANNEL_ETH0RX, UDMA_MODE_AUTO,
                           (void *)(ETH_BASE + MAC_O_DATA),
                           &pucBuf[2], lTempLen>>2);
    uDMAChannelEnable(UDMA_CHANNEL_ETH0RX);

    // Issue a software request to start the channel running.
    uDMAChannelRequest(UDMA_CHANNEL_ETH0RX);

    // Wait for the previous transmission to be complete.
    while(HWREGBITW(&g_ulFlags, FLAG_RXPKTPEND) == 1)
    {
    }

    // See if there are extra bytes to read into the buffer.
    if(lRemainder)
    {
        // If the remainder is more than 3 bytes then the buffer was never big
        // enough and data must be tossed.
        if(lRemainder > 3)
        {
            // Read any remaining WORDS (that did not fit into the buffer).
            while(lRemainder > 0)
            {
                ulTemp = HWREG(ulBase + MAC_O_DATA);
                lRemainder -= 4;
            }
        }

        // Read the last word from the FIFO.
        *((unsigned long *)&pucData[0]) = HWREG(ulBase + MAC_O_DATA);

        // The current buffer position is lTempLen plus the two bytes read
        // from the first word.
        pucBuffer = &pucBuf[lTempLen + 2];

        // Read off each individual byte and save it.
        for(iIdx = 0; iIdx < lRemainder; iIdx++)
        {
            pucBuffer[iIdx] = pucData[iIdx];
        }
    }

    // If frame was larger than the buffer, return the "negative" frame length.
    lFrameLen -= 6;
    if(lFrameLen > lBufLen)
    {
        return(-lFrameLen);
    }

    // Return the Frame Length
    return(lFrameLen);
}

//*****************************************************************************
// Transmit a packet using DMA instead of directly writing the FIFO if the
// alignment will allow it.
//*****************************************************************************
static long
EthernetPacketPutDMA(unsigned long ulBase, unsigned char *pucBuf,long lBufLen)
{
    unsigned long ulTemp;

    // If the buffer is not aligned on an odd half-word then it cannot use DMA.
    // This is because the two packet length bytes are written in front of the
    // packet, and the packet data must have two bytes that can be pulled off
    // to become a word and leave the remainder of the buffer word aligned.
    if(((unsigned long)pucBuf & 3) != 2)
    {
        // If there is not proper alignment the packet must be sent without
        // using DMA.
        return(EthernetPacketPut(ulBase, pucBuf, lBufLen));
    }

    // Indicate that a packet is being sent.
    HWREGBITW(&g_ulFlags, FLAG_TXPKT) = 1;

    // Build and write WORD 0 (see format above) to the transmit FIFO.
    ulTemp = (unsigned long)(lBufLen - 14);
    ulTemp |= (*pucBuf++) << 16;
    ulTemp |= (*pucBuf++) << 24;
    HWREG(ulBase + MAC_O_DATA) = ulTemp;

    // Force an extra word to be transferred if the end of the buffer is not
    // aligned on a word boundary.  The math is actually lBufLen - 2 + 3 to
    // insure that the proper number of bytes are written.
    lBufLen += 1;

    // Configure the TX DMA channel to transfer the packet buffer.
    uDMAChannelTransferSet(UDMA_CHANNEL_ETH0TX, UDMA_MODE_AUTO,
                           pucBuf, (void *)(ETH_BASE + MAC_O_DATA),
                           lBufLen>>2);

    // Enable the Ethernet Transmit DMA channel.
    uDMAChannelEnable(UDMA_CHANNEL_ETH0TX);

    // Issue a software request to start the channel running.
    uDMAChannelRequest(UDMA_CHANNEL_ETH0TX);

    // Wait for the previous transmission to be complete.
    while((HWREGBITW(&g_ulFlags, FLAG_TXPKT) == 1) &&
          EthernetSpaceAvail(ETH_BASE))
    {
    }

    // Take back off the byte that we added above.
    return(lBufLen - 1);

}

//*****************************************************************************
// respond to commands sent by the user (client)
//
//*****************************************************************************
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

    case grip:
        Shared_Ram_dataWrite_m3(grip, grip_cmd);
        break;

    case magnitude:
        Shared_Ram_dataWrite_m3(magnitude, magnitude_cmd);
        break;

    case degree:
        Shared_Ram_dataWrite_m3(degree, degree_cmd);
        break;

    case inverse_x:
        Shared_Ram_dataWrite_m3(inverse_x, inverse_x_cmd);
        break;

    case inverse_y:
        Shared_Ram_dataWrite_m3(inverse_y, inverse_y_cmd);
        break;

    case inverse_the:
        Shared_Ram_dataWrite_m3(inverse_the, inverse_the_cmd);
        break;

    default:
        break;
    }
}


//*****************************************************************************
// This example demonstrates the use of the Ethernet Controller with the uIP
// TCP/IP stack.
//*****************************************************************************
int
main(void)
{
    uip_ipaddr_t ipaddr;
    static struct uip_eth_addr sTempAddr;
    long lPeriodicTimer, lARPTimer;
    unsigned long ulUser0, ulUser1;
    unsigned long ulTemp;
    int command_word;

    // Disable Protection
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;


    // Sets up PLL, M3 running at 75MHz and C28 running at 150MHz
    SysCtlClockConfigSet(SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xF) |
                         SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_2 |
                         SYSCTL_XCLKDIV_4);
    RAMMReqSharedMemAccess((S0_ACCESS | S1_ACCESS),C28_MASTER);

#ifdef _STANDALONE
#ifdef _FLASH
    //  Send boot command to allow the C28 application to begin execution
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH);
#else
    //  Send boot command to allow the C28 application to begin execution
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_RAM);
#endif
#endif

#ifdef _FLASH
// Copy time critical code and Flash setup code to RAM
// This includes the following functions:  InitFlash();
// The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
// symbols are created by the linker. Refer to the device .cmd file.
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
    FlashInit();
#endif

    PinoutSet();

    // Enable clock supply for the following peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);

    // Disable clock supply for the watchdog modules
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG1);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG0);

    // Enable Pullups on EPWM(1-8)A/B capable pins
    GPIOPadConfigSet(GPIO_PORTA_BASE, 0xFF, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTB_BASE, 0xFF, GPIO_PIN_TYPE_STD_WPU);

    // Give C28 control of all GPIOs
    GPIOPinConfigureCoreSelect(GPIO_PORTA_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    GPIOPinConfigureCoreSelect(GPIO_PORTB_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    GPIOPinConfigureCoreSelect(GPIO_PORTD_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    GPIOPinConfigureCoreSelect(GPIO_PORTE_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    GPIOPinConfigureCoreSelect(GPIO_PORTF_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    GPIOPinConfigureCoreSelect(GPIO_PORTG_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);
    GPIOPinConfigureCoreSelect(GPIO_PORTJ_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT);

        // Unlock the register to change the commit register value for Port B Pin 7
        // This disables the NMI functionality on the pin and allows other muxing
        // options to be used
        HWREG(GPIO_PORTB_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
        // Write to commit register
        HWREG(GPIO_PORTB_BASE+GPIO_O_CR) |= 0x000000FF;
        // Delay
        for (i=0;i<20;i++){};

    // Adjust the pointer to be aligned on an odd half word address so that
    // DMA can be used.
    uip_buf = (u8_t *)(((unsigned long)ucUIPBuffer + 3) & 0xfffffffe);

    // Enable the uDMA controller and set up the control table base.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    uDMAEnable();
    uDMAControlBaseSet(g_sDMAControlTable);

    // Configure the DMA TX channel
    uDMAChannelAttributeDisable(UDMA_CHANNEL_ETH0TX, UDMA_ATTR_ALL);
    uDMAChannelControlSet(UDMA_CHANNEL_ETH0TX,
                          UDMA_SIZE_32 | UDMA_SRC_INC_32 |
                          UDMA_DST_INC_NONE | UDMA_ARB_8);

    // Set user/company specific MAC octets
    // (for this code we are using A8-63-F2-00-00-80)
    // 0x00 MACOCT3 MACOCT2 MACOCT1
    ulUser0 = 0x00F263A8;

    // 0x00 MACOCT6 MACOCT5 MACOCT4
    ulUser1 = 0x00800000;

    // User needs to program user specific MAC address into Flash
    // and read it on power up
    if((ulUser0 == 0xffffffff) || (ulUser1 == 0xffffffff))
    {
        // We should never get here.  This is an error if the MAC address has
        // not been programmed into the device.  Exit the program.
        while(1)
        {
        }
    }

    // Enable and Reset the Ethernet Controller.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
    SysCtlPeripheralReset(SYSCTL_PERIPH_ETH);

    // Configure SysTick for a periodic interrupt.
    SysTickPeriodSet(SysCtlClockGet(SYSTEM_CLOCK_SPEED) / SYSTICKHZ);
    SysTickEnable();
    IntRegister(FAULT_SYSTICK, SysTickIntHandler);
    SysTickIntEnable();

    // Configure the DMA channel for Ethernet receive.
    uDMAChannelAttributeDisable(UDMA_CHANNEL_ETH0RX, UDMA_ATTR_ALL);
    uDMAChannelControlSet(UDMA_CHANNEL_ETH0RX,
                          UDMA_SIZE_32 | UDMA_SRC_INC_NONE |
                          UDMA_DST_INC_32 | UDMA_ARB_8);

    // Initialize the Ethernet Controller and disable all Ethernet Controller
    // interrupt sources.
    EthernetIntDisable(ETH_BASE, (ETH_INT_PHY | ETH_INT_MDIO |
                                  ETH_INT_RXER | ETH_INT_RXOF |
                                  ETH_INT_TX | ETH_INT_TXER | ETH_INT_RX));
    ulTemp = EthernetIntStatus(ETH_BASE, false);
    EthernetIntClear(ETH_BASE, ulTemp);

    // Initialize the Ethernet Controller for operation.
    EthernetInitExpClk(ETH_BASE, SysCtlClockGet(SYSTEM_CLOCK_SPEED));

    // Configure the Ethernet Controller for normal operation.
    // - Full Duplex
    // - TX CRC Auto Generation
    // - TX Padding Enabled
    EthernetConfigSet(ETH_BASE, (ETH_CFG_TX_DPLXEN | ETH_CFG_TX_CRCEN |
                                 ETH_CFG_TX_PADEN));

    // Wait for the link to become active.
    while((EthernetPHYRead(ETH_BASE, PHY_MR1) & 0x0004) == 0)
    {
    }

    // Enable the Ethernet Controller.
    EthernetEnable(ETH_BASE);

    // Enable and register the Ethernet interrupt.
    IntRegister(INT_ETH, EthernetIntHandler);
    IntEnable(INT_ETH);
#ifndef REV_0_SILICON
    IntRegister(INT_UDMA, EthernetIntHandler);
    IntEnable(INT_UDMA);
#endif
    // Enable the Ethernet RX Packet interrupt source.
    EthernetIntEnable(ETH_BASE, ETH_INT_RX);

    // Enable all processor interrupts.
    IntMasterEnable();

    // Initialize the uIP TCP/IP stack.
    uip_init();
#ifdef USE_STATIC_IP
    uip_ipaddr(ipaddr, DEFAULT_IPADDR0, DEFAULT_IPADDR1, DEFAULT_IPADDR2,
               DEFAULT_IPADDR3);
    uip_sethostaddr(ipaddr);
    ShowIPAddress(ipaddr);
    uip_ipaddr(ipaddr, DEFAULT_NETMASK0, DEFAULT_NETMASK1, DEFAULT_NETMASK2,
               DEFAULT_NETMASK3);
    uip_setnetmask(ipaddr);
#else
    uip_ipaddr(ipaddr, 0, 0, 0, 0);
    uip_sethostaddr(ipaddr);
    uip_ipaddr(ipaddr, 0, 0, 0, 0);
    uip_setnetmask(ipaddr);
#endif

    // Convert the 24/24 split MAC address from ram into a 32/16 split MAC
    // address needed to program the hardware registers, then program the MAC
    // address into the Ethernet Controller registers.
    sTempAddr.addr[0] = ((ulUser0 >>  0) & 0xff);
    sTempAddr.addr[1] = ((ulUser0 >>  8) & 0xff);
    sTempAddr.addr[2] = ((ulUser0 >> 16) & 0xff);
    sTempAddr.addr[3] = ((ulUser1 >>  0) & 0xff);
    sTempAddr.addr[4] = ((ulUser1 >>  8) & 0xff);
    sTempAddr.addr[5] = ((ulUser1 >> 16) & 0xff);

    // Program the hardware with it's MAC address (for filtering).
    EthernetMACAddrSet(ETH_BASE, (unsigned char *)&sTempAddr);
    uip_setethaddr(sTempAddr);

    // Initialize the TCP/IP Application (e.g. web server).
    httpd_init();

#ifndef USE_STATIC_IP

    // Initialize the DHCP Client Application.
    dhcpc_init(&sTempAddr.addr[0], 6);
    dhcpc_request();
#endif

    // Main Application Loop.
    lPeriodicTimer = 0;
    lARPTimer = 0;
    httpd_clear_command();
    while(true)
    {

        // Wait for an event to occur.  This can be either a System Tick event,
        // or an RX Packet event.
        while(!g_ulFlags)
        {
        }

        // If SysTick, Clear the SysTick interrupt flag and increment the
        // timers.
        if(HWREGBITW(&g_ulFlags, FLAG_SYSTICK) == 1)
        {
            HWREGBITW(&g_ulFlags, FLAG_SYSTICK) = 0;
            lPeriodicTimer += SYSTICKMS;
            lARPTimer += SYSTICKMS;
        }

        // Check for an RX Packet and read it.
        if(HWREGBITW(&g_ulFlags, FLAG_RXPKT))
        {
            // Get the packet and set uip_len for uIP stack usage.
            uip_len = (unsigned short)EthernetPacketGetDMA(ETH_BASE, uip_buf,
                                                           sizeof(ucUIPBuffer));

            // Clear the RX Packet event and re-enable RX Packet interrupts.
            if(HWREGBITW(&g_ulFlags, FLAG_RXPKT) == 1)
            {
                HWREGBITW(&g_ulFlags, FLAG_RXPKT) = 0;
                EthernetIntEnable(ETH_BASE, ETH_INT_RX);
            }

            // Process incoming IP packets here.
            if(BUF->type == htons(UIP_ETHTYPE_IP))
            {

                uip_arp_ipin();
                uip_input(); //uip_process (calls uip_appcall())

                if(httpd_get_command(&command_word))
                {
                    httpd_clear_command();
                    printf("%d",command_word);
                    x = command_word;
                    EthernetProcessCMD(command_word);

                }

                // If the above function invocation resulted in data that
                // should be sent out on the network, the global variable
                // uip_len is set to a value > 0.
                if(uip_len > 0)
                {
                    uip_arp_out();
                    EthernetPacketPutDMA(ETH_BASE, uip_buf, uip_len);
                    uip_len = 0;
                }
            }

            // Process incoming ARP packets here.
            else if(BUF->type == htons(UIP_ETHTYPE_ARP))
            {
                uip_arp_arpin();

                // If the above function invocation resulted in data that
                // should be sent out on the network, the global variable
                // uip_len is set to a value > 0.
                if(uip_len > 0)
                {
                    EthernetPacketPutDMA(ETH_BASE, uip_buf, uip_len);
                    uip_len = 0;
                }
            }
        }

        // Process TCP/IP Periodic Timer here.
        if(lPeriodicTimer > UIP_PERIODIC_TIMER_MS)
        {
            lPeriodicTimer = 0;
            for(ulTemp = 0; ulTemp < UIP_CONNS; ulTemp++)
            {
                uip_periodic(ulTemp);

                // If the above function invocation resulted in data that
                // should be sent out on the network, the global variable
                // uip_len is set to a value > 0.
                if(uip_len > 0)
                {
                    uip_arp_out();
                    EthernetPacketPutDMA(ETH_BASE, uip_buf, uip_len);
                    uip_len = 0;
                }
            }

#if UIP_UDP
            for(ulTemp = 0; ulTemp < UIP_UDP_CONNS; ulTemp++)
            {
                uip_udp_periodic(ulTemp);

                // If the above function invocation resulted in data that
                // should be sent out on the network, the global variable
                // uip_len is set to a value > 0.
                if(uip_len > 0)
                {
                    uip_arp_out();
                    EthernetPacketPutDMA(ETH_BASE, uip_buf, uip_len);
                    uip_len = 0;
                }
            }
#endif
        }

        // Process ARP Timer here.
        if(lARPTimer > UIP_ARP_TIMER_MS)
        {
            lARPTimer = 0;
            uip_arp_timer();
        }
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
    case grip:
        m3_r_w_array[4] = value;
        break;
    case magnitude:
        m3_r_w_array[5] = value;
        break;
    case degree:
        m3_r_w_array[6] = value;
        break;
    case inverse_x:
        m3_r_w_array[7] = value;
        break;
    case inverse_y:
        m3_r_w_array[8] = value;
        break;
    case inverse_the:
        m3_r_w_array[9] = value;
        break;



    }
}

void Shared_Ram_dataRead_m3(void)
{
   // int index = 0;
       // m3_r_w_array[index] = m3_r_array[index];
}
