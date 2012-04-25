/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - apic.c
*/
#include <lateralOS/ism/apic.h>

#include <lateralOS/mm/vmm.h>

struct io_apic io_apic_tbl[IO_APIC_NUM] = {0};

void
apic_enable(void) {

}

void
apic_disable(void) {

}

void
apic_init(void) {
	apic_map();
}

void
apic_eoi(uint int_no) {

}

void
apic_io_set(uint base_addr) {
	uint i;

	for(i=0; i<IO_APIC_NUM; i++) {
		if(io_apic_tbl[i].base_addr == 0) {
			io_apic_tbl[i].base_addr = base_addr;
			break;
		}
	}

	k_printf("0x%x\r\n", base_addr);	
}

void
apic_map(void) {
	
}
