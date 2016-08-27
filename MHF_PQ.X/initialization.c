
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

#include "initialization.h"  /* User funct/params, such as InitApp            */
#include "eCAN.h"

//C:\Program Files (x86)\Microchip\xc16\v1.10\src\peripheral_30F_24H_33F\src


#if defined(__TESTBOARD__)
/* Configuração de pinos para o Test Board */
/* RA4 -> LED3 */
/* RB0 -> AN2 */
/* RB1 -> LED1 */
/* RB4 -> LED2 */

#elif defined(__PROTOTIPO__)
/* Configuração de pinos para o prototipo */
/* RA4 -> RESET */
/* RB0 -> LED1  */
/* RB1 -> AN3 */
/* RB4 -> LED2 */

#endif


#if defined(__MASTER__)
/* Configuração para operação como Mestre */

#elif defined(__SLAVE__)
/* Configuração para Operação como Escravo */

#endif


/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

extern int ADC1BufferA;
extern int ADC1BufferB;


void Delayus(unsigned int delay)
{
    int i;
    for (i = 0; i < delay; i++)
    {
    __asm__ volatile ("repeat #39");
    __asm__ volatile ("nop");
    }
}

void InitIO(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */

    /* Setup analog functionality and port direction */

    // Unlock Registers
//*************************************************************
__builtin_write_OSCCONL(OSCCON & ~(1<<6));
/*  bit 6 IOLOCK: I/O Lock Enable bit
        1 = I/O Lock is active
        0 = I/O Lock is not active   */

  //  ANSELA = 0xffff;

     ANSELAbits.ANSA0 = 1;  // AN0
     ANSELAbits.ANSA1 = 1;  // AN1
     ANSELAbits.ANSA4 = 0; // LED 3 ou RESET

    // ANSELA = 0xfff0;

     ANSELBbits.ANSB2 = 1; // PGC
     ANSELBbits.ANSB3 = 1; // PGD
     ANSELBbits.ANSB8 = 1; // Int 1
 //    ANSELB = 0xffff;

     
#if defined(__INV01__)

     #elif defined(__INV02__)

     #elif defined(__INV03__)

#endif

     
#if defined(__TESTBOARD__)
/* Configuração de pinos para o Test Board */
/* RA4 -> LED3 */
/* RB0 -> AN2 */
/* RB1 -> LED1 */
/* RB4 -> LED4 */

         // Set pins to Analog -> 1 or Digital -> 0;
     ANSELBbits.ANSB0 = 1; // AN2
     ANSELBbits.ANSB1 = 0; // LED 1

     TRISAbits.TRISA4 = 0; // Set pin as Output -> LED 3
     TRISBbits.TRISB1 = 0; // Set pin as Output -> LED 1
     TRISBbits.TRISB4 = 0; // Set pin as Output -> LED 2

#elif defined(__PROTOTIPO__)
/* Configuração de pinos para o prototipo */
/* RA4 -> RESET */
/* RB0 -> LED1  */
/* RB1 -> AN3 */
/* RB4 -> LED4 */

      // Set pins to Analog -> 1 or Digital -> 0;
     ANSELBbits.ANSB0 = 0; // LED1
     ANSELBbits.ANSB1 = 1; // AN3

     TRISAbits.TRISA4 = 0; // Set pin as Output -> RESET
     TRISBbits.TRISB0 = 0; // Set pin as Output -> LED 1
     TRISBbits.TRISB4 = 0; // Set pin as Output -> LED 2

#endif


     TRISBbits.TRISB15 = 0; // Set pin as Output -> PWM1L
     TRISBbits.TRISB14 = 0; // Set pin as Output -> PWM1H
     TRISBbits.TRISB13 = 0; // Set pin as Output -> PWM2L
     TRISBbits.TRISB12 = 0; // Set pin as Output -> PWM2H
     TRISBbits.TRISB11 = 0; // Set pin as Output -> SYNCO1

  //   TRISBbits.TRISB10 = 1; // Set pin as Imput -> SYNCI1

     TRISBbits.TRISB10 = 0; // Set pin as OutPut -> REFCLKO



    RPINR0 = 0;  // Empty register -> Taken from code example
    // Assign External Interrupt 1 (INT1) to the Corresponding RPn Pin bits
    RPINR0bits.INT1R = 0b0101000; // RP40

    RPINR1 = 0;  // Empty register -> Taken from code example
    // Assign External Interrupt 2 (INT2) to the Corresponding RPn Pin bits
    RPINR1bits.INT2R = 0b0101001; // RP41

    // Assign CAN1 RX Input (CRX1) to the Corresponding RPn Pin bits
    RPINR26bits.C1RXR = 0b0100101;  // RP37

    // Peripheral Output Function is Assigned to RP38 Output Pin bits
    RPOR2bits.RP38R = 0b001110; // RPn tied to CAN1 transmit



#ifdef __dsPIC33EP64GP502__
/* For GP dsPIC Only*/

  //Peripheral Output Function is Assigned to RP43 Output Pin bits
  // SYNCO1(1) 101101 RPn tied to PWM primary time base sync output
     RPOR4bits.RP43R = 0b101101; //  RPn tied to PWM primary time base sync output

#endif


#ifdef __dsPIC33EP64MC502__
/* For MC dsPIC Only*/
    // Assign PWM Synchronization Input 1 to the Corresponding RPn Pin bits
   //  RPINR37bits.SYNCI1R = 0b0101010; // RP42

//     REFCLKO O ? Yes Reference clock output.
// REFCLKO 110001 RPn tied to Reference Clock output
     RPOR4bits.RP42R = 0b110001; //

     
  //Peripheral Output Function is Assigned to RP43 Output Pin bits
  // SYNCO1(1) 101101 RPn tied to PWM primary time base sync output
     RPOR4bits.RP43R = 0b101101; //  RPn tied to PWM primary time base sync output

#endif




     TRISBbits.TRISB5 = 1; // Set pin as Imput -> Assign CAN1 RX Input (CRX1)
     TRISBbits.TRISB6 = 0; // Set pin as OutPut -> CAN1 transmit (C1TX)





     // Lock Registers
//*************************************************************
__builtin_write_OSCCONL(OSCCON | (1<<6));



  /* Initialize peripherals */

#if defined(__TESTBOARD__)
/* Configuração de pinos para o Test Board */
/* RA4 -> LED3 */
/* RB0 -> AN2 */
/* RB1 -> LED1 */
/* RB4 -> LED2 */

PORTAbits.RA4 = 1; // Led 3
PORTBbits.RB1 = 1; // Led 1
PORTBbits.RB4 = 1; // Led 2


LATBbits.LATB1 = 1; // Led 1
LATBbits.LATB4 = 1; // Led 2
LATAbits.LATA4 = 1; // Led 3

#elif defined(__PROTOTIPO__)
/* Configuração de pinos para o prototipo */
/* RA4 -> RESET */
/* RB0 -> LED1  */
/* RB1 -> AN3 */
/* RB4 -> LED2 */
/*Initialize the Ports */

PORTAbits.RA4 = 1; // RESET
PORTBbits.RB0 = 0; // Led 1
PORTBbits.RB4 = 0; // Led 2


LATBbits.LATB0 = 0;  // led 1
LATBbits.LATB4 = 0; // Led 2
LATAbits.LATA4 = 1; //RESET


#endif

}



