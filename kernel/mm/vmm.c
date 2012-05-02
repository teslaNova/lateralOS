/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - vmm.c
*/
#include <lateralOS/mm/vmm.h>
#include <lateralOS/mm/pmm.h>
#include <lateralOS/mm/mmap.h>

#include <lateralOS/types.h>

#include <lateralOS/diag.h>

#define VA_TO_DIR(x) (x >> 22)
#define VA_TO_TBL(x) ((x >> 12) & 0x3FF)

#define DIR_TO_VA(x) (x << 22)
#define TBL_TO_VA(x) (x << 12)

struct page_directory *kernel_pd = 0;
struct page_directory *current_pd = 0;

void
vmm_init(void) {
	struct vmm_flags flags = {	.present = 1, 
								.privileged = 1,
								.writeable = 1,
								.pwt = 0,
								.dis_cache = 0,
								.dirty = 0};

	kernel_pd = (struct page_directory *)(MMAP_PHYS_PAGING);
	memset(kernel_pd, 0, sizeof(struct page_directory));

	vmm_set_directory(kernel_pd);

	/* kernel page pdes & ptes */
	vmm_map_range(flags, 
		MMAP_PHYS_PAGING,
		MMAP_PHYS_PAGING, 
		(MMAP_PHYS_PAGING + MMAP_SIZE_PAGING) / PAGE_ALIGNMENT);

	/* pmm */
	flags.writeable = 0;
	vmm_map_range(flags,
		MMAP_PHYS_PMM,
		MMAP_PHYS_PMM,
		(MMAP_SIZE_PMM + MMAP_PHYS_PMM) / PAGE_ALIGNMENT);

	/* kernel identity mapping */
	vmm_map_range(flags,
		(uint)(&KERNEL_START),
		(uint)(&KERNEL_START),
		((uint)(&KERNEL_END)-(uint)(&KERNEL_START)) / PAGE_ALIGNMENT);

	/* video identity mapping */
	flags.privileged = 0;

	vmm_map_range(flags,
		MMAP_PHYS_VIDEO,
		MMAP_PHYS_VIDEO,
		(MMAP_PHYS_VIDEO + MMAP_SIZE_VIDEO) / PAGE_ALIGNMENT);

	/* alles nötige gemapped, also ab gehts */
	vmm_enable();

/*	not working yet
	struct page_directory *copy_pd = vmm_create_directory(0x400000);
	vmm_set_directory(copy_pd);*/
}

void
vmm_map(struct vmm_flags flags, uint phys_addr, uint virt_addr) {
	struct page_directory_entry *t_pde = 0;
	struct page_table_entry *t_pte = 0;
	struct page_table *t_pt = 0;
	uint tmp_va;

	assert(current_pd != 0);
	assert(virt_addr > 0);
	assert(phys_addr > 0);

	t_pde = &current_pd->entry[VA_TO_DIR(virt_addr)];
	t_pt = vmm_get_table(flags, VA_TO_DIR(virt_addr));
	t_pte = &t_pt->entry[VA_TO_TBL(virt_addr)];

	/* überprüfen, ob die entsprechende pte schon existiert */
	if(t_pte->page == 0 && t_pte->present == 0) {
		t_pte->present 		= flags.present;
		t_pte->writeable 	= flags.writeable;
		t_pte->privileged 	= flags.privileged;
		t_pte->pwt 			= flags.pwt;
		t_pte->dis_cache 	= flags.dis_cache;
		t_pte->page			= ((uint)phys_addr) >> 12;
	} else {
		/* error: doppelter eintrag */
	}
}

void
vmm_unmap(uint virt_addr) {
	struct vmm_flags flags = {0};
	struct page_directory_entry *t_pde = 0;
	struct page_table_entry *t_pte = 0;
	struct page_table *t_pt = 0;

	assert(current_pd != 0);
	assert(virt_addr > 0);
	
	t_pde = &current_pd->entry[VA_TO_DIR(virt_addr)];
	t_pt = vmm_get_table(flags, VA_TO_DIR(virt_addr));
	t_pte = &t_pt->entry[VA_TO_TBL(virt_addr)];

	memset(t_pte, 0, sizeof(struct page_table_entry));
}

