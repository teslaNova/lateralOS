/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - task.h
*/
#include <lateralOS/types.h>
#include <lateralOS/mm/mm.h>
#include <lateralOS/ism/ism.h>

#ifndef K_TASK_H_
#define K_TASK_H_

#define TASK_TICKS_MAX 	2
#define TASK_STACK_SIZE PAGE_ALIGNMENT
#define TASK_NAME_SIZE	30

enum task_privilege {
	TASK_PL_RING0 = 0,
	TASK_PL_RING3,
};

enum task_priority {
	TASK_PR_LOW = 0,
	TASK_PR_MED,
	TASK_PR_HIGH,
	TASK_PR_VERY_HIGH,
};

enum task_state {
	TASK_STATE_WAITING = 0,
	TASK_STATE_RUNNING,
	TASK_STATE_SUSPENDED,
};

struct thread {
	struct thread *next_thread;
	struct thread *prev_thread;
};

struct task {
	uint pid;
	byte name[TASK_NAME_SIZE + 1];

	byte privilege;
	byte priority;
	byte status;

	uint ticks_left;
	uint ticks_max;

	struct page_directory *pd;

	byte *stack;
	byte *ustack;

	struct task *next_task;
	struct task *prev_task;

	struct thread *first_thread;
};

struct task*
task_create(byte *name, enum task_privilege privilege, enum task_priority priority, uint initial_eip);

void
task_destroy(uint pid);

inline struct task*
task_get_task_list(void);

struct task*
task_get_next_task(struct task *c);

void
task_reset(void);

#endif /* K_TASK_H_ */

