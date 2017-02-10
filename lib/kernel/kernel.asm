section .text
bits 32

; halt forever
global asm_halt:function
asm_halt:
    cli
    hlt
    jmp asm_halt

; load the idt
global asm_load_idt:function
asm_load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

global asm_init_paging:function
asm_init_paging:
    ; enable PAE
    mov eax, cr4
    bts eax, 5
    mov cr4, eax
    ; load the page directory table
    mov eax, [esp + 4]
    mov cr3, eax
    ; enable paging
    mov eax, cr0
    bts eax, 31
    mov cr0, eax
    ret

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
