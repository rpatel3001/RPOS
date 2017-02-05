#ifndef _KERNEL_H
#define _KERNEL_H

// asm level keyboard ISR
void keyboard_handler(void);

// load the ISR
void load_idt(uintptr_t *idt_ptr);

// checks to make sure we can run on this hardware
bool cpuid_supported(void);
bool longmode_supported(void);

// halt forever
void asm_halt(void);

#endif