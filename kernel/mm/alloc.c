/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - alloc.c
*/
#include <lateralOS/mm/mm.h>

#include <lateralOS/lock.h>

struct mm_chunk *k_heap = 0;

struct mm_chunk *
request_chunks(void) {
	struct mm_chunk *chunk;
	uint i, base;

	struct vmm_flags flags = {
		.writeable = 1,
		.privileged = 1,
		.present = 1,
	};

	base =	(uint)vmm_alloc_range(flags, 1, MMAP_VIRT_ALLOC_START, MMAP_VIRT_ALLOC_END);
	chunk = (struct mm_chunk *)(base);
	
	for(i=1; i<MM_CHUNKS_PER_PAGE; i++) {
		chunk->size = MM_CHUNK_UNUSED;
		chunk->first_block = 0;
		chunk->next_chunk = (struct mm_chunk *)(base + (i * sizeof(struct mm_chunk)));
		chunk = chunk->next_chunk;
	}

	chunk->next_chunk = 0;
	return (struct mm_chunk *)(base);
}

void *
k_malloc(uint size) {
	static byte alloc_lock;

	struct mm_chunk *chunk;
	struct mm_block *block;
	uint actual_size = size + sizeof(struct mm_block);

	lock_acquire(&alloc_lock);

	struct vmm_flags flags = {
		.writeable = 1,
		.privileged = 1,
		.present = 1,
	};

	if(actual_size > 0x7FFFFFFF) {
		lock_release(&alloc_lock);
		return (void *)0;
	}

	if(k_heap == 0) {
		k_heap = request_chunks();

		if(k_heap == 0) {
			lock_release(&alloc_lock);
			return (void *)0;
		}
	}

	chunk = k_heap;

	while(chunk->size < actual_size && chunk->next_chunk != 0) {
		chunk = chunk->next_chunk;
	}

	if(chunk->size < actual_size && chunk->next_chunk == 0) {
		chunk->next_chunk = request_chunks();
		chunk = chunk->next_chunk;

		if(chunk == 0) {
			lock_release(&alloc_lock);
			return (void *)0;
		}
	}

	if(chunk->size >= actual_size) {
		if(chunk->size == MM_CHUNK_UNUSED) {
			chunk->size = MM_CHUNK_SIZE;
			chunk->first_block = vmm_alloc_range(flags,
				MM_CHUNK_SIZE / PAGE_ALIGNMENT,
				MMAP_VIRT_ALLOC_START,
				MMAP_VIRT_ALLOC_END);

			memset(chunk->first_block, 0, MM_CHUNK_SIZE);
		}

		block = chunk->first_block;

		if(block == 0) {
			lock_release(&alloc_lock);
			return (void *)0;
		}

		while(block->reserved == 1 /* maybe check here if it overruns (in case of bufferoverflow) */) {
			block = (struct mm_block *)((uint)block + block->size);
		}
		
		block->reserved = 1;
		block->size = (size & 0x7FFFFFFF);
		chunk->size -= actual_size;

		lock_release(&alloc_lock);
		return (void *)((uint)block + sizeof(struct mm_block));
	}

	lock_release(&alloc_lock);
	return (void *)0;
}

void *
k_realloc(void *base, uint size) {
	void *new = k_malloc(size);

	if(new == 0) {
		k_free(base);
		return 0;
	}

	memcpy(new, base, size);
	k_free(base);
	
	return new;
}

void
k_free(void *base) {
	static byte free_lock;

	struct mm_chunk *chunk = k_heap;
	struct mm_block *block;

	lock_acquire(&free_lock);

	if(chunk == 0) {
		return;
	}

	do {
		if(chunk->size == MM_CHUNK_UNUSED) {
			continue;
		}

		block = chunk->first_block;

		if(block == 0) {
			continue;
		}

		do {
			if(((uint)block - (uint)chunk->first_block) >= MM_CHUNK_SIZE) {
				break;
			}

			if(block->size == 0) {
				break;
			}

			void *block_base = (void *)((uint)block + sizeof(struct mm_block));

			if(block_base == base) {
				block->reserved = 0;
				chunk->size += block->size + sizeof(struct mm_block);

				/*
					enhance!
				*/

				lock_release(&free_lock);
				return;
			}

			block = (struct mm_block *)((uint)block + block->size);
		} while(block->reserved == 1);

		chunk = chunk->next_chunk;
	} while(chunk->size != MM_CHUNK_UNUSED && chunk->next_chunk != 0);

	lock_release(&free_lock);
}
