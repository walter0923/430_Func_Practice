/*
 * Rf.h
 *
 *  Created on: 2019/8/6
 *      Author: Unier
 */

#ifndef RF_H_
#define RF_H_
#include "KEYSCAN.h"

#define RF_TEST 0
#define IIC_MODE 0 // 1: master 0: slave
#define IIC_ADDW		0x70
#define IIC_ADDR		0x71
#if IIC_MODE
#define IIC_PORT	GPIO_PORT_P9
#else
#define IIC_PORT	GPIO_PORT_P3
#endif



void IicInit(void);
void RfInit(void);
void RfSend(uint16_t data);
void RfDataSend(uint8_t data);
void RfCleanFIFO(void);

void IicSda(uint16_t hl);
void IicSck(uint16_t hl);
void IicStart(void);
void IicStop(void);
void IicAck(void);
void IicNack(void);
uint8_t IicWaitAck(void);
void IicWaitAckDelay(uint16_t delay);
void IicSendByte(uint8_t data);
uint32_t IiCByteWrite(uint8_t pBuffer);
void IiCSingleByteWrite(uint16_t add, uint8_t pBuffer);
void IiCFirstByteWrite(uint16_t add, uint8_t pBuffer);
void IiCNextByteWrite(uint8_t pBuffer);
void IiCFinishByteWrite(uint8_t pBuffer);

uint8_t IicReadByte(void);
uint32_t IiCRead(uint8_t WriteAddr, uint8_t* pBuffer, uint8_t numByteToRead);
void IicDelay10us(uint16_t Cnt);
uint8_t IicReadByte(void);

#endif /* RF_H_ */
