; Declare constants for the multiboot header.
MAGIC    equ 0x1BADB002
; flags to 4K align the kernel, provide memory info, and provide graphics info
FLAGS    equ 0x00000007
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
	header_start:
		; required entries
		dd MAGIC
		dd FLAGS
		dd CHECKSUM
		; filler addr fields
		times 5 dd 0
		; preferred graphics info
		; mode: 0 = graphics, 1 = text
		dd 1
		; width
		dd 80
		; height
		dd 25
		; bits per pixel: 0 for text
		dd 0
	header_end:

section .bss
alignb 4096
	kernel_PD0:
		resq 512
	kernel_PD1:
		resq 512
	kernel_PD2:
		resq 512
	kernel_PD3:
		resq 512

; paging structures
alignb 32
	kernel_PDP:
		resq 4

; stack space
alignb 16
	stack_bottom:
		resb 16384 ; 16 KiB
	stack_top:

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
		; put the kernel page directories into the kernel page directory pointer table
		; kernel_PD0
		mov edx, kernel_PD0
		sub edx, KERNEL_VMA_OFFS
		bts edx, 0
		mov ecx, kernel_PDP
		sub ecx, KERNEL_VMA_OFFS
		mov [ecx], edx
		; kernel_PD1
		mov edx, kernel_PD1
		sub edx, KERNEL_VMA_OFFS
		bts edx, 0
		mov ecx, kernel_PDP
		sub ecx, KERNEL_VMA_OFFS
		mov [ecx+8], edx
		; kernel_PD2
		mov edx, kernel_PD2
		sub edx, KERNEL_VMA_OFFS
		bts edx, 0
		mov ecx, kernel_PDP
		sub ecx, KERNEL_VMA_OFFS
		mov [ecx+16], edx
		; kernel_PD3
		mov edx, kernel_PD3
		sub edx, KERNEL_VMA_OFFS
		bts edx, 0
		mov ecx, kernel_PDP
		sub ecx, KERNEL_VMA_OFFS
		mov [ecx+24], edx

		; mark the first page as a 2 MiB huge page starting at 0x0
		mov edx, kernel_PD0
		sub edx, KERNEL_VMA_OFFS
		mov dword [edx], 0x83

		; also set up a page at the higher half address that points to 0x0
		; calculate which PDP entry to insert into
		mov eax, KERNEL_VMA_OFFS
		shr eax, 30
		; calculate address of the PD we need
		imul ebx, eax, 4096
		mov edx, kernel_PD0
		sub edx, KERNEL_VMA_OFFS
		add edx, ebx
		bts edx, 0
		; load the PD into the PDP
		mov ecx, kernel_PDP
		sub ecx, KERNEL_VMA_OFFS
		mov [ecx + eax * 8], edx
		; calculate which PD page to use
		mov eax, KERNEL_VMA_OFFS
		shr eax, 21
		and eax, 0x1FF
		; insert the first page of the 3rd page directory
		mov dword [edx + eax * 8 - 1], 0x83

		; enable PAE
		mov edx, cr4
		bts edx, 5
		mov cr4, edx

		; load the page directory pointer table
		mov edx, kernel_PDP
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
		lea ecx, [kernel_main]
		jmp ecx
	halt: hlt
	.end:
