/* 
 * File:   protect.h
 * Author: SuperRusla
 *
 * Created on March 19, 2013, 1:41 PM
 */

#ifndef PROTECT_H
#define	PROTECT_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif
#endif	/* PROTECT_H */

/******************************************************************************/
/* Initialization Function Prototypes                                         */
/******************************************************************************/


/*
0x00 -> Normal operation
0x01 -> Activate PWM
0x02 -> Desactive PWM
0x03 -> Problemas no Módulo 1
0x04 -> Problemas no Módulo 2
0x05 -> Problemas no Módulo 3
 * 
.
.
.
0x3D -> INT0 on Master
0x3E -> INT1 on Master
0x3F -> INT2 on Master
*/


/******************************************************************************/
/* Initialization Function Prototypes                                         */
/******************************************************************************/

void PWMdes(void);         /* Desabilita modulos PWM */
void SendStatus(unsigned int status);   /*Send converter status (6 bits only)*/
void StatusCheckSlave(unsigned int status); /* Verify and decide what to do */
unsigned int StatusCheckMaster(unsigned int status); /* return action from status  */