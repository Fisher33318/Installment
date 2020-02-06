//###########################################################################
//
// FILE:    eqep_pos_speed_c28.c
// TITLE:   EQEP Speed and Position measurement (eqep_pos_speed)
//
//! \addtogroup control_example_list
//! <h1> EQEP Speed and Position measurement (eqep_pos_speed) </h1>
//!
//!    This example requires the following hardware connections from EPWM1 and
//!    GPIO pins (simulating QEP sensor) to QEP peripheral
//!
//! - GPIO20/EQEP1A <- GPIO0/EPWM1A (simulates EQEP Phase A signal)
//! - GPIO21/EQEP1B <- GPIO1/EPWM1B (simulates EQEP Phase B signal)
//! - GPIO23/EQEP1I <- GPIO4 (simulates EQEP Index Signal)
//!
//!    This example will provide position measurement using the capture unit, and
//!    speed measurement using unit time out. This example uses the IQMath library. It is used
//!    merely to simplify high-precision calculations.
//!
//!    See DESCRIPTION in Example_posspeed.c for more details on the calculations
//!    performed in this example.
//!
//! In addition to this file, the following files must be included in this project: \n
//! - Example_posspeed.c - includes all eQEP functions
//! - Example_EPwmSetup.c - sets up EPWM1A and EPWM1B as simulated QA and QB encoder signals
//! - Example_posspeed.h - includes initialization values for pos and speed structure
//!
//! \b Notes:
//! - Maximum speed is configured to 6000rpm(BaseRpm)
//! - Minimum speed is assumed at 10rpm for capture pre-scalar selection
//! - Pole pair is configured to 2 (pole_pairs)
//! - QEP Encoder resolution is configured to 4000counts/revolution (mech_scaler)
//!   which means: 4000/4 = 1000 line/revolution quadrature encoder (simulated by EPWM1)
//! - EPWM1 (simulating QEP encoder signals) is configured for 5kHz frequency or 300 rpm
//!      (=4*5000 cnts/sec * 60 sec/min)/4000 cnts/rev)
//!
//!     \b SPEEDRPM_FR: High Speed Measurement is obtained by counting the QEP input pulses
//!              for 10ms (unit timer set to 100Hz).
//!
//!     \b SPEEDRPM_FR = { (Position Delta)/10ms } * 60 rpm
//!
//!
//!     \b SPEEDRPM_PR: Low Speed Measurement is obtained by measuring time period of QEP edges.
//!              Time measurement is averaged over 64edges for better results and
//!              capture unit performs the time measurement using pre-scaled SYSCLK
//!
//!     Note that pre-scaler for capture unit clock is selected such that
//!     capture timer does not overflow at the required minimum RPM speed
//!
//!  \b Watch \b Variables:
//!  - \b qep_posspeed_r.SpeedRpm_fr - Speed meas. in rpm using QEP position counter
//!  - \b qep_posspeed_r.SpeedRpm_pr - Speed meas. in rpm using capture unit
//!  - \b qep_posspeed_r.theta_mech - Motor mechanical angle (Q15)
//!  - \b qep_posspeed_r.theta_elec - Motor electrical angle (Q15)
//
//###########################################################################
// $TI Release: F28M35x Support Library v207 $
// $Release Date: Mon Sep 21 16:49:54 CDT 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Example_posspeed.h"   // Example specific Include file
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define C28_FREQ    150         //CPU frequency in MHz
#define PI			3.14159265359
#define h 			0.001 //sampling time
//initial pwm
void initEpwm1();
void initEpwm2();
void initEpwm6();
void initEpwm7();

//cpu timer
__interrupt void cpu_timer0_isr(void);
__interrupt void cpu_timer1_isr(void);
__interrupt void cpu_timer2_isr(void);

//set duty cycle
void changeDuty1(float); //duty cycle >74 ,encoder the same value = 628 rad/s
void changeDuty2(float);
void changeDuty6(float);
void changeDuty7(float);

//calculate real velocity from encoder
//motor control
//turn on/off motor
void servoOn();
void servoOff();
void grip();
void InverseKinematic(float,float,float,float,float,float);//theta 是弧度
void movecar(float,float);



float c28_r_array[2048];     // mapped to S2 of shared RAM owned by M3
//long int c28_r_w_array[2048];   // mapped to S0 of shared RAM owned by c28

#pragma DATA_SECTION(c28_r_array,"SHARERAMS2");
//#pragma DATA_SECTION(c28_r_w_array,"SHARERAMS0");

void Shared_Ram_dataRead_c28(); // function to read data from shared RAM owned by M3
//void Shared_Ram_dataWrite_c28(int);// function to write data from shared RAM owned by c28


void linetracking();
void computespeed();
void turn_left();
void turn_right();


//car parameters
float ratio = 0.03247; //motor:wheel=154:5
const float rw = 0.06; //radius of wheel
const float R = 0.2; //half distance of two wheel
const float distance = 0.45; // from reference frame to the head of car
float controlmatrix[2][2];
float inv_controlmatrix[2][2];

