#include <msp430.h>
#include "KEYSCAN.h"
#include "Rf.h"
#include "IO.h"
#include "HeadandFOOT.h"
#include "MEMORY.h"

/*
 *
 * Date 2019 09/01
 *
 */

void MainFunction(uint16_t func);

uint8_t a[2];
uint16_t ShakeDuty = 512 - 1;
uint16_t NowFunc;

void ClockInit(void){
	if(CALBC1_1MHZ == 0xFF){
		while(1);
	}

	DCOCTL = 0;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
}

void main(void){
//uint16_t i,j;

    WDTCTL = WDTPW + WDTHOLD;
    ClockInit();
    //SightInit();
    //TorchInit();
    TimerInit();
    //ScanInit();
    IicInit();
#if IIC_MODE
#else
    GpioInit();
    ShakeInit();
    HeadFootInit();
    MemoryInit();
    MemoryRead();
#endif

    __bis_SR_register(GIE);

    //P3OUT &= ~BIT7;

    ScanDelay10us(20000);
    //P3OUT |= BIT7;
#if IIC_MODE
#else
    MainFunction(ZERO);   //reset Head & Foot position
#endif

    RfInit();

#if RF_TEST
    P4DIR |= BIT1 | BIT2 | BIT3 | BIT4 | BIT5;	//for test
    P4OUT |= BIT1 | BIT2 | BIT3 | BIT4 | BIT5;

#endif

    while(1){
#if IIC_MODE
    	KeyProc();
    	//RfDataSend(0xAC);
#else
    	IiCFirstByteWrite(IIC_ADDW, 0x25);
    	IiCNextByteWrite(0x80);
    	IiCFinishByteWrite(0x80);
    	IiCFirstByteWrite(IIC_ADDW, 0x00);
    	IiCNextByteWrite(0x40);
    	IiCFinishByteWrite(0x0A);
		a[0] = 0x00;
		a[1] = 0x00;
		while(1){
			IiCRead(0x16, a, 2);
			if(a[1] & 0x40){
				break;
			}
		}
		a[0] = 0x00;
		a[1] = 0x00;
		IiCRead(0x28, a, 2);
		MainFunction(a[1]);
#if RF_TEST
		P4OUT |= BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
		P8OUT |= BIT1;
		P4OUT &= ~(a[1] << 1);

		if(a[1] == 0x01){
			P4DIR |= 0x02;
			P4OUT &= ~0x02;
		}
#endif
#endif

    }
}
#if IIC_MODE
#else
void MainFunction(uint16_t func){
	NowFunc = func;
	switch(NowFunc){
		case G_LIGHT:
			SaveFlag = 0;
			GLight();
			break;

		case SHAKE:
			SaveFlag = 0;
			Shake_On();
			break;

		case OFF:
			SaveFlag = 0;
			Shake_Off();
			break;

		case HEAD_UP:
			SaveFlag = 0;
			HeadTargetPosition = 1429;
			HeadUpDown(UP);
			break;

		case HEAD_DOWN:
			SaveFlag = 0;
			HeadTargetPosition = 1429;
			HeadUpDown(DOWN);
			break;


		case FOOT_UP:
			SaveFlag = 0;
			FootTargetPosition = 0xFF;
			FootUpDown(UP);
			break;

		case FOOT_DOWN:
			SaveFlag = 0;
			FootTargetPosition = 0xFF;
			FootUpDown(DOWN);
			break;

		case HFOFF:
			SaveFlag = 0;
			FootMotoSTOP();
			HeadMotoSTOP();
			break;

		case ZERO:
			//FootNowPosition = 0;
			//HeadNowPosition = 0;
			FootUpDown(DOWN);
			HeadUpDown(DOWN);
			break;

		case TV:
			SaveFlag = 0;
			HeadTargetPosition = TvHeadPosition;
			FootTargetPosition = TvFootPosition;
			if(FootNowPosition > FootTargetPosition){
				FootUpDown(DOWN);
			}else if(FootNowPosition < FootTargetPosition){
				FootUpDown(UP);
			}

			if(HeadNowPosition > HeadTargetPosition){
				HeadUpDown(DOWN);
			}else if(HeadNowPosition < HeadTargetPosition){
				HeadUpDown(UP);
			}


			break;

		case ZG:
			SaveFlag = 0;
			HeadTargetPosition = ZgHeadPosition;
			FootTargetPosition = ZgFootPosition;

			if(FootNowPosition > FootTargetPosition){
				FootUpDown(DOWN);
			}else if(FootNowPosition < FootTargetPosition){
				FootUpDown(UP);
			}

			if(HeadNowPosition > HeadTargetPosition){
				HeadUpDown(DOWN);
			}else if(HeadNowPosition < HeadTargetPosition){
				HeadUpDown(UP);
			}

			break;

		case FLAT:
			SaveFlag = 0;
			HeadTargetPosition = 0;
			FootTargetPosition = 0;
			FootUpDown(DOWN);
			HeadUpDown(DOWN);
			break;

		case SET:
			SaveFlag = 1;
			break;

		case M_ONE:
			if(SaveFlag == 1){
				Mem[0] = HeadNowPosition;
				Mem[3] = FootNowPosition;
				MemoryWrite();
				MemoryRead();
				SaveFlag = 0;
			}else{
				HeadTargetPosition = Mem[0];
				FootTargetPosition = Mem[3];

				if(FootNowPosition > FootTargetPosition){
					FootUpDown(DOWN);
				}else if(FootNowPosition < FootTargetPosition){
					FootUpDown(UP);
				}

				if(HeadNowPosition > HeadTargetPosition){
					HeadUpDown(DOWN);
				}else if(HeadNowPosition < HeadTargetPosition){
					HeadUpDown(UP);
				}
			}
			break;

		case M_TWO:
			if(SaveFlag == 1){
				Mem[1] = HeadNowPosition;
				Mem[4] = FootNowPosition;
				MemoryWrite();
				MemoryRead();
				SaveFlag = 0;
			}else{
				HeadTargetPosition = Mem[1];
				FootTargetPosition = Mem[4];
				if(FootNowPosition > FootTargetPosition){
					FootUpDown(DOWN);
				}else if(FootNowPosition < FootTargetPosition){
					FootUpDown(UP);
				}

				if(HeadNowPosition > HeadTargetPosition){
					HeadUpDown(DOWN);
				}else if(HeadNowPosition < HeadTargetPosition){
					HeadUpDown(UP);
				}
			}
			break;

		case M_THREE:
			if(SaveFlag == 1){
				Mem[2] = HeadNowPosition;
				Mem[5] = FootNowPosition;
				MemoryWrite();
				MemoryRead();
				SaveFlag = 0;
			}else{
				HeadTargetPosition = Mem[2];
				FootTargetPosition = Mem[5];
				if(FootNowPosition > FootTargetPosition){
					FootUpDown(DOWN);
				}else if(FootNowPosition < FootTargetPosition){
					FootUpDown(UP);
				}

				if(HeadNowPosition > HeadTargetPosition){
					HeadUpDown(DOWN);
				}else if(HeadNowPosition < HeadTargetPosition){
					HeadUpDown(UP);
				}
			}
			break;
	}
}
#endif