void INTx_IO_Init(void)
{

        INTCON1bits.NSTDIS = 1; // Interrupt nesting is disabled
    //    INTCON2 = 0x800F;       /*Setup INT1, INT2, INT3 & INT4 pins to interupt */
                                /*on falling edge and set up INT0 pin to interupt */
                                /*on rising edge */

        INTCON2bits.GIE = 0;  //  Global Interrupt Enable bit
        INTCON2bits.DISI = 0; // DISI Instruction Status bit
        INTCON2bits.SWTRAP = 0;  //  Software Trap Status bit

        INTCON2bits.INT0EP = 1; // External Interrupt 0 Edge Detect Polarity Select bit
        INTCON2bits.INT1EP = 1; // External Interrupt 1 Edge Detect Polarity Select bit
        INTCON2bits.INT2EP = 0; // External Interrupt 2 Edge Detect Polarity Select bit

        IFS0bits.INT0IF = 0;    /*Reset INT0 interrupt flag */
        IEC0bits.INT0IE = 1;    /*Enable INT0 Interrupt Service Routine */
        IPC0bits.INT0IP = 1; // Interrupt is priority 1

        IFS1bits.INT1IF = 0;    /*Reset INT1 interrupt flag */
        IEC1bits.INT1IE = 1;    /*Enable INT1 Interrupt Service Routine */
        IPC5bits.INT1IP = 1; // Interrupt is priority 1

        IFS1bits.INT2IF = 0;    /*Reset INT2 interrupt flag */
        IEC1bits.INT2IE = 1;    /*Enable INT2 Interrupt Service Routine */
        IPC7bits.INT2IP = 1; // Interrupt is priority 1



   //     INTCON2bits.GIE = 0;  //  Global Interrupt Enable bit
        INTCON2bits.GIE = 1;  //  Global Interrupt Enable bit

}




void initTMR1(void)
{
        T1CON = 0;              // Stop and Timer 1 reset

 	IFS0bits.T1IF = 0;      // Reset Timer1 interrupt flag
	IPC0bits.T1IP = 6;      // Timer1 Interrupt priority level=4
 	IEC0bits.T1IE = 1;      // Enable Timer1 interrupt

        T1CONbits.TCS = 0; // Internal clock (FP)@ 70 MHz
        T1CONbits.TCKPS = 0;  // Timer1 Input Clock Prescale Select bits (1:1)
        // Se tudo estiver certo, estamos em 70 MHz

        T1CONbits.TGATE = 0;  // Gated time accumulation disabled
        T1CONbits.TSIDL = 0;  // Continue module operation in Idle mode
 	TMR1=  0x0000;
	PR1 = 1750;           // Timer1 period register = 1750 -> 40kHz
	T1CONbits.TON = 1;      // Enable Timer1 and start the counter
}


/*=============================================================================
Timer 3 is setup to time-out every 0.25 us (40Khz Rate). As a result, the module
will stop sampling and trigger a conversion on every Timer3 time-out, i.e., Ts=0.25us.
At that time, the conversion process starts and completes Tc=14*Tad periods later.

When the conversion completes, the module starts sampling again. However, since Timer3
is already on and counting, about (Ts-Tc)us later, Timer3 will expire again and trigger
next conversion.
=============================================================================*/
void initTMR3(void)
{
        T2CON = 0x0000; // put in known state
        T3CON = 0x0000; // put in known state
         //Stop Timer 3
        T3CONbits.TON = 0;
        T2CONbits.T32 = 0; // Timerx and Timery act as two 16-bit timers
        T3CONbits.TCS = 0; // Internal clock (FP) -> 70 MHz
        T3CONbits.TCKPS = 0; // Timer3 Input Clock Prescale Select bits(1) 1:1
   
        TMR3 = 0x0000;  // Initialize timer counter
    
#if defined(__INV01__)
 //   PR3 = 1832;  // Timer3 period register = 1750 -> 40kHz
        PR3 = 610; // Frequencia estava em 13.3kHz
     #elif defined(__INV02__)
  // Fosc ~= 73.4 MHz
   //     PR3 = 1832;  // Timer3 period register = 1832 -> 40kHz
         PR3 = 610; // Frequencia estava em 13.3kHz
     #elif defined(__INV03__)
 // PR3 = 1832;  // Timer3 period register = 1750 -> 40kHz
          PR3 = 610; // Frequencia estava em 13.3kHz
#endif
       
        INTCON2bits.GIE = 0;  //  Global Interrupt Enable bit

        IPC2bits.T3IP = 0x06; // Set Timer3 Interrupt Priority Level

        IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
        IEC0bits.T3IE = 1; // 1 = Interrupt request is enabled

         INTCON2bits.GIE = 1;  //  Global Interrupt Enable bit
        //Start Timer 3
        T3CONbits.TON = 1;
}



