; Declare constants for the multiboot header.
ARCH 		equ 0x00000000
HEADER_LEN 	equ header_end - header_start
MAGIC    	equ  0xE85250D6
CHECKSUM 	equ -(MAGIC + ARCH + HEADER_LEN)

section .multiboot
align 4
header_start:
	dd MAGIC
	dd ARCH
	dd HEADER_LEN
	dd CHECKSUM

	; end tag
	dw 0
	dw 0
	dd 8
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
	mov esp, stack_top

	; enable write protect for testing page faults
	mov edx, cr0
	bts edx, 16
	mov cr0, edx
	extern kernel_main
	call kernel_main

	extern asm_halt
	jmp asm_halt
.end:
