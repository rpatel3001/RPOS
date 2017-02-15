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
	higher_PD:
		resq 512

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

		; also set up a page at 0xC0000000 that points to 0x0
		; insert the 3rd page directory (3GiB+)
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

		; pop off the multiboot information
		pop ebx
		pop eax

		; enable paging
		mov edx, cr0
		bts edx, 31
		mov cr0, edx

		; reset the stack to the virtual address
		mov esp, stack_top

		; push multiboot registers
		push eax
		push ebx

		; enable write protect for testing page faults
		mov edx, cr0
		bts edx, 16
		mov cr0, edx

		; invalidate the identity mapping
		;mov dword [boot_PDP], 0
		;mov dword [boot_PDP + 4], 0
		;mov dword [boot_PD], 0
		;mov dword [boot_PD + 4], 0
		;invlpg [0]

		; jump to the kernel proper
		extern kernel_main
		lea ecx, [kernel_main]
		jmp ecx
	.end:
