#include <stddef.h>
#include <stdint.h>
#include "keyboard_map.h"
#include "keyboard.h"

extern void keyboard_handler(void);
extern uint8_t read_port(uint16_t port);
extern void write_port(uint16_t port, uint8_t data);
extern void load_idt(uint32_t *idt_ptr);

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256

void (*callback)(char);

void keyboard_handler_main(void) {
	uint8_t status;
	uint8_t keycode;

	/* write EOI */
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		if (keycode > 127)
			return;
		callback(keyboard_map[keycode]);
	}
}

//initialize the keyboard
void kb_init(void (*handler)(char)) {
	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	write_port(0x21 , read_port(0x21) & 0xFD);
	callback = handler;
}

struct IDT_entry {
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

//initialize the IDT
void idt_init(void) {
	uint32_t keyboard_address;
	uint32_t idt_address;
	uint32_t idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (uint32_t)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;


	/*     Ports
	*  PIC1 PIC2
	*Command 0x20 0xA0
	*Data  0x21 0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);
	write_port(0xA1 , 0x00);

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (uint32_t)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}