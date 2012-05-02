/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - mm.h
*/
#ifndef K_MM_H_
#define K_MM_H_

#include <lateralOS/types.h>

#include <lateralOS/mm/mmap.h>
#include <lateralOS/mm/pmm.h>
#include <lateralOS/mm/vmm.h>

#define MM_CHUNK_SIZE 		PAGE_ALIGNMENT * 2
#define MM_CHUNK_UNUSED		(MM_CHUNK_SIZE + 1)

#define MM_CHUNKS_PER_PAGE	PAGE_ALIGNMENT / sizeof(struct mm_chunk)

struct mm_block {
	byte reserved 	: 1;
	uint size 		: 31;
};

struct mm_chunk {
	uint size;

	struct mm_block *first_block;
	struct mm_chunk *next_chunk;
};

void *
k_malloc(uint size);

void *
k_realloc(void *base, uint size);

void
k_free(void *base);

#endif /* K_MM_H_ */
