section .text
bits 32

; halt forever
global asm_halt:function
asm_halt:
    cli
    hlt
    jmp asm_halt

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
