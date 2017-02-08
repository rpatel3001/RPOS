#include <stddef.h>
#include <stdint.h>
#include <keyboard.h>
#include <keyboard_keymap.h>
#include <terminal.h>
#include <serial.h>
#include <string.h>
#include <asm.h>
#include <kernel/interrupt.h>
#include "isr.h"
#include "kernel.h"

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
	serial_writestring("ERROR: ");
	serial_writestring(msg);
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
	terminal_putchar('\f');
	terminal_writestring("ERROR: ");
	terminal_writestring(msg);
	asm_halt();
}

void abort_code(char* msg, uint32_t err) {
	kernel_printregisters();
	serial_writestring("ERROR: ");
	serial_writestring(msg);
	serial_writestring("CODE: ");
	serial_writeint16(err);
	serial_putchar('\n');
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
	terminal_putchar('\f');
	terminal_writestring("ERROR: ");
	terminal_writestring(msg);
	terminal_writestring("CODE: ");
	terminal_writeint16(err);
	terminal_putchar('\n');
	asm_halt();
}

void handle_interrupt(isr_stack_frame *s) {
	if (s->int_no == 0) {
		isr_00();
	} else if (s->int_no == 0x01) {
		isr_01();
	} else if (s->int_no == 0x02) {
		isr_02();
	} else if (s->int_no == 0x03) {
		isr_03();
	} else if (s->int_no == 0x04) {
		isr_04();
	} else if (s->int_no == 0x05) {
		isr_05();
	} else if (s->int_no == 0x06) {
		isr_06();
	} else if (s->int_no == 0x07) {
		isr_07();
	} else if (s->int_no == 0x08) {
		isr_08(s->err);
	} else if (s->int_no == 0x0a) {
		isr_0a(s->err);
	} else if (s->int_no == 0x0b) {
		isr_0b(s->err);
	} else if (s->int_no == 0x0c) {
		isr_0c(s->err);
	} else if (s->int_no == 0x0d) {
		isr_0d(s->err);
	} else if (s->int_no == 0x0e) {
		isr_0e(s->err);
	} else if (s->int_no == 0x10) {
		isr_10();
	} else if (s->int_no == 0x11) {
		isr_11(s->err);
	} else if (s->int_no == 0x12) {
		isr_12();
	} else if (s->int_no == 0x13) {
		isr_13();
	} else if (s->int_no == 0x14) {
		isr_14();
	} else if (s->int_no == 0x1e) {
		isr_1e(s->err);
	} else if (s->int_no == 0x21) {
		keyboard_ISR();
	}
}

void add_isr(IDT_entry idt[IDT_SIZE], size_t vec, uintptr_t isr) {
	idt[vec].offset_lowerbits = isr & 0xffff;
	idt[vec].selector = get_cs(); /* KERNEL_CODE_SEGMENT_OFFSET */
	idt[vec].zero = 0;
	idt[vec].type_attr = 0x8e; /* INTERRUPT_GATE */
	idt[vec].offset_higherbits = (isr & 0xffff0000) >> 16;
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

	// enable ISRs
	IDT_entry idt[IDT_SIZE];
	add_isr(idt, 0x00, (uintptr_t)asm_isr_00);
	add_isr(idt, 0x01, (uintptr_t)asm_isr_01);
	add_isr(idt, 0x02, (uintptr_t)asm_isr_02);
	add_isr(idt, 0x03, (uintptr_t)asm_isr_03);
	add_isr(idt, 0x04, (uintptr_t)asm_isr_04);
	add_isr(idt, 0x05, (uintptr_t)asm_isr_05);
	add_isr(idt, 0x06, (uintptr_t)asm_isr_06);
	add_isr(idt, 0x07, (uintptr_t)asm_isr_07);
	add_isr(idt, 0x08, (uintptr_t)asm_isr_08);
	add_isr(idt, 0x0a, (uintptr_t)asm_isr_0a);
	add_isr(idt, 0x0b, (uintptr_t)asm_isr_0b);
	add_isr(idt, 0x0c, (uintptr_t)asm_isr_0c);
	add_isr(idt, 0x0d, (uintptr_t)asm_isr_0d);
	add_isr(idt, 0x0e, (uintptr_t)asm_isr_1e);
	add_isr(idt, 0x10, (uintptr_t)asm_isr_10);
	add_isr(idt, 0x11, (uintptr_t)asm_isr_11);
	add_isr(idt, 0x12, (uintptr_t)asm_isr_12);
	add_isr(idt, 0x13, (uintptr_t)asm_isr_13);
	add_isr(idt, 0x14, (uintptr_t)asm_isr_14);
	add_isr(idt, 0x1e, (uintptr_t)asm_isr_1e);

	add_isr(idt, KEYBOARD_INT_VEC, (uintptr_t)asm_isr_21);
	
	idt_init(idt);
	enable_interrupt(KEYBOARD_INT_VEC);

	kb_init(&kernel_handlechar);

	while (1);
}
