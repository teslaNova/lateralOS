/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - cpu.h
*/
#ifndef K_CPU_H_
#define K_CPU_H_

#include <lateralOS/types.h>

#define CPU_NUM 4

struct cpu {
	char id[100];

	byte apic_id;
	
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
	} support;
};

void
cpu_init();

struct cpu *
cpu_get(uint i);

void
mp_parse(void);

void
acpi_parse(void);

#endif /* K_CPU_H_ */
