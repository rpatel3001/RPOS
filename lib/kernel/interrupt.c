#include <serial.h>
#include <util.h>
#include <string.h>
#include <kernel/kernel.h>
#include "interrupt.h"

#define PIC1_PORT 0x20
#define PIC2_PORT 0xA0

IDT_entry IDT[IDT_SIZE];
//initialize the IDT
void idt_init() {
	/* ICW1 - begin initialization */
	write_port(PIC1_PORT , 0x11);
	write_port(PIC2_PORT , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(PIC1_PORT + 1, INTERRUPT_OFFSET);
	write_port(PIC2_PORT + 1, INTERRUPT_OFFSET + 8);

	/* ICW3 - setup cascading */
	write_port(PIC1_PORT + 1, 0x00);
	write_port(PIC2_PORT + 1, 0x00);

	/* ICW4 - environment info */
	write_port(PIC1_PORT + 1, 0x01);
	write_port(PIC2_PORT + 1, 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(PIC1_PORT + 1, 0xff);
	write_port(PIC2_PORT + 1, 0xff);

	load_idt(IDT);

	serial_writestring("PICs initialized\n");
}

void load_idt(IDT_entry idt[IDT_SIZE]) {
	memcpy(IDT, idt, sizeof(IDT_entry) * IDT_SIZE);
	uintptr_t idt_address;
	uintptr_t idt_ptr[2];
	/* fill the IDT descriptor */
	idt_address = (uintptr_t)IDT ;
	idt_ptr[0] = (sizeof(IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	asm_load_idt(idt_ptr);
}

uint8_t get_interrupt_mask(uint8_t pic) {
	if (pic == 1) {
		return read_port(PIC1_PORT + 1);
	} else {
		return read_port(PIC2_PORT + 1);
	}
}

bool is_interrupt_enabled(uint8_t interrupt) {
	interrupt -= INTERRUPT_OFFSET;
	if (interrupt > 7) {
		return get_interrupt_mask(2) & (1 << (interrupt - 8));
	} else {
		return get_interrupt_mask(1) & (1 << interrupt);
	}
}

void enable_interrupt(uint8_t interrupt) {
	interrupt -= INTERRUPT_OFFSET;
	if (interrupt > 7) {
		write_port(PIC2_PORT + 1, get_interrupt_mask(2) & ~(1 << (interrupt - 8)));
	} else {
		write_port(PIC1_PORT + 1, get_interrupt_mask(1) & ~(1 << interrupt));
	}
}

void disable_interrupt(uint8_t interrupt) {
	interrupt -= INTERRUPT_OFFSET;
	if (interrupt > 7) {
		write_port(PIC2_PORT + 1, get_interrupt_mask(2) | (1 << (interrupt - 8)));
	} else {
		write_port(PIC1_PORT + 1, get_interrupt_mask(1) | (1 << interrupt));
	}
}

void set_interrupt_mask(uint8_t pic, uint8_t mask) {
	if (pic == 1) {
		write_port(PIC1_PORT + 1, mask);
	} else if (pic == 2) {
		write_port(PIC2_PORT + 1, mask);
	}
}

void send_eoi(uint8_t interrupt) {
	interrupt -= INTERRUPT_OFFSET;
	if (interrupt > 7) {
		write_port(PIC2_PORT, 0x20);
	}
	write_port(PIC1_PORT, 0x20);
}