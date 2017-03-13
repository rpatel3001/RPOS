#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define IDT_SIZE 256
#define INTERRUPT_OFFSET 0x20

typedef struct {
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
} __attribute__ ((packed)) IDT_entry;

void idt_init();
void add_isr(size_t vec, uintptr_t isr);

bool is_interrupt_enabled(uint8_t interrupt);
void enable_interrupt(uint8_t interrupt);
void disable_interrupt(uint8_t interrupt);
void set_interrupt_mask(uint8_t pic, uint8_t mask);
uint8_t get_interrupt_mask(uint8_t pic);
void send_eoi(uint8_t interrupt);

#endif