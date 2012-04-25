.set MB_MAGIC, 0x1BADB002
.set MB_FLAGS, 0x3
.set MB_CHKSM, -(MB_MAGIC + MB_FLAGS)

.set STACK_SIZE, 0x8000

.global _init, init
.extern _main

.text
	.long MB_MAGIC
	.long MB_FLAGS
	.long MB_CHKSM

init:
_init:
	
	movl $(stack + STACK_SIZE), %esp

	pushl $0
	popf

	pushl %ebx
	pushl %eax

	call _main

	cli
	hlt

.bss
	.comm stack, STACK_SIZE
	
