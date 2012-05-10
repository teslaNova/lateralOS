/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - task.c
*/
#include <lateralOS/task.h>

#include <lateralOS/utils.h>
#include <lateralOS/descriptors.h>

#include <lateralOS/screen.h>

struct task *tasks = 0;
extern struct page_directory *kernel_pd;

void
prepare_stack(struct task *task, uint eip) {
	struct ism_context sd = {0};

	if(task->privilege == TASK_PL_RING0) {
		sd.s.gs = SEL_R0_DATA;
		sd.s.es = SEL_R0_DATA;
		sd.s.fs = SEL_R0_DATA;
		sd.s.ds = SEL_R0_DATA;

		sd.i.cs = SEL_R0_CODE;
		sd.i.ss = SEL_R0_DATA;
	} else {
		sd.s.gs = SEL_R3_DATA;
		sd.s.es = SEL_R3_DATA;
		sd.s.fs = SEL_R3_DATA;
		sd.s.ds = SEL_R3_DATA;

		sd.i.cs = SEL_R3_CODE;
		sd.i.ss = SEL_R3_DATA;
	}

	sd.i.eip = eip;
	sd.i.eflags._b._if = 1;

	sd.i.uesp = task->ustack;

	sd.r.old_esp = task->stack;

	memcpy(task->stack, (void *)(&sd), sizeof(struct ism_context));
}

void
insert_task(struct task *task) {
	struct task *it, *tmp;

	if(tasks == 0) {
		tasks = task;
		return;
	}

	if(tasks->priority < task->priority) {
		tmp = tasks;
		tasks = task;
		task->next_task = tmp;
		tmp->prev_task = task;
		return;
	}

	it = tasks;

	while(it->priority >= task->priority && it->next_task != 0) {
		it = it->next_task;
	}

	if(it->next_task == 0) {
		it->next_task = task;
		task->prev_task = it;
	} else if(it->priority < task->priority) {
		it->prev_task->next_task = task;
		
		task->next_task = it;
		task->prev_task = it->prev_task;

		it->prev_task = task;
	}
}

struct task*
task_create(byte *name, enum task_privilege privilege, enum task_priority priority, uint initial_eip) {
	static uint pid = 0;
	struct task *task = k_malloc(sizeof(struct task));
	uint name_size;

	if(task == 0) {
		return 0;
	}

	memset(task, 0, sizeof(struct task));
	task->stack = k_malloc(TASK_STACK_SIZE);

	if(task->stack == 0) {
		k_free(task);
		return 0;
	}

	if(privilege == TASK_PL_RING3) {
		task->ustack = k_malloc(TASK_STACK_SIZE);

		if(task->ustack == 0) {
			k_free(task->stack);
			k_free(task);
			return 0;
		}

		memset(task->ustack, 0, TASK_STACK_SIZE);
	}

	memset(task->stack, 0, TASK_STACK_SIZE);

	task->pid = ++pid;

	if(name != 0) {
		name_size = strlen(name);
		name_size = name_size <= TASK_NAME_SIZE ? name_size : TASK_NAME_SIZE;
		memcpy(task->name, name, name_size);

	}

	task->privilege = privilege;
	task->priority = priority;
	task->status = TASK_STATE_WAITING;
	
	task->ticks_max = TASK_TICKS_MAX;
	task->ticks_left = TASK_TICKS_MAX;

	task->pd = kernel_pd; // just for now

	insert_task(task);
	prepare_stack(task, initial_eip);
}

void
task_destroy(uint pid) {
	struct task *it = tasks;

	if(it == 0) {
		return;
	}

	while(it != 0) {
		if(it->pid == pid) {
			if(it->prev_task != 0) {
				if(it->next_task != 0) {
					it->prev_task->next_task = it->next_task;
					it->next_task->prev_task = it->prev_task;
				} else {
					it->prev_task->next_task = 0;
				}
			} else if(it->next_task != 0) {
				it->next_task->prev_task = 0;
			}

			k_free(it);
			return;
		}

		it = it->next_task;
	}
}

struct task*
task_get_task_list(void) {
	return tasks;
}

struct task*
task_get_next_task(struct task *c) {
	struct task *it = (c == 0 ? tasks : c);

	if(it == 0) {
		return 0;
	}

	if(it->ticks_left > 0) {
		it->ticks_left--;
		return it;
	}

	while((it = it->next_task) != 0) {
		if(it->status == TASK_STATE_WAITING
		&& it->ticks_left > 0) {
			k_printf("%s", it->name);
			it->status = TASK_STATE_RUNNING;
			it->ticks_left++;
			
			return it;
		}
	}

	task_reset();
	return task_get_next_task(0);
}

void
task_reset(void) {
	struct task *it = tasks;

	if(it == 0) {
		return;
	}

	while(it != 0) {
		it->ticks_left = it->ticks_max;

		if(it->status != TASK_STATE_SUSPENDED) {
			it->status = TASK_STATE_WAITING;
		}

		it = it->next_task;
	}
}

