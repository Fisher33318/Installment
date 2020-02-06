//###########################################################################
// FILE:   httpd.h
// TITLE:  Web Server Application Header
//###########################################################################
// $TI Release: F28M35x Support Library v207 $
// $Release Date: Mon Sep 21 16:49:54 CDT 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HTTPD_H__
#define __HTTPD_H__

//*****************************************************************************
// Web Server Application Entry Points.
//*****************************************************************************
void httpd_init(void);
void httpd_appcall(void);
void httpd_test(void);

//*****************************************************************************
// Web Server Application State Variable Definition.
//*****************************************************************************
struct httpd_state
{
    u8_t state;
    u16_t count;
};

#endif // __HTTPD_H__



