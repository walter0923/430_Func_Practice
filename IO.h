/*
 * IO.h
 *
 *  Created on: 2019/8/9
 *      Author: Unier
 */

#ifndef IO_H_
#define IO_H_
#include "KEYSCAN.h"

extern uint16_t ShakeDuty;
extern uint16_t MOnePosition;
void SightInit(void);
void SightOnOff(void);
void TorchInit(void);
void TorchOnOff(void);
void GpioInit(void);
void GLight(void);
void ShakeInit(void);
void Shake_On(void);
void Shake_Off(void);
#endif /* IO_H_ */
