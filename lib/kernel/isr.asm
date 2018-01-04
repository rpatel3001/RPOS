section .text
bits 32

; interrupt handlers go here

; save registers and push esp, call the isr, then pop the two arguments
asm_handle_interrupt:
	cli
	pushad
	push esp
	extern handle_interrupt
	call handle_interrupt
	pop eax
	popad
	add esp, 8
	sti
	iret

global asm_isr_00:function
asm_isr_00:
	push 0x00
	push 0x00
	jmp asm_handle_interrupt

global asm_isr_01:function
asm_isr_01:
	push 0x00
	push 0x01
	jmp asm_handle_interrupt
	iret

global asm_isr_02:function
asm_isr_02:
	push 0x00
	push 0x02
	jmp asm_handle_interrupt

global asm_isr_03:function
asm_isr_03:
	push 0x00
	push 0x03
	jmp asm_handle_interrupt

global asm_isr_04:function
asm_isr_04:
	push 0x00
	push 0x04
	jmp asm_handle_interrupt

global asm_isr_05:function
asm_isr_05:
	push 0x00
	push 0x05
	jmp asm_handle_interrupt

global asm_isr_06:function
asm_isr_06:
	push 0x00
	push 0x06
	jmp asm_handle_interrupt

global asm_isr_07:function
asm_isr_07:
	push 0x00
	push 0x07
	jmp asm_handle_interrupt

global asm_isr_08:function
asm_isr_08:
	push 0x08
	jmp asm_handle_interrupt

global asm_isr_09:function
asm_isr_09:
	push 0x09
	jmp asm_handle_interrupt

global asm_isr_0a:function
asm_isr_0a:
	push 0x0a
	jmp asm_handle_interrupt

global asm_isr_0b:function
asm_isr_0b:
	push 0x0b
	jmp asm_handle_interrupt

global asm_isr_0c:function
asm_isr_0c:
	push 0x0c
	jmp asm_handle_interrupt

global asm_isr_0d:function
asm_isr_0d:
	push 0x0d
	jmp asm_handle_interrupt

global asm_isr_0e:function
asm_isr_0e:
	push 0x0e
	jmp asm_handle_interrupt

global asm_isr_10:function
asm_isr_10:
	push 0x00
	push 0x10
	jmp asm_handle_interrupt

global asm_isr_11:function
asm_isr_11:
	push 0x11
	jmp asm_handle_interrupt

global asm_isr_12:function
asm_isr_12:
	push 0x00
	push 0x12
	jmp asm_handle_interrupt

global asm_isr_13:function
asm_isr_13:
	push 0x00
	push 0x13
	jmp asm_handle_interrupt

global asm_isr_14:function
asm_isr_14:
	push 0x00
	push 0x14
	jmp asm_handle_interrupt

global asm_isr_1e:function
asm_isr_1e:
	push 0x1e
	jmp asm_handle_interrupt

global asm_isr_20:function
asm_isr_20:
	push 0x00
	push 0x20
	jmp asm_handle_interrupt

global asm_isr_21:function
asm_isr_21:
	push 0x00
	push 0x21
	jmp asm_handle_interrupt

global asm_isr_22:function
asm_isr_22:
	push 0x00
	push 0x22
	jmp asm_handle_interrupt

global asm_isr_23:function
asm_isr_23:
	push 0x00
	push 0x23
	jmp asm_handle_interrupt

global asm_isr_24:function
asm_isr_24:
	push 0x00
	push 0x24
	jmp asm_handle_interrupt

global asm_isr_25:function
asm_isr_25:
	push 0x00
	push 0x25
	jmp asm_handle_interrupt

global asm_isr_26:function
asm_isr_26:
	push 0x00
	push 0x26
	jmp asm_handle_interrupt

global asm_isr_27:function
asm_isr_27:
	push 0x00
	push 0x27
	jmp asm_handle_interrupt

global asm_isr_28:function
asm_isr_28:
	push 0x00
	push 0x28
	jmp asm_handle_interrupt

global asm_isr_29:function
asm_isr_29:
	push 0x00
	push 0x29
	jmp asm_handle_interrupt

global asm_isr_2a:function
asm_isr_2a:
	push 0x00
	push 0x2a
	jmp asm_handle_interrupt

global asm_isr_2b:function
asm_isr_2b:
	push 0x00
	push 0x2b
	jmp asm_handle_interrupt

global asm_isr_2c:function
asm_isr_2c:
	push 0x00
	push 0x2c
	jmp asm_handle_interrupt

global asm_isr_2d:function
asm_isr_2d:
	push 0x00
	push 0x2d
	jmp asm_handle_interrupt

global asm_isr_2e:function
asm_isr_2e:
	push 0x00
	push 0x2e
	jmp asm_handle_interrupt

global asm_isr_2f:function
asm_isr_2f:
	push 0x00
	push 0x2f
	jmp asm_handle_interrupt
