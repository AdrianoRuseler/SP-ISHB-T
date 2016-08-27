/*!
* \file ecan.h
* \brief Header file for CAN related functions
* \details 
* \author Jatinder Gharoo
* \author Adriano Ruseler
* \version 1.1
* \copyright The MIT License (MIT).
*/

#ifndef __ecan_H
#define __ecan_H

/* CAN Baud Rate Configuration 		*/
/* Fosc ~= 146,8MHz  -> FP = 73,4 MHz  */
#define FCAN  	    73400000   /* CANCKS = 0; ECAN Module Clock (FCAN)  = FP */
#define BITRATE     1000000  /* 1 Mbps */
#define NTQ 	    20		// 20 Time Quanta in a Bit Time
// #define BRP_VAL	    ((FCAN/(2*NTQ*BITRATE))-1)



/* defines used in ecan.c */
/* ECAN message buffer length */
#define ECAN1_MSG_BUF_LENGTH 	4 /* Numero de mensagens contidas na RAM */

/* ECAN message type identifiers */
#define CAN_MSG_DATA	0x01 /* 0b001 Data Frame ? carries data from transmitter to the receivers */
#define CAN_MSG_RTR     0x02 /* 0b010 Remote Frame ? transmitted by a node on the bus, to request transmission of a data
frame with the same identifier from another node */

#define CAN_FRAME_EXT	0x03 /* 0b011  Extended Data Frame ? intended for extended messages that use 29 identifier bits */
#define CAN_FRAME_STD	0x04 /* 0b100 Standard Data Frame ? intended for standard messages that use 11 identifier bits */
#define CAN_BUF_FULL	0x05 /* 0b101 */
#define CAN_BUF_EMPTY	0x06 /* 0b110 */

/* filter and mask defines */
/* Macro used to write filter/mask ID to Register CiRXMxSID and
CiRXFxSID. For example to setup the filter to accept a value of
0x123, the macro when called as CAN_FILTERMASK2REG_SID(0x123) will
write the register space to accept message with ID ox123
USE FOR STANDARD MESSAGES ONLY */
#define CAN_FILTERMASK2REG_SID(x) ((x & 0x07FF)<< 5) /* AND nos Bits (0-10) << 5 */

/* the Macro will set the "MIDE" bit in CiRXMxSID */
#define CAN_SETMIDE(sid) (sid | 0x0008) /* OR no Bit 3 - Seta Bit 3 (0-15)*/
/* the macro will set the EXIDE bit in the CiRXFxSID to
accept extended messages only */
#define CAN_FILTERXTD(sid) (sid | 0x0008) /* Seta Bit 3 (0-15) */
/* the macro will clear the EXIDE bit in the CiRXFxSID to
accept standard messages only */
#define CAN_FILTERSTD(sid) (sid & 0xFFF7) /* AND com Zero no bit 3  (Zera bit 3)*/
/* Macro used to write filter/mask ID to Register CiRXMxSID, CiRXMxEID and
CiRXFxSID, CiRXFxEID. For example to setup the filter to accept a value of
0x123, the macro when called as CAN_FILTERMASK2REG_SID(0x123) will
write the register space to accept message with ID 0x123
USE FOR EXTENDED MESSAGES ONLY */
#define CAN_FILTERMASK2REG_EID0(x) (x & 0xFFFF)
#define CAN_FILTERMASK2REG_EID1(x) (((x & 0x1FFC)<< 3)|(x & 0x3)) /* Coleta Bit 2-12 <<3 | Coleta bit 0-1 */

/* DMA CAN Message Buffer Configuration */
typedef unsigned int ECAN1MSGBUF [ECAN1_MSG_BUF_LENGTH][8];

#ifdef _HAS_DMA_
extern ECAN1MSGBUF  ecan1msgBuf __attribute__((space(dma)));
#else
extern ECAN1MSGBUF  ecan1msgBuf __attribute__((space(xmemory)));

// extern ECAN1MSGBUF  ecan1msgBuf __attribute__((space(ymemory)));
#endif

/**
 \struct mID
 *  CAN message structure in RAM
 */

typedef struct{
	/* keep track of the buffer status */
	unsigned char buffer_status;
	/* RTR message or data message */
	unsigned char message_type;
	/* frame type extended or standard */
	unsigned char frame_type;
	/* buffer being used to send and receive messages */
	unsigned char buffer;
	/* 29 bit id max of 0x1FFF FFFF
	*  11 bit id max of 0x7FF */
	unsigned long id;
//	unsigned char data[8];
        unsigned int  data_word[2];   /* adicionado */
	unsigned char data_length;
}mID;



/*! \fn void sendECAN(mID *message)
\brief Sends CAN message.
\param mID *message
\return void
 * 
 * 
*/
void sendECAN(mID *message);

/*! \fn void rxECAN(mID *message)
\brief Receives CAN message.
\param mID *message
\return void
 * 
 *
*/
void rxECAN(mID *message);

/*! \fn void clearRxFlags(unsigned char buffer_number)
\brief Clear CAN message flags.
\param unsigned char buffer_number
\return void
 *
 * 
 *
*/
void clearRxFlags(unsigned char buffer_number);

#endif
