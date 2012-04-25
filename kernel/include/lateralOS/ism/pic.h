/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - pic.h
*/
#ifndef K_PIC_H_
#define K_PIC_H_

#include <lateralOS/types.h>

#define PIC_RELOC_MASTER 	0x20
#define PIC_RELOC_SLAVE 	0x28

enum {
	PIC_P_CMD_MASTER = 0x20,
	PIC_P_DAT_MASTER = 0x21,

	PIC_P_CMD_SLAVE = 0xA0,
	PIC_P_DAT_SLAVE = 0xA1,

	PIC_CMD_EOI		= 0x20,
	PIC_CMD_INIT	= 0x11,

	PIC_DAT_DONE	= 1,
};

enum pic_flags {
	PIC_FL_MASK = 0,
	PIC_FL_UNMASK,
};

void
pic_enable(void);

void
pic_disable(void);

void
pic_init(void);

void
pic_eoi(uint int_no);

void
pic_mask(uint int_line, enum pic_flags mode);

#endif /* K_PIC_H_ */
