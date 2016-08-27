/*!
* \file protect.c
* \brief Source file for routines to protect hardware
* \details
* \author Adriano Ruseler
* \version 1.0
* \copyright The MIT License (MIT).
*/

/* Device header file */
#if defined(__dsPIC33E__)
	#include <p33Exxxx.h>
#elif defined(__dsPIC33F__)
	#include <p33Fxxxx.h>
#endif

#include <stdint.h>        /* Includes uint16_t definition   */
#include <stdbool.h>       /* Includes true/false definition */
#include <libq.h>          /* include fixed point library */

#include "main_interrupt.h"  /*  */
#include "initialization.h"          /* User funct/params, such as InitApp    */
#include "eCAN.h"
#include "protect.h"

/******************************************************************************/
/* Interrupt Vector Options                                                   */
/******************************************************************************/

/* Example Syntax:                                                            */
/*                                                                            */
/* void __attribute__((interrupt,auto_psv)) <Vector Name>(void)               */
/* {                                                                          */
/*     <Clear Interrupt Flag>                                                 */
/* }                                                                          */

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

extern int VsinQ15[];
extern int DutyCycleQ15;
extern int MasterDutyCycle;
extern int MasterDutyCycleQ15;
extern unsigned int TableIndex; // Indice da tabela Look-Up Table
extern unsigned int ControlDutyCycle;
extern unsigned int status;
extern int GpwmQ15;
extern int ADCValues[];
extern int Vsin; /* Para testes apenas */
extern int eCAN_cnt;
extern int errorFlag;
extern unsigned int StatusActionH;
extern unsigned int TableIndexL;

extern mID canTxMessage; /**< Transmition buffer for CAN */
extern mID canRxMessage; /**< Receive buffer for CAN */



/******************************************************************************/
/* Protection Functions                                                             */
/******************************************************************************/

/**
   * Desables PWM module
   * @param  No input value.
   * @return No return value.
   */
void PWMdes(void){         /* Desabilita modulos PWM */

PTCONbits.PTEN = 0;   /* Enable the PWM Module */


#if defined(__PROTOTIPO__)
/* Configura��o de pinos para o prototipo */
/* RA4 -> RESET */
/* RB0 -> LED1  */
/* RB1 -> AN3 */
/* RB4 -> LED2 */
/*Initialize the Ports */

//PORTAbits.RA4 = 0; // RESET
//LATAbits.LATA4 = 0; //RESET

#endif

}

/**
   * Desables PWM module
   * @param  No input value.
   * @return No return value.
   */
// Retorna Action -> O que o Slave deve fazer
unsigned int StatusCheckMaster(unsigned int status)
{
    switch(status){
        case 0x0000:
            /* Normal Operation*/
            return 0x00;
            break; // Tudo OK!!
        case 0x01:
          //  PTCONbits.PTEN = 1;   /* Enable the PWM Module */      
            return 0x01;
            break;
        case 0x02:
         //   PTCONbits.PTEN = 1;   /* Disable the PWM Module */
            return 0x02;
            break;
        case 0x03:  /* Error in Module 01*/
        //     SendStatus(0x02);    /* Disable All PWM Module */
            return 0x02;
            break;
        case 0x04: /* Error in Module 02*/
        //     SendStatus(0x02);    /* Disable All PWM Module */
            return 0x02;
            break;
        case 0x05: /* Error in Module 02*/
        //     SendStatus(0x02);    /* Disable All PWM Module */
            return 0x02;
            break;
        case 0x3F:   // Int 2
       //     PTCONbits.PTEN = 0;   /* Disable the PWM Module */
            return 0x02;
            break;
        case 0x3E:  // Int 1
          //  PTCONbits.PTEN = 0;   /* Disable the PWM Module */
            return 0x02;
            break;
         case 0x3D:  // Int 0
          //  PTCONbits.PTEN = 0;   /* Disable the PWM Module */
            return 0x02;
            break;
    }
    return 0x00; // Default State
}





void StatusCheckSlave(unsigned int status)
{
    switch(status){
        case 0x00:
            /* Normal Operation*/
            break; // Tudo OK!!
        case 0x01:
            PTCONbits.PTEN = 1;   /* Enable the PWM Module */
            errorFlag=1;
            break;
        case 0x02:
            PTCONbits.PTEN = 0;   /* Disable the PWM Module */
            break;
        case 0x3F:   // Int 2
            PTCONbits.PTEN = 0;   /* Disable the PWM Module */
            break;
        case 0x3E:  // Int 1
          //  PTCONbits.PTEN = 0;   /* Disable the PWM Module */
            break;
         case 0x3D:  // Int 0
          //  PTCONbits.PTEN = 0;   /* Disable the PWM Module */
            break;
    }
}




void SendStatus(unsigned int status) // For Master and Slave? Just Slave
{
//    status;
       StatusActionH=(status & 0x003F) << 10; /* Move bits*/
       TableIndexL=(canTxMessage.data_word[0] & 0x03FF); // Garante bits superiores com 0
       canTxMessage.data_word[0]=(StatusActionH | TableIndexL); /* Controle de tens�o */
      
     	/* Send message  */
       sendECAN(&canTxMessage);

}
