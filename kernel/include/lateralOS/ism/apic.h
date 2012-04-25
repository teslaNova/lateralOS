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
apic_io_set(uint base_addr);

void
apic_map(void);

#endif /* K_APIC_H_ */