void
vmm_map_range(struct vmm_flags flags, uint phys_base_addr, uint virt_base_addr, uint pages) {
	assert(current_pd != 0);
	assert(virt_base_addr > 0);
	assert(phys_base_addr > 0);
	assert(pages > 0);

	while(pages-- > 0) {
		vmm_map(flags, phys_base_addr + (pages * PAGE_ALIGNMENT), virt_base_addr + (pages * PAGE_ALIGNMENT));
	}
}

void
vmm_unmap_range(uint virt_base_addr, uint pages) {
	assert(current_pd != 0);
	assert(virt_base_addr > 0);
	assert(pages > 0);

	while(pages-- > 0) {
		vmm_unmap(virt_base_addr + (pages * PAGE_ALIGNMENT));
	}
}

void
vmm_set_directory(struct page_directory *pd) {
	assert(pd != 0);

	__asm__ volatile (
		"mov %0, %%eax;"
		"mov %%eax, %%cr3"
		:: "m" (pd)
	);

	current_pd = pd;
}

struct page_directory *
vmm_create_directory(uint virt_base_addr) {
	struct page_directory *pd;
	struct vmm_flags flags = {.writeable = 1, .present = 1, .privileged = 0};

	assert(kernel_pd != 0);

	pd = pmm_alloc();
	virt_base_addr = (virt_base_addr == 0 ? (uint)pd : virt_base_addr);
	
	vmm_map(flags, (uint)pd, virt_base_addr);

	memcpy(virt_base_addr, kernel_pd, sizeof(struct page_directory));

	return pd;
}

struct page_table *
vmm_get_table(struct vmm_flags flags, uint pde_num) {
	struct page_directory_entry *pde;
	uint tmp_va = 0, tmp_pa;

	assert(pde_num <= 1024);
	assert(current_pd != 0);

	pde = &current_pd->entry[pde_num];

	if(pde->present == 1) {
		/* fehlalarm */
		return (struct page_table *)(pde->pt << 12);
	}

	/* virtuelle adresse erstellen */
	if(current_pd == kernel_pd) {
		tmp_va 	= MMAP_PHYS_PAGING
				+ pde_num * PAGE_ALIGNMENT;
		tmp_pa 	= tmp_va;
	} else {
		tmp_va	= ALIGN_DOWN((uint)current_pd)
				+ pde_num * PAGE_ALIGNMENT;
		tmp_pa	= (uint)pmm_alloc();
	}

	/* pde eintrag überarbeiten */
	pde->present = 1;
	pde->writeable = 1;

	if(current_pd == kernel_pd) {
		pde->privileged = flags.privileged;
	} else {
		pde->privileged = 0;
	}

	pde->pt = (tmp_va >> 12);

	/* nur mappen, wenn es kein kernel pd ist.
		die 4mb nach dem kernel wurden schon
		gemappt */
	if(tmp_va != tmp_pa) {
		vmm_map(flags, tmp_pa, tmp_va);
	}

	return (struct page_table *)(tmp_va);
}

void
vmm_enable(void) {
	__asm__ volatile (
		"mov %cr0, %eax;"
		"or $0x80000000, %eax;"
		"mov %eax, %cr0"
	);
}

void
vmm_disable(void) {
	__asm__ volatile (
		"mov %cr0, %eax;"
		"and %eax, 0x7FFFFFFF;"
		"mov %eax, %cr0"
	);
}

