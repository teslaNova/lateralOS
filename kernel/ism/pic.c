/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - pic.c
*/
#include <lateralOS/ism/pic.h>

#include <lateralOS/diag.h>
#include <lateralOS/utils.h>

void
pic_init(void) {
	byte mm, ms;

	mm = inb(PIC_P_DAT_MASTER);
	ms = inb(PIC_P_DAT_SLAVE);

	outb(PIC_P_CMD_MASTER, PIC_CMD_INIT);
	outb(PIC_P_CMD_SLAVE, PIC_CMD_INIT);

	outb(PIC_P_DAT_MASTER, PIC_RELOC_MASTER);
	outb(PIC_P_DAT_SLAVE, PIC_RELOC_SLAVE);

	outb(PIC_P_DAT_MASTER, 4);
	outb(PIC_P_DAT_SLAVE, 2);

	outb(PIC_P_DAT_MASTER, PIC_DAT_DONE);
	outb(PIC_P_DAT_SLAVE, PIC_DAT_DONE);

	outb(PIC_P_DAT_MASTER, mm);
	outb(PIC_P_DAT_SLAVE, ms);
}

void
pic_enable(void) {
	outb(PIC_P_DAT_SLAVE, 0x0);
	outb(PIC_P_DAT_MASTER, 0x0);
}

void
pic_disable(void) {
	outb(PIC_P_DAT_SLAVE, 0xFF);
	outb(PIC_P_DAT_MASTER, 0xFF);
}

void
pic_eoi(uint int_no) {
	if(int_no < 8) {
		outb(PIC_P_CMD_MASTER, PIC_CMD_EOI);
		outb(PIC_P_DAT_MASTER, (byte)int_no);	
	} else {
		outb(PIC_P_CMD_SLAVE, PIC_CMD_EOI);
		outb(PIC_P_DAT_SLAVE, (byte)int_no);
	}
}

void
pic_mask(uint int_no, enum pic_flags mode) {
	ushort port = PIC_P_DAT_MASTER;

	assert(mode >= PIC_FL_MASK && mode <= PIC_FL_UNMASK);

	if(int_no >= 8) {
		int_no -= 8;
		port = PIC_P_DAT_SLAVE;
	}

	if(mode == PIC_FL_UNMASK) {
		outb(port, inb(port) & ~(1 << int_no));
	} else {
		outb(port, inb(port) | (1 << int_no));
	}
}