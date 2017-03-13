#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

// struct to represent a keypress event
typedef struct {
	bool shift;
	bool control;
	bool alt;
	bool super;
	bool function;
	uint8_t keycode;
} key_press;

void keyboard_ISR(void);

//initialize the keyboard
void kb_init(void (*callback)(key_press));

#endif