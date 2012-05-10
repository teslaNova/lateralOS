/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - lock.h
*/
#ifndef K_LOCK_H_
#define K_LOCK_H_

#include <lateralOS/types.h>

/* SPINLOCK */
void inline
lock_acquire(byte *l);

void inline
lock_release(byte *l);

/* SEMAPHORE */
void
sem_wait(byte *s);

void
sem_signal(byte *s);


#endif /* K_LOCK_H_ */