void initPWM_Slave(void)
{

    PTCON =  0x0000;   /* Desable the PWM Module  - known state */

    /* ~~~~~~~~~~~~~~~~~~~~~~ PWM1 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* PWM1 I/O Control Register register */
    IOCON1 = 0x0000;  /* Put register in known state */
    IOCON1bits.PENH = 1;   /* PWM1H is controlled by PWM module */
    IOCON1bits.PENL = 1;   /* PWM1L is controlled by PWM module */
    IOCON1bits.PMOD = 0;   /* PWM I/O pin pair is in the Complementary Output mode */
    IOCON1bits.POLH = 0;   /* PWMxH pin is active-low */
    IOCON1bits.POLL = 0;   /* PWMxL pin is active-low */
    IOCON1bits.OVRENH = 0; /* PWM generator controls PWMxH pin */
    IOCON1bits.OVRENL = 0; /* PWM generator controls PWMxL pin */

    
    /* PWMCONx: PWM CONTROL REGISTER */
    PWMCON1 = 0x0000;  /* Put register in known state */
    PWMCON1bits.FLTIEN = 0; /* Fault interrupt is disabled and FLTSTAT bit is cleared */
    PWMCON1bits.CLIEN = 0; /* Current-limit interrupt disabled and CLSTAT bit is cleared */
    PWMCON1bits.TRGIEN = 0; /* Trigger event interrupts are disabled and TRGSTAT bit is cleared */
    PWMCON1bits.ITB = 0; /* PTPER register provides timing for this PWM generator */
    PWMCON1bits.MDCS = 1; /* MDC register provides duty cycle information for this PWM generator */
    PWMCON1bits.DTC = 0; /* Negative dead time actively applied for Complementary Output mode */
    PWMCON1bits.MTBS = 0; /* PWM generator uses the primary master time base for 
                           synchronization and as the clock source for the PWM generation logic */
    PWMCON1bits.CAM = 0; /* Edge-Aligned mode is enabled */
    PWMCON1bits.XPRES = 0; /* External pins do not affect PWM time base */
    PWMCON1bits.IUE = 0; /* Updates to the active MDC/PDCx/DTx/ALTDTRx/PHASEx
                          registers are synchronized to the PWM time base */
    FCLCON1 = 0x0003;  /* Fault Mode bits for PWM Generator # Fault input is disabled */

    /* Load PDTR1 and ALTDTR2 register with preset dead time value */
    DTR1    = 70;               /* Deadtime setting */
                                /* Deadtime = DTR1*7.1428nsec = 500nsec */
    ALTDTR1 = 70;               /* Deadtime setting */
                                /* Deadtime = ALTDTR1*7.1428nsec = 500nsec */
#ifdef __INV01__
    /* PHASE1 register */
    PHASE1 = PHASE1inv01;                 /* 0 Graus  */
#endif
#ifdef __INV02__
    /* PHASE1 register */
    PHASE1 = PHASE1inv02;                 /* 60 Graus  */
#endif
#ifdef __INV03__
    /* PHASE1 register */
    PHASE1 = PHASE1inv03;                 /* 120 Graus  */
#endif

  //  PDC1 = 5250; /* Master Duty Cycle is 75% of the period */

    

    /* ~~~~~~~~~~~~~~~~~~~~~~ PWM2 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     /* PWM2 I/O Control Register register */
    IOCON2 = 0x0000;  /* Put register in known state */
    IOCON2bits.PENH = 1;   /* PWM1H is controlled by PWM module */
    IOCON2bits.PENL = 1;   /* PWM1L is controlled by PWM module */
    IOCON2bits.PMOD = 0;   /* PWM I/O pin pair is in the Complementary Output mode */
    IOCON2bits.POLH = 1;   /* PWMxH pin is active-high */
    IOCON2bits.POLL = 1;   /* PWMxL pin is active-high */
    IOCON2bits.OVRENH = 0; /* PWM generator controls PWMxH pin */
    IOCON2bits.OVRENL = 0; /* PWM generator controls PWMxL pin */

    /* PWMCONx: PWM CONTROL REGISTER */
    PWMCON2 = 0x0000;  /* Put register in known state */
    PWMCON2bits.FLTIEN = 0; /* Fault interrupt is disabled and FLTSTAT bit is cleared */
    PWMCON2bits.CLIEN = 0; /* Current-limit interrupt disabled and CLSTAT bit is cleared */
    PWMCON2bits.TRGIEN = 0; /* Trigger event interrupts are disabled and TRGSTAT bit is cleared */
    PWMCON2bits.ITB = 0; /* PTPER register provides timing for this PWM generator */
    PWMCON2bits.MDCS = 1; /* MDC register provides duty cycle information for this PWM generator */
    PWMCON2bits.DTC = 1; /* Positive dead time actively applied for all output modes */
    PWMCON2bits.MTBS = 0; /* PWM generator uses the primary master time base for
                           synchronization and as the clock source for the PWM generation logic */
    PWMCON2bits.CAM = 0; /* Edge-Aligned mode is enabled */
    PWMCON2bits.XPRES = 0; /* External pins do not affect PWM time base */
    PWMCON2bits.IUE = 0; /* Updates to the active MDC/PDCx/DTx/ALTDTRx/PHASEx
                          registers are synchronized to the PWM time base */
    FCLCON2 = 0x0003;  /* Fault Mode bits for PWM Generator # Fault input is disabled */

     /* Load PDTR2 and ALTDTR2 register with preset dead time value */
    DTR2    = 70;               /* Deadtime setting */
                                /* Deadtime = DTR2*7.1428nsec = 500nsec */
    ALTDTR2 = 70;               /* Deadtime setting */
                                /* Deadtime = ALTDTR2*7.1428nsec = 500nsec */


#ifdef __INV01__
    /* PHASE2 register */
    PHASE2 = PHASE2inv01;              /*180 + 0 (graus) */
#endif
#ifdef __INV02__
    /* PHASE2 register */
    PHASE2 = PHASE2inv02;               /*180 + 60 (graus) */
#endif
#ifdef __INV03__
    /* PHASE2 register */
    PHASE2 = PHASE2inv03;               /*180 + 120 (graus) */
#endif


     /* PDCx: PWM GENERATOR DUTY CYCLE REGISTER */
  //  PDC2 = 5250; /* PDC2 = PTPER - MDC */


 /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

      /* PWM3 I/O Control Register register */
    IOCON3 = 0x0000;  /* Put register in known state */
    /* PWMCONx: PWM CONTROL REGISTER */
    PWMCON3 = 0x0000;  /* Put register in known state */

    FCLCON3 = 0x0003;  /* Fault Mode bits for PWM Generator # Fault input is disabled */
    
 /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    /* When using the SYNCI1 feature, the user application must program
     * the period register with a value that is slightly larger than the
     * expected period of the external synchronization input signal. */
    
    /* Primary Master Time Base (PMTMR) Period Value bits */
    /* Fosc ~= 146,8MHz  */
    PTPER = PTPERval+1;               /* Fosc = 140 MHz; PWM Input Clock Prescaler = 1  */
                                /* Period = PTPER*7.1428nsec = 50us / 20kHz */

 //   MDC = 5250; /* Master Duty Cycle is 75% of the period */
    
    /* PWM Time Base Control Register */   
    PTCONbits.PTSIDL = 0; /* PWM time base runs in CPU Idle mode */
    PTCONbits.SEIEN = 0; /* Special Event Interrupt is disabled */
    PTCONbits.EIPU = 0; /* Active Period register updates occur on PWM cycle boundaries */
    PTCONbits.SYNCPOL = 0; /* SYNCI1/SYNCO1 is active-high */
    PTCONbits.SYNCOEN = 1; /* SYNCO1 output is disabled */
    PTCONbits.SYNCEN = 1; /* External synchronization of primary time base is enabled */
  //  PTCONbits.SYNCSRC = 0; /* Synchronous Source Selection bits -> SYNCI 1 input from PPS  */
    PTCONbits.SYNCSRC = 2; /* Synchronous Source Selection bits -> PTGO16 */
    
    PTCONbits.SEVTPS = 0; /* 1:1 Postscaler generates Special Event Trigger on every compare match event */

    PTCON2bits.PCLKDIV = PCLKDIVval; /* Divide by 1, maximum PWM timing resolution (power-on default) */

    PTCONbits.PTEN = 1;   /* Enable the PWM Module */
}



