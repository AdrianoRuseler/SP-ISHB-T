/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__dsPIC33E__)
	#include <p33Exxxx.h>
#elif defined(__dsPIC33F__)
	#include <p33Fxxxx.h>
#endif

#include <stdint.h>        /* Includes uint16_t definition   */
#include <stdbool.h>       /* Includes true/false definition */
#include <libq.h>          /* include fixed point library */

#include "initialization.h"          /* User funct/params, such as InitApp    */
#include "eCAN.h"
#include "main_interrupt.h"  /*  */
#include "protect.h"



/******************************************************************************/
/* Interrupt Vector Options                                                   */
/******************************************************************************/
/*                                                                            */
/* Refer to the C30 (MPLAB C Compiler for PIC24F MCUs and dsPIC33F DSCs) User */
/* Guide for an up to date list of the available interrupt options.           */
/* Alternately these names can be pulled from the device linker scripts.      */
/*                                                                            */
/*                                                                            */
/* dsPIC33E Primary Interrupt Vector Names:                                   */
/*                                                                            */
/* _INT0Interrupt     _IC4Interrupt      _U4TXInterrupt                       */
/* _IC1Interrupt      _IC5Interrupt      _SPI3ErrInterrupt                    */
/* _OC1Interrupt      _IC6Interrupt      _SPI3Interrupt                       */
/* _T1Interrupt       _OC5Interrupt      _OC9Interrupt                        */
/* _DMA0Interrupt     _OC6Interrupt      _IC9Interrupt                        */
/* _IC2Interrupt      _OC7Interrupt      _PWM1Interrupt                       */
/* _OC2Interrupt      _OC8Interrupt      _PWM2Interrupt                       */
/* _T2Interrupt       _PMPInterrupt      _PWM3Interrupt                       */
/* _T3Interrupt       _DMA4Interrupt     _PWM4Interrupt                       */
/* _SPI1ErrInterrupt  _T6Interrupt       _PWM5Interrupt                       */
/* _SPI1Interrupt     _T7Interrupt       _PWM6Interrupt                       */
/* _U1RXInterrupt     _SI2C2Interrupt    _PWM7Interrupt                       */
/* _U1TXInterrupt     _MI2C2Interrupt    _DMA8Interrupt                       */
/* _AD1Interrupt      _T8Interrupt       _DMA9Interrupt                       */
/* _DMA1Interrupt     _T9Interrupt       _DMA10Interrupt                      */
/* _NVMInterrupt      _INT3Interrupt     _DMA11Interrupt                      */
/* _SI2C1Interrupt    _INT4Interrupt     _SPI4ErrInterrupt                    */
/* _MI2C1Interrupt    _C2RxRdyInterrupt  _SPI4Interrupt                       */
/* _CM1Interrupt      _C2Interrupt       _OC10Interrupt                       */
/* _CNInterrupt       _QEI1Interrupt     _IC10Interrupt                       */
/* _INT1Interrupt     _DCIEInterrupt     _OC11Interrupt                       */
/* _AD2Interrupt      _DCIInterrupt      _IC11Interrupt                       */
/* _IC7Interrupt      _DMA5Interrupt     _OC12Interrupt                       */
/* _IC8Interrupt      _RTCCInterrupt     _IC12Interrupt                       */
/* _DMA2Interrupt     _U1ErrInterrupt    _DMA12Interrupt                      */
/* _OC3Interrupt      _U2ErrInterrupt    _DMA13Interrupt                      */
/* _OC4Interrupt      _CRCInterrupt      _DMA14Interrupt                      */
/* _T4Interrupt       _DMA6Interrupt     _OC13Interrupt                       */
/* _T5Interrupt       _DMA7Interrupt     _IC13Interrupt                       */
/* _INT2Interrupt     _C1TxReqInterrupt  _OC14Interrupt                       */
/* _U2RXInterrupt     _C2TxReqInterrupt  _IC14Interrupt                       */
/* _U2TXInterrupt     _QEI2Interrupt     _OC15Interrupt                       */
/* _SPI2ErrInterrupt  _U3ErrInterrupt    _IC15Interrupt                       */
/* _SPI2Interrupt     _U3RXInterrupt     _OC16Interrupt                       */
/* _C1RxRdyInterrupt  _U3TXInterrupt     _IC16Interrupt                       */
/* _C1Interrupt       _USB1Interrupt     _ICDInterrupt                        */
/* _DMA3Interrupt     _U4ErrInterrupt    _PWMSpEventMatchInterrupt            */
/* _IC3Interrupt      _U4RXInterrupt     _PWMSecSpEventMatchInterrupt         */
/*                                                                            */
/* For alternate interrupt vector naming, simply add 'Alt' between the prim.  */
/* interrupt vector name '_' and the first character of the primary interrupt */
/* vector name.  There is no Alternate Vector or 'AIVT' for the 33E family.   */
/*                                                                            */
/* For example, the vector name _ADC2Interrupt becomes _AltADC2Interrupt in   */
/* the alternate vector table.                                                */
/*                                                                            */
/* Example Syntax:                                                            */
/*                                                                            */
/* void __attribute__((interrupt,auto_psv)) <Vector Name>(void)               */
/* {                                                                          */
/*     <Clear Interrupt Flag>                                                 */
/* }                                                                          */
/*                                                                            */
/* For more comprehensive interrupt examples refer to the C30 (MPLAB C        */
/* Compiler for PIC24 MCUs and dsPIC DSCs) User Guide in the                  */
/* <compiler installation directory>/doc directory for the latest compiler    */
/* release.                                                                   */
/*                                                                            */
/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/


