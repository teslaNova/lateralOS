/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - descriptors.c
*/
#include <lateralOS/descriptors.h>

#include <lateralOS/diag.h>
#include <lateralOS/utils.h>

#include <lateralOS/screen.h>

struct desc_register 	gdtr = {0}, idtr = {0};
struct gdt_entry		gdt[DESC_GDT_SIZE] = {0};
struct idt_entry		idt[DESC_IDT_SIZE] = {0};

void
gdt_init(void) {
	gdt_set(0, 0, 0, 0, 0); // null gate
	gdt_set(1, 0, 0xFFFFFFFF, DESC_CODE_FLAGS, DESC_CODE_ACCESS);
	gdt_set(2, 0, 0xFFFFFFFF, DESC_DATA_FLAGS, DESC_DATA_ACCESS);

	gdtr.size = (sizeof(struct gdt_entry) * DESC_GDT_SIZE) - 1;
	gdtr.offset = (unsigned int)gdt;

	gdt_load();
}

void
gdt_set(uint gate, uint base, uint limit,  byte flags, byte access) {
	struct gdt_entry *cur;

	assert(gate < DESC_GDT_SIZE);

	cur = &gdt[gate];
	cur->limit_lo = (limit & 0xFFFF);
	cur->limit_hi = ((limit >> 16) & 0xF);

	cur->base_lo = (base & (0xFFFFFF));
	cur->base_hi = ((base >> 24) & 0xFF);

	cur->access = access;
	cur->flags = flags;

	cur->access |= DESC_ACCESS_ALWAYS;

	/*if(gate > 0)
	k_printf("\t\tGDT Entry #%x:\r\n\t\t\t"
			"Limit:  0x%x|%x\r\n\t\t\t"
			"Base:   0x%x|%x\r\n\t\t\t"
			"Access: %x\r\n\t\t\t"
			"Flags:  %x\r\n",
			gate, cur->limit_hi, cur->limit_lo,
			cur->base_hi, cur->base_lo,
			cur->access, cur->flags);*/
}

void
gdt_load(void) {
	__asm__("lgdt %0;"
			"ljmp %1, $_post_init_gdt;"
			"_post_init_gdt:"
			"movl %2, %%eax;"
			"movw %%ax, %%ds;"
			"movw %%ax, %%es;"
			"movw %%ax, %%fs;"
			"movw %%ax, %%gs;"
			"movw %%ax, %%ss;"
			:: "m"(gdtr), "i"(SEL_R0_CODE), "i"(SEL_R0_DATA));
}

void
idt_init(void) {
	idtr.size = (sizeof(struct idt_entry) * DESC_IDT_SIZE) - 1;
	idtr.offset = (unsigned int)idt;

	idt_load();
}

void
idt_set(uint gate, uint handler, uint type, uint dpl) {
	struct idt_entry *cur;

	assert(gate < DESC_IDT_SIZE);

	cur = &idt[gate];

	cur->handler_lo = (handler & 0xFFFF);
	cur->handler_hi = (handler >> 16) & 0xFFFF;
	cur->selector = (ushort)SEL_R0_CODE;
	cur->storage = (DESC_GATE_INT == type) ? 0 : 1;
	cur->type = type;
	cur->dpl = dpl;
	cur->present = 1;

	/*k_printf("\t\tIDT #%x:\r\n\t\t\t"
		"handler 0x%x|%x\r\n\t\t\t"
		"selector: %x\r\n\t\t\t"
		"storage: %x\r\n\t\t\t"
		"type: %x\r\n\t\t\t"
		"dpl: %x\r\n\t\t\t"
		"present: %x\r\n\r\n",
		gate, cur->handler_hi, cur->handler_lo,
		cur->selector, cur->storage,
		cur->type, cur->dpl, cur->present);*/
}

void
idt_load(void) {
	__asm__("lidt %0;"
			:: "m"(idtr));
}

