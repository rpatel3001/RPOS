section .text
bits 32

asm_handle_interrupt:
	pushad
	push esp
	extern handle_interrupt
	call handle_interrupt
	pop eax
	popad
	add esp, 8
	iret

; asm level keyboard ISR
global asm_keyboard_ISR:function
asm_keyboard_ISR:
	pushad
	mov ecx,0
	div ecx
    extern keyboard_ISR
	call keyboard_ISR
	popad
    iret

global asm_isr_00:function
asm_isr_00:
	cli
	push 0x00
	push 0x00
	jmp asm_handle_interrupt

global asm_isr_01:function
asm_isr_01:
	cli
	push 0x00
	push 0x01
	jmp asm_handle_interrupt
	iret

global asm_isr_02:function
asm_isr_02:
	cli
	push 0x00
	push 0x02
	jmp asm_handle_interrupt

global asm_isr_03:function
asm_isr_03:
	cli
	push 0x00
	push 0x03
	jmp asm_handle_interrupt

global asm_isr_04:function
asm_isr_04:
	cli
	push 0x00
	push 0x04
	jmp asm_handle_interrupt

global asm_isr_05:function
asm_isr_05:
	cli
	push 0x00
	push 0x05
	jmp asm_handle_interrupt

global asm_isr_06:function
asm_isr_06:
	cli
	push 0x00
	push 0x06
	jmp asm_handle_interrupt

global asm_isr_07:function
asm_isr_07:
	cli
	push 0x00
	push 0x07
	jmp asm_handle_interrupt

global asm_isr_08:function
asm_isr_08:
	cli
	push 0x08
	jmp asm_handle_interrupt

global asm_isr_0a:function
asm_isr_0a:
	cli
	push 0x0a
	jmp asm_handle_interrupt

global asm_isr_0b:function
asm_isr_0b:
	cli
	push 0x0b
	jmp asm_handle_interrupt

global asm_isr_0c:function
asm_isr_0c:
	cli
	push 0x0c
	jmp asm_handle_interrupt

global asm_isr_0d:function
asm_isr_0d:
	cli
	push 0x0d
	jmp asm_handle_interrupt

global asm_isr_0e:function
asm_isr_0e:
	cli
	push 0x0e
	jmp asm_handle_interrupt

global asm_isr_10:function
asm_isr_10:
	cli
	push 0x00
	push 0x10
	jmp asm_handle_interrupt

global asm_isr_11:function
asm_isr_11:
	cli
	push 0x11
	jmp asm_handle_interrupt

global asm_isr_12:function
asm_isr_12:
	cli
	push 0x00
	push 0x12
	jmp asm_handle_interrupt

global asm_isr_13:function
asm_isr_13:
	cli
	push 0x00
	push 0x13
	jmp asm_handle_interrupt

global asm_isr_14:function
asm_isr_14:
	cli
	push 0x00
	push 0x14
	jmp asm_handle_interrupt

global asm_isr_1e:function
asm_isr_1e:
	cli
	push 0x1e
	jmp asm_handle_interrupt