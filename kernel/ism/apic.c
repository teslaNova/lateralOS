/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - apic.c
*/
#include <lateralOS/ism/apic.h>

#include <lateralOS/mm/vmm.h>

#include <lateralOS/cpu.h>

struct io_apic io_apic_tbl[IO_APIC_NUM] = {0};

void
apic_local_init(void) {
	apic_local_write(LAPIC_REG_DFR, LAPIC_DFR_FLAT);
	apic_local_write(LAPIC_REG_TPR, 0);

	apic_local_write(LAPIC_REG_LINT0, 0x20 | LAPIC_LVT_DM_EXT);
	apic_local_write(LAPIC_REG_LINT1, 0x30 | LAPIC_LVT_DM_NMI);

	k_printf("\t\t- err: %x\r\n", 0xFF & apic_local_read(LAPIC_REG_LERR));
}

void
apic_local_enable(void) {
	struct cpu *cpu = cpu_get_local();

	cpu_msr_write(MSR_APIC_BASE, cpu->lapic_base 
			| LAPIC_MSR_ENA
			| (cpu->is_bsp ? LAPIC_MSR_BSP : 0));

	apic_local_write(LAPIC_REG_SPV, LAPIC_SPV_ENA | 0xFF);
}

void
apic_local_disable(void) {
	cpu_msr_write(MSR_APIC_BASE, cpu_get_local()->lapic_base 
			| (cpu_get_local()->is_bsp ? LAPIC_MSR_BSP : 0));

	apic_local_write(LAPIC_REG_SPV, 0);
}

void
apic_local_eoi(uint int_no) {
	apic_local_write(LAPIC_REG_EOI, 0);
}

void
apic_local_write(uint reg, uint data) {
	*(uint *)(cpu_get_local()->lapic_base + reg) = data;
}

uint
apic_local_read(uint reg) {
	return *(uint *)(cpu_get_local()->lapic_base + reg);
}

void
apic_io_init(void) {
	uint i, j;

	for(j=0; j<IO_APIC_NUM; j++) {
		for(i=0; i<IOAPIC_RDT_NUM; i++) {
			apic_io_write(j, IOAPIC_REG_RDT + (i * 2), ((0x20 + i) + (j * IOAPIC_RDT_NUM)) | IOAPIC_DM_EXT);
		}
	}
}

void
apic_io_set(uint base_addr) {
	uint i;

	for(i=0; i<IO_APIC_NUM; i++) {
		if(io_apic_tbl[i].base_addr == base_addr) {
			break;
		}

		if(io_apic_tbl[i].base_addr == 0) {
			io_apic_tbl[i].base_addr = base_addr;
			break;
		}
	}	
}

void
apic_io_write(uint n, uint reg, uint data) {
	uint ioapic = io_apic_tbl[n].base_addr;

	if(ioapic == 0) {
		return;
	}

	*(uint *)(ioapic + IOAPIC_MREG_SEL) = reg;
	*(uint *)(ioapic + IOAPIC_MREG_WIN) = data;
}

uint
apic_io_read(uint n, uint reg) {
	uint ioapic = io_apic_tbl[n].base_addr;

	if(ioapic == 0) {
		return;
	}

	*(uint *)(ioapic + IOAPIC_MREG_SEL) = reg;
	return *(uint *)(ioapic + IOAPIC_MREG_WIN);
}

/* NOT FINISHED YET */
void
apic_init(void) {
	static byte io = 0;

	apic_map();

	/* init io apics once */
	if(io == 0) {
		apic_io_init();
		++io;
	}

	apic_local_init();
}

void
apic_map(void) {
	uint i;
	struct vmm_flags flags = {.present = 1, .writeable = 1, .privileged = 1};

	/* map each lapic and ioapic base addr */
	for(i=0; i<IO_APIC_NUM; i++) {
		if(io_apic_tbl[i].base_addr == 0) {
			//*(byte *)(io_apic_tbl[i].base_addr) = '0';
			break;
		}

		vmm_map(flags, io_apic_tbl[i].base_addr, io_apic_tbl[i].base_addr);
	}

	for(i=0; i<cpu_count(); i++) {
		uint lapic_base = cpu_get(i)->lapic_base;

		vmm_map(flags, lapic_base, lapic_base);
	}
}
