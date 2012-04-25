/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - diag.h
*/
#ifndef K_DIAG_H_
#define K_DIAG_H_

#include <lateralOS/lateralOS.h>
#include <lateralOS/screen.h>

#ifdef assert
# undef assert
#endif 

#ifndef NDEBUG
# define assert( expr ) if((expr) == 0) {												\
							k_cls();													\
							k_printf("assert(): failed; expected '"#expr"', got '%x'",	\
										(expr),											\
										__FILE__, __LINE__);							\
							panic();													\
						}
#else
# define assert( expr )	
#endif

#endif /* K_DIAG_H_ */