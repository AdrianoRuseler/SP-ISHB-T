/*!
* \file system.c
* \brief Source file for System Level Functions 
* \details
* \author Adriano Ruseler
* \version 1.0
* \copyright The MIT License (MIT).
*/




/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__dsPIC33E__)
	#include <p33Exxxx.h>
#elif defined(__dsPIC33F__)
	#include <p33Fxxxx.h>
#endif

#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include <libq.h>            /* include fixed point library */
#include <limits.h>         /* IMPLEMENTATION-DEFINED LIMITS */

#include "system.h"          /* variables/params used by system.c             */

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c.                                          */
/*                                                                            */
/******************************************************************************/

/* Refer to the device Family Reference Manual Oscillator section for
information about available oscillator configurations.  Typically
this would involve configuring the oscillator tuning register or clock
switching useing the compiler's __builtin_write_OSCCON functions.
Refer to the C Compiler for PIC24 MCUs and dsPIC DSCs User Guide in the
compiler installation directory /doc folder for documentation on the
__builtin functions.*/

/* TODO Add clock switching code if appropriate.  An example stub is below.   */
void ConfigureOscillator(void)
{


        /* Disable Watch Dog Timer */
        RCONbits.SWDTEN = 0;


         //The settings below set up the oscillator and PLL for 70 MIPS as
    //follows:
    //            Crystal Frequency  * (DIVISOR+2)
    // Fcy =     ---------------------------------
    //              PLLPOST * (PRESCLR+2) * 4
	// Crystal  = 20 MHz
	// Fosc		= 140 MHz
	// Fcy		= 70 MIPS

	PLLFBD =  42;		        // M=42
	CLKDIVbits.PLLPOST = 0;		// N1=2
	CLKDIVbits.PLLPRE = 1;		// N2=3
        CLKDIVbits.DOZEN = 0;            //  000 = FCY divided by 1
        CLKDIVbits.ROI = 1;   //  Interrupts will clear the DOZEN bit and the
                              //  processor clock and peripheral clock ratio is set to 1:1

        
   //     OSCTUN=0;		       // Tune FRC oscillator, if FRC is used




        /* When clock switch occurs switch to Primary Osc (HS, XT, EC) */
        // 011 = Primary Oscillator (XT, HS, EC) with PLL
        __builtin_write_OSCCONH(0x03);  /* Set OSCCONH for clock switch */
        // 1 = Request oscillator switch to selection specified by NOSC<2:0> bits
        __builtin_write_OSCCONL(0x01);  /* Start clock switching */

        
        while(OSCCONbits.COSC != 0b011);
        /* Wait for Clock switch to occur */
        /* Wait for PLL to lock, only if PLL is needed */
         while(OSCCONbits.LOCK != 1); 


//     REFOCON: REFERENCE OSCILLATOR CONTROL REGISTER

//     ROON: Reference Oscillator Output Enable bit
  REFOCONbits.ROON =0; // Reference oscillator output disabled
  REFOCONbits.ROSSLP =1; // 1 = Reference oscillator output continues to run in Sleep

//     ROSEL: Reference Oscillator Source Select bit
//1 = Oscillator crystal used as the reference clock
//0 = System clock used as the reference clock
//
  REFOCONbits.ROSEL = 0;
//RODIV<3:0>: Reference Oscillator Divider bits(1)
//1111 = Reference clock divided by 32,768
//1110 = Reference clock divided by 16,384
//1101 = Reference clock divided by 8,192
//1100 = Reference clock divided by 4,096
//1011 = Reference clock divided by 2,048
//1010 = Reference clock divided by 1,024
//1001 = Reference clock divided by 512
//1000 = Reference clock divided by 256
//0111 = Reference clock divided by 128
//0110 = Reference clock divided by 64
//0101 = Reference clock divided by 32
//0100 = Reference clock divided by 16
//0011 = Reference clock divided by 8
//0010 = Reference clock divided by 4
//0001 = Reference clock divided by 2
//0000 = Reference clock
 REFOCONbits.RODIV=0b0000;
 
 REFOCONbits.ROON =1; // Reference oscillator output enabled
 

}

