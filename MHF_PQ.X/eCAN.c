

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


/************* START OF GLOBAL DEFINITIONS **********/


/************** END OF GLOBAL DEFINITIONS ***********/


/******************************************************************************
*
*    Function:			sendECAN
*    Description:       sends the message on a CAN bus
*
*    Arguments:			*message: a pointer to the message structure
*						containing all the information about the message
*	 Author:            Jatinder Gharoo
*
*
******************************************************************************/

//
//extern mID canTxMessage;
//extern mID canRxMessage;
//  unsigned int  data_word[2];   /* adicionado */

// extern ECAN1MSGBUF ecan1msgBuf;

void sendECAN(mID *message)
{
	unsigned long word0=0;
	unsigned long word1=0;
	unsigned long word2=0;

	/*
	Message Format:
	Word0 : 0bUUUx xxxx xxxx xxxx
			     |____________|||
 					SID10:0   SRR IDE(bit 0)
	Word1 : 0bUUUU xxxx xxxx xxxx
			   	   |____________|
						EID17:6
	Word2 : 0bxxxx xxx0 UUU0 xxxx
			  |_____||	     |__|
			  EID5:0 RTR   	  DLC

	Remote Transmission Request Bit for standard frames
	SRR->	"0"	 Normal Message
		"1"      Message will request remote transmission
	Substitute Remote Request Bit for extended frames
	SRR->	should always be set to "1" as per CAN specification

	Extended  Identifier Bit
	IDE-> 	"0"  Message will transmit standard identifier
	   		"1"  Message will transmit extended identifier

	Remote Transmission Request Bit for extended frames
	RTR-> 	"0"  Message transmitted is a normal message
			"1"  Message transmitted is a remote message
	Don't care for standard frames
	*/

	/* check to see if the message has an extended ID */
	if(message->frame_type==CAN_FRAME_EXT)
	{
		/* get the extended message id EID28..18*/
		word0=(message->id & 0x1FFC0000) >> 16;
		/* set the SRR and IDE bit */
		word0=word0+0x0003;
		/* the the value of EID17..6 */
		word1=(message->id & 0x0003FFC0) >> 6;
		/* get the value of EID5..0 for word 2 */
		word2=(message->id & 0x0000003F) << 10;
	}
	else
	{
		/* get the SID */
		word0=((message->id & 0x000007FF) << 2); // Coleta 11 bits LSB << 2
	}
	/* check to see if the message is an RTR message */
	if(message->message_type==CAN_MSG_RTR)
	{
		if(message->frame_type==CAN_FRAME_EXT)
			word2=word2 | 0x0200;
		else
			word0=word0 | 0x0002;
                

		ecan1msgBuf[message->buffer][0]=word0;
		ecan1msgBuf[message->buffer][1]=word1;
		ecan1msgBuf[message->buffer][2]=word2;
	}
	else
	{
		word2=word2+(message->data_length & 0x0F);
                
		ecan1msgBuf[message->buffer][0]=word0;
		ecan1msgBuf[message->buffer][1]=word1;
		ecan1msgBuf[message->buffer][2]=word2;
                
                
                ecan1msgBuf[message->buffer][3]= message->data_word[0]; /* Indice */
		ecan1msgBuf[message->buffer][4]= message->data_word[1]; /* Controle de tensão */
              
	}
        
	/* set the message for transmission */
        C1TR01CONbits.TXREQ0=1;


//        while(C1TR01CONbits.TXREQ0==1) // espera transmitir a mensagem;

// if(C1TR01CONbits.TXREQ0==0)
   /* Message was placed successfully on the bus */
//    LED2 = ~LED2; // Toggle led 2

}



/******************************************************************************
*
*    Function:			rxECAN
*    Description:       moves the message from the DMA memory to RAM
*
*    Arguments:			*message: a pointer to the message structure in RAM
*						that will store the message.
*	 Author:            Jatinder Gharoo
*
*
******************************************************************************/
void rxECAN(mID *message)
{
	unsigned int ide=0;
	unsigned int rtr=0;
	unsigned long id=0;

	/*
	Standard Message Format:
	Word0 : 0bUUUx xxxx xxxx xxxx
			     |____________|||
 					SID10:0   SRR IDE(bit 0)
	Word1 : 0bUUUU xxxx xxxx xxxx
			   	   |____________|
						EID17:6
	Word2 : 0bxxxx xxx0 UUU0 xxxx
			  |_____||	     |__|
			  EID5:0 RTR   	  DLC
	word3-word6: data bytes
	word7: filter hit code bits

	Remote Transmission Request Bit for standard frames
	SRR->	"0"	 Normal Message
			"1"  Message will request remote transmission
	Substitute Remote Request Bit for extended frames
	SRR->	should always be set to "1" as per CAN specification

	Extended  Identifier Bit
	IDE-> 	"0"  Message will transmit standard identifier
	   		"1"  Message will transmit extended identifier

	Remote Transmission Request Bit for extended frames
	RTR-> 	"0"  Message transmitted is a normal message
			"1"  Message transmitted is a remote message
	Don't care for standard frames
	*/

	/* read word 0 to see the message type */
	ide=ecan1msgBuf[message->buffer][0] & 0x0001;

	/* check to see what type of message it is */
	/* message is standard identifier */
	if(ide==0)
	{
		message->id=(ecan1msgBuf[message->buffer][0] & 0x1FFC) >> 2;
		message->frame_type=CAN_FRAME_STD;
		rtr=ecan1msgBuf[message->buffer][0] & 0x0002;
	}
	/* mesage is extended identifier */
	else
	{
		id=ecan1msgBuf[message->buffer][0] & 0x1FFC;
		message->id=id << 16;
		id=ecan1msgBuf[message->buffer][1] & 0x0FFF;
		message->id=message->id+(id << 6);
		id=(ecan1msgBuf[message->buffer][2] & 0xFC00) >> 10;
		message->id=message->id+id;
		message->frame_type=CAN_FRAME_EXT;
		rtr=ecan1msgBuf[message->buffer][2] & 0x0200;
	}
	/* check to see what type of message it is */
	/* RTR message */
	if(rtr==1)
	{
		message->message_type=CAN_MSG_RTR;
	}
	/* normal message */
	else
	{
		message->message_type=CAN_MSG_DATA;

                message->data_word[0]=(unsigned int)ecan1msgBuf[message->buffer][3] ;   /* adicionado */
                message->data_word[1]=(unsigned int)ecan1msgBuf[message->buffer][4] ;   /* adicionado */

		message->data_length=(unsigned char)(ecan1msgBuf[message->buffer][2] & 0x000F);
	}
	clearRxFlags(message->buffer);
}

/******************************************************************************
*
*    Function:			clearRxFlags
*    Description:       clears the rxfull flag after the message is read
*
*    Arguments:			buffer number to clear
*	 Author:            Jatinder Gharoo
*
*
******************************************************************************/
void clearRxFlags(unsigned char buffer_number)
{
	if((C1RXFUL1bits.RXFUL1) && (buffer_number==1))
		/* clear flag */
		C1RXFUL1bits.RXFUL1=0;
	/* check to see if buffer 2 is full */
	else if((C1RXFUL1bits.RXFUL2) && (buffer_number==2))
		/* clear flag */
		C1RXFUL1bits.RXFUL2=0;
	/* check to see if buffer 3 is full */
	else if((C1RXFUL1bits.RXFUL3) && (buffer_number==3))
		/* clear flag */
		C1RXFUL1bits.RXFUL3=0;
	else;

}


