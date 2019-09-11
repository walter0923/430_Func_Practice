/*
 * KEYSCAN.h
 *
 *  Created on: 2019/8/6
 *      Author: Unier
 */

#ifndef KEYSCAN_H_
#define KEYSCAN_H_
#include "KEYSCAN.h"

#define uint32_t	unsigned long int
#define uint16_t	unsigned int
#define uint8_t		unsigned char
#define	TV			0x01
#define	ZG			0x02
#define	HEAD_UP		0x03
#define	FOOT_UP		0x04
#define	STOP		0x05
#define	HEAD_DOWN	0x06
#define	FOOT_DOWN	0x07
#define	WBPA_N		0x08
#define	WBPA_P		0x09
#define	FOURTY		0x0A
#define	FOURTH_FIVE	0x0B
#define	FIFTY		0x0C
#define	OFF			0x0D
#define	FLAT		0x0E
#define	SHAKE		0x0F
#define	M_ONE		0x10
#define	M_TWO		0x11
#define	M_THREE		0x12
#define	G_LIGHT		0x13
#define	SET			0x14
#define	TORCH		0x15
#define	HFOFF		0x19
#define ZERO		0xEE

extern uint16_t TC;
extern uint16_t IicCnt;
extern uint16_t KeyMap[5][5];
extern uint16_t NowFunc;
extern uint16_t SightCnt;

void TimerInit(void);
void ScanInit(void);
void KeyProc(void);
void ScanDelay10us(uint16_t Cnt);

#endif /* KEYSCAN_H_ */
