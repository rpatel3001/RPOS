#include <stddef.h>
#include <stdint.h>
#include <keyboard.h>
#include <keyboard_keymap.h>
#include <terminal.h>
#include <serial.h>
#include <string.h>
#include <util.h>
#include <kernel/interrupt.h>
#include <timer.h>
#include "isr.h"
#include "kernel.h"

void handle_interrupt(isr_stack_frame *s) {
	if(s->int_no < 0x20) {
		serial_writeint16(s->int_no);
		serial_writestring(" exception\n");
	}
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
	} else if (s->int_no == PIT_INT_VEC) {
		timer_isr();
	} else if (s->int_no == KEYBOARD_INT_VEC) {
		keyboard_ISR();
	}
	send_eoi(s->int_no);
}

void add_isr(IDT_entry idt[IDT_SIZE], size_t vec, uintptr_t isr) {
	idt[vec].offset_lowerbits = isr & 0xffff;
	idt[vec].selector = get_cs(); /* KERNEL_CODE_SEGMENT_OFFSET */
	idt[vec].zero = 0;
	idt[vec].type_attr = 0x8e; /* INTERRUPT_GATE */
	idt[vec].offset_higherbits = (isr >> 16) & 0xffff;
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

	if (outchar == 'h' && kp.control) {
		// halt on ctrl + h
		abort("HALT SIGNAL RECEIVED!\n");
	} else if (outchar == 'c' && kp.control) {
		// clear the screen on ctrl + c
		terminal_putchar('\f');
	} else if (!outchar) {
		// don't do anything if the key isn't printable
		return;
	} else {
		// print to terminal, add to linebuffer, and print to serial if it's a full line
		terminal_putchar(outchar);
		if (outchar == '\t') {
			do {
				linebuffer[line_index++] = ' ';
			} while (line_index % 8 != 0 && line_index < VGA_WIDTH);
		} else if (outchar == '\b' ) {
			--line_index;
		} else if (outchar != 127) {
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
}

#define PAGE_WRITABLE 2
#define PAGE_PRESENT  1
uint64_t page_dir_tab[4] __attribute__((aligned(0x20)));
uint64_t page_dir[512] __attribute__((aligned(0x1000)));
uint64_t page_tab[512] __attribute__((aligned(0x1000)));
void init_paging(void) {
	// add the page directory as the first entry in the page directory table
	page_dir_tab[0] = (uintptr_t)&page_dir | PAGE_PRESENT;
	// add the page table ass the first entry in the page directory
	page_dir[0] = (uintptr_t)&page_tab | PAGE_PRESENT | PAGE_WRITABLE;
	// identity map the first 2 MiB
	for(size_t i = 0; i < 512; ++i) {
		page_tab[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_WRITABLE;
	}
	asm_init_paging((uintptr_t)&page_dir_tab);
	serial_writestring("paging initialized\n");
}

void kernel_main(void) {
	// save the inital eax value for later comparison
	uint32_t eax = get_eax();

	// initialize serial first because a lot of debugging stuff uses it
	serial_init();
	terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_init();
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

	init_paging();

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
	add_isr(idt, 0x0e, (uintptr_t)asm_isr_0e);
	add_isr(idt, 0x10, (uintptr_t)asm_isr_10);
	add_isr(idt, 0x11, (uintptr_t)asm_isr_11);
	add_isr(idt, 0x12, (uintptr_t)asm_isr_12);
	add_isr(idt, 0x13, (uintptr_t)asm_isr_13);
	add_isr(idt, 0x14, (uintptr_t)asm_isr_14);
	add_isr(idt, 0x1e, (uintptr_t)asm_isr_1e);

	add_isr(idt, PIT_INT_VEC, (uintptr_t)asm_isr_20);
	add_isr(idt, KEYBOARD_INT_VEC, (uintptr_t)asm_isr_21);

	idt_init(idt);
	load_idt(idt);

	enable_interrupt(KEYBOARD_INT_VEC);
	enable_interrupt(PIT_INT_VEC);

	timer_init(1000);
	kb_init(&kernel_handlechar);

	serial_writestring("accessing phys addr 0\n");
	uint8_t* ptr = 0;
	serial_writeint16(*ptr);
	serial_putchar('\n');
	*ptr = 0xB0;
	serial_writeint16(*ptr);
	serial_writestring("\naccessed\n");

	for (int i = 0; true; ++i) {
		terminal_writeint10(i);
		terminal_putchar('\n');
		sleep(1000);
	}

	while (1);
}