void initPTG(void)
{

/*    PTGO16 PWM Time Base Synchronous Source for PWM(1)
      PTGO17 PWM Time Base Synchronous Source for PWM(1)  */


 /* PTGCST: PTG CONTROL/STATUS REGISTER */
    PTGCST = 0x0000; /* Put in known state */
 
/* PTGCON: PTG Control Register */
   PTGCON = 0x0000; /* Put in known state */



   /*  PTGBTE: PTG BROADCAST TRIGGER ENABLE REGISTER  */
   /* This register is read only when the PTG module is
    *  executing step commands (PTGEN = 1 and PTGSTRT = 1). */

     _STEP0 = PTGWHI | (0xF); // INT2 External Interrupt
     _STEP1 = PTGTRIG | (0x10); // Trigger PTGO16 PWM Time Base Synchronous Source for PWM
     _STEP2 = PTGJMP | (0x0); // Jump to _STEP0}

  PTGCSTbits.PTGEN = 1; /* PTG module is enabled */
  PTGCSTbits.PTGSTRT = 1; /* Start to sequentially execute commands (Continuous mode) */

}



/*=============================================================================
ADC Initialization
 * AN0 -> Sensor de Tensão
 * AN1 -> Sensor de Corrente
 * AN3 -> Duty Cycle / VREF+
 *
=============================================================================*/
void initADC1(void)
{

      AD1CON1bits.ADON = 0;		// Turn on the A/D converter


                AD1CON1bits.SSRC   = 2;		// Sample Clock Source: GP Timer starts conversion
                AD1CON1bits.SSRCG = 0;          //  Timer3 compare ends sampling and starts conversion


		AD1CON1bits.ASAM   = 1;		// ADC Sample Control: Sampling begins immediately after conversion
		AD1CON1bits.AD12B  = 0;		// 10-bit, 4-channel ADC operation


/*DMA buffers are written in the order of conversion. The module provides an address to the DMA
channel that is the same as the address used for the non-DMA stand-alone buffer. */
		AD1CON1bits.ADDMABM = 1; 	// DMA buffers are built in conversion order mode


//    11 = Signed fractional
//    10 = Fractional
//    01 = Signed integer
//    00 = Integer
#if defined __MASTER__
		AD1CON1bits.FORM   = 3;		// Data Output Format: Signed Fraction (Q15 format)
#elif defined __SLAVE__
          AD1CON1bits.FORM   = 0;		// Data Output Format:
#endif
          
	

 //  AD1CON2: ADC1 CONTROL REGISTER 2
                AD1CON2bits.VCFG = 000;   /*  VREFH = AVDD;  VREFL = Avss*/
                AD1CON2bits.CSCNA = 1;   /* Scan inputs for CH0+ during Sample MUXA */

//   1 = Uses channel input selects for Sample MUXA on first sample and Sample MUXB on next sample
//   0 = Always uses channel input selects for Sample MUXA
		AD1CON2bits.ALTS = 0; // Always uses channel input selects for Sample MUXA
		AD1CON2bits.CHPS = 0; // Converts CH0
                AD1CON2bits.BUFM = 0; //  Always starts filling the buffer from the start address.
                AD1CON2bits.SMPI  =2; // Generates interrupt after completion of every 2nd sample/conversion operation


//  AD1CON3: ADC1 CONTROL REGISTER 3
		AD1CON3bits.ADRC = 0;		// ADC Clock is derived from Systems Clock
		AD1CON3bits.ADCS = 10;		// ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/70M)*1 = 14.28ns
									// ADC Conversion Time for 10-bit Tc=12*Tab = 0.171us

	//  AD1CON4: ADC1 CONTROL REGISTER 4
		AD1CON4bits.ADDMAEN = 0;	// Conversion results stored in ADC1BUF0 through ADC1BUFF registers; DMA will not be used
                AD1CON4bits.DMABL   = 0;	// Allocates 1 word of buffer to each analog input

       // AD1CHS123: ADC1 INPUT CHANNEL 1, 2, 3 SELECT REGISTER
         AD1CHS123 = 0x0000;


         /*AD1CHS0: ADC1 INPUT CHANNEL 0 SELECT REGISTER*/
          AD1CHS0 = 0x0000;

          AD1CSSH = 0x0000; /* AD1CSSH: ADC1 INPUT SCAN SELECT REGISTER HIGH(1) */
          AD1CSSL = 0x0000; /* ADC1 INPUT SCAN SELECT REGISTER LOW(1,2) */

          AD1CSSLbits.CSS0 = 1;  // Enable AN0 for channel scan
          AD1CSSLbits.CSS1 = 1;  // Enable AN1 for channel scan



#if defined(__TESTBOARD__)
/* Configuração de pinos para o Test Board */
/* RA4 -> LED3 */
/* RB0 -> AN2 */
/* RB1 -> LED1 */
/* RB4 -> LED4 */

     AD1CSSLbits.CSS2 = 1;  // Enable AN3 for channel scan

#elif defined(__PROTOTIPO__)
/* Configuração de pinos para o prototipo */
/* RA4 -> RESET */
/* RB0 -> LED1  */
/* RB1 -> AN3 */
/* RB4 -> LED4 */
  AD1CSSLbits.CSS3 = 1;  // Enable AN3 for channel scan

#endif
          
        

          INTCON2bits.GIE = 0;  //  Global Interrupt Enable bit

        IFS0bits.AD1IF = 0;			// Clear the A/D interrupt flag bit
        IEC0bits.AD1IE = 1;			// Do Not Enable A/D interrupt

        INTCON2bits.GIE = 1;  //  Global Interrupt Enable bit

        AD1CON1bits.ADON = 1;		// Turn on the A/D converter
}



