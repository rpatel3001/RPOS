#ifndef _KERNEL_H
#define _KERNEL_H

#include <stdbool.h>
#include <stdint.h>

// load the ISR
void asm_load_idt(uintptr_t *idt_ptr);

// check if cpuid is available
bool cpuid_supported(void);

// halt forever
void asm_halt(void);

//print a message and halt
void abort(char* msg);
void abort_code(char* msg, uint32_t err);

#endif