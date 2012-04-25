/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - mmap.h
*/
#ifndef K_MMAP_H_
#define K_MMAP_H_

#include <lateralOS/types.h>

#define PAGE_ALIGNMENT	0x1000
#define ALIGN_UP(x)		(((x) + (PAGE_ALIGNMENT-1)) & ~(PAGE_ALIGNMENT-1))
#define ALIGN_DOWN(x)	((x) & ~(PAGE_ALIGNMENT-1))

extern uint KERNEL_START;
extern uint KERNEL_END;

/* PHYS */
#define MMAP_SIZE_VIDEO		(80*25*2)
#define MMAP_SIZE_PAGING	0x1000000 // 4 mb
extern uint MMAP_SIZE_PMM;

#define MMAP_PHYS_PAGING	ALIGN_UP((unsigned)(&KERNEL_END)) + PAGE_ALIGNMENT
#define MMAP_PHYS_PMM		MMAP_PHYS_PAGING + MMAP_SIZE_PAGING + PAGE_ALIGNMENT
#define MMAP_PHYS_VIDEO		0xb8000

#define MMAP_VIRT_KERNEL_START	0
#define MMAP_VIRT_KERNEL_END	0x10000000

#define MMAP_VIRT_USER_START 	MMAP_VIRT_KERNEL_END
#define MMAP_VIRT_USER_END		0x40000000

#endif /* K_MMAP_H_ */