uint
vmm_get_related_adress_space(uint l, uint h, uint n) {
	struct vmm_flags flags = {.privileged = 0};
	struct page_table *pt;
	struct page_table_entry *pte;
	uint i, j, k;
	uint va = 0;

	assert(current_pd != 0);
	assert(n > 0);

//	k_printf("0x%x (%x) - 0x%x (%x) -> ", l, VA_TO_DIR(l), h, VA_TO_DIR(h));

	if(h == 0) {
		h = (uint)(-1);
	}

	if(flags.reverse == 0) {
		for(i=VA_TO_DIR(l), k=0; i<VA_TO_DIR(h), k<n; i++) {
			pt = vmm_get_table(flags, i);

			if(i == 0) {
				j = VA_TO_TBL(l);
			} else {
				j = 0;
			}

			for(j; j<1024, k<n; j++) {
				pte = &pt->entry[j];

				if(pte->present == 0) {
					if(k == 0) {
						va = DIR_TO_VA(i) | TBL_TO_VA(j);
					}

					k++;
				} else if(pte->present == 1) {
					k = 0;
				}
			}
		}
	} else {
		k_printf("..");
		for(i=VA_TO_DIR(h), k=0; i>VA_TO_DIR(l), k<n; i++) {
			pt = vmm_get_table(flags, i);

			if(i == 0) {
				j = VA_TO_TBL(l);
			} else {
				j = 0;
			}

			for(j=1023; j>=0, k<n; j--) {
				pte = &pt->entry[j];

				if(pte->present == 0) {
					if(k == 0) {
						va = DIR_TO_VA(i) | TBL_TO_VA(j);
					}

					k++;
				} else if(pte->present == 1) {
					k = 0;
				}
			}
		}

		if(va > 0) {
			va -= (PAGE_ALIGNMENT * (n-1));
		}
	}

	if(k < n || va + (PAGE_ALIGNMENT * n) > h) {
		va = 0;
	}

//	k_printf("0x%x\r\n", va);

	return va;
}

uint
vmm_get_related_adress_space_reverse(uint l, uint h, uint n) {
	struct vmm_flags flags = {.privileged = 0};
	struct page_table *pt;
	struct page_table_entry *pte;
	uint i, j, k;
	uint va = 0;

	assert(current_pd != 0);
	assert(n > 0);

	if(h == 0) {
		h = (uint)(-1);
	}

	for(i=VA_TO_DIR(h), k=0; i>=VA_TO_DIR(l), k<n; i--) {
		pt = vmm_get_table(flags, i);

		if(i == 0) {
			j = VA_TO_TBL(h);
		} else {
			j = (uint)(-1);
		}

		for(j; j>=0, k<n; j--) {
			pte = &pt->entry[j];

			if(pte->present == 0) {
				if(k == 0) {
					k_printf("%x -- ",va);
					va = DIR_TO_VA(i) | TBL_TO_VA(j);
				}

				k++;
			} else if(pte->present == 1) {
				k = 0;
			}
		}
	}

	if(k < n || va + (PAGE_ALIGNMENT * n) > h) {
		va = 0;
	} 

	if(va > 0) {
		va -= (PAGE_ALIGNMENT * (n-1));
	}

	k_printf("0x%x\r\n", va);

	return va;
}

void *
vmm_alloc(struct vmm_flags flags, uint n) {
	return vmm_alloc_range(flags, n, 0, (uint)(-1));
}

void *
vmm_alloc_range(struct vmm_flags flags, uint n, uint l, uint h) {
	uint va, pa, i;

	assert(current_pd != 0);
	assert(n > 0);

	pa = (uint)pmm_alloc();
	va = (flags.reverse 
			? vmm_get_related_adress_space_reverse(l, h, n) 
			: vmm_get_related_adress_space(l, h, n));
	
	/* ouch, we're out of memory! :-( */
	if(va == 0 || pa == 0) {
		return (void *)0;
	}

	for(i=0; i<n; i++) {
		vmm_map(flags, (uint)pmm_alloc(), va + (PAGE_ALIGNMENT * i));
	}

	return (void *)(va);
}

void
vmm_free(void *virt_base_addr, uint n) {
	assert(current_pd != 0);
	assert(virt_base_addr != 0);
	assert(n > 0);

	vmm_unmap_range((uint)virt_base_addr, n);
}