/*=============================================================================
ADC Initialization
 * AN0 -> Sensor de Tensão
 * AN1 -> Sensor de Corrente
 * AN3 -> Duty Cycle / VREF+
 *
=============================================================================*/
void initADC1DMAch2(void)
{

      AD1CON1bits.ADON = 0;		// Turn on the A/D converter

		AD1CON1bits.FORM   = 3;		// Data Output Format: Signed Fraction (Q15 format)
                AD1CON1bits.SSRC   = 2;		// Sample Clock Source: GP Timer starts conversion
                AD1CON1bits.SSRCG = 0;          //  Timer3 compare ends sampling and starts conversion


		AD1CON1bits.ASAM   = 1;		// ADC Sample Control: Sampling begins immediately after conversion
		AD1CON1bits.AD12B  = 0;		// 10-bit, 4-channel ADC operation

                AD1CON1bits.SIMSAM = 0;     // Samples individual channels sequentially


/*DMA buffers are written in the order of conversion. The module provides an address to the DMA
channel that is the same as the address used for the non-DMA stand-alone buffer. */
		AD1CON1bits.ADDMABM = 1; 	// DMA buffers are built in conversion order mode


 //  AD1CON2: ADC1 CONTROL REGISTER 2
                AD1CON2bits.VCFG = 000;   /*  VREFH = AVDD;  VREFL = Avss*/
                AD1CON2bits.CSCNA = 1;   /* Scan inputs for CH0+ during Sample MUXA */

//   1 = Uses channel input selects for Sample MUXA on first sample and Sample MUXB on next sample
//   0 = Always uses channel input selects for Sample MUXA
		AD1CON2bits.ALTS = 0; // Always uses channel input selects for Sample MUXA
		AD1CON2bits.CHPS = 0; // Converts CH0
                AD1CON2bits.BUFM = 0; //  Always starts filling the buffer from the start address.
                AD1CON2bits.SMPI  =0; // Generates interrupt after completion of every sample/conversion operation


//  AD1CON3: ADC1 CONTROL REGISTER 3
		AD1CON3bits.ADRC = 0;		// ADC Clock is derived from Systems Clock
		AD1CON3bits.ADCS = 0;		// ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/70M)*1 = 14.28ns
									// ADC Conversion Time for 10-bit Tc=12*Tab = 0.171us

	//  AD1CON4: ADC1 CONTROL REGISTER 4
		AD1CON4bits.ADDMAEN = 1;	// Conversion results stored in ADC1BUF0 register, for transfer to RAM using DMA
                AD1CON4bits.DMABL   = 0;	// Allocates 1 word of buffer to each analog input

       // AD1CHS123: ADC1 INPUT CHANNEL 1, 2, 3 SELECT REGISTER
         AD1CHS123 = 0x0000;


         /*AD1CHS0: ADC1 INPUT CHANNEL 0 SELECT REGISTER*/
          AD1CHS0 = 0x0000;

          AD1CSSH = 0x0000; /* AD1CSSH: ADC1 INPUT SCAN SELECT REGISTER HIGH(1) */
          AD1CSSL = 0x0000; /* ADC1 INPUT SCAN SELECT REGISTER LOW(1,2) */

          AD1CSSLbits.CSS0 = 1;  // Enable AN0 for channel scan
          AD1CSSLbits.CSS1 = 1;  // Enable AN1 for channel scan
          AD1CSSLbits.CSS3 = 1;  // Enable AN3 for channel scan

          INTCON2bits.GIE = 0;  //  Global Interrupt Enable bit

        IFS0bits.AD1IF = 0;			// Clear the A/D interrupt flag bit
        IEC0bits.AD1IE = 1;			// Do Not Enable A/D interrupt

        INTCON2bits.GIE = 1;  //  Global Interrupt Enable bit

        AD1CON1bits.ADON = 1;		// Turn on the A/D converter
}



void initPWM_Master(void)
{

    PTCON =  0x0000;   /* Desable the PWM Module  - known state */

    /* ~~~~~~~~~~~~~~~~~~~~~~ PWM1 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* PWM1 I/O Control Register register */
    IOCON1 = 0x0000;  /* Put register in known state */
    IOCON1bits.PENH = 1;   /* PWM1H is controlled by PWM module */
    IOCON1bits.PENL = 1;   /* PWM1L is controlled by PWM module */
    IOCON1bits.PMOD = 0;   /* PWM I/O pin pair is in the Complementary Output mode */
    IOCON1bits.POLH = 1;   /* PWMxH pin is active-low */
    IOCON1bits.POLL = 1;   /* PWMxL pin is active-low */
    IOCON1bits.OVRENH = 0; /* PWM generator controls PWMxH pin */
    IOCON1bits.OVRENL = 0; /* PWM generator controls PWMxL pin */


    /* PWMCONx: PWM CONTROL REGISTER */
    PWMCON1 = 0x0000;  /* Put register in known state */
    PWMCON1bits.FLTIEN = 0; /* Fault interrupt is disabled and FLTSTAT bit is cleared */
    PWMCON1bits.CLIEN = 0; /* Current-limit interrupt disabled and CLSTAT bit is cleared */
    PWMCON1bits.TRGIEN = 0; /* Trigger event interrupts are disabled and TRGSTAT bit is cleared */
    PWMCON1bits.ITB = 0; /* PTPER register provides timing for this PWM generator */
    PWMCON1bits.MDCS = 1; /* MDC register provides duty cycle information for this PWM generator */
    PWMCON1bits.DTC = 1; /* Negative dead time actively applied for Complementary Output mode */
    PWMCON1bits.MTBS = 0; /* PWM generator uses the primary master time base for
                           synchronization and as the clock source for the PWM generation logic */
    PWMCON1bits.CAM = 0; /* Edge-Aligned mode is enabled */
    PWMCON1bits.XPRES = 0; /* External pins do not affect PWM time base */
    PWMCON1bits.IUE = 0; /* Updates to the active MDC/PDCx/DTx/ALTDTRx/PHASEx
                          registers are synchronized to the PWM time base */
    FCLCON1 = 0x0003;  /* Fault Mode bits for PWM Generator # Fault input is disabled */

    /* Load PDTR1 and ALTDTR2 register with preset dead time value */
    DTR1    = 70;               /* Deadtime setting */
                                /* Deadtime = DTR1*7.1428nsec = 500nsec */
    ALTDTR1 = 70;               /* Deadtime setting */
                                /* Deadtime = ALTDTR1*7.1428nsec = 500nsec */
    /* PHASE1 register */
    PHASE1 = PHASE1inv01;                 /* No phase shift */

    PDC1 = 5250; /* Master Duty Cycle is 75% of the period */



    /* ~~~~~~~~~~~~~~~~~~~~~~ PWM2 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
     /* PWM2 I/O Control Register register */
    IOCON2 = 0x0000;  /* Put register in known state */
    IOCON2bits.PENH = 1;   /* PWM1H is controlled by PWM module */
    IOCON2bits.PENL = 1;   /* PWM1L is controlled by PWM module */
    IOCON2bits.PMOD = 0;   /* PWM I/O pin pair is in the Complementary Output mode */
    IOCON2bits.POLH = 0;   /*  PWMxL pin is active-high  */
    IOCON2bits.POLL = 0;   /*  PWMxL pin is active-high  */
    IOCON2bits.OVRENH = 0; /* PWM generator controls PWMxH pin */
    IOCON2bits.OVRENL = 0; /* PWM generator controls PWMxL pin */

    /* PWMCONx: PWM CONTROL REGISTER */
    PWMCON2 = 0x0000;  /* Put register in known state */
    PWMCON2bits.FLTIEN = 0; /* Fault interrupt is disabled and FLTSTAT bit is cleared */
    PWMCON2bits.CLIEN = 0; /* Current-limit interrupt disabled and CLSTAT bit is cleared */
    PWMCON2bits.TRGIEN = 0; /* Trigger event interrupts are disabled and TRGSTAT bit is cleared */
    PWMCON2bits.ITB = 0; /* PTPER register provides timing for this PWM generator */
    PWMCON2bits.MDCS = 1; /* MDC register provides duty cycle information for this PWM generator */
    PWMCON2bits.DTC = 0; /* Positive dead time actively applied for all output modes */
    PWMCON2bits.MTBS = 0; /* PWM generator uses the primary master time base for
                           synchronization and as the clock source for the PWM generation logic */
    PWMCON2bits.CAM = 0; /* Edge-Aligned mode is enabled */
    PWMCON2bits.XPRES = 0; /* External pins do not affect PWM time base */
    PWMCON2bits.IUE = 0; /* Updates to the active MDC/PDCx/DTx/ALTDTRx/PHASEx
                          registers are synchronized to the PWM time base */
    FCLCON2 = 0x0003;  /* Fault Mode bits for PWM Generator # Fault input is disabled */

     /* Load PDTR2 and ALTDTR2 register with preset dead time value */
    DTR2    = 70;               /* Deadtime setting */
                                /* Deadtime = DTR2*7.1428nsec = 500nsec */
    ALTDTR2 = 70;               /* Deadtime setting */
                                /* Deadtime = ALTDTR2*7.1428nsec = 500nsec */
    /* PHASE2 register */
    PHASE2 = PHASE2inv01;  /*  3500 */

     /* PDCx: PWM GENERATOR DUTY CYCLE REGISTER */
//   PDC2 = 5250; /* PDC2 = PTPER - MDC */


 /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

      /* PWM3 I/O Control Register register */
    IOCON3 = 0x0000;  /* Put register in known state */
    /* PWMCONx: PWM CONTROL REGISTER */
    PWMCON3 = 0x0000;  /* Put register in known state */

    FCLCON3 = 0x0003;  /* Fault Mode bits for PWM Generator # Fault input is disabled */

 /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* Primary Master Time Base (PMTMR) Period Value bits */
    PTPER = PTPERval;               /* Fosc = 140 MHz; PWM Input Clock Prescaler = 1  */
                                /* Period = PTPER*7.1428nsec = 50us / 20kHz */

    MDC = 5250; /* Master Duty Cycle is 75% of the period */

    /* PWM Time Base Control Register */

    PTCONbits.PTSIDL = 0; /* PWM time base runs in CPU Idle mode */
    PTCONbits.SEIEN = 0; /* Special Event Interrupt is disabled */
    PTCONbits.EIPU = 0; /* Active Period register updates occur on PWM cycle boundaries */
    PTCONbits.SYNCPOL = 0; /* SYNCI1/SYNCO1 is active-high */
    PTCONbits.SYNCOEN = 1; /* SYNCO1 output is disabled */
    PTCONbits.SYNCEN = 0; /* External synchronization of primary time base is disabled */
    PTCONbits.SYNCSRC = 0; /* Synchronous Source Selection bits -> SYNCI 1 input from PPS  */
    /*011 = PTGO17(2)
      010 = PTGO16(2)*/

    PTCONbits.SEVTPS = 0; /* 1:1 Postscaler generates Special Event Trigger on every compare match event */
    PTCON2bits.PCLKDIV = PCLKDIVval; /* Divide by 1, maximum PWM timing resolution (power-on default) */

    PTCONbits.PTEN = 1;   /* Enable the PWM Module */
}



