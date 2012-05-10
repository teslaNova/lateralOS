/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - alloc.h
*/
#include <lateralOS/lock.h>

void
lock_acquire(byte *l) {
	__asm__ volatile (
		".acquire:"
		"lock bts $0, %0;"
		"jnc .acquire;"
		:"=m"(l)
	);
}

void
lock_release(byte *l) {
	__asm__ volatile (
		"lock btr $0, %0;"
		:"=m"(l)
	);
}

void
sem_wait(byte *s) {
	uint t = *s;
	while(*s == t);
}

void
sem_signal(byte *s) {
	*s++;
}
