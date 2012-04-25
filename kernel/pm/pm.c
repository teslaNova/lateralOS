/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - pm.c
*/
#include <lateralOS/pm/pm.h>
#include <lateralOS/pm/acpi.h>
#include <lateralOS/pm/apm.h>

#include <lateralOS/cpu.h>

void
pm_init(void) {
	if(cpu_get(0)->support.acpi) {
		acpi_init();
	} else {
		apm_init();
	}
}