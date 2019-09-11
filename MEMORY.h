/*
 * MEMORY.h
 *
 *  Created on: 2019/8/10
 *      Author: Unier
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#define M_OneAdd	0x1880
#define M_TwoAdd	(0x1880 + 2)
#define M_Threeadd	(0x1880 + 4)

extern uint16_t Mem[6];
extern uint16_t SaveFlag;

void MemoryInit(void);
void MemoryWrite(void);
void MemoryRead(void);

#endif /* MEMORY_H_ */
