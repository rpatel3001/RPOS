#ifndef _ISR_H
#define _ISR_H

#define KEYBOARD_INT_VEC 0x21
void asm_keyboard_ISR(void);

void asm_isr_00(void);
void asm_isr_01(void);
void asm_isr_02(void);
void asm_isr_03(void);
void asm_isr_04(void);
void asm_isr_05(void);
void asm_isr_06(void);
void asm_isr_07(void);
void asm_isr_08(void);
void asm_isr_0a(void);
void asm_isr_0b(void);
void asm_isr_0c(void);
void asm_isr_0d(void);
void asm_isr_0e(void);
void asm_isr_10(void);
void asm_isr_11(void);
void asm_isr_12(void);
void asm_isr_13(void);
void asm_isr_14(void);
void asm_isr_1e(void);

#endif