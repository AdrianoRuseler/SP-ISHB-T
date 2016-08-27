/* 
 * File:   initialization.h
 * Author: SuperRusla
 *
 * Created on September 17, 2012, 3:11 PM
 */

#ifndef INITIALIZATION_H
#define	INITIALIZATION_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* INITIALIZATION_H */


/* PTG Commands */
#define PTGCTRL (0x0<<0)
#define PTGADD  (0x1<<4)
#define PTGCOPY (0x1<<4)
#define PTGWHI  (0x4<<4) /* Wait for a Low to High edge input from selected
                            PTG trigger input as described by OPTION<3:0> */
#define PTGWLO  (0x4<<4) /* Wait for a High to Low edge input from selected
                            PTG trigger input as described by OPTION<3:0> */
#define PTGIRQ  (0x7<<4) /* Generate individual interrupt request as described by OPTION3<:0> */
#define PTGTRIG (0x8<<4)
#define PTGJMP  (0xA<<4)
#define PTGJMPC0 (0xC<<4)
#define PTGJMPC1 (0xE<<4)


/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/
// Define qual inversor, INV01 INV02 INV03
#define __INV03__

// Define se esta no TestBoard
//#define __TESTBOARD__
#define __PROTOTIPO__

//#define __MASTER__
#define __SLAVE__


// Fosc ~= 146,8MHz  -> FP = 73,4 MHz
#define fPWM 20000
#define Ninv 3
#define Fosc 146800000
//PCLKDIV
//110 (6)= Divide-by-64
//101 (5)= Divide-by-32
//100 (4)= Divide-by-16
//011 (3)= Divide-by-8
//010 (2)= Divide-by-4
//001 (1)= Divide-by-2
//000 (0)= Divide-by-1
#define PCLKDIVval 0
//#define Prescale (2^PCLKDIVval)

#define  PTPERval  7340   //(Fosc/((2^PCLKDIVval)*fPWM))
//#define PTPERval 61167

#define PHASE1inv01 0
#define PHASE1inv02  1223 // (PTPERval/6)
#define PHASE1inv03  2447 // (PTPERval/3)

#define PHASE2inv01 3670 // (PHASE1inv01 + PTPERval/2)
#define PHASE2inv02 4893 // (PHASE1inv02 + PTPERval/2)
#define PHASE2inv03 6117 // (PHASE1inv03 + PTPERval/2)

#define GpwmVal  3670 // (PTPERval/2)



#if defined(__TESTBOARD__)
/* Configuração de pinos para o Test Board */
/* RA4 -> LED3 */
/* RB0 -> AN2 */
/* RB1 -> LED1 */
/* RB4 -> LED2 */
 #define   LED1   LATBbits.LATB1
 #define   LED2   LATBbits.LATB4
 #define   LED3   LATAbits.LATA4


#elif defined(__PROTOTIPO__)
/* Configuração de pinos para o prototipo */
/* RA4 -> RESET */
/* RB0 -> LED1  */
/* RB1 -> AN3 */
/* RB4 -> LED2 */
 //LATBbits.LATB0 = 0;  // led 1
//LATBbits.LATB4 = 0; // Led 2

 #define   LED1   LATBbits.LATB0
 #define   LED2   LATBbits.LATB4
 #define   RESET   LATAbits.LATA4

#endif




/******************************************************************************/
/* Initialization Function Prototypes                                         */
/******************************************************************************/

void InitIO(void);         /* I/O  Initialization */
void INTx_IO_Init(void);   /* Init Interrupts  */
void initTMR1(void);      /* Timer 1 Initialization */
void initTMR3(void);      /* Timer 3 Initialization */
void initDMAch0CANTx(void); /* DMA Channel 0 Configuration for ECAN Transmission */
void initDMAch1CANRx(void); /* DMA Channel 1 Configuration for ECAN Reception */
void initDMAch2ADC1(void); /* DMA Channel 2 Configuration for ADC1 */
void initeCAN(void);  /*  */
void init_PWM(void);  /*  */
void initPWM_Slave(void);  /*   */
void initPWM_Master(void); /*   */
void initPTG(void); /*     */
void initADC1(void); /* ADC Initialization */
void initADC1DMAch2(void);/* ADC Initialization with DMA */
void Delayus(unsigned int delay);  /*     */