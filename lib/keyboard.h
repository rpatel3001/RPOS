#ifndef _KEYBOARD_H
#define _KEYBOARD_H

//initialize the keyboard
void kb_init(void (*callback)(char));

//initialize the IDT
void idt_init(void);

#endif