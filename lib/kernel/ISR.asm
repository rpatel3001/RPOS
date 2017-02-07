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

global asm_div_by_zero_ISR:function
asm_div_by_zero_ISR:
	pushad
	extern div_by_zero_ISR
	call div_by_zero_ISR
	popad
	iret