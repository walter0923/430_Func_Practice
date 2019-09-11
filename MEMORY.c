/*
 * MEMORY.c
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

uint16_t SaveFlag = 0;

uint16_t Mem[6] = {0}; //H1 H2 H3 F1 F2 F3

void MemoryInit(void){
	FCTL2 = FWKEY + FSSEL_2;
}

void MemoryWrite(void){
	uint16_t i;
	uint16_t * Flash_ptr;                         // Initialize Flash pointer
	Flash_ptr = (uint16_t *)0x1040;

	__disable_interrupt();                    // 5xx Workaround: Disable global

	FCTL3 = FWKEY;                            // Clear Lock bit
	FCTL1 = FWKEY+ERASE;                      // Set Erase bit
	*Flash_ptr = 0;           				  // Dummy write to erase Flash seg
	while(FCTL3&BUSY);

	FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

	for(i = 0; i < 6; i++){
		*Flash_ptr = Mem[i];
		while(FCTL3&BUSY);
		Flash_ptr ++;
	}


	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

	//ScanDelay10us(10);
	__bis_SR_register(GIE);
}

void MemoryRead(void){
	uint16_t i;
	uint16_t * Flash_ptr;                         // Initialize Flash pointer
	Flash_ptr = (uint16_t *)0x1040;

	__disable_interrupt();

	for(i = 0; i < 6; i ++){
		Mem[i] = *Flash_ptr;
		*Flash_ptr ++;
		if(Mem[i] == 0xFFFF){
			Mem[i] = 0;
		}
	}

	//ScanDelay10us(10);
	__bis_SR_register(GIE);
}
