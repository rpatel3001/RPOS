; Declare constants for the multiboot header.
MAGIC    equ 0x1BADB002
FLAGS    equ 0
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
	header_start:
		dd MAGIC
		dd FLAGS
		dd CHECKSUM
	header_end:

section .bss
align 4
	stack_bottom:
		resb 16384 ; 16 KiB
	stack_top:

section .text
bits 32
	global _start:function
	_start:
		; init stack and save multiboot registers
		mov esp, stack_top
		push eax
		push ebx

		; enable write protect for testing page faults
		mov edx, cr0
		bts edx, 16
		mov cr0, edx

		; jump to the kernel proper
		pop ebx
		pop eax
		extern kernel_main
		jmp kernel_main
	.end:
