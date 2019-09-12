#include <msp430.h>
#include "KEYSCAN.h"
#include "Rf.h"
#include "IO.h"
#include "HeadandFOOT.h"
#include "MEMORY.h"
uint16_t ScanLine[5] = { BIT0, BIT1, BIT2, BIT3, BIT4 };
uint16_t KeyPad[5] = { BIT0, BIT1, BIT2, BIT3, BIT4 };
/*uint16_t KeyMap[5][5] = {
 {TV, ZG, HEAD_UP, FOOT_UP, STOP},
 {HEAD_DOWN, FOOT_DOWN, WBPA_N, WBPA_P, FOURTY},
 {FOURTH_FIVE, FIFTY, OFF, FLAT, SHAKE},
 {M_ONE, M_TWO, M_THREE, G_LIGHT, SET},
 {TORCH, 0x16, 0x17, 0x18, 0x19},
 };*/

uint16_t KeyMap[5][5] = {
		{ TV, ZG, HEAD_UP, FOOT_UP, TORCH},
		{ STOP, HEAD_DOWN, FOOT_DOWN, WBPA_N, 0x16 },
		{ WBPA_P, FOURTY, FOURTH_FIVE,FIFTY, 0x17 },
		{ OFF, FLAT, SHAKE, M_ONE, 0x18 },
		{ M_TWO, M_THREE, G_LIGHT, SET, 0x19}
};
uint16_t TC;
uint16_t LastKey = 0x00, KeyTemp = 0x00;
uint16_t SightCnt;

void TimerInit(void) {
	CCTL0 = CCIE;
	CCR0 = 100;
	TACTL = TASSEL_2 + MC_1;
}
/*
void ScanInit(void) {
	TimerInit();

	P1DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4;			//scan port
	P1OUT |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4; 	//reset all scan line to High

	P3DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4);		//key port
	//P3REN |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4;
	//P3OUT |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4;

	P2DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4;			// LED for test
	P2OUT |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4;

}

void KeyProc(void) {
	uint16_t i, j, key_temp = 0x1F, key = 0;
	//LastKey = 0x00;
	for (i = 0; i < 5; i++) {
		key_temp = 0x1F;
		P1OUT |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4; // reset all scan line to low
		P1OUT &= ~(ScanLine[i]);					//output scan line

		key_temp = P3IN & (BIT0 | BIT1 | BIT2 | BIT3 | BIT4);

		if (key_temp != 0x1F) {
			ScanDelay10us(100);				//delay
			if ((key_temp == (P3IN & (BIT0 | BIT1 | BIT2 | BIT3 | BIT4)))) {
				KeyTemp = 1;
				for (j = 0; j < 5; j++) {
					if (!(key_temp & KeyPad[j])) {
						key = KeyMap[i][j];
						break;
					}
				}
			}

			if ((key != 0) && (LastKey == 0x00)) {
				P2OUT |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4;	// test LED
				P2OUT &= ~key;  //app in here
				if (key == TORCH) {
					TorchOnOff();
				} else {
					RfDataSend(key);
				}
				SightOnOff();
				LastKey = key;
				break;
			}
		}
		ScanDelay10us(10);
	}

	if (key == 0x00) {
		if ((LastKey == HEAD_UP) || (LastKey == FOOT_UP)
				|| (LastKey == HEAD_DOWN) || (LastKey == FOOT_UP)) {
			//send stop
			RfDataSend(HFOFF);
		}

		LastKey = 0x00;
	}
	if (LastKey == 0x00) {
		KeyTemp = 0;
	}

	//ScanDelay10us(1000);
	//GPIO_setOutputHighOnPin(GPIO_PORT_P6, (GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4));
}
*/
void ScanDelay10us(uint16_t Cnt) {
	TC = 0;
	while (TC < Cnt) {;}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMERA0_ISR(void) {
	TC++;
	IicCnt++;
	HeadStepTC10us ++;
	FootStepTC10us ++;

	if (HeadStepTC10us >= 100) {
		HeadStepTC10us = 0;
		HeadStepTC1ms++;
		/*if ((NowFunc == ZG) || (NowFunc == TV) || (NowFunc == HEAD_UP) || (NowFunc == HEAD_DOWN) || (NowFunc == FLAT) || (NowFunc == ZERO)) {
			HeadNoMoveDetc();	//30ms no set not cnt shout down head & foor motor
		}*/
	}

	if (FootStepTC10us >= 100) {
		FootStepTC10us = 0;
		FootStepTC1ms++;
		/*if ((NowFunc == ZG) || (NowFunc == TV) || (NowFunc == FOOT_UP) || (NowFunc == FOOT_DOWN) || (NowFunc == FLAT) || (NowFunc == ZERO)) {
			FootNoMoveDetc();	//30ms no set not cnt shout down head & foor motor
		}*/
	}

	TACTL &= ~TAIFG;
}
