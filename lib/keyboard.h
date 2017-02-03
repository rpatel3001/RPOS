#ifndef _KEYBOARD_H
#define _KEYBOARD_H

//initialize the keyboard
void kb_init(void (*callback)(uint8_t));

//initialize the IDT
void idt_init(void);

#endif