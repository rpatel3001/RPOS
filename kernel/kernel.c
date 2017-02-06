#include <stddef.h>
#include <stdint.h>
#include <keyboard.h>
#include <keyboard_keymap.h>
#include <terminal.h>
#include <serial.h>
#include <string.h>
#include <asm.h>
#include "kernel.h"
#include "interrupt.h"

// print the raw cpu registers
void kernel_printregisters(void) {
	uint32_t eax = get_eax();
	uint32_t ebx = get_ebx();
	uint32_t ecx = get_ecx();
	uint32_t edx = get_edx();
	uint32_t esi = get_esi();
	uint32_t edi = get_edi();
	uint32_t ebp = get_ebp();
	uint32_t esp = get_esp();
	uint32_t flags = get_flags();
	uint16_t ss = get_ss();
	uint16_t cs = get_cs();
	uint16_t ds = get_ds();
	uint16_t es = get_es();
	uint16_t fs = get_fs();
	uint16_t gs = get_gs();
	serial_writestring("  eax:\t");
	serial_writeint16(eax);
	serial_writestring("\n  ebx:\t");
	serial_writeint16(ebx);
	serial_writestring("\n  ecx:\t");
	serial_writeint16(ecx);
	serial_writestring("\n  edx:\t");
	serial_writeint16(edx);
	serial_writestring("\n  esi:\t");
	serial_writeint16(esi);
	serial_writestring("\n  edi:\t");
	serial_writeint16(edi);
	serial_writestring("\n  ebp:\t");
	serial_writeint16(ebp);
	serial_writestring("\n  esp:\t");
	serial_writeint16(esp);
	serial_writestring("\nflags:\t");
	serial_writeint16(flags);
	serial_writestring("\n   ss:\t");
	serial_writeint16(ss);
	serial_writestring("\n   cs:\t");
	serial_writeint16(cs);
	serial_writestring("\n   ds:\t");
	serial_writeint16(ds);
	serial_writestring("\n   es:\t");
	serial_writeint16(es);
	serial_writestring("\n   fs:\t");
	serial_writeint16(fs);
	serial_writestring("\n   gs:\t");
	serial_writeint16(gs);
	serial_writestring("\n");
}

// print a message and hang the machine
void abort(char* msg) {
	kernel_printregisters();
	serial_writestring(msg);
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
	terminal_putchar('\f');
	terminal_writestring("ERROR: ");
	terminal_writestring(msg);
	asm_halt();
}

// translate a scancode into an ascii character
char key_to_char(key_press kp) {
	return keymap[kp.keycode][kp.shift ? 1 : 0];
}

// callback for when a key is pressed
char linebuffer[VGA_WIDTH+1];
size_t line_index = 0;
void kernel_handlechar(key_press kp) {
	char outchar = key_to_char(kp);

	// halt on ctrl + h
	if (outchar == 'h' && kp.control) {
		abort("HALT SIGNAL RECEIVED!\n");
	}

	// don't do anything if the key isn't printable
	if (!outchar) {
		return;
	}

	// print to terminal, add to linebuffer, and print to serial if it's a full line
	terminal_putchar(outchar);
	if (outchar == '\t') {
		do {
			linebuffer[line_index++] = ' ';
		} while (line_index % 8 != 0 && line_index < VGA_WIDTH);
	} else {
		linebuffer[line_index++] = outchar;
	}
	if (outchar == '\n' || line_index == VGA_WIDTH) {
		serial_writestring(linebuffer);
		// clear the line buffer
		memset(linebuffer, 0, VGA_WIDTH);
		if (line_index == VGA_WIDTH) {
			serial_putchar('\n');
		}
		line_index = 0;
	}
}

void kernel_main(void) {
	// save the inital eax value for later comparison
	uint32_t eax = get_eax();

	// initialize serial first because a lot of debugging stuff uses it
	serial_init();
	terminal_init();
	terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	serial_writestring("terminal initialized\n");

	// do some checks to make sure we can fully boot
	if (eax != 0x36D76289) {
		abort("multiboot2 magic number not found\n");
	} else {
		serial_writestring("multiboot2 magic number found\n");
	}
	if (!cpuid_supported()) {
		abort("CPUID not supported\n");
	} else {
		serial_writestring("CPUID supported\n");
	}
	if (!longmode_supported()) {
		abort("long mode not supported\n");
	} else {
		serial_writestring("long mode supported\n");
	}

	IDT_entry idt[IDT_SIZE];

	/* populate IDT entry of keyboard's interrupt */
	uintptr_t keyboard_address = (uintptr_t)keyboard_handler;
	idt[0x21].offset_lowerbits = keyboard_address & 0xffff;
	idt[0x21].selector = get_cs(); /* KERNEL_CODE_SEGMENT_OFFSET */
	idt[0x21].zero = 0;
	idt[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
	idt[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
	idt_init(idt);
	kb_init(&kernel_handlechar);
	enable_interrupt(1);

	while (1);
}