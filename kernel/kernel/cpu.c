/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - cpu.c
*/
#include <lateralOS/cpu.h>
#include <lateralOS/mp/mp.h>

#include <lateralOS/ism/apic.h>

#include <lateralOS/diag.h>

#include <lateralOS/utils.h>

#include <lateralOS/screen.h>

struct cpu_ent cpu_tbl[CPU_NUM] = {0};
uint cpu_ec = 0;

void cpuid(struct regs *regs) {
	assert(regs != 0);

	__asm__ (
		"mov %4, %%eax;"
		"cpuid;"
		"mov %%eax, %0;"
		"mov %%ebx, %1;"
		"mov %%ecx, %2;"
		"mov %%edx, %3;"
		: "=r" (regs->eax), "=r" (regs->ebx), "=r" (regs->ecx), "=r" (regs->edx)
		: "r" (regs->eax)
		: "memory"
	);
}

void cpu_init() {
	struct regs data = {0};
	struct cpu_ent *cpu = 0;
	uint i, j;

	assert(cpu_ec < CPU_NUM);

	cpu = &cpu_tbl[cpu_ec++];

	if(cpu_ec-1 == 0) {
		cpu->is_bsp = 1;
	}

	/* get features */
	data.eax = 1;
	cpuid(&data);

	cpu->apic_id = (data.ebx >> 24);
	
	cpu->support.x2apic = (data.ecx & (1 << 21)) ? 1 : 0;
	cpu->support.apic = (data.edx & (1 << 9)) ? 1 : 0;

	cpu->support.x87 = (data.edx & 1) ? 1 : 0;

	cpu->support.acpi = (data.edx & (1 << 22)) ? 1 : 0;

	cpu->support.sse = (data.edx & (1 << 25)) ? 1 : 0;
	cpu->support.sse2 = (data.edx & (1 << 26)) ? 1 : 0;
	cpu->support.sse3 = (data.ecx & 1) ? 1 : 0;
	cpu->support.sse4_1 = (data.ecx & (1 << 19)) ? 1 : 0;
	cpu->support.sse4_2 = (data.ecx & (1 << 20)) ? 1 : 0;

	cpu->support.msr = (data.edx & (1 << 5)) ? 1 : 0;

	/* get brand string */
	i = 0x80000002;
	j = 0;

	while(i < 0x80000005) {
		data.eax = i++;
		cpuid(&data);

		memcpy(cpu->id + (j++*4), &(data.eax), 4); 
		memcpy(cpu->id + (j++*4), &(data.ebx), 4);
		memcpy(cpu->id + (j++*4), &(data.ecx), 4);
		memcpy(cpu->id + (j++*4), &(data.edx), 4);
	}
}

struct cpu_ent *
cpu_get(uint i) {
	assert(i < CPU_NUM);

	return &cpu_tbl[i];
}

struct cpu_ent *
cpu_get_local(void) {
	struct regs data;

	data.eax = 1;
	cpuid(&data);

	return cpu_get((data.ebx >> 24));
}

uint
cpu_count(void) {
	return cpu_ec;
}

uint64
cpu_msr_read(uint msr) {
	uint hi, lo;

	if(cpu_get_local()->support.msr == 0) {
		return 0;
	}

	__asm__ (
		"rdmsr"
		: "=a"(lo), "=d"(hi)
		: "c" (msr)
	);

	return ((uint64)hi) << 32 | lo;
}

void
cpu_msr_write(uint msr, uint64 d) {
	uint lo = d & 0xFFFFFFFF,
		hi = (d >> 32) & 0xFFFFFFFF;

	if(cpu_get_local()->support.msr == 0) {
		return 0;
	}

	__asm__ (
		"wrmsr"
		:: "a"(lo), "d"(hi),
			"c" (msr)
	);
}
