//###########################################################################
// FILE:   httpd.c
// This file has been modified from its original uIP distribution to add
// functionality needed for the enet_uip example.
//
// TITLE:  Ethernet GPIO control using uIP TCP/IP stack
//###########################################################################
// $TI Release: F28M35x Support Library v207 $
// $Release Date: Mon Sep 21 16:49:54 CDT 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "uip.h"
#include "httpd.h"
#include <stdlib.h>
//*****************************************************************************
// Macro for easy access to buffer data
//*****************************************************************************
#define BUF_APPDATA ((u8_t *)uip_appdata)

//*****************************************************************************
// Definitions of HTTP Server States
//*****************************************************************************
#define HTTP_NOGET      0
#define HTTP_FILE       1
#define HTTP_TEXT       2
#define HTTP_FUNC       3
#define HTTP_END        4

//*****************************************************************************
// Global for keeping up with web server state.
//*****************************************************************************
struct httpd_state *hs;

int index;

//*****************************************************************************
// command words
//*****************************************************************************
#define INVALID_CMD      0xA5
#define INVALID_INPUT    0xA6
#define NO_CMD           0xA6

#define velocity            0xA1
#define omega               0xA2
#define left                0xA3
#define right               0xA4
#define grip                0xA7
#define magnitude           0xA9
#define degree              0xAA
#define inverse_x           0xAB
#define inverse_y           0xAC
#define inverse_the         0xAD
#define get_velocity        4
#define get_omega           5
#define get_left            6
#define get_right           7

static int static_web_page_fully_sent = 0;
static int command;
static int selected;

float velocity_cmd;
float omega_cmd;
float left_cmd;
float right_cmd;
float grip_cmd;
float magnitude_cmd;
float degree_cmd;
float inverse_x_cmd;
float inverse_y_cmd;
float inverse_the_cmd;
char url_data[6];
//*****************************************************************************
// Default Web Page - allocated in three segments to allow easy update of a
// counter that is incremented each time the page is sent.
//*****************************************************************************
static const char page_not_found[] =
    "HTTP/1.0 404 OK\r\n"
    "Server: UIP/1.0 (http://www.sics.se/~adam/uip/)\r\n"
    "Content-type: text/html\r\n\r\n"
    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/html4/loose.dtd\">"
    "<html>"
    "<head>"
    "<title>Page Not Found!</title>"
    "</head>"
    "<body>"
    "Page Not Found!"
    "</body>"
    "</html>";

//*****************************************************************************
// Default Web Page - allocated in three segments to allow easy update of a
// counter that is incremented each time the page is sent.
//*****************************************************************************
static const char default_page_buf1of3[] =
    "HTTP/1.0 200 OK\r\n"
    "Server: UIP/1.0 (http://www.sics.se/~adam/uip/)\r\n"
    "Content-type: text/html\r\n\r\n"
    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/html4/loose.dtd\">"
    "<html>"
    "<head>"
    "<title>Welcome to the uIP web server!</title>"
    "</head>"
    "<script language=\"JavaScript\">"
    "var Rx = false;"
    "var http = false;"
    "function Rcv()"
    "{"
    "if(http.readyState == 4)"
    "{"
    "if(http.status == 200)"
    "{"
    "resp = http.responseText;"
    "if(Rx)"
    "{"
    "var v2 = document.getElementById(\"I2\");"
    "v2.value = resp;"
    "}"
    "}"
    "}"
    "}"
    "function cmd(input)"
    "{"
    "if(input == 'C0')"
    "{"
    "Rx = false;"
    "}"
    "else if(input == 'C1')"
    "{"
    "Rx = true;"
    "}"
    "var v1 = document.getElementById(\"I1\");"
    "if(window.XMLHttpRequest)"
    "{"
    "http = new XMLHttpRequest();"
    "}"
    "else if(window.ActiveXObject)"
    "{"
    "http = new ActiveXObject(\"Microsoft.XMLHTTP\");"
    "}"
    "if(http)"
    "{"
    "http.open(\"GET\", \"cmd?=\" + input + v1.value + \"&id\" + Math.random(), true);"
    "http.onreadystatechange = Rcv;"
    "http.send(null);"
    "}"

    "}"
    "</script>";