/* DMA Channel 0 Configuration for ECAN Transmission */
void initDMAch0CANTx(void)
{

     DMA0CONbits.CHEN = 0x0; /* Channel disabled */
     
 /* DMAXCON: DMA CHANNEL X CONTROL REGISTER */
     DMA0CONbits.SIZE = 0x0; /* Data Transfer Size bit -> Word */
     DMA0CONbits.DIR = 0x1; /* Read from DPSRAM (or RAM) address, write to peripheral address */
     DMA0CONbits.HALF = 0; /* Initiate interrupt when all of the data has been moved */
     DMA0CONbits.NULLW = 0; /* Null Data Peripheral Write Mode Select bit - Normal */
     DMA0CONbits.AMODE = 0x2; /* DMA Addressing Mode: Peripheral Indirect Addressing mode */
     DMA0CONbits.MODE = 0x0; /* Operating Mode: Continuous, Ping-Pong modes disabled */

/* DMAXREQ: DMA CHANNEL X IRQ SELECT REGISTER */
   DMA0REQbits.FORCE = 0; /* Automatic DMA transfer initiation by DMA Request */
   DMA0REQbits.IRQSEL = 0x46;  /* ECAN1 ? TX Data Request */


/* DMAXCNT: DMA CHANNEL X TRANSFER COUNT REGISTER */
    DMA0CNT = 7; /* Set Number of DMA Transfer per ECAN message to 8 words */

/* DMAXPAD: DMA CHANNEL X PERIPHERAL ADDRESS REGISTER */
    DMA0PAD = (volatile unsigned int)&C1TXD; /* Peripheral Address: ECAN Transmit Register */

    /* DMAXSTAH: DMA CHANNEL X START ADDRESS REGISTER A */
    	#ifdef _HAS_DMA_
	DMA0STAL = __builtin_dmaoffset(ecan1msgBuf);
	DMA0STAH = __builtin_dmapage(ecan1msgBuf);
	#else
	DMA0STAL = (unsigned int)&ecan1msgBuf;
	DMA0STAH = 0x0000;
	#endif

//   INTCON2bits.GIE = 0;  //  Global Interrupt Enable bit
    /* Channel Enable: Enable DMA Channel 0 */
    DMA0CONbits.CHEN = 0x1;
    /* Channel Interrupt Enable: Enable DMA Channel 0 Interrupt */
    IEC0bits.DMA0IE = 1;

//    INTCON2bits.GIE = 1;  //  Global Interrupt Enable bit
}



