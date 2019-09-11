/*
 * RF.c
 *
 *  Created on: 2019/8/6
 *      Author: Unier
 */
#include <msp430.h>
#include "KEYSCAN.h"
#include "Rf.h"
#include "IO.h"
#include "HeadandFOOT.h"
#include "MEMORY.h"

uint16_t IicCnt;
void IicInit(void) {
#if IIC_MODE
	P1DIR |= BIT6 | BIT7;	//BIT7: SCL,  BIT7: SDA
	P1OUT |= BIT6 | BIT7;
	P3DIR |= BIT7;			//RF CS
	P3OUT |= BIT7;
#else
	P1DIR |= BIT6 | BIT7;	//BIT7: SCL,  BIT7: SDA
	P1OUT |= BIT6 | BIT7;
	P3DIR |= BIT7;			//RF CS
	P3OUT |= BIT7;
#endif
}

void IicSda(uint16_t hl) {
	if (hl) {
		P1OUT |= BIT7;
	} else {
		P1OUT &= ~BIT7;
	}
}

void IicSck(uint16_t hl) {
	if (hl) {
		P1OUT |= BIT6;
	} else {
		P1OUT &= ~BIT6;
	}
}

void IicAck(void) {
	P1DIR |= BIT7;
	IicSda(0);
	IicDelay10us(2);
	IicSck(0);
	IicDelay10us(2);
	IicSck(1);
	IicDelay10us(2);
	IicSck(0);
	IicSda(1);
}

void IicNack(void) {
	IicSda(1);
	IicDelay10us(2);
	IicSck(0);
	IicDelay10us(2);
	IicSck(1);
	IicDelay10us(2);
	IicSck(0);
}

void IicStart(void) {
	IicSda(1);  //Start condition
	IicSck(1);
	IicDelay10us(2);
	IicSda(0);
	IicDelay10us(2);
	IicSck(0);
	IicDelay10us(2);
}

void IicStop(void) {
	IicSda(0);
	IicSck(1);
	IicDelay10us(2);
	IicSda(0);
	IicDelay10us(2);
	IicSda(1);
}

