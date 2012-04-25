/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	CSTDLIB - stdarg.h
*/
#ifndef CSTDLIB_H_
#define CSTDLIB_H_ 

#define va_list			__builtin_va_list
#define va_start(v, l)	__builtin_va_start((v), l)
#define va_end			__builtin_va_end
#define va_arg			__builtin_va_arg
#define va_copy(d, s)	__builtin_va_copy((d), (s))

#endif /* CSTDLIB_H_ */