/* Example 21-7: DMA Channel 1 Configuration for ECAN1 Reception */
void initDMAch1CANRx(void)
{

      DMA1CONbits.CHEN = 0x0; /* Channel disabled */

     /* DMAXCON: DMA CHANNEL X CONTROL REGISTER */
     DMA1CONbits.SIZE = 0x0; /* Data Transfer Size bit -> Word */
     DMA1CONbits.DIR = 0x0; /* Transfer Direction bit ( Peripheral to device RAM ) */
     DMA1CONbits.HALF = 0; /* Initiate interrupt when all of the data has been moved */
     DMA1CONbits.NULLW = 0; /* Null Data Peripheral Write Mode Select bit - Normal */
     DMA1CONbits.AMODE = 0x2; /* DMA Addressing Mode: Peripheral Indirect Addressing mode */
     DMA1CONbits.MODE = 0x0; /* Operating Mode: Continuous, Ping-Pong modes disabled */

/* DMAXREQ: DMA CHANNEL X IRQ SELECT REGISTER */
   DMA1REQbits.FORCE = 0; /* Automatic DMA transfer initiation by DMA Request */
   DMA1REQbits.IRQSEL = 0x22;  /* ECAN1 ? RX Data Ready */


/* DMAXCNT: DMA CHANNEL X TRANSFER COUNT REGISTER */
    DMA1CNT = 7; /* Set Number of DMA Transfer per ECAN message to 8 words */

/* DMAXPAD: DMA CHANNEL X PERIPHERAL ADDRESS REGISTER */
    DMA1PAD = (volatile unsigned int) &C1RXD;  /* Peripheral Address: ECAN1 Receive Register */

        /* DMAXSTAH: DMA CHANNEL X START ADDRESS REGISTER A */
    	#ifdef _HAS_DMA_
	DMA1STAL = __builtin_dmaoffset(ecan1msgBuf);
	DMA1STAH = __builtin_dmapage(ecan1msgBuf);
	#else
	DMA1STAL =(unsigned int)&ecan1msgBuf[1];
	DMA1STAH = 0x0000;
	#endif

//    INTCON2bits.GIE = 0;  //  Global Interrupt Enable bit
    /* Channel Enable: Enable DMA Channel 1 */
    DMA1CONbits.CHEN = 0x1;
    /* Channel Interrupt Enable: Enable DMA Channel 1 Interrupt */
    IEC0bits.DMA1IE = 1;
 //   INTCON2bits.GIE = 1;  //  Global Interrupt Enable bit
}




/* DMA Channel 2 Configuration for ADC USE*/
void initDMAch2ADC1(void)
{

     DMA2CONbits.CHEN = 0x0; /* Channel disabled */

 /* DMAXCON: DMA CHANNEL X CONTROL REGISTER */
     DMA2CONbits.SIZE = 0x0; /* Data Transfer Size bit -> Word */
     DMA2CONbits.DIR = 0x0; /*Read from Peripheral address, write to RAM address */
     DMA2CONbits.HALF = 0; /* Initiate interrupt when all of the data has been moved */
     DMA2CONbits.NULLW = 0; /* Null Data Peripheral Write Mode Select bit - Normal */
     DMA2CONbits.AMODE = 0x2; /* DMA Addressing Mode: Peripheral Indirect Addressing mode */
     DMA2CONbits.MODE = 0x0; /* Operating Mode: Continuous, Ping-Pong modes disabled */

/* DMAXREQ: DMA CHANNEL X IRQ SELECT REGISTER */
   DMA2REQbits.FORCE = 0; /* Automatic DMA transfer initiation by DMA Request */
   DMA2REQbits.IRQSEL = 0x0D;  /* 00001101 = ADC1 ? ADC1 Convert done */


/* DMAXCNT: DMA CHANNEL X TRANSFER COUNT REGISTER */
    DMA2CNT = 2; /* Set Number of DMA Transfer */

/* DMAXPAD: DMA CHANNEL X PERIPHERAL ADDRESS REGISTER */
    DMA2PAD = (unsigned int)&ADC1BUF0; /* Peripheral Address: ADC1 */

    /* DMAXSTAH: DMA CHANNEL X START ADDRESS REGISTER A */
        #ifdef _HAS_DMA_
	DMA0STAL = __builtin_dmaoffset(&ADC1BufferA);
	DMA0STAH = __builtin_dmapage(&ADC1BufferA);

	DMA0STBL = __builtin_dmaoffset(&ADC1BufferB);
	DMA0STBH = __builtin_dmapage(&ADC1BufferB);
	#else
	DMA0STAL = (unsigned int)&ADC1BufferA;
	DMA0STAH = 0x0000;

	DMA0STBL = (unsigned int)&ADC1BufferB;
	DMA0STBH = 0x0000;
	#endif


//   INTCON2bits.GIE = 0;  //  Global Interrupt Enable bit
  
    /* Channel Interrupt Enable: Enable DMA Channel 0 Interrupt */
    IFS1bits.DMA2IF = 0;
    IEC1bits.DMA2IE = 1;

//    INTCON2bits.GIE = 1;  //  Global Interrupt Enable bit

      /* Channel Enable: Enable DMA Channel 0 */
    DMA0CONbits.CHEN = 0x1;

    
}

