/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - screen.c
*/
#include <lateralOS/screen.h>

#include <lateralOS/diag.h>
#include <stdarg.h>

const ushort video_width = (80*25*2);
ushort *video = (ushort *)0xB8000;
byte	pos_x = 0;
byte	pos_y = 0;

void
k_cls(void) {
	ushort i;

	for(i=0; i<video_width; i++) {
		*(video + i) = 0;
	}

	pos_x = 0;
	pos_y = 0;
}

void
k_putc(char c) {
	short pos;

	if(c == '\r') {
		pos_x = 0;
		return;
	} else if(c == '\n') {
		pos_y++;
		return;
	} else if(c == '\t') {
		pos_x += 4;
		return;
	}

	pos = pos_x + (pos_y * 80);
	*(video + pos) = (short)(0x0F << 8 | c);

	if(++pos_x >= 80) {
		pos_x = 0;
		pos_y++;
	}
}

void
k_putn(const int n, const int base) {
	const byte chrtbl[]= "0123456789ABCDEF";
	const int buflen = 64;

	assert(base > 0);

	char buf[buflen], tmp[buflen];
	char is_neg = (n < 0 ? 1 : 0);
	int j = 0, i = (is_neg ? -n : n), q;

	memset(buf, 0, buflen);
	memset(tmp, 0, buflen);

	do {
		q = (int)(i % base);
		i /= base;

		tmp[j++] = chrtbl[q];
	} while(i > 0);

	if(is_neg) {
		buf[0] = '-';
	} else {
		j -= 1;
	}

	for(q=(is_neg ? 1 : 0); q <= j; q++) {
		buf[q] = tmp[j - q];
	}

	k_puts(buf);
}

void
k_puts(const char *msg) {
	while(*msg != 0x0) {
		k_putc(*msg);
		msg++;
	}
}

void
k_printf(const char *format, ...) {
	va_list ap;
	va_start(ap, format);

	while(*format != 0) {
		while(*format != 0 && *format != '%') {
			k_putc(*format++);
		}

		if(*format == 0) {
			break;
		}

		switch(*++format) {
			case 'c': {
				k_putc((char)va_arg(ap, int));
			} break;

			case 's': {
				k_puts(va_arg(ap, char*));
			} break;

			case 'd': {
				k_putn(va_arg(ap, int), 10);
			} break;

			case 'u': {
				k_putn(va_arg(ap, uint), 10);
			}

			case 'x': {
				k_putn(va_arg(ap, uint), 16);
			} break;

			case 'p': {
				k_putn((int)va_arg(ap, void*), 16);
			};

			default: break;
		}

		++format;
	}

	va_end(ap);
}
