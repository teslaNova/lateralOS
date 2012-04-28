/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - ism.c
*/
#include <lateralOS/ism/ism.h>

#include <lateralOS/ism/gateways.h>
#include <lateralOS/ism/pic.h>
#include <lateralOS/ism/apic.h>

#include <lateralOS/descriptors.h>
#include <lateralOS/cpu.h>

#include <lateralOS/diag.h>

#include <lateralOS/screen.h>

#include <lateralOS/lateralOS.h>

#define idts(int_no) idt_set(int_no,									\
	(uint)(irq_gateway0 + ((irq_gateway1 - irq_gateway0) * int_no)), 	\
	DESC_GATE_INT, DESC_DPL_R0);

struct isr isr_tbl[ISR_NUM];

void
ism_init(void) {
	uint i;

	k_puts("\t\t-> ISRs\r\n");
	
	for(i=0; i<ISR_NUM; i++) {
		idts(i);
	}

	k_puts("\t\t-> PIC\r\n");
	pic_init();

	if(cpu_get_local()->support.apic) {
		pic_disable();

		k_puts("\t\t-> APIC (IO, Local)\r\n");
		
		apic_init();
		apic_local_enable();
	} else {
		pic_enable();
	}
}

void
ism_disable(void) {
	asm ("cli");
}

void
ism_enable(void) {
	asm ("sti");
}

void
ism_handler(struct ism_context *c) {
	if(c->int_no <= 0x1F) {
		ism_exception_handler(c);
	} else {
		ism_interrupt_handler(c);
	}

	if(cpu_get_local()->support.apic) {
		apic_local_eoi(c->int_no);
	} else {
		pic_eoi(c->int_no);
	}
}

void
ism_exception_handler(struct ism_context *c) {
	k_cls();
	
	switch(c->int_no) {
		case EXCEPTION_INVALID_OPCODE:
		case EXCEPTION_DIVIDE_ERROR: {
			/* opcodes überschreiben. */
			*(uint *)(c->i.eip) = 0x90;
		} break;

		default: {
			panic();
		};
	};
}

void
ism_interrupt_handler(struct ism_context *c) {

	k_printf("Got IRQ: %x (%x)\r", c->int_no, c->int_no == 0x20 ? inb(0x60) : 0);

	if(isr_tbl[c->int_no].handler != 0) {
		isr_tbl[c->int_no].handler(c);
		isr_tbl[c->int_no].called++;
	}
}

void
ism_set_isr(uint int_no, void (*handler)(struct ism_context *c)) {
	assert(int_no < ISR_NUM);
	assert(handler != 0);

	isr_tbl[int_no].handler = handler;
}
