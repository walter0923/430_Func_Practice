/*
 * HEADandFOOT.h
 *
 *  Created on: 2019/8/10
 *      Author: Unier
 */

#ifndef HEADANDFOOT_H_
#define HEADANDFOOT_H_

#define prototype (0)

#define FULLSTEP 1429

#define UP 0
#define DOWN 1

#if prototype
#define HeadUpPin	BIT3
#define HeadDownPin	BIT4
#else
#define HeadUpPin	BIT4
#define HeadDownPin	BIT3
#endif

#define HeadGrup (HeadUpPin | HeadDownPin)
#define HeadStepCntPin BIT0

#if prototype
#define FootUpPin	BIT6
#define FootDownPin	BIT7
#else
#define FootUpPin	BIT7
#define FootDownPin	BIT6
#endif

#define FootGrup (FootUpPin | FootDownPin)
#define FootStepCntPin BIT5

#define TvHeadPosition	1001
#define TvFootPosition	429

#define ZgHeadPosition	429
#define ZgFootPosition	715


extern uint32_t HeadStepCnt;
extern uint32_t FootStepCnt;
extern uint16_t HeadStepTC10us;
extern uint16_t FootStepTC10us;
extern uint16_t HeadStepTC1ms;
extern uint16_t FootStepTC1ms;
extern uint16_t HeadLastMotion;
extern uint16_t FootLastMotion;
extern uint16_t HeadNowPosition;
extern uint16_t FootNowPosition;
extern uint16_t HeadTargetPosition;
extern uint16_t FootTargetPosition;
extern uint16_t HeadDeadPoint;
extern uint16_t FootDeadpoint;

void HeadSetpCntInit(void);
void HeadFootInit(void);
void HeadInit(void);
void HeadUpDown(uint8_t ud);

void FootSetpCntInit(void);
void FootInit(void);
void FootUpDown(uint8_t ud);

void HeadNoMoveDetc(void);
void FootNoMoveDetc(void);
void MotoProtectDelay(void);
void HeadMotoSTOP(void);
void FootMotoSTOP(void);
void HeadHallFunc(void);
void FootHallFunc(void);

#endif /* HEADANDFOOT_H_ */