//path output
float wr = 0; //angular velocity of right wheel
float wl = 0; //angular velocity of left wheel
float v = 0;
float w = 0;
float l = 0;
float r = 0;
float forward = 0;
float theta = 0;
float radius = 0;
float K = 0.1;
float speed = 0.125;
float rotate = 0;
float f1 = 8;
float f2 = 10;

float Xg = 0;
float Yg = 0;
float Tg = 0;

float xc = 0;
float yc = 0;
float dr = 0;
float dl = 0;
void main(void) {
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F28M35x_SysCtrl.c file.
	InitSysCtrl();

#ifdef _FLASH
// Copy time critical code and Flash setup code to RAM
// This includes the following functions:  InitFlash();
// The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
// symbols are created by the linker. Refer to the device .cmd file.
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t) &RamfuncsLoadSize);

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
	InitFlash();
#endif

// Step 2. Initialize GPIO:
// This example function is found in the F28M35x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
	EALLOW;

// For this case only init GPIO for eQEP1 and ePWM1
// This function is found in F28M35x_EQep.c
//	InitEQep1Gpio();
//	InitEQep2Gpio();

	InitEPwm1Gpio();
	InitEPwm2Gpio();
	InitEPwm6Gpio();
	InitEPwm7Gpio();

	EALLOW;

//	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;
	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;

	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;


	GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;    // GPIO4 control motorA direction
	GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;  // Normally low
	GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;    // GPIO5 control motorB direction
	GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;  // Normally low

//	GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;    // GPIO8 servo on/off
//	GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;  // Normally low

//	GpioCtrlRegs.GPADIR.bit.GPIO6 = 0;   //grip input
	GpioCtrlRegs.GPADIR.bit.GPIO7 = 0;   //car state

	GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
	EDIS;

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
	DINT;

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F28M35x_PieCtrl.c file.
	InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F28M35x_DefaultIsr.c.
// This function is found in F28M35x_PieVect.c.
	InitPieVectTable();

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
	EALLOW;
	// This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.TINT1 = &cpu_timer1_isr;
	PieVectTable.TINT2 = &cpu_timer2_isr;
	EDIS;
	// This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
	initEpwm1();  // This function exists in Example_EPwmSetup.c
	initEpwm2();
	initEpwm6();
	initEpwm7();

	InitCpuTimers();

	// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	// C28_FREQ in MHz, 1 second Period (in uSeconds)

	ConfigCpuTimer(&CpuTimer0, C28_FREQ, 10000);   //T=0.01s
	ConfigCpuTimer(&CpuTimer1, C28_FREQ, 50000);   //T=0.05s
	ConfigCpuTimer(&CpuTimer2, C28_FREQ, 1000);    //T=0.001s

	CpuTimer0Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit
									// = 0
	CpuTimer1Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit
									// = 0
	CpuTimer2Regs.TCR.all = 0x4000; // Use write-only instruction to set TSS bit
									// = 0

// Step 5. User specific code, enable interrupts:
// Enable CPU INT1 which is connected to CPU-Timer 0:
	IER |= M_INT1;
	IER |= M_INT13;
	IER |= M_INT14;

// Enable TINT0 in the PIE: Group 3 interrupt 1
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

// Enable global Interrupts and higher priority real-time debug events:
	EINT;
	// Enable Global interrupt INTM
	ERTM;
	// Enable Global real-time interrupt DBGM

	controlmatrix[0][0] = rw/2;
	controlmatrix[0][1] = rw/2;
	controlmatrix[1][0] = -rw/R;
	controlmatrix[1][1] = rw/R;

	inv_controlmatrix[0][0] = 1/rw;
	inv_controlmatrix[0][1] = -R/(2*rw);
	inv_controlmatrix[1][0] = 1/rw;
	inv_controlmatrix[1][1] = R/(2*rw);

	//GpioDataRegs.GPASET.bit.GPIO22 = 1;
	GpioDataRegs.GPASET.bit.GPIO4 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;
	GpioDataRegs.GPASET.bit.GPIO22 = 1;




	for (;;) {


	}
}