static const char default_page_buf2of3[] =
    "<html>"
    "<head>"
    "<title>SONATA web server</title>"
    "</head>"
    "<body>"
    "<center>"
    "<h1>Web server by Concerto</h1>"
    "<p>This demo shows Ethernet communication using &micro;IP stack"
    "<p>Enter a value of 6 or 7 and press \"ToggleLED\" button to toggle the respective LED."
    "<p>Press \"GetStatus\" to get the status of the selected LED."
    "<p>&nbsp;"
    "<hr size=\"3\" width=\"1000\">"
    "<p>&nbsp;"
    "<table>"
    "<tr><td width=\"3000\">Command word to Concerto</td><td width=\"3000\">Input data from Concerto</td></tr>"
    "<tr>"
    "<td><br/><input id=\"B1\" value=\"ToggleLED\" onclick=\"cmd('C0');\"type=\"button\"><input maxlength=\"3\" size=\"4\" id=\"I1\" type=\"text\"></td>"
    "<td><input id=\"B2\" value=\"GetStatus\" onclick=\"cmd('C1');\" type=\"button\"><input maxlength=\"10\" size=\"10\" id=\"I2\" type=\"text\"></td>"
    "</tr>"
    "</table>"
    "<br/><br/><br/><br/>";
static const char default_page_buf3of3[] =
    "Copyright &copy; 2009-2011 Texas Instruments Incorporated. All rights reserved."
    "</center>"
    "</body>"
    "</html>";
char empty_char[] = " ";
typedef struct
{
    char *response_data;
    int length;
}response;
response response_to_client;

//*****************************************************************************
// Initialize the web server.
// Starts to listen for incoming connection requests on TCP port 80.
//*****************************************************************************
void
httpd_init(void)
{
    // Listen to port 80.
    uip_listen(HTONS(80));
}

//*****************************************************************************
// Parse command, 'C0' to toggle LED, 'C1' to get status of the LED
//****************************************************************************
void
httpd_parse_command_word(void)
{
    // Toggle command
    if( (BUF_APPDATA[10] == 'C') && (BUF_APPDATA[11] == '0'))
    {
    	double temp ;
    	url_data[0]=BUF_APPDATA[13];
    	url_data[1]=BUF_APPDATA[14];
    	url_data[2]=BUF_APPDATA[15];
    	url_data[3]=BUF_APPDATA[16];
    	url_data[4]=BUF_APPDATA[17];
    	url_data[5]=BUF_APPDATA[18];

        switch(BUF_APPDATA[12])
        {
        case '1':
            command = velocity;
            temp = atof(url_data);
            velocity_cmd = (float)temp;
            selected = 0;
            break;

        case '2':
            command = omega;
            temp = atof(url_data);
            omega_cmd = (float)temp;
            selected = 1;
            break;

        case '3':
            command = left;
            temp = atof(url_data);
            left_cmd = (float)temp;
            selected = 2;
            break;

        case '4':
            command = right;
            temp = atof(url_data);
            right_cmd = (float)temp;
            selected = 3;
            break;

        case '5':
            command = grip;
            temp = atof(url_data);
            grip_cmd = (float)temp;
            selected = 4;
            break;

        case '6':
            command = magnitude;
            temp = atof(url_data);
            magnitude_cmd = (float)temp;
            selected = 5;
            break;

        case '7':
            command = degree;
            temp = atof(url_data);
            degree_cmd = (float)temp;
            selected = 6;
            break;

        case '8':
            command = inverse_x;
            temp = atof(url_data);
            inverse_x_cmd = (float)temp;
            selected = 7;
            break;

        case '9':
            command = inverse_y;
            temp = atof(url_data);
            inverse_y_cmd = (float)temp;
            selected = 8;
            break;

        case '0':
            command = inverse_the;
            temp = atof(url_data);
            inverse_the_cmd = (float)temp;
            selected = 9;
            break;

        default:
            command = INVALID_INPUT;
            selected = 10;

            break;
        }

    }

    // Get status command
    else if((BUF_APPDATA[10] == 'C')&& (BUF_APPDATA[11] == '1'))
    {
        switch(selected)
        {
        case 0:
            command = get_velocity;
            break;

        case 1:
            command = get_omega;
            break;

        case 2:
            command = get_left;
            break;

        case 3:
            command = get_right;
            break;

        default:
            command = INVALID_INPUT;
            break;
        }

    }

    // Invalid command
    else
    {
        command = INVALID_CMD;
    }

}

