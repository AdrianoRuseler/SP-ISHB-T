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
extern int rho; // Fator que pondera a raz�o ciclica de cada inversor
extern int rho_d;
extern unsigned int TableIndex; // Indice da tabela Look-Up Table
extern unsigned int TableIndexL; // Indice da tabela Look-Up Table
unsigned int TableIDXeCAN=0;
extern unsigned int ControlDutyCycle;
extern unsigned int status;
extern unsigned int GpwmQ15;
extern int ADCValues[];
extern int Vsin; /* Para testes apenas */
extern int eCAN_cnt;
extern int errorFlag;

extern mID canTxMessage;
extern mID canRxMessage;
extern unsigned int statusH;
extern unsigned int StatusActionH;
extern unsigned int StatusAction;

/* Example code for Timer3 ISR*/
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{

LATBbits.LATB0 = 0;  // Toggle led 1

while (!_AD1IF); // Wait for all 3 conversions to complete
  _AD1IF = 0; // Clear conversion done status bit


  ADCValues[0] = ADC1BUF0; // Read the AN0 conversion result -> Sensor de Tens�o
  ADCValues[1] = ADC1BUF1; // Read the AN1 conversion result -> Sensor de corrente
  ADCValues[2] = ADC1BUF2; // Read the AN3 conversion result

// 0x03 -> Problemas no M�dulo 1
// 0x04 -> Problemas no M�dulo 2
// 0x05 -> Problemas no M�dulo 3
  /* (2^n-1-2^(n-1))*2^(16-n)=32704 para  n=10 bits*/
/* y = x/3.3*(32704)*/

#ifdef __SLAVE__

  if(errorFlag){
#ifdef __INV01__
  if((ADCValues[0])>=(350)){
    // SendStatus(0x03); // Modulo 1 com problemas
     errorFlag=0;
  }
#endif
#ifdef __INV02__
     if((ADCValues[0])>=(350)){
   //  SendStatus(0x04); // Modulo 2 com problemas
     errorFlag=0;
     }
#endif
#ifdef __INV03__
     if((ADCValues[0])>=(350)){
   //  SendStatus(0x05); // Modulo 3 com problemas
     errorFlag=0;
     }
#endif
  }

  rho = (ADCValues[2]-512)<<5; // Sa�da do PI
  // rho varia de -1 a 1 em Q14
//  Integer  

#endif



LATBbits.LATB0 =1;  // Clear led 1

  // Ajuste para o formato desejado!
//DutyCycleQ15=(ADCValues[2]>>1)+16384; // Divide entrada ADC por 2 e soma com 0.5;

//MasterDutyCycle=(int32_t)(__builtin_mulss(7000,ADCValues[2]) >> 15);

//MasterDutyCycle = MasterDutyCycle + GpwmQ15;


if(TableIndex==667)// 667 elementos, o �ltimo possui indice 666
    TableIndex =0;// Aponta para o inicio da tabela


  if(eCAN_cnt==4)
  { /* Atualiza valores via CAN */

      #if defined(__MASTER__)
     // Verifica se ocorreu algum erro nos m�dulos;

      // Ajuste para o formato desejado!
     DutyCycleQ15=(ADCValues[2]>>1)+16384; // Divide entrada ADC por 2 e soma com 0.5; 
     
  //   ecan1msgBuf[0][3]= TableIndex; /* Controle de tens�o */
   //  ecan1msgBuf[0][4]= DutyCycleQ15; /* Indice */

      TableIDXeCAN=canRxMessage.data_word[0]; // Slave status
     // canRxMessage.data_word[0]=0x0000; // Reset status to Zero
      status=(TableIDXeCAN & 0xFC00) >> 10; /* Move bits*/
      StatusAction=StatusCheckMaster(status);  // Verifica o status enviado

      StatusActionH=(StatusAction & 0x003F) << 10; /* Move bits*/

      TableIndexL=(TableIndex & 0x03FF); // Garante bits superiores com 0
       canTxMessage.data_word[0]=StatusActionH | TableIndexL; /* Controle de tens�o */
       canTxMessage.data_word[1]=DutyCycleQ15; /* Indice */

      if(StatusActionH){
         C1TR01CONbits.TX0PRI=0b11; // For Master
      }else{
         C1TR01CONbits.TX0PRI=0b10; // For Master
      }
       
     	/* Send message  */
       sendECAN(&canTxMessage);
        
      #elif defined(__SLAVE__)
      
        TableIDXeCAN = ((canRxMessage.data_word[0]) & 0x3FF);  // Retira 10 bits iniciais
        TableIDXeCAN = TableIDXeCAN+4; // Corrigi atraso ( Rever isto)

         if(TableIDXeCAN==TableIndex)
            if(LATBbits.LATB0)
                LATBbits.LATB0=0;
            else
                LATBbits.LATB0=1;
          else
            if(LATBbits.LATB4)
                LATBbits.LATB4=0;
            else
                LATBbits.LATB4=1;

     TableIndex=TableIDXeCAN;  /*  */
 //    ControlDutyCycle=canRxMessage.data_word[1]; /*  */
     DutyCycleQ15=canRxMessage.data_word[1]; /* Varia entre 0 e 1  */
    #endif

   eCAN_cnt=0; /* Zera contador */
  }
  else
   eCAN_cnt+=1;

// DutyCycleQ15=16384; // d = 0.5

// DutyCycleQ15=(int32_t)(__builtin_mulss(DutyCycleQ15,(1-rho)) >> 15);
rho_d=16384-rho; // 0 a 2 em Q14
DutyCycleQ15=(int32_t)(__builtin_mulss(DutyCycleQ15,rho_d) >> 14);  // Deve ficar entre 0 e 1



MasterDutyCycleQ15 = (int32_t)(__builtin_mulss(DutyCycleQ15,VsinQ15[TableIndex]) >> 15); // Varia de -1*d a +1*d

// MasterDutyCycleQ15=DutyCycleQ15; // Varia de 0 - 1

/*Computes the product p0 x p1. Function arguments are signed integers,
and the function result is a signed long integer */
MasterDutyCycle = (int32_t)(__builtin_mulss(MasterDutyCycleQ15,GpwmQ15) >> 15);  // para senoide � 15
// erro de 1 bit -> toler�vel por enquanto
MasterDutyCycle = MasterDutyCycle + GpwmQ15;

TableIndex +=1;
/* 7340 */
// Gpwm=2^Qn/7000 = 2^15/7000= 4096/875 = 4.68114257
 /* ________________________________  7000 -> 0.213623046 em Q15
             /|        /\
            / |       /  \
 ........../..|....../....\..........  3500 -> 0.106811523 em Q15
       \  /   |     /      \
   _____\/____|____/________\________   0
*/

 MDC = MasterDutyCycle; /* Master Duty Cycle Register */
    
 //LATBbits.LATB4 = 1; //Clear Led 2
IFS0bits.T3IF = 0; //Clear Timer3 interrupt flag
   

//AD1CON1bits.SAMP = 1; // Start sampling
      
}



