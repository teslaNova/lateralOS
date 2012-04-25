/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - pmm.h
*/
#include <lateralOS/types.h>

struct multiboot_mmap_entry;

void
pmm_init(struct multiboot_mmap_entry *mmap, unsigned int entries, uint total);

void *
pmm_alloc(void);

void
pmm_free(void *page);