//*****************************************************************************
// return the status along with the command
//*****************************************************************************
int
httpd_get_command(int *command_word)
{
    if((command != INVALID_CMD) && (command != INVALID_INPUT) &&
       (command != NO_CMD))
    {
        *command_word = command;
        return(1);
    }
    else
    {
        return(0);
    }
}

//*****************************************************************************
// clear the command, so that we won't execute it again
//*****************************************************************************
void
httpd_clear_command(void)
{
    command = NO_CMD;
}

//*****************************************************************************
// insert a response
//*****************************************************************************

void
httpd_insert_response(int data_length,char *data)
{
    response_to_client.response_data = data;
    response_to_client.length = data_length;
}

//*****************************************************************************
// HTTP Application Callback Function
//*****************************************************************************
void
httpd_appcall(void)
{
    switch(uip_conn->lport)
    {
    // This is the web server:
    case HTONS(80):
    {

        // Pick out the application state from the uip_conn structure.
        hs = (struct httpd_state *)&(uip_conn->appstate);

        // We use the uip_ test functions to deduce why we were
        // called. If uip_connected() is non-zero, we were called
        // because a remote host has connected to us. If
        // uip_newdata() is non-zero, we were called because the
        // remote host has sent us new data, and if uip_acked() is
        // non-zero, the remote host has acknowledged the data we
        // previously sent to it. */
        if(uip_connected())
        {
            // Since we have just been connected with the remote host, we
            // reset the state for this connection. The ->count variable
            // contains the amount of data that is yet to be sent to the
            // remote host, and the ->state is set to HTTP_NOGET to signal
            // that we haven't received any HTTP GET request for this
            // connection yet.
            hs->state = HTTP_NOGET;
            hs->count = 0;
            return;
        }
        else if(uip_poll())
        {
            // If we are polled ten times, we abort the connection. This is
            // because we don't want connections lingering indefinitely in
            // the system.
            if(hs->count++ >= 10)
            {
                uip_abort();
            }
            return;
        }
        else if(uip_newdata() && hs->state == HTTP_NOGET)
        {

            if(BUF_APPDATA[0] != 'G' || BUF_APPDATA[1] != 'E' ||
               BUF_APPDATA[2] != 'T' || BUF_APPDATA[3] != ' ')
            {
                uip_abort();
                return;
            }

            // Check to see what we should send.
            if((BUF_APPDATA[4] == '/') && (BUF_APPDATA[5] == ' '))
            {
                static_web_page_fully_sent = 0;
                uip_send(default_page_buf1of3,sizeof(default_page_buf1of3) - 1);
            }
            else if((BUF_APPDATA[4] == '/') && (BUF_APPDATA[5] == 'c')&&
                    (BUF_APPDATA[6] == 'm')&&(BUF_APPDATA[7] == 'd'))
            {
                httpd_parse_command_word();

                // send an empty character. when this function exits the values
                // will be updated. The response will be
                // sent to client when client acks the empty character
                uip_send(empty_char,sizeof(empty_char)-1);
            }
            else
            {
                uip_send(page_not_found,sizeof(page_not_found) - 1);
                hs->count = 3;
            }
        }
        else if(uip_acked())
        {

            hs->count++;
            if(static_web_page_fully_sent == 0)
            {
                if(hs->count == 1)
                {
                    uip_send(default_page_buf2of3,
                             sizeof(default_page_buf2of3) - 1);
                }
                else if(hs->count == 2)
                {
                    uip_send(default_page_buf3of3,
                             sizeof(default_page_buf3of3) - 1);
                    static_web_page_fully_sent = 1;
                }
                else
                {
                    uip_close();
                }
            }
            else
            {
                if(hs->count == 1)
                {
                    uip_send(response_to_client.response_data,
                             response_to_client.length);
                }
                else
                {
                    uip_close();
                }

            }
        }

        // Finally, return to uIP. Our outgoing packet will soon be on its
        // way...
        return;
    }

    default:
    {
        // Should never happen.
        uip_abort();
        break;
    }
    }
}