uint8_t IicWaitAck(void) {
	uint8_t re;
	P1DIR &= ~BIT7;
	//IicDelay10us(10);
	IicSck(1);
	IicDelay10us(2);
#if IIC_MODE
	if ((P1IN & BIT7)) {
#else
		if ((P1IN & BIT7)) {
#endif
		re = 0;
	} else {
		re = 1;
	}
	IicSck(0);
	IicDelay10us(2);
	P1DIR |= BIT7;
	return re;
}

void IicWaitAckDelay(uint16_t delay) {
	uint8_t ack_status;

	//while(delay--){
	ack_status = IicWaitAck();
	if (ack_status) {
		//IicStop();
	} else {
		//break;
	}
	//}
}

void IicSendByte(uint8_t data) {
	uint8_t i;

	for (i = 0; i < 8; i++) {
		if (data & 0x80) {
			IicSda(1);
		} else {
			IicSda(0);
		}

		IicDelay10us(2);
		IicSck(1);
		IicDelay10us(2);
		IicSck(0);
		//if(i == 7){
		//IicSda(1);
		//}
		data <<= 1;
	}
	IicDelay10us(2);
	IicWaitAckDelay(10);
}

void IiCSingleByteWrite(uint16_t add, uint8_t pBuffer) {
	IicStart();

	IicSendByte(add);
	IicDelay10us(2);

	IicSendByte(pBuffer);
	IicDelay10us(2);

	IicStop();
}

void IiCFirstByteWrite(uint16_t add, uint8_t pBuffer) {
	IicStart();

	IicSendByte(add);
	IicDelay10us(2);

	IicSendByte(pBuffer);
	IicDelay10us(2);

}

void IiCNextByteWrite(uint8_t pBuffer) {
	IicSendByte(pBuffer);
	IicDelay10us(2);
}

void IiCFinishByteWrite(uint8_t pBuffer) {
	IicSendByte(pBuffer);
	IicDelay10us(2);

	IicStop();
}

uint8_t IicReadByte(void) {
	uint8_t i;
	uint8_t value = 0;

	P1DIR &= ~BIT7;
	//P1REN |= BIT7;
	//P1OUT |= BIT7;
	IicSck(0);
	IicDelay10us(5);
	for (i = 0; i < 8; i++) {
		value <<= 1;
		IicSck(1);
		IicDelay10us(2);
#if IIC_MODE
		if (P1IN & BIT7) {
#else
			if(P1IN & BIT7) {
#endif
			value |= 1;
		} else {
			value &= 0xFE;
		}
		IicSck(0);

		IicDelay10us(2);
	}
	P1DIR |= BIT7;
	return value;
}

uint32_t IiCRead(uint8_t WriteAddr, uint8_t* pBuffer, uint8_t numByteToRead) {
	uint8_t a;
	IicStart();

	IicSendByte(IIC_ADDW);
	//IicDelay10us(10);

	IicSendByte(WriteAddr);
	//IicDelay10us(10);

	IicStart();

	IicSendByte(IIC_ADDR);
	IicDelay10us(2);

	while (numByteToRead) {
		*pBuffer = IicReadByte();

		if (numByteToRead != 1) {
			IicAck();
		} else {
			IicNack();
		}
		pBuffer++;
		numByteToRead--;
	}

	IicStop();
	return 0;
}

void IicDelay10us(uint16_t Cnt) {
	IicCnt = 0;
	while (IicCnt < Cnt)
		;
}

void RfInit(void) {
	//IicInit();
	IiCFirstByteWrite(IIC_ADDW, 0x0A);
	IiCNextByteWrite(0x25);
	IiCFinishByteWrite(0x03);

	IiCFirstByteWrite(IIC_ADDW, 0x02);
	IiCNextByteWrite(0x40);
	IiCFinishByteWrite(0x60);

	IiCFirstByteWrite(IIC_ADDW, 0x05);
	IiCNextByteWrite(0x7F);
	IiCFinishByteWrite(0xA6);

	IiCFirstByteWrite(IIC_ADDW, 0x0F);
	IiCNextByteWrite(0x66);
	IiCFinishByteWrite(0x1D);

	IiCFirstByteWrite(IIC_ADDW, 0x0D);
	IiCNextByteWrite(0x60);
	IiCFinishByteWrite(0x03);

	IiCFirstByteWrite(IIC_ADDW, 0x1A);
	IiCNextByteWrite(0x00);
	IiCFinishByteWrite(0xF7);

	IiCFirstByteWrite(IIC_ADDW, 0x01);		//open auto ack
	IiCNextByteWrite(0x20);
	IiCFinishByteWrite(0x7F);

	IiCFirstByteWrite(IIC_ADDW, 0x0B);		//resend 16 times
	IiCNextByteWrite(0x8F);
	IiCFinishByteWrite(0x7F);
}

void RfDataSend(uint8_t data) {
	uint8_t st[2] = { 0x00 };

	//RfCleanFIFO();

	IiCFirstByteWrite(IIC_ADDW, 0x26);
	IiCNextByteWrite(0x80);
	IiCFinishByteWrite(0x80);

	IiCFirstByteWrite(IIC_ADDW, 0x27);
	IiCNextByteWrite(0x01);
	IiCFinishByteWrite(data);

	IiCFirstByteWrite(IIC_ADDW, 0x00);
	IiCNextByteWrite(0x80);
	IiCFinishByteWrite(0x0A);
	while (1) {
		IiCRead(0x16, st, 2);
		if (st[1] & 0x40) {
			break;
		}
	}
}

void RfCleanFIFO(void){
	IiCFirstByteWrite(IIC_ADDW, 0x26);		//
	IiCNextByteWrite(0x80);
	IiCFinishByteWrite(0x80);
}
