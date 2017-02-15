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
alignb 4096
	boot_PD:
		resq 512
	higher_PD:
		resq 512

; stack space
alignb 16
	stack_bottom:
		resb 16384 ; 16 KiB
	stack_top:

; paging structures
alignb 32
	boot_PDP:
		resq 4

extern KERNEL_VMA_OFFS
section .text
bits 32
	global _start:function
	_start:
		; set up the stack at the physical address
		mov esp, stack_top
		sub esp, KERNEL_VMA_OFFS

		; save multiboot registers
		push eax
		push ebx

		; UNTIL WE ENABLE PAGING, EVERYTHING MUST BE A PHYSICAL ADDRESS
		; HENCE WE SUBTRACT THE OFFSET FROM ALL ADDRESSES
		; set up and enable paging
		; put the initial page directory into the page directory pointer table
		mov edx, boot_PD
		sub edx, KERNEL_VMA_OFFS
		bts edx, 0
		mov ecx, boot_PDP
		sub ecx, KERNEL_VMA_OFFS
		mov [ecx], edx

		; mark the first page as a 2 MiB huge page starting at 0x0
		mov edx, 0x83
		mov ecx, boot_PD
		sub ecx, KERNEL_VMA_OFFS
		mov [ecx], edx

		; also set up a page at the higher half address that points to 0x0
		mov edx, higher_PD
		sub edx, KERNEL_VMA_OFFS
		bts edx, 0
		mov ecx, boot_PDP
		sub ecx, KERNEL_VMA_OFFS

		; calculate which PDP entry to insert into
		mov eax, KERNEL_VMA_OFFS
		shr eax, 30
		mov [ecx + eax * 8], edx

		; insert the first (huge) page of the 3rd page directory
		mov edx, 0x83
		mov ecx, higher_PD
		sub ecx, KERNEL_VMA_OFFS
		
		; calculate which PD page to use
		mov eax, KERNEL_VMA_OFFS
		shr eax, 21
		and eax, 0x1FF
		mov [ecx + eax * 8], edx

		; enable PAE
		mov edx, cr4
		bts edx, 5
		mov cr4, edx

		; load the page directory pointer table
		mov edx, boot_PDP
		sub edx, KERNEL_VMA_OFFS
		mov cr3, edx

		pop ebx
		pop eax

		; enable paging
		mov edx, cr0
		bts edx, 31
		mov cr0, edx

		; reset the stack to the virtual address
		mov esp, stack_top

		; enable write protect for testing page faults
		mov edx, cr0
		bts edx, 16 
		mov cr0, edx

		push ebx
		push eax
		; need to push an extra value for kernel_main's parameter to be right
		; will look into this later
		push 0

		; jump to the kernel proper
		extern kernel_main
		jmp kernel_main
	.end:
