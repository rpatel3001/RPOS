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
; stack space
alignb 16
	stack_bottom:
		resb 16384 ; 16 KiB
	stack_top:

; paging structures
alignb 32
	boot_PDP:
		resq 4
alignb 4096
	boot_PD:
		resq 512

section .text
bits 32
	global _start:function
	_start:
		; init stack and save multiboot registers
		mov esp, stack_top
		push eax
		push ebx

		; set up and enable paging
		; put the initial page directory into the page directory pointer table
		mov edx, boot_PD
		bts edx, 0
		mov [boot_PDP], edx
		; mark the first page as a 2 MiB huge page starting at 0x0
		mov edx, 0x83
		mov [boot_PD], edx
		; enable PAE
		mov edx, cr4
		bts edx, 5
		mov cr4, edx
		; load the page directory pointer table
		mov edx, boot_PDP
		mov cr3, edx
		; enable paging
		mov edx, cr0
		bts edx, 31
		mov cr0, edx
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
