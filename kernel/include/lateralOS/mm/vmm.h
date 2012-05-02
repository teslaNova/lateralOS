/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - vmm.h
*/
#ifndef K_VMM_H_
#define K_VMM_H_

#include <lateralOS/types.h>

struct page_directory_entry {
	byte present 	: 1;
	byte writeable 	: 1;
	byte privileged	: 1;
	byte pwt		: 1;
	byte dis_cache	: 1;
	byte accessed	: 1;
	byte _u0		: 1;
	byte _r0		: 1;	// pat
	byte _r1		: 1;	// necessary if cr4.pge is set.
	byte _u1		: 3;
	uint pt			: 20;
};

struct page_table_entry {
	byte present 	: 1;
	byte writeable 	: 1;
	byte privileged	: 1;
	byte pwt		: 1;
	byte dis_cache	: 1;
	byte accessed	: 1;
	byte dirty		: 1;
	byte _r0		: 1;	// pat
	byte _r1		: 1;	// necessary if cr4.pge is set.
	byte _u0		: 3;
	uint page		: 20;
};

struct page_directory {
	struct page_directory_entry entry[1024];
};

struct page_table {
	struct page_table_entry entry[1024];
};

struct vmm_flags {
	/* paging */
	byte present	: 1;
	byte writeable	: 1;
	byte privileged	: 1;
	byte pwt		: 1;
	byte dis_cache	: 1;
	byte dirty		: 1;

	/* order */
	byte reverse	: 1;
};

void
vmm_init(void);

/* PAGING */
void
vmm_map(struct vmm_flags flags, uint phys_addr, uint virt_addr);

void
vmm_unmap(uint virt_addr);

void inline
vmm_map_range(struct vmm_flags flags, uint phys_base_addr, uint virt_base_addr, uint pages);

void inline
vmm_unmap_range(uint virt_base_addr, uint pages);

void inline
vmm_set_directory(struct page_directory *pd);

struct page_directory *
vmm_create_directory(uint virt_base_addr);

struct page_table *
vmm_get_table(struct vmm_flags flags, uint pde_num);

void inline
vmm_enable(void);

void inline
vmm_disable(void);

uint
vmm_get_related_adress_space(uint l, uint h, uint n);

uint
vmm_get_related_adress_space_reverse(uint l, uint h, uint n);

/* MEMORY ALLOCATION */
void *
vmm_alloc(struct vmm_flags flags, uint n);

void *
vmm_alloc_range(struct vmm_flags flags, uint n, uint l, uint h);

void
vmm_free(void *virt_base_addr, uint n);

#endif /* K_VMM_H_ */
