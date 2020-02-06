//###########################################################################
//
// FILE:	Example_EpwmSetup.c
//
// TITLE:	Pos speed measurement using EQEP peripheral
//
// DESCRIPTION:
//
// This file contains source for the ePWM initialization for the 
// pos/speed module
//
//###########################################################################
// $TI Release: F28M35x Support Library v207 $ 
// $Release Date: Mon Sep 21 16:49:54 CDT 2015 $ 
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Example_posspeed.h"   // Example specific Include file

#if (DSP28_F28M35Hx_150MHZ) || (DSP28_F28M36Hx_150MHZ) || (DSP28_F28M36Px_150MHZ)     /* 150 Mhz device */
  #define CPU_CLK   150e6
#endif
#if (DSP28_F28M35Mx_100MHZ) || (DSP28_F28M36Hx_100MHZ)     /* 100 Mhz device */
  #define CPU_CLK   100e6
#endif

#define PWM_CLK   5e3			//5Khz PWM
#define SP        CPU_CLK/(2*PWM_CLK)
#define TBCTLVAL  0x200E           // up-down count, timebase=SYSCLKOUT
#define PERIOD	  CPU_CLK/PWM_CLK
#define DUTY1	  0
#define DUTY2	  0
#define DUTY6     0
#define DUTY7     0


void initEpwm1()
{/*
	EALLOW;
	GpioTripRegs.GPTRIP6SEL.bit.GPTRIP6SEL = 8;
	EPwm1Regs.TBSTS.all=0;
	EPwm1Regs.TBPHS.half.TBPHS =0;
	EPwm1Regs.TBCTR=0;

	EPwm1Regs.CMPCTL.all=0x50;     // immediate mode for CMPA and CMPB   
	EPwm1Regs.CMPA.half.CMPA=SP/2;
	EPwm1Regs.CMPB=0;
	
	EPwm1Regs.AQCTLA.all=0x60;     // CTR=CMPA when inc->EPWM1A=1, when dec->EPWM1A=0
	EPwm1Regs.AQCTLB.all=0x09;     // CTR=PRD ->EPWM1B=1, CTR=0 ->EPWM1B=0  
	EPwm1Regs.AQSFRC.all=0;
	EPwm1Regs.AQCSFRC.all=0;
	
	EPwm1Regs.TZSEL.all=0;
	EPwm1Regs.TZCTL.all=0;
	EPwm1Regs.TZEINT.all=0;
	EPwm1Regs.TZFLG.all=0;
	EPwm1Regs.TZCLR.all=0;
	EPwm1Regs.TZFRC.all=0;

	EPwm1Regs.ETSEL.all=0x0A;      // Interrupt on PRD 
	EPwm1Regs.ETPS.all=1;	
	EPwm1Regs.ETFLG.all=0;
	EPwm1Regs.ETCLR.all=0;
	EPwm1Regs.ETFRC.all=0;
	
	EPwm1Regs.PCCTL.all=0;
	
	EPwm1Regs.TBCTL.all=0x0010+TBCTLVAL; // Enable Timer
	EPwm1Regs.TBPRD=SP;*/


	 EPwm1Regs.TBPRD = PERIOD;                        // Set timer period
	 EPwm1Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
	 EPwm1Regs.TBCTR = 0x0000;                      // Clear counter

	 // Setup TBCLK
	 EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;    // Count up
	 EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;       // Disable phase loading
	 EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;      // Clock ratio to SYSCLKOUT
	 EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	 EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;   // Load registers every ZERO
	 EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	 EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	 EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

     // Setup compare
     EPwm1Regs.CMPA.half.CMPA =  PERIOD*DUTY1/100;

     // Set actions
     EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
     EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;
     EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;
     EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;

}
void initEpwm2()
{
	 EPwm2Regs.TBPRD = PERIOD;                        // Set timer period
	 EPwm2Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
	 EPwm2Regs.TBCTR = 0x0000;                      // Clear counter

	 // Setup TBCLK
	 EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;    // Count up
	 EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;       // Disable phase loading
	 EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;      // Clock ratio to SYSCLKOUT
	 EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	 EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;   // Load registers every ZERO
	 EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	 EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	 EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	 // Setup compare
	 EPwm2Regs.CMPA.half.CMPA = PERIOD*DUTY2/100;

	 // Set actions
	 EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	 EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET;

	 EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;
	 EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;


}

void initEpwm6()	//OK
{
	 EPwm6Regs.TBPRD = PERIOD;                        // Set timer period
	 EPwm6Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
	 EPwm6Regs.TBCTR = 0x0000;                      // Clear counter

	 // Setup TBCLK
	 EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;    // Count up
	 EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;       // Disable phase loading
	 EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;      // Clock ratio to SYSCLKOUT
	 EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	 EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;   // Load registers every ZERO
	 EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	 EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	 EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	 // Setup compare
	 EPwm6Regs.CMPA.half.CMPA = PERIOD*DUTY6/100;

	 // Set actions
	 EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	 EPwm6Regs.AQCTLA.bit.ZRO = AQ_SET;

	 EPwm6Regs.AQCTLB.bit.CAU = AQ_SET;
	 EPwm6Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
}

void initEpwm7()
{
	 EPwm7Regs.TBPRD = PERIOD;                        // Set timer period
	 EPwm7Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
	 EPwm7Regs.TBCTR = 0x0000;                      // Clear counter

	 // Setup TBCLK
	 EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;    // Count up
	 EPwm7Regs.TBCTL.bit.PHSEN = TB_DISABLE;       // Disable phase loading
	 EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;      // Clock ratio to SYSCLKOUT
	 EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	 EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;   // Load registers every ZERO
	 EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	 EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	 EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	 // Setup compare
	 EPwm7Regs.CMPA.half.CMPA = PERIOD*DUTY7/100;

	 // Set actions
	 EPwm7Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	 EPwm7Regs.AQCTLA.bit.ZRO = AQ_SET;

	 EPwm7Regs.AQCTLB.bit.CAU = AQ_SET;
	 EPwm7Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
}


void changeDuty1(float duty)	//OK
{
	 EPwm1Regs.CMPA.half.CMPA = PERIOD*duty/100;// implicit cast float to uint16 (float will be truncated)
}
void changeDuty2(float duty)	//OK
{
	 EPwm2Regs.CMPA.half.CMPA = PERIOD*duty/100;// implicit cast float to uint16 (float will be truncated)
}
void changeDuty7(float duty)
{
	 EPwm7Regs.CMPA.half.CMPA = PERIOD*duty/100;// implicit cast float to uint16 (float will be truncated)
}
void changeDuty6(float duty)	//OK
{
	 EPwm6Regs.CMPA.half.CMPA = PERIOD*duty/100;// implicit cast float to uint16 (float will be truncated)
}

