/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - sched.c
*/
#include <lateralOS/sched.h>

#include <lateralOS/ism/gateways.h>
#include <lateralOS/ism/ism.h>
#include <lateralOS/task.h>

struct task* current_task = 0;

void
scheduler_init(void) {
	ism_set_isr(INT_TIMER, schedule);
}

void
schedule(struct ism_context *c) {
	k_cls();

	if(current_task != 0) {
		/*
			maybe check here if stack is over limit or
			is near it, to allocate more memory.
		*/
		k_printf("0x%x = 0x%x\r\n", current_task->stack, (byte *)c);
		current_task->stack = (byte *)(c);
	}

	current_task = task_get_next_task(current_task);

	if(current_task == 0) {
		return;
	}

	print_stack((uint)c, sizeof(struct ism_context));

	if(current_task->stack != (byte *)(c)) {
		switch_task((uint)(current_task->stack));
	}
}
