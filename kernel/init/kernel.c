/*
	lateralOS 						(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - kernel.c
*/
#include <multiboot.h>

#include <lateralOS/lateralOS.h>

#include <lateralOS/screen.h>

#include <lateralOS/cpu.h>
#include <lateralOS/mp/mp.h>

#include <lateralOS/pm/pm.h>
#include <lateralOS/pm/acpi.h>

#include <lateralOS/mm/mmap.h>
#include <lateralOS/mm/pmm.h>
#include <lateralOS/mm/vmm.h>

#include <lateralOS/descriptors.h>

#include <lateralOS/ism/ism.h>

#include <lateralOS/timer.h>

#include <lateralOS/types.h>

#include <lateralOS/diag.h>

void exception_test(void) {
	uint a = 6/0;
	k_printf("after divide-by-zero");

	asm volatile (".word 0x12");
	k_printf("after invalid opcode");
}

void main(uint mb_magic, multiboot_info_t *mb_info) {
	k_cls();

	k_printf(	"--------------------------------------------------\r\n"
				"                 %s (v%s)\r\n"
				"--------------------------------------------------\r\n"
				"\r\n",
				KRNL_NAME, KRNL_VER);

	if(mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		k_printf(	"Multiboot Magic incorrect.\r\n"
					"Should be %x, is: %x\r\n",
					MULTIBOOT_BOOTLOADER_MAGIC, mb_magic);
		panic();
	}

	k_puts("Collecting information..\r\n");
	cpu_init();

	if(cpu_get(0)->support.acpi) {
		acpi_parse();
	} else {
		mp_parse();
	}

	k_printf("\tMemory: %d MB total\r\n", (mb_info->mem_lower + mb_info->mem_upper) / 1024);
	k_printf("\tCPU:    %s\r\n", cpu_get(0)->id);
	k_printf("\tAPIC:   %s\r\n", (cpu_get(0)->support.apic ? (cpu_get(0)->support.x2apic ? "x2APIC" : "xAPIC") : "not supported"));
	k_printf("\tSSE:	%s\r\n", (cpu_get(0)->support.sse 
									? (cpu_get(0)->support.sse2)
										? (cpu_get(0)->support.sse3)
											? (cpu_get(0)->support.sse4_1)
												? (cpu_get(0)->support.sse4_2)
													? "SSE SSE2 SSE3 SSE 4.1 SSE 4.2"
													: "SSE SSE2 SSE3 SSE 4.1"
												: "SSE SSE2 SSE3"
											: "SSE SSE2"
										: "SSE"
									: "not supported"));

	k_puts("\r\nInitializing...\r\n");
	k_puts("\tPhysical Memory Manager\r\n");
	pmm_init((multiboot_memory_map_t *)(mb_info->mmap_addr), 
		mb_info->mmap_length/sizeof(multiboot_memory_map_t),
		mb_info->mem_lower + mb_info->mem_upper);
	
	k_puts("\tVirtual Memory Manager\r\n");
	vmm_init();

	k_puts("\tInterrupt Service Manager\r\n");
	ism_init();

	k_puts("\tDescriptors\r\n");
	gdt_init();
	idt_init();

	k_puts("\tPower Management\r\n");
	pm_init();

	k_puts("\r\nEnabling Interrupts\r\n");
	ism_enable();

/*	k_puts("\tTimer (PIT, ..)\r\n");
	timer_init();
*/
	//exception_test();

	while(1);
}

void
panic(void) {
	k_printf("\r\n\r\nSystem halted.\r\n");

	__asm__ volatile ("cli");

	while(1) {
		__asm__ volatile ("hlt");
	}
}