__interrupt void cpu_timer0_isr(void)   //control loop interrupt
{
	//Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

__interrupt void cpu_timer1_isr(void)   //path interrupt
{
	//CpuTimer1.InterruptCount++;


//	 The CPU acknowledges the interrupt.
	EDIS;
}

__interrupt void cpu_timer2_isr(void) {	//car control
	EALLOW;

//	CpuTimer2.InterruptCount++;
//	Shared_Ram_dataRead_c28();
	if (radius > 0)
	{
		linetracking();
	}
	else if (Xg != 0)
	{
		InverseKinematic(0,0,0,Xg,Yg,Tg);
	}
	else
	{
		computespeed();
	}
	grip();
	// The CPU acknowledges the interrupt.



	EDIS;
}



void InverseKinematic(float xi ,float yi ,float thetai ,float xf ,float yf ,float thetaf )//theta 是弧度
{

    if ((thetaf - thetai) <= 0.002 && (thetai - thetaf) <= 0.002 )//theta of initial and final are very close
    {
        dr = sqrtf((xf - xi) * (xf * xi) + (yf - yi) * (yf - yi));
        dl = sqrtf((xf - xi) * (xf * xi) + (yf - yi) * (yf - yi));
    }
    else
    {
        xc = (xi * tanf(thetaf) - xf*tanf(thetai) + tanf(thetai) * tanf(thetaf) * (yi - yf))  / (tanf(thetaf) - tanf(thetai));
        yc = (xi + yi * tanf(thetai) - xf - yf * tanf(thetaf))/(tanf(thetai) - tanf(thetaf));
        dr = (thetaf - thetai) * (R / 2 + sqrtf((xc - xi)*(xc - xi) + (yc - yi)*(yc - yi)));
        dl = -(thetaf - thetai) * (R / 2 - sqrtf((xc - xi)*(xc - xi) + (yc - yi)*(yc - yi)));
    }
	if((dr + dl)/2 <= 1)
	{
		speed = (dr + dl)*(0.15)/2;
	}
	else
	{
		speed = 0.15;
	}

	if (dl < 0)
	{
		rotate = speed * 5 * ((dl - dr) * 2)/((dl + dr) * R);
	}
	else if (dl > 0)
	{
		rotate = speed * 5 * ((dr - dl) * 2)/((dl + dr) * R);
	}
	wl = inv_controlmatrix[0][0] * speed + inv_controlmatrix[0][1] * rotate;
	wr = inv_controlmatrix[1][0] * speed + inv_controlmatrix[1][1] * rotate;
	changeDuty1(100 - wl*6000/(250*2*PI) - f1);
	changeDuty2(wr*6000/(250*2*PI) + f2);

}

void grip()
{
    if (forward == 1)//positive voltage go forward
    {
    	changeDuty6(0);
    	changeDuty7(50);
    }
    else if (forward == 2)//negative voltage go backward
    {
    	changeDuty7(0);
    	changeDuty6(50);
    }
    else if (forward == 0)
    {
    	changeDuty6(0);
    	changeDuty7(0);
    }

}
/*void Shared_Ram_dataWrite_c28(int index)
{


    for(index = 0; index < 2048; index ++)
    {
        c28_r_w_array[index] = 70000+index;
    }
}
*/
void Shared_Ram_dataRead_c28()
{
   /* int index;

    for(index = 0; index < 2048; index ++)
    {
        c28_r_w_array[index] = c28_r_array[index];
    }*/
	v = c28_r_array[0];
	w = c28_r_array[1];
	l = c28_r_array[2];
	r = c28_r_array[3];
	forward = c28_r_array[4];
	theta = c28_r_array[5];
	radius = c28_r_array[6];
	Xg = c28_r_array[7];
	Yg = c28_r_array[8];
	Tg = c28_r_array[9];
	//left = c28_r_array[6];

}

void turn_left()
{
	changeDuty1(100);
	changeDuty2(l);//13.9
}

void turn_right()
{
	changeDuty2(0);
	changeDuty1(100-r);//9.7
}
void computespeed()
{
	if(l==0&&r==0)
	{
		wl = inv_controlmatrix[0][0]*v + inv_controlmatrix[0][1]* w;
		wr = inv_controlmatrix[1][0]*v + inv_controlmatrix[1][1]* w;
		changeDuty1(100 - wl*6000/(250*2*PI) - f1);
		changeDuty2(wr*6000/(250*2*PI) + f2);
	}
	else if (l>0)
	{
		turn_left();
	}
	else if(r>0)
	{
		turn_right();
	}
}
void linetracking()
{

	if(radius <= 50)
	{
		rotate = 0;
		wl = inv_controlmatrix[0][0] * speed + inv_controlmatrix[0][1] * rotate;
		wr = inv_controlmatrix[1][0] * speed + inv_controlmatrix[1][1] * rotate;
		changeDuty1(100 - wl*6000/(250*2*PI) - f1);
		changeDuty2(wr*6000/(250*2*PI) + f2);
	}
	else
	{
//		rotate = theta * K;
		if (theta > 1.57)
		{
			rotate = -(3 * ( PI - theta ) + radius / 20) * K;
		}
		else if (theta > 0)
		{
			rotate = -(3 * ( PI / 2 - theta ) + radius / 20) * K;
		}
		else if (theta < -1.57)
		{
			rotate = (-3 * ( - PI - theta ) + radius / 20) * K;
		}
		else if (theta < 0)
		{
			rotate = (-3 * ( - PI / 2 - theta ) + radius / 20) * K;
		}
		wl = inv_controlmatrix[0][0] * speed + inv_controlmatrix[0][1] * rotate;
		wr = inv_controlmatrix[1][0] * speed + inv_controlmatrix[1][1] * rotate;
		changeDuty1(100 - wl*6000/(250*2*PI) - f1);
		changeDuty2(wr*6000/(250*2*PI) + f2);
	}
}


























