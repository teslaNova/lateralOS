/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - descriptors.h
*/
#ifndef K_DESCRIPTORS_H_
#define K_DESCRIPTORS_H_

#include <lateralOS/types.h>

enum {
	DESC_ACCESS_PRESENT 	= (1 << 7),

	DESC_ACCESS_RING0		= (0),
	DESC_ACCESS_RING1		= (1 << 5),
	DESC_ACCESS_RING2		= (1 << 6),
	DESC_ACCESS_RING3		= DESC_ACCESS_RING1 | DESC_ACCESS_RING2,

	DESC_ACCESS_EXEC		= (1 << 3),

	DESC_ACCESS_EVERYONE	= (1 << 2),

	DESC_ACCESS_READWRITE	= (1 << 1),

	DESC_ACCESS_ALWAYS		= (1 << 4),
};

enum {
	DESC_FLAGS_PAGE_SIZE	= (1 << 3),

	DESC_FLAGS_32_BIT		= (1 << 2),
};

#define DESC_GDT_SIZE	 	3
#define DESC_IDT_SIZE		256

#define DESC_CODE_FLAGS		DESC_FLAGS_32_BIT | DESC_FLAGS_PAGE_SIZE
#define DESC_CODE_ACCESS	DESC_ACCESS_RING0		\
							| DESC_ACCESS_PRESENT	\
							| DESC_ACCESS_READWRITE \
							| DESC_ACCESS_EXEC

#define DESC_DATA_FLAGS		DESC_FLAGS_32_BIT | DESC_FLAGS_PAGE_SIZE
#define DESC_DATA_ACCESS	DESC_ACCESS_RING0		\
							| DESC_ACCESS_READWRITE	\
							| DESC_ACCESS_PRESENT


enum {
	DESC_GATE_TASK = 0x5,
	DESC_GATE_INT16,
	DESC_GATE_TRAP16,

	DESC_GATE_INT = 0xE,
	DESC_GATE_TRAP,

	DESC_DPL_R0 = 0,
	DESC_DPL_R1,
	DESC_DPL_R2,
	DESC_DPL_R3,
};

enum {
	SEL_R0_CODE = 0x8,
	SEL_R0_DATA = 0x10,

	SEL_R3_CODE = 0x18,
	SEL_R3_DATA = 0x20,
};

struct desc_register {
	ushort size;
	uint offset;
};

struct gdt_entry {
	ushort limit_lo;
	uint base_lo : 24;
	byte access;
	byte limit_hi : 4;
	byte flags : 4;
	byte base_hi;
};

struct idt_entry {
	ushort handler_lo;
	ushort selector;
	byte _u0;
	byte type : 4;
	byte storage : 1;
	byte dpl : 2;
	byte present : 1;
	ushort handler_hi;
};

void
gdt_init(void);

void
gdt_set(uint gate, uint base, uint limit,  byte flags, byte access);

void inline
gdt_load(void);

void
idt_init(void);

void
idt_set(uint gate, uint handler, uint type, uint dpl);

void inline
idt_load(void);

#endif /* K_DESCRIPTORS_H_ */