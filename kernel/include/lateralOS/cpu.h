/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - cpu.h
*/
#ifndef K_CPU_H_
#define K_CPU_H_

#include <lateralOS/types.h>

#define CPU_NUM 8

struct cpu_ent {
	char id[100];

	byte apic_id;
	uint lapic_base;

	byte is_bsp;
	
	struct {
		byte apic : 1;
		byte x2apic : 1;
		byte x87 : 1;

		byte acpi : 1;

		byte sse : 1;
		byte sse2 : 1;
		byte sse3 : 1;
		byte sse4_1 : 1;
		byte sse4_2 : 1;
		byte mmx : 1;

		byte msr : 1;
	} support;
};

void
cpu_init();

struct cpu_ent *
cpu_get(uint i);

struct cpu_ent *
cpu_get_local(void);

uint
cpu_count(void);

enum {
	MSR_APIC_BASE = 0x1B,
};

uint64
cpu_msr_read(uint msr);

void
cpu_msr_write(uint msr, uint64 d);

#endif /* K_CPU_H_ */
