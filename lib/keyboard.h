#ifndef _KEYBOARD_H
#define _KEYBOARD_H

typedef struct key_press {
	bool shift;
	bool control;
	bool alt;
	bool super;
	bool function;
	uint8_t keycode;
} key_press;

//initialize the keyboard
void kb_init(void (*callback)(key_press));

//initialize the IDT
void idt_init(void);

#endif