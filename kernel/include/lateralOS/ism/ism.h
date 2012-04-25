/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - ism.h
*/
#ifndef K_ISM_H_
#define K_ISM_H_

#include <lateralOS/types.h>

#define ISR_NUM		256

struct ism_context {
	struct cregs c;
	struct segs s;
	struct regs r;

	uint int_no;

	struct iregs i;
};

struct isr {
	uint called;
	void (*handler)(struct ism_context *r);
};

enum {
	EXCEPTION_DIVIDE_ERROR = 0,
	EXCEPTION_DEBUG,
	EXCEPTION_NMI_INT,
	EXCEPTION_BREAKPOINT,
	EXCEPTION_OVERFLOW,
	EXCEPTION_BOUND_RANGE_EXCEEDED,
	EXCEPTION_INVALID_OPCODE,
	EXCEPTION_DOUBLE_FAULT,
	EXCEPTION_COPROCESSOR_OVERRUN,
	EXCEPTION_INVALID_TSS,
	EXCEPTION_SEGMENT_NOT_PRESENT,
	EXCEPTION_STACK_SEGMENT_FAULT,
	EXCEPTION_GENERAL_PROTECTION,
	EXCEPTION_PAGE_FAULT,
	EXCEPTION_R0,
	EXCEPTION_MATH_FAULT,
	EXCEPTION_ALIGNMENT_CHECK,
	EXCEPTION_MACHINE_CHECK,
	EXCEPTION_SIMD_FP_EXCEPTION,
};

void
ism_init(void);

void
ism_disable(void);

void
ism_enable(void);

void
ism_handler(struct ism_context *c);

void
ism_exception_handler(struct ism_context *c);

void
ism_interrupt_handler(struct ism_context *c);

void
ism_set_isr(uint int_no, void (*handler)(struct ism_context *r));

#endif /* K_ISM_H_ */
