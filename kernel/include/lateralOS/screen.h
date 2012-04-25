/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - screen.h
*/
#ifndef K_SCREEN_H_
#define K_SCREEN_H_

#include <lateralOS/types.h>

void
k_cls(void);

void
k_putc(char c);

void
k_putn(const uint64 n, const int base);

void
k_puts(const char *msg);

void
k_printf(const char *format, ...);

#endif /* K_SCREEN_H_ */
