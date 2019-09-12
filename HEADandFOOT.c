/*
 * HEADandFOOT.c
 *
 *  Created on: 2019/8/10
 *      Author: Unier
 */
#include <msp430.h>
#include "KEYSCAN.h"
#include "Rf.h"
#include "IO.h"
#include "HeadandFOOT.h"
#include "MEMORY.h"

uint32_t HeadStepCnt;
uint32_t FootStepCnt;
uint16_t HeadStepTC10us;
uint16_t FootStepTC10us;
uint16_t HeadStepTC1ms;
uint16_t FootStepTC1ms;
uint16_t HeadLastMotion;
uint16_t FootLastMotion;
uint16_t HeadNowPosition;
uint16_t FootNowPosition;
uint16_t HeadTargetPosition;
uint16_t FootTargetPosition;
uint16_t HeadDeadPoint;
uint16_t FootDeadPoint;
uint8_t HNMD_Flag = 0;
uint8_t FNMD_Flag = 0;

void HeadFootInit(void){
	HeadSetpCntInit();
	FootSetpCntInit();
	HeadInit();
	FootInit();
	//HeadNoMoveDetc();			// wait move to dead point
	//FootNoMoveDetc();			// wait move to dead point
}

void HeadInit(void){
	P1DIR |= HeadGrup;
	HeadMotoSTOP();				//make sure motor not move
}

void FootInit(void){
	P2SEL &= ~FootGrup;
	P2DIR |= FootGrup;
	//P3DIR |= BIT5;
	//P3OUT |= BIT5;				//dead triger
	FootMotoSTOP();				//make sure motor not move
}

void HeadSetpCntInit(void){
	P1DIR &= ~HeadStepCntPin;
	P1REN |= HeadStepCntPin;                            // Enable P1.4 internal resistance
	P1OUT |= HeadStepCntPin;                            // Set P1.4 as pull-Up resistance
	P1IE  |= HeadStepCntPin;                             // P1.4 interrupt enabled
	P1IES &= ~HeadStepCntPin;                            // P1.4 Hi/Lo edge
	P1IFG &= ~HeadStepCntPin;                           // P1.4 IFG cleared
}

void FootSetpCntInit(void){
	P2DIR &= ~FootStepCntPin;
	//P2REN |= FootStepCntPin;                            // Enable P1.4 internal resistance
	//P2OUT |= FootStepCntPin;                            // Set P1.4 as pull-Up resistance
	P2IE  |= FootStepCntPin;                             // P1.4 interrupt enabled
	P2IES &= ~FootStepCntPin;                            // P1.4 Hi/Lo edge
	P2IFG &= ~FootStepCntPin;                           // P1.4 IFG cleared
}

void HeadNoMoveDetc(void){
    if(HNMD_Flag == 1){
        if(HeadStepTC1ms > 1000){
            HeadMotoSTOP();			//stop move
            HeadDeadPoint = 1;
            NowFunc = 0;
            HNMD_Flag = 0;
        }
    }
}

void FootNoMoveDetc(void){
    if(FNMD_Flag == 1){
        if(FootStepTC1ms > 1000){
            FootMotoSTOP();			//stop move
            FootDeadPoint = 1;
            NowFunc = 0;
            FNMD_Flag = 0;
        }
    }
}

void HeadUpDown(uint8_t ud){
	//HeadMotoSTOP();
	HeadStepCnt = 0;			//reset step cnt
	HeadStepTC1ms = 0;

	if(ud != HeadLastMotion){
		/*if(HeadDeadPoint == 1){
			P3OUT &= ~BIT5;
			TC = 0;
			while(TC < 10000){;}

			P3OUT |= BIT5;
			HeadDeadPoint = 0;
		}else{*/
			MotoProtectDelay();
			HeadStepCnt = 0;
		//}
	}

	if(ud){
	    P1OUT |= HeadDownPin;
		P1OUT &= ~HeadUpPin;
		//
	}else{
		P1OUT |= HeadUpPin;
		P1OUT &= ~HeadDownPin;
	}

	HNMD_Flag = 1;
	HeadLastMotion = ud;
}

void FootUpDown(uint8_t ud){
	//FootMotoSTOP();
	FootStepCnt = 0;			//reset step cnt
	FootStepTC1ms = 0;
	if(ud != FootLastMotion){
		/*if(FootDeadPoint == 1){
			P3OUT &= ~BIT5;
			TC = 0;
			while(TC < 10000){;}

			P3OUT |= BIT5;
			FootDeadPoint = 0;
		}else{*/
			MotoProtectDelay();
			FootStepCnt = 0;
		//}
	}

	if(ud){
	    P2OUT |= FootDownPin;
		P2OUT &= ~FootUpPin;
	}else{
		P2OUT |= FootUpPin;
		P2OUT &= ~FootDownPin;
	}

	FNMD_Flag = 1;
	FootLastMotion = ud;
}

void MotoProtectDelay(void){
	/*HeadStepTC1ms = 0;
	while(HeadStepTC1ms < 10){;}
	HeadStepTC10us = 0;
	HeadStepTC1ms = 0;
	FootStepTC10us = 0;
	FootStepTC1ms = 0;*/
	TC = 0;
	while(TC < 50){;}

}

void HeadMotoSTOP(void){
	P1OUT |= HeadGrup;			//make sure motor not move
}

void FootMotoSTOP(void){
	P2OUT |= FootGrup;			//make sure motor not move
}

void HeadHallFunc(void){
    if(P1IN & HeadStepCntPin){
        HeadStepTC1ms = 0;
        if(HeadLastMotion == UP){
            HeadNowPosition ++;
        }else if(HeadLastMotion == DOWN){
            if(HeadNowPosition > 0){
                HeadNowPosition --;
            }
        }

        if(NowFunc != ZERO){
            if(HeadNowPosition == HeadTargetPosition){
                HeadMotoSTOP();
                //NowFunc = 0;
            }
        }
    }
    HeadStepTC1ms = 0;
}

void FootHallFunc(void){
    if(P2IN & FootStepCntPin){
        FootStepTC1ms = 0;
        if(FootLastMotion == UP){
            FootNowPosition ++;
        }else if(FootLastMotion == DOWN){
            if(FootNowPosition > 0){
                FootNowPosition --;
            }
        }

        if(NowFunc != ZERO){
            if(FootNowPosition == FootTargetPosition){
                FootMotoSTOP();
            }
        }
    }
    FootStepTC1ms = 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    P1IFG &= ~HeadStepCntPin;
	if(P1IFG & HeadStepCntPin){
	    //HeadStepTC1ms = 0;
	    HeadHallFunc();
		FootHallFunc();
		//HeadNowPotition += HeadStepCnt;
	}
	//HeadStepTC1ms = 0;
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){
    P2IFG &= ~FootStepCntPin;
	if(P2IFG & FootStepCntPin){
	    //FootStepTC1ms = 0;
	    FootHallFunc();
	    HeadHallFunc();
		//HeadNowPotition += HeadStepCnt;
	}
	//FootStepTC1ms = 0;

}
