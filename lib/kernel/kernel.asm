section .text

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
    iret
