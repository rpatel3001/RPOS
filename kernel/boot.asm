; Declare constants for the multiboot header.
MBALIGN  equ  1<<0              ; align loaded modules on page boundaries
MEMINFO  equ  1<<1              ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

; Declare a multiboot header that marks the program as a kernel. These are
; magic values that are documented in the multiboot standard. The bootloader
; will search for this signature in the first 8 KiB of the kernel file,
; aligned at a 32-bit boundary. The signature is in its own section so the 
; header can be forced to be within the first 8 KiB of the kernel file.
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; The multiboot standard does not define the value of the stack pointer
; register (esp) and it is up to the kernel to provide a stack. This allocates
; room for a small stack by creating a symbol at the bottom of it, then
; allocating 16384 bytes for it, and finally creating a symbol at the top. The
; stack grows downwards on x86. The stack is in its own section so it can be
; marked nobits, which means the kernel file is smaller because it does not
; contain an uninitialized stack. The stack on x86 must be 16-byte aligned
; according to the System V ABI standard and de-facto extensions. The compiler
; will assume the stack is properly aligned and failure to align the stack
; will result in undefined behavior.
section .bss
align 4
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
; Declare _start as a function symbol with the given symbol size.
section .text

global get_eax:function
get_eax:
	ret

global get_ebx:function
get_ebx:
	push ebx
	pop eax
	ret

global get_ecx:function
get_ecx:
	push ecx
	pop eax
	ret

global get_edx:function
get_edx:
	push edx
	pop eax
	ret

global get_esi:function
get_esi:
	push esi
	pop eax
	ret

global get_edi:function
get_edi:
	push edi
	pop eax
	ret

global get_ebp:function
get_ebp:
	push ebp
	pop eax
	ret

global get_esp:function
get_esp:
	push esp
	pop eax
	ret

global get_flags:function
get_flags:
	pushfd
	pop eax
	ret

global get_ss:function
get_ss:
	push ss
	pop eax
	ret

global get_cs:function
get_cs:
	push cs
	pop eax
	ret

global get_ds:function
get_ds:
	push ds
	pop eax
	ret

global get_es:function
get_es:
	push es
	pop eax
	ret

global get_fs:function
get_fs:
	push fs
	pop eax
	ret

global get_gs:function
get_gs:
	push gs
	pop eax
	ret

get_eip: 
	mov eax, [esp]
	ret

global read_port:function
read_port:
	mov edx, [esp + 4]
	in al, dx
	ret

global write_port:function
write_port:
	mov   edx, [esp + 4]
	mov   al, [esp + 4 + 4]
	out   dx, al
	ret

global load_idt:function
load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret

global keyboard_handler:function
keyboard_handler:
	extern keyboard_handler_main
	call    keyboard_handler_main
	iretd

global _start:function
_start:
	; The bootloader has loaded us into 32-bit protected mode on a x86
	; machine. Interrupts are disabled. Paging is disabled. The processor
	; state is as defined in the multiboot standard. The kernel has full
	; control of the CPU.
 
	; To set up a stack, we set the esp register to point to the top of our
	; stack (as it grows downwards on x86 systems). This is necessarily done
	; in assembly as languages such as C cannot function without a stack.
	mov esp, stack_top
 
	; This is a good place to initialize crucial processor state before the
	; high-level kernel is entered. It's best to minimize the early
	; environment where crucial features are offline. Note that the
	; processor is not fully initialized yet: Features such as floating
	; point instructions and instruction set extensions are not initialized
	; yet. The GDT should be loaded here. Paging should be enabled here.
 
	; Enter the high-level kernel. The ABI requires the stack is 16-byte
	; aligned at the time of the call instruction (which afterwards pushes
	; the return pointer of size 4 bytes).

	extern kernel_main
	call kernel_main

	; If the system has nothing more to do, put the computer into an
	; infinite loop.
	cli
.hang:	hlt
	jmp .hang
.end:
