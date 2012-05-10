/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - sched.h
*/
#ifndef K_SCHED_H_
#define K_SCHED_H_

struct ism_context;

void
scheduler_init(void);

void
schedule(struct ism_context *c);

#endif /* K_SCHED_H_ */
