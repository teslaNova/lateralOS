/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - types.h
*/
#ifndef K_TYPES_H_
#define K_TYPES_H_

//#define naked __attribute__ ((naked))

typedef unsigned char 			byte;
typedef unsigned short 			ushort;
typedef unsigned int			uint;
typedef unsigned long long int	uint64;
typedef long long int			int64;

struct cregs {
	uint	cr0, cr2, cr3, cr4;
};

struct segs {
	uint	ds, es, fs, gs;
};

struct regs {
	uint 	edi, esi,
			ebp, old_esp,
			ebx, edx, ecx, eax;
};

struct iregs {
	uint	eip, cs, eflags, uesp, ss;
};

#endif /* K_TYPES_H_ */
