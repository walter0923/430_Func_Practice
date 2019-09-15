/*
 * IO.c
 *
 *  Created on: 2019/8/9
 *      Author: Unier
 */
#include <msp430.h>
#include "KEYSCAN.h"
#include "Rf.h"
#include "IO.h"
#include "HeadandFOOT.h"
#include "MEMORY.h"

uint16_t DutyStep = 1;
uint16_t MOnePosition = 0;
/*
void SightInit(void){
	P2SEL &= ~BIT6;
	P2DIR |= BIT6;
	P2OUT |= BIT6;
}

void SightOnOff(void){
	P2OUT &= ~BIT6;
	ScanDelay10us(1000);
	P2OUT |= BIT6;
}

void TorchInit(void){
	P1DIR |= BIT5;
	P1OUT &= ~BIT5;
}

void TorchOnOff(void){
	P1OUT ^= BIT5;
}
*/
void GpioInit(void){
	P3DIR |= BIT4;
	P3OUT |= BIT4;
}

void GLight(void){
	P3OUT ^= BIT4;
}

void ShakeInit(void){
	P2DIR |= BIT1;                            // P2.2 and P2.3 output
	P2SEL |= BIT1;                            // P2.2 and P2.3 options select
	//P8OUT |= BIT4;
	TA1CCR0 = 51-1;                          // PWM Period
	TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
	TA1CCR1 = 51 - 1;                            // CCR1 PWM duty cycle
	TA1CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR
}

void Shake_Off(void){
	DutyStep = 1;
	ShakeDuty = 51 - 1;
	TA1CCR1 = ShakeDuty;
	//Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, ShakeDuty);
}

void Shake_On(void){
	switch(DutyStep){
		case 1:
			ShakeDuty = 5;
			DutyStep = 2;
			break;

		case 2:
			ShakeDuty = 4;
			DutyStep = 3;
			break;

		case 3:
			ShakeDuty = 1;
			DutyStep = 1;
			break;
	}
	//Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, ShakeDuty);
	TA1CCR1 = ShakeDuty;
}
