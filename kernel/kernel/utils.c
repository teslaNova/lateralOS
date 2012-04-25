/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - utils.c
*/
#include <lateralOS/utils.h>

void
memset(void *data, int c, uint size) {
	while(size) {
		((char *)data)[--size] = (char)c;
	}
}

void
memcpy(void *dest, void *src, uint size) {
	while(size-- > 0) {
		*(byte *)(dest+size) = *(byte *)(src+size);
	}
}

int
memcmp(void *m1, void *m2, uint size) {
	while(((byte *)m1)[size-1] == ((byte *)m2)[size-1] && size > 0) {
		--size;
	}

	if(size > 0) {
		return (((byte *)m1)[size-1] > ((byte *)m2)[size-1]) ? -1 : 1;
	}

	return 0;
}

byte
inb(ushort port) {
	byte data;

	__asm__ (
		"inb %w1, %b0"
		: "=a" (data)
		: "d" (port)
	);

	return data;
}

void
outb(ushort port, byte data) {
	__asm__ (
		"outb %b0, %w1"
		:: "a" (data), "d" (port)
	);
}
