section .text
bits 32

call_isr:
	pushad
	cld
    call    eax
    popad
	ret

; asm level keyboard ISR
global asm_keyboard_ISR:function
asm_keyboard_ISR:
    extern keyboard_ISR
	mov eax, keyboard_ISR
	call call_isr
    iret