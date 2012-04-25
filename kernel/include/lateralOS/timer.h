/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - timer.h
*/
#ifndef K_TIMER_H_
#define K_TIMER_H_

#include <lateralOS/lateralOS.h>

#define PIT_REG_CH0			0x40
#define PIT_REG_CH1			0x41
#define PIT_REG_CH2			0x42
#define PIT_REG_MODE		0x43

#define PIT_MODE_SEL_CH0	0
#define PIT_MODE_SEL_CH1	(1 << 6)
#define PIT_MODE_SEL_CH2	(1 << 7)

//#define PIT_MODE_ACS_

#define PIT_MODE_OP_

void
timer_init();


#endif /* K_TIMER_H_ */