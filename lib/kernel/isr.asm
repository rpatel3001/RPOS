section .text
bits 32

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
	pushad
	extern isr_00
	call isr_00
	popad
	iret

global asm_isr_01:function
asm_isr_01:
	pushad
	extern isr_01
	call isr_01
	popad
	iret

global asm_isr_02:function
asm_isr_02:
	pushad
	extern isr_02
	call isr_02
	popad
	iret

global asm_isr_03:function
asm_isr_03:
	pushad
	extern isr_03
	call isr_03
	popad
	iret

global asm_isr_04:function
asm_isr_04:
	pushad
	extern isr_04
	call isr_04
	popad
	iret

global asm_isr_05:function
asm_isr_05:
	pushad
	extern isr_05
	call isr_05
	popad
	iret

global asm_isr_06:function
asm_isr_06:
	pushad
	extern isr_06
	call isr_06
	popad
	iret

global asm_isr_07:function
asm_isr_07:
	pushad
	extern isr_07
	call isr_07
	popad
	iret

global asm_isr_08:function
asm_isr_08:
	pushad
	extern isr_08
	call isr_08
	popad
	iret

global asm_isr_0a:function
asm_isr_0a:
	pushad
	extern isr_0a
	call isr_0a
	popad
	iret

global asm_isr_0b:function
asm_isr_0b:
	pushad
	extern isr_0b
	call isr_0b
	popad
	iret

global asm_isr_0c:function
asm_isr_0c:
	pushad
	extern isr_0c
	call isr_0c
	popad
	iret

global asm_isr_0d:function
asm_isr_0d:
	pushad
	extern isr_0d
	call isr_0d
	popad
	iret

global asm_isr_0e:function
asm_isr_0e:
	pushad
	extern isr_0e
	call isr_0e
	popad
	iret

global asm_isr_10:function
asm_isr_10:
	pushad
	extern isr_10
	call isr_10
	popad
	iret

global asm_isr_11:function
asm_isr_11:
	pushad
	extern isr_11
	call isr_11
	popad
	iret

global asm_isr_12:function
asm_isr_12:
	pushad
	extern isr_12
	call isr_12
	popad
	iret

global asm_isr_13:function
asm_isr_13:
	pushad
	extern isr_13
	call isr_13
	popad
	iret

global asm_isr_14:function
asm_isr_14:
	pushad
	extern isr_14
	call isr_14
	popad
	iret

global asm_isr_1e:function
asm_isr_1e:
	pushad
	extern isr_1e
	call isr_1e
	popad
	iret