section .text

; halt forever
global asm_halt:function
asm_halt:
    cli
    hlt
    jmp asm_halt

; load the idt
global load_idt:function
load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

; asm level keyboard ISR
global keyboard_handler:function
keyboard_handler:
    extern keyboard_handler_main
    call    keyboard_handler_main
    iret

; check if CPUID is supported
global cpuid_supported:function
cpuid_supported:
    ; If we can flip bit 21 of EFLAGS, CPUID is available.

    ; copy EFLAGS to eax and ecx
    pushfd
    pop eax
    mov ecx, eax

    ; Flip the ID bit and put it back into EFLAGS
    xor eax, 1 << 21
    push eax
    popfd

    ; Copy the new EFLAGS to eax and compare to ecx
    pushfd
    pop eax
    cmp eax, ecx

    ; if they're equal, the bit wasn't flipped
    je .no_cpuid
    mov eax, 1
    ret
.no_cpuid:
    mov eax, 0
    ret

; check if long mode is supported
global longmode_supported:function
longmode_supported:
	; first determine if extended cpuid functions are available
	mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb .no_longmode

	; check if long mode is available
	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .no_longmode
	mov eax, 1
	ret
.no_longmode:
	mov eax, 0
	ret