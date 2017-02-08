#ifndef _ISR_H
#define _ISR_H

#define KEYBOARD_INT_VEC 0x21
void asm_keyboard_ISR(void);

#define DIV_BY_ZERO_INT_VEC 0x00
void asm_div_by_zero_ISR(void);

#endif