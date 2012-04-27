/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - apic.h
*/
#ifndef K_APIC_H_
#define K_APIC_H_

#include <lateralOS/types.h>

#define IO_APIC_NUM		4

/*
	MP und/oder ACPI Tabelle auslesen um herauszufinden,
	wie viele und wo die IO APIC(s) sind.

	Kleine Anleitung gibt es hier:
		http://f.osdev.org/viewtopic.php?f=1&t=23582
*/

enum {
	LAPIC_REG_ID 	= 0x20,
	LAPIC_REG_VER	= 0x30,
	LAPIC_REG_TPR	= 0x80,
	LAPIC_REG_APR	= 0x90,
	LAPIC_REG_PPR	= 0xA0,
	LAPIC_REG_EOI	= 0xB0,
	LAPIC_REG_RRD	= 0xC0,
	LAPIC_REG_LDR	= 0xD0,
	LAPIC_REG_DFR	= 0xE0,
	LAPIC_REG_SPV	= 0xF0,

	// ..

	LAPIC_REG_ERR	= 0x280,

	// ..

	LAPIC_REG_ISR 	= 0x100,
	LAPIC_REG_TMR	= 0x180,
	LAPIC_REG_IRR	= 0x200,

	// ...

	LAPIC_REG_LINT0	= 0x350,
	LAPIC_REG_LINT1 = 0x360,
	LAPIC_REG_LERR	= 0x370,

	// ..
};

enum {
	LAPIC_SPV_ENA	= (1 << 8),
	LAPIC_DFR_FLAT 	= (0xF << 28),


	LAPIC_LVT_DM_EXT= (0b111 << 8),
	LAPIC_LVT_DM_NMI= (1 << 10),

	LAPIC_LVT_TM_LVL= (1 << 15),

	LAPIC_LVT_POL_LO= (1 << 13),

	LAPIC_LVT_MASK	= (1 << 16),
};

enum {
	LAPIC_MSR_ENA	= (1 << 11),
	LAPIC_MSR_BSP	= (1 << 8),
};

struct io_apic {
	uint base_addr;
};

void
apic_enable(void);

void
apic_disable(void);

void
apic_init(void);

void
apic_eoi(uint int_no);

void
apic_local_write(uint reg, uint data);

uint
apic_local_read(uint reg);

void
apic_io_set(uint base_addr);

void
apic_io_write(uint n, uint reg, uint data);

uint
apic_io_read(uint n, uint reg);

enum {
	IOAPIC_MREG_SEL = 0,
	IOAPIC_MREG_WIN = 0x10,

	IOAPIC_REG_ID	= 0,
	IOAPIC_REG_VER,
	IOAPIC_REG_ARB,

	IOAPIC_REG_RDT	= 0x10,

	IOAPIC_RDT_NUM	= 24,

	IOAPIC_DM_EXT 	= (0b111 << 8),
	IOAPIC_DM_NMI	= (0b100 << 8),
};

void
apic_map(void);

#endif /* K_APIC_H_ */
