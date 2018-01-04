#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>

typedef struct {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} isr_stack_frame;

#define PIT_INT_VEC 0x20
#define KEYBOARD_INT_VEC 0x21

// exceptions, int 0x00 through 0x1f
// divide by zero
void isr_00(void);
void asm_isr_00(void);

// debug interrupt
void isr_01(void);
void asm_isr_01(void);

// non maskable interrupt
void isr_02(void);
void asm_isr_02(void);

// breakpoint
void isr_03(void);
void asm_isr_03(void);

// overflow
void isr_04(void);
void asm_isr_04(void);

// bound range exceeded
void isr_05(void);
void asm_isr_05(void);

// invalid opcode
void isr_06(void);
void asm_isr_06(void);

// device not available
void isr_07(void);
void asm_isr_07(void);

// double fault
void isr_08(uint32_t err);
void asm_isr_08(void);

// coprocessor segment overrun
void isr_09(void);
void asm_isr_09(void);

// invalid TSS
void isr_0a(uint32_t err);
void asm_isr_0a(void);

// segment not present
void isr_0b(uint32_t err);
void asm_isr_0b(void);

// stack segment fault
void isr_0c(uint32_t err);
void asm_isr_0c(void);

// general protection fault
void isr_0d(uint32_t err);
void asm_isr_0d(void);

// page fault
void isr_0e(uint32_t err);
void asm_isr_0e(void);

// x87 floating point exception
void isr_10(void);
void asm_isr_10(void);

// alignment check
void isr_11(uint32_t err);
void asm_isr_11(void);

// machine check
void isr_12(void);
void asm_isr_12(void);

// SIMD floating point exception
void isr_13(void);
void asm_isr_13(void);

// virtualization exception
void isr_14(void);
void asm_isr_14(void);

// security exception
void isr_1e(uint32_t err);
void asm_isr_1e(void);

// IQRs, mapped to 0x20 to 0x2f
void asm_isr_20(void);
void asm_isr_21(void);
void asm_isr_22(void);
void asm_isr_23(void);
void asm_isr_24(void);
void asm_isr_25(void);
void asm_isr_26(void);
void asm_isr_27(void);
void asm_isr_28(void);
void asm_isr_29(void);
void asm_isr_2a(void);
void asm_isr_2b(void);
void asm_isr_2c(void);
void asm_isr_2d(void);
void asm_isr_2e(void);
void asm_isr_2f(void);

#endif