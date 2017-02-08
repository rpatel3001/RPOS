#ifndef _ISR_H
#define _ISR_H

typedef struct isr_stack_frame {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} isr_stack_frame;

#define KEYBOARD_INT_VEC 0x21
void asm_keyboard_ISR(void);

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

#endif