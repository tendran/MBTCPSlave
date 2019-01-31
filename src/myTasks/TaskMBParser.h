/*
 * TaskMBParser.h
 *
 *  Created on: 25.01.2019
 *      Author: E6430
 */

#ifndef MYTASKS_TASKMBPARSER_H_
#define MYTASKS_TASKMBPARSER_H_

#include "stdlib.h"
#include "FreeRTOS.h"
#include "string.h"
#include "task.h"
#include "stream_buffer.h"
#include "message_buffer.h"
#include "queue.h"
#include "TaskW5500.h"		// <-- for common message declaration

#define MAX_REGS_IN_FRAME	126
// data model defines
#define HOLDING_REGS_COUNT	200


typedef struct{
	StreamBufferHandle_t 	inputStream;
	QueueHandle_t 			outputMessageQueue;
	uint8_t					sourceSocketNo;
}sParserInOutParameter;

typedef enum{
	FCN_READ_HOLD_REGS			= (uint8_t)0x03,
	FCN_WRITE_SING_REG			= (uint8_t)0x06,
	FCN_WRITE_MULT_REGS			= (uint8_t)0x10
}eFcnCode;

typedef struct __attribute__((packed)){
	uint16_t		TransID;
	uint16_t		ProtocolID;
	uint16_t		Length;		// number of bytes of data + 1 byte for unit id
	uint8_t			UnitID;
}sMBAPHeader;

typedef struct __attribute__((packed)){
	eFcnCode		FunctionCode;
	uint8_t		Data[2*MAX_REGS_IN_FRAME + 1];
}sPDU;

typedef struct __attribute__((packed)){
	sMBAPHeader		MBAP;
	sPDU			PDU;
}sADUFrame;

typedef struct{
	int16_t	HoldingRegs[HOLDING_REGS_COUNT];
}sDataModel;

#define BYTES_IN_WORD_SWAP(word) ((word>>8) | (word<<8))

// global data model
sDataModel		DataModel;

void vTaskMBParser(void *pvParameters);

// processing different function codes
void puiProcessReadHoldingRegs(sADUFrame *request_frame);

// data model functions
void DataModelInit();
int16_t uiGetRegisterValue(uint16_t register_no);					// register_no 0... HOLDING_REGS_COUNT-1
void uiSetRegisterValue(uint16_t register_no, int16_t value);		// register_no 0... HOLDING_REGS_COUNT-1

#endif /* MYTASKS_TASKMBPARSER_H_ */
