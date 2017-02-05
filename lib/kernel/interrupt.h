#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#define IDT_SIZE 256

struct IDT_entry {
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
};

//initialize the IDT
void idt_init(void);

#endif