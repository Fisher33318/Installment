//###########################################################################
//
// FILE:    Example_posspeed.h
//
// TITLE:   Pos/speed measurement using EQEP peripheral
//
// DESCRIPTION:
//
// Header file containing data type and object definitions and
// initializers.
//
//###########################################################################
// $TI Release: F28M35x Support Library v207 $
// $Release Date: Mon Sep 21 16:49:54 CDT 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __POSSPEED__
#define __POSSPEED__

#include "DSP28x_Project.h"
#include "IQmathLib.h"         // Include header for IQmath library
/*-----------------------------------------------------------------------------
Define the structure of the POSSPEED Object
-----------------------------------------------------------------------------*/
typedef struct {int theta_elec;         // Output: Motor Electrical angle (Q15)
                int theta_mech;         // Output: Motor Mechanical Angle (Q15)
                int DirectionQep;       // Output: Motor rotation direction (Q0)
                int QEP_cnt_idx;        // Variable: Encoder counter index (Q0)
                int theta_raw;          // Variable: Raw angle from Timer 2 (Q0)
                unsigned int mech_scaler;        // Parameter: 0.9999/total count, total count = 4000 (Q26)
                int pole_pairs;         // Parameter: Number of pole pairs (Q0)
                int cal_angle;          // Parameter: Raw angular offset between encoder and phase a (Q0)
                int index_sync_flag;    // Output: Index sync status (Q0)

                Uint32 SpeedScaler;     // Parameter :  Scaler converting 1/N cycles to a GLOBAL_Q speed (Q0) - independently with global Q
                _iq Speed_pr;           // Output :  speed in per-unit
                Uint32 BaseRpm;         // Parameter : Scaler converting GLOBAL_Q speed to rpm (Q0) speed - independently with global Q
                int32 SpeedRpm_pr;      // Output : speed in r.p.m. (Q0) - independently with global Q

                _iq  oldpos;            // Input: Electrical angle (pu)
                _iq Speed_fr;           // Output :  speed in per-unit
                int32 SpeedRpm_fr;      // Output : Speed in rpm  (Q0) - independently with global Q
                void (*init)();         // Pointer to the init function
                void (*calc)();         // Pointer to the calc function
                } POSSPEED;

/*-----------------------------------------------------------------------------
Define a POSSPEED_handle
-----------------------------------------------------------------------------*/
typedef POSSPEED *POSSPEED_handle;

/*-----------------------------------------------------------------------------
Default initializer for the POSSPEED Object.//mech_scalar 4000=>16776  2000=>33554
-----------------------------------------------------------------------------*/
#define POSSPEED_DEFAULTS_1 {0x0, 0x0,0x0,0x0,0x0,33554,2,0,0x0,\
      188,0,6000,0,\
      0,0,0,\
      (void (*)(long))POSSPEED_Init_1,\
      (void (*)(long))POSSPEED_Calc_1 }

#define POSSPEED_DEFAULTS_2 {0x0, 0x0,0x0,0x0,0x0,33554,2,0,0x0,\
      188,0,6000,0,\
      0,0,0,\
      (void (*)(long))POSSPEED_Init_2,\
      (void (*)(long))POSSPEED_Calc_2 }

#define POSSPEED_DEFAULTS_3 {0x0, 0x0,0x0,0x0,0x0,16776,2,0,0x0,\
      94,0,6000,0,\
      0,0,0,\
      (void (*)(long))POSSPEED_Init_3,\
      (void (*)(long))POSSPEED_Calc_3 }

/*-----------------------------------------------------------------------------
Prototypes for the functions in posspeed.c
-----------------------------------------------------------------------------*/
void POSSPEED_Init_1(void);
void POSSPEED_Calc_1(POSSPEED_handle);
void POSSPEED_Init_2(void);
void POSSPEED_Calc_2(POSSPEED_handle);
void POSSPEED_Init_3(void);
void POSSPEED_Calc_3(POSSPEED_handle);
#endif /*  __POSSPEED__ */