/*=============================================================================
                         VARIABLES
=============================================================================*/

extern unsigned int Count;
extern unsigned int TableIndex; // Indice da tabela Look-Up Table
extern unsigned int TableIndexL; // Indice da tabela Look-Up Table >> 10
extern int VsinQ15[];
extern unsigned int status;

extern mID canTxMessage;
extern mID canRxMessage;

extern unsigned int StatusActionH;
extern unsigned int StatusAction;

/*
_INT0Interrupt() is the INT0 interrupt service routine (ISR).
The routine must have global scope in order to be an ISR.
The ISR name is chosen from the device linker script.
*/
void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void) //  OK
{

#ifdef __SLAVE__
     SendStatus(0x03); // Modulo 1 com problemas

#endif

#ifdef __MASTER__
    // SendStatus(0x3D); // Sutdown Al

#endif


      IFS0bits.INT0IF = 0;    //Clear the INT0 interrupt flag or else
                                //the CPU will keep vectoring back to the ISR
}

/*
_INT1Interrupt() is the INT1 interrupt service routine (ISR).
The routine must have global scope in order to be an ISR.
The ISR name is chosen from the device linker script.
*/
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void)
{
#ifdef __SLAVE__
     SendStatus(0x03); // Modulo 1 com problemas

#endif

#ifdef __MASTER__
  //  SendStatus(0x3E); // Sutdown All
#endif


     IFS1bits.INT1IF = 0;    //Clear the INT1 interrupt flag or else
                                //the CPU will keep vectoring back to the ISR
}

/*
_INT2Interrupt() is the INT2 interrupt service routine (ISR).
The routine must have global scope in order to be an ISR.
The ISR name is chosen from the device linker script.
*/
void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(void)  // OK
{

#ifdef __MASTER__

    StatusActionH=( 0x01 & 0x003F) << 10; /* Move bits*/
    TableIndexL=((canTxMessage.data_word[0]) & 0x03FF); // Garante bits superiores com 0
    canTxMessage.data_word[0]=StatusActionH | TableIndexL; /* Controle de tensão */
    
     	/* Send message  */
       sendECAN(&canTxMessage); 
#endif
    
    IFS1bits.INT2IF = 0;    //Clear the INT2 interrupt flag or else
                                //the CPU will keep vectoring back to the ISR
}



void __attribute__((interrupt,no_auto_psv)) _T1Interrupt( void )
{
	/* reset Timer 1 interrupt flag */
          IFS0bits.T1IF = 0;

}


/* Example code for Timer5 ISR*/
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void)
{
/* Interrupt Service Routine code goes here */

 // LATAbits.LATA4 = ~LATAbits.LATA4;   // Toggle Led 3


IFS1bits.T5IF = 0; //Clear Timer3 interrupt flag
}


/*-----------------------*/


/*=============================================================================
                        ADC INTERRUPT SERVICE ROUTINE
=============================================================================*/

void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void)
{
    
     IFS0bits.AD1IF = 0;		// Clear the ADC1 Interrupt Flag
}


/*=============================================================================
                      DMA0 INTERRUPT SERVICE ROUTINE
=============================================================================*/

void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
     
    
     IFS0bits.DMA0IF = 0;		// Clear the DMA0 Interrupt Flag
}


/*=============================================================================
                      DMA1 INTERRUPT SERVICE ROUTINE
=============================================================================*/

void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt(void)
{

      IFS0bits.DMA1IF = 0;// Clear the DMA1 Interrupt Flag
}


/*=============================================================================
                      DMA2 INTERRUPT SERVICE ROUTINE
=============================================================================*/

void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt(void)
{


    

      IFS1bits.DMA2IF = 0;// Clear the DMA2 Interrupt Flag
}



/* Interrupt Service Routine 1                      */
/* No fast context save, and no variables stacked   */

void __attribute__((interrupt,no_auto_psv))_C1Interrupt(void)
{
	/* check to see if the interrupt is caused by receive */
    if(C1INTFbits.RBIF)
    {
     	    /* check to see if buffer 1 is full */
	    if(C1RXFUL1bits.RXFUL1)
	    {
			/* set the buffer full flag and the buffer received flag */
			canRxMessage.buffer_status=CAN_BUF_FULL;
			canRxMessage.buffer=1;
		}
		/* check to see if buffer 2 is full */
		else if(C1RXFUL1bits.RXFUL2)
		{
			/* set the buffer full flag and the buffer received flag */
			canRxMessage.buffer_status=CAN_BUF_FULL;
			canRxMessage.buffer=2;
		}
		/* check to see if buffer 3 is full */
		else if(C1RXFUL1bits.RXFUL3)
		{
			/* set the buffer full flag and the buffer received flag */
			canRxMessage.buffer_status=CAN_BUF_FULL;
			canRxMessage.buffer=3;
		}
		else;


                  rxECAN(&canRxMessage);
                 
#ifdef __SLAVE__
                 /* Check for Status Action differnt from Zero; */
         //  TableIDXeCAN=canRxMessage.data_word[0]+4;
        StatusActionH=((canRxMessage.data_word[0]) & 0xFC00); /* Get 6 bits high*/
        if(StatusActionH){
        StatusAction = StatusActionH >> 10; // rotate low
        StatusCheckSlave(StatusAction);  // Verifica o status enviado
        }
#endif

		/* clear flag */
		C1INTFbits.RBIF = 0;
	}
	else if(C1INTFbits.TBIF)
    {

     //       LED1=~LED1;
	    /* clear flag */
		C1INTFbits.TBIF = 0;
	}
	else;

    
	/* clear interrupt flag */
	IFS2bits.C1IF=0;
      
}