/* Example 21-9: Code Example for Configuring ECAN? Bit Timing Parameters */
void initeCAN(void)
{

/*  111 = Set Listen All Messages mode
    110 = Reserved
    101 = Reserved
    100 = Set Configuration mode
    011 = Set Listen Only Mode
    010 = Set Loopback mode
    001 = Set Disable mode
    000 = Set Normal Operation mode   */

    /* Set the ECAN module for Configuration Mode before writing into the Baud
    Rate Control Registers */
    C1CTRL1bits.REQOP = 4; /* Set Configuration mode */
    /* Wait for the ECAN module to enter into Configuration Mode */
    while(C1CTRL1bits.OPMODE!=4);


//        WIN: SFR Map Window Select bit
//        1 = Use filter window
//        0 = Use buffer window
    C1CTRL1bits.WIN = 0;

    
    /* FCAN is selected to be FP by clearing the CANCKS bit  */
    /* FCAN = FP = 73.4 MHz */
    C1CTRL1bits.CANCKS = 0x0;
    /*
	Bit Time = (Sync Segment + Propagation Delay + Phase Segment 1 + Phase Segment 2)=20*TQ
	Phase Segment 1 = 8TQ
	Phase Segment 2 = 6Tq
	Propagation Delay = 5Tq
	Sync Segment = 1TQ
	CiCFG1<BRP> =(FCAN /(2 ×N×FBAUD))– 1
	BIT RATE OF 1Mbps
	*/
	C1CFG1bits.BRP = 0x1;
	/* Synchronization Jump Width set to 4 TQ */
	C1CFG1bits.SJW = 0x3;
	/* Phase Segment 1 time is 8 TQ */
	C1CFG2bits.SEG1PH=0x7;
	/* Phase Segment 2 time is set to be programmable */
	C1CFG2bits.SEG2PHTS = 0x1;
	/* Phase Segment 2 time is 6 TQ */
	C1CFG2bits.SEG2PH = 0x5;
	/* Propagation Segment time is 5 TQ */
	C1CFG2bits.PRSEG = 0x4;
	/* Bus line is sampled three times at the sample point */
	C1CFG2bits.SAM = 0x1;
        /* CAN bus line filter is not used for wake-up */
        C1CFG2bits.WAKFIL =0x0;

        /* 4 CAN Messages to be buffered in DMA RAM */
	C1FCTRLbits.DMABS=0b000;

	/* Filter configuration */
	/* enable window to access the filter configuration registers */
	C1CTRL1bits.WIN=0b1;


/******************************************************************************/
	/* select acceptance mask 0 filter 0 buffer 1 */
	C1FMSKSEL1bits.F0MSK=0;  /* Acceptance Mask 0 registers contain mask */
	/* configure accpetence mask 0 - match the id in filter 0
	setup the mask to check every bit of the standard message,
	the macro when called as CAN_FILTERMASK2REG_SID(0x7FF) will
	write the register C1RXM0SID to include every bit in filter comparison
	*/
	 C1RXM0SID=CAN_FILTERMASK2REG_SID(0x7FF); // 0b0000011111111111
      //  C1RXM0SID=CAN_FILTERMASK2REG_SID(0x0000); // 0b00000 acept all

	/* configure accpetence filter 0
	setup the filter to accept a standard id of 0x123,
	the macro when called as CAN_FILTERMASK2REG_SID(0x123) will
	write the register C1RXF0SID to accept only standard id of 0x123
	*/
         #if defined __SLAVE__
	C1RXF0SID=CAN_FILTERMASK2REG_SID(0x123); // For Slave
         #elif defined __MASTER__
        C1RXF0SID=CAN_FILTERMASK2REG_SID(0x125); // For Master
        #endif

	/* set filter to check for standard ID and accept standard id only */
	C1RXM0SID=CAN_SETMIDE(C1RXM0SID);
	C1RXF0SID=CAN_FILTERSTD(C1RXF0SID);    
   

        /* acceptance filter to use buffer 1 for incoming messages */
	C1BUFPNT1bits.F0BP=0b0001;
	/* enable filter 0 */
	C1FEN1bits.FLTEN0=1;


/******************************************************************************/

	/* select acceptance mask 1 filter 1 and buffer 2 */
	C1FMSKSEL1bits.F1MSK=0b01; /* Acceptance Mask 1 registers contain mask */
	/* configure accpetence mask 1 - match id in filter 1
	setup the mask to check every bit of the extended message,
	the macro when called as CAN_FILTERMASK2REG_EID0(0xFFFF)
	will write the register C1RXM1EID to include extended
	message id bits EID0 to EID15 in filter comparison.
	the macro when called as CAN_FILTERMASK2REG_EID1(0x1FFF)
	will write the register C1RXM1SID to include extended
	message id bits EID16 to EID28 in filter comparison.
	*/
	C1RXM1EID=CAN_FILTERMASK2REG_EID0(0xFFFF);
	C1RXM1SID=CAN_FILTERMASK2REG_EID1(0x1FFF);
	/* configure acceptance filter 1
	configure accpetence filter 1 - accept only XTD ID 0x12345678
	setup the filter to accept only extended message 0x12345678,
	the macro when called as CAN_FILTERMASK2REG_EID0(0x5678)
	will write the register C1RXF1EID to include extended
	message id bits EID0 to EID15 when doing filter comparison.
	the macro when called as CAN_FILTERMASK2REG_EID1(0x1234)
	will write the register C1RXF1SID to include extended
	message id bits EID16 to EID28 when doing filter comparison.
	*/
	C1RXF1EID=CAN_FILTERMASK2REG_EID0(0x5678);
	C1RXF1SID=CAN_FILTERMASK2REG_EID1(0x1234);
	/* filter to check for extended ID only */
	C1RXM1SID=CAN_SETMIDE(C1RXM1SID);
	C1RXF1SID=CAN_FILTERXTD(C1RXF1SID);

      
	/* acceptance filter to use buffer 2 for incoming messages */
	C1BUFPNT1bits.F1BP=0b0010;
        
	/* enable filter 1 */
	C1FEN1bits.FLTEN1=1;
/******************************************************************************/


	/* select acceptance mask 1 filter 2 and buffer 3 */
	C1FMSKSEL1bits.F2MSK=0b01;
	/* configure acceptance filter 2
	configure accpetence filter 2 - accept only XTD ID 0x12345679
	setup the filter to accept only extended message 0x12345679,
	the macro when called as CAN_FILTERMASK2REG_EID0(0x5679)
	will write the register C1RXF1EID to include extended
	message id bits EID0 to EID15 when doing filter comparison.
	the macro when called as CAN_FILTERMASK2REG_EID1(0x1234)
	will write the register C1RXF1SID to include extended
	message id bits EID16 to EID28 when doing filter comparison.
	*/
	C1RXF2EID=CAN_FILTERMASK2REG_EID0(0x5679);
	C1RXF2SID=CAN_FILTERMASK2REG_EID1(0x1234);
	/* filter to check for extended ID only */
	C1RXF2SID=CAN_FILTERXTD(C1RXF2SID);
	/* acceptance filter to use buffer 3 for incoming messages */
	C1BUFPNT1bits.F2BP=0b0011;
	/* enable filter 2 */
	C1FEN1bits.FLTEN2=1;


/******************************************************************************/   

	/* clear window bit to access ECAN control registers */
	C1CTRL1bits.WIN=0;
//        WIN: SFR Map Window Select bit
//        1 = Use filter window
//        0 = Use buffer window

	/* put the module in normal mode */
	C1CTRL1bits.REQOP=0;
	while(C1CTRL1bits.OPMODE != 0);

      	/* clear the buffer and overflow flags */
	C1RXFUL1=C1RXFUL2=C1RXOVF1=C1RXOVF2=0x0000;
	/* ECAN1, Buffer 0 is a Transmit Buffer */
	C1TR01CONbits.TXEN0=1;
	/* ECAN1, Buffer 1 is a Receive Buffer */
	C1TR01CONbits.TXEN1=0;
	/* ECAN1, Buffer 2 is a Receive Buffer */
	C1TR23CONbits.TXEN2=0;
	/* ECAN1, Buffer 3 is a Receive Buffer */
	C1TR23CONbits.TXEN3=0;

        
	/* Message Buffer 0 Priority Level */
	
//11 = Highest message priority
//10 = High intermediate message priority
//01 = Low intermediate message priority
//00 = Lowest message priority

       #if defined __SLAVE__   // Send  system error
	C1TR01CONbits.TX0PRI=0b11; // For Slave
       #elif defined __MASTER__
        C1TR01CONbits.TX0PRI=0b10; // For Master
       #endif
        
        INTCON2bits.GIE = 0;  //  Global Interrupt Enable bit

	/* configure the device to interrupt on the receive buffer full flag */
	/* clear the buffer full flags */
	C1RXFUL1=0;
	C1INTFbits.RBIF=0;

//    	/* Enable ECAN1 Interrupt */
	IEC2bits.C1IE=1;
        
//        /* enable Transmit interrupt */
 	C1INTEbits.TBIE=1;
        
//	/* Enable Receive interrupt */
	C1INTEbits.RBIE=1;

        INTCON2bits.GIE = 1;  //  Global Interrupt Enable bit
        
}

