#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#define IDT_SIZE 256
#define INTERRUPT_OFFSET 0x20

typedef struct IDT_entry {
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
} IDT_entry;

void idt_init();
void load_idt(IDT_entry IDT[IDT_SIZE]);

bool is_interrupt_enabled(uint8_t interrupt);
void enable_interrupt(uint8_t interrupt);
void disable_interrupt(uint8_t interrupt);
void set_interrupt_mask(uint8_t pic, uint8_t mask);
uint8_t get_interrupt_mask(uint8_t pic);
void send_eoi(uint8_t interrupt);

#endif