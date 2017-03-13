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
void isr_00(void);
void asm_isr_00(void);
void isr_01(void);
void asm_isr_01(void);
void isr_02(void);
void asm_isr_02(void);
void isr_03(void);
void asm_isr_03(void);
void isr_04(void);
void asm_isr_04(void);
void isr_05(void);
void asm_isr_05(void);
void isr_06(void);
void asm_isr_06(void);
void isr_07(void);
void asm_isr_07(void);
void isr_08(uint32_t err);
void asm_isr_08(void);
void isr_0a(uint32_t err );
void asm_isr_0a(void);
void isr_0b(uint32_t err);
void asm_isr_0b(void);
void isr_0c(uint32_t err);
void asm_isr_0c(void);
void isr_0d(uint32_t err);
void asm_isr_0d(void);
void isr_0e(uint32_t err);
void asm_isr_0e(void);
void isr_10(void);
void asm_isr_10(void);
void isr_11(uint32_t err);
void asm_isr_11(void);
void isr_12(void);
void asm_isr_12(void);
void isr_13(void);
void asm_isr_13(void);
void isr_14(void);
void asm_isr_14(void);
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