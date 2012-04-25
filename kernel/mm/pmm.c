/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - pmm.c
*/
#include <lateralOS/mm/pmm.h>
#include <lateralOS/mm/mmap.h>

#include <lateralOS/utils.h>

#include <lateralOS/diag.h>

#include <multiboot.h>

uint *pmm_sp;
uint MMAP_SIZE_PMM;

struct mem_region {
	uint start;
	uint end;
};

void
protected_region(struct mem_region *mrs, uint len, uint prs, uint pre) {
	uint i, j;
	struct mem_region tmp = {0};

	assert(pre > prs);
	assert(pre != 0);

	for(i=0; i<len; i++) {
		if(mrs[i].start == mrs[i].end) {
			memset(&mrs[i], 0, sizeof(struct mem_region));
			continue;
		}

		if(mrs[i].start < prs && mrs[i].end > prs) {
			if(mrs[i].end > pre) {
				//	+---+-------+---+
				//	^	^		^	^
				//	mrs				mre
				//		prs		pre

				tmp.end   = ALIGN_DOWN(mrs[i].end);
				tmp.start = ALIGN_UP(pre);

				/* find a free region to store the found space */
				for(j=0; j<len; j++) {
					if(mrs[j].start == 0 && mrs[j].end == 0) {
						memcpy(&mrs[j], &tmp, sizeof(struct mem_region));
						break;
					}
				}
			}

			//	+---+-------+
			//	^	^		^
			//	mrs
			//		prs		pre
			mrs[i].end = ALIGN_DOWN(prs);
		}

		if(mrs[i].start >= prs && mrs[i].start <= pre) {
			if(mrs[i].end < pre) {
				//	+---+-------+
				//	^	^		^	
				//	mrs	mre
				//	prs			pre

				memset(&mrs[i], 0, sizeof(struct mem_region));
			} else if(mrs[i].end > pre) {
				//	+-----------+---+
				//	^			^	^
				//	mrs				mre
				//	prs			pre

				mrs[i].start = ALIGN_UP(pre);
			}
		}
	}
}

void
pmm_init(multiboot_memory_map_t *mmap, uint entries, uint total) {
	uint i, j, n;
	struct mem_region mrs[255] = {0};
	uint 	ks = (uint)(&KERNEL_START),
			ke = (uint)(&KERNEL_END);

	assert(mmap != 0);
	assert(entries > 0);
	assert(total > 0);

	MMAP_SIZE_PMM = 0;

	/* copy mmap information into the mem region table */
	for(i=0; i<entries; i++) {
		mrs[i].start	= ALIGN_UP(mmap[i].addr);
		mrs[i].end		= ALIGN_DOWN(mmap[i].addr + mmap[i].len);
	}

	/* check, if critical regions are inside the table */
	protected_region(mrs, 255, ks, ke);
	protected_region(mrs, 255, MMAP_PHYS_VIDEO, (MMAP_PHYS_VIDEO + MMAP_SIZE_VIDEO));
	protected_region(mrs, 255, MMAP_PHYS_PAGING, (MMAP_PHYS_PAGING + MMAP_SIZE_PAGING));

	/* finally, calculate the pmm stack size and
		remove conflicting adresses in the mrs */
	for(i=0; i<255; i++) {
		if(mrs[i].end == 0) {
			continue;
		}

		MMAP_SIZE_PMM += (mrs[i].end - mrs[i].start) / PAGE_ALIGNMENT;
	}

	protected_region(mrs, 255, MMAP_PHYS_PMM, ALIGN_UP(MMAP_PHYS_PMM + MMAP_SIZE_PMM));

	/* fill the stack */
	pmm_sp = (uint *)(MMAP_PHYS_PMM);
	*pmm_sp++ = 0;	// root

	for(i=0; i<255; i++) {
		n = (mrs[i].end - mrs[i].start) / PAGE_ALIGNMENT;

		for(j=0; j<n; j++) {
			*pmm_sp++ = mrs[i].start + (j * PAGE_ALIGNMENT);
		}
	}

	pmm_sp--;
}

void *
pmm_alloc(void) {
	if(*pmm_sp == 0) {
		return 0; // no more memory available
	}

	return (void *)(*pmm_sp--);
}

void
pmm_free(void *page) {
	*++pmm_sp = (uint)page;
}



