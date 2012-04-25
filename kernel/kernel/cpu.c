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

struct cpu cpus[CPU_NUM] = {0};

inline
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
	static uint ci = 0;
	struct regs data = {0};
	struct cpu *cpu = 0;
	uint i, j;

	assert(ci < CPU_NUM);

	cpu = &cpus[ci++];

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

	if(cpu != 0) {
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
}

struct cpu *
cpu_get(uint i) {
	assert(i < CPU_NUM);

	return &cpus[i];
}

void
mp_parse(void) {
	struct mp_floating_pointer *mp_fp;
	uint tmp;

	uint i;
	uint loc[3][2] = {
		{0, 		0x400},
		{0x9FC00, 0xA0000},
		{0xE0000, 0xFFFFF}
	};

	char verify(byte *data, uint len) {
		uint i;
		byte sum;

		for(i=0, sum=0; i<len; i++) {
			sum += data[i];
		}

		return sum;
	}

	struct mp_floating_pointer *locate_floating_pointer(uint start, uint end) {
		struct mp_floating_pointer *mp_fp = (struct mp_floating_pointer *)start;
		

		while((uint)mp_fp < end) {
			if(mp_fp->sig == MP_SIG_FP) {
				if(verify((byte *)mp_fp, mp_fp->len * 16) == 0) {
					return mp_fp;
				}
			}

			mp_fp++;
		}

		return 0;
	}

	for(i=0; i<3; i++) {
		mp_fp = locate_floating_pointer(loc[i][0], loc[i][1]);

		if(mp_fp != 0) {
			break;
		}
	}

	if(mp_fp == 0) {
		k_cls();
		k_printf("Unable to find MP Configuration Tables\r\n");

		panic();
	}

	if(mp_fp->conf->sig != MP_SIG_CONF) {
		k_cls();
		k_printf("Unable to find MP Configuration Tables\r\n");

		panic();
	}


	byte *base_entry = ((byte *)mp_fp->conf) + sizeof(struct mp_config_table);

	for(i=0; i<mp_fp->conf->entries; i++) {
		switch(*base_entry) {
			case MP_ET_PROC: {
				base_entry += MP_ES_PROC;
			} break;

			case MP_ET_BUS: {
				base_entry += MP_ES_DEF;
			} break;

			case MP_ET_IOAPIC: {
				struct mp_entry_ioapic *ioapic = (struct mp_entry_ioapic *)base_entry; 
				
				apic_io_set(ioapic->address);

				base_entry += MP_ES_DEF;
			} break;

			case MP_ET_IOINT: {
				base_entry += MP_ES_DEF;
			} break;

			case MP_ET_LINT: {
				base_entry += MP_ES_DEF;
			} break;
		}
	}
}
