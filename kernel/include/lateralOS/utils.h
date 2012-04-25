/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - utils.h
*/
#ifndef K_UTILS_H_
#define K_UTILS_H_

#include <lateralOS/types.h>

void
memset(void *data, int c, uint size);

void
memcpy(void *dest, void *src, uint size);

int
memcmp(void *m1, void *m2, uint size);

byte inline
inb(ushort port);

void inline
outb(ushort port, byte data);

#endif /* K_UTILS_H_ */