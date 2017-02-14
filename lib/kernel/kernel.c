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

void shutdown(void) {
	write_port(0xF4, 0);
}

void handle_interrupt(isr_stack_frame *s) {
	if (s->int_no < 0x20) {
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
	} else if (outchar == 's' && kp.control) {
		// shutdown on ctrl + s
		shutdown();
	} else if (outchar == 'c' && kp.control) {
		// clear the screen on ctrl + c
		terminal_putchar('\f');
	} else if (!outchar) {
		// don't do anything if the key isn't printable
		serial_writestring("couldn't print character\n");
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
	for (size_t i = 0; i < 512; ++i) {
		page_tab[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_WRITABLE;
	}
	asm_init_paging((uintptr_t)&page_dir_tab);
	serial_writestring("Paging Initialized\n");
}

multiboot_info mbi;
void read_mbi(uint32_t* ptr) {
	uint32_t flags = ptr[0];
	if (flags & 1) {
		mbi.mem_present = true;
		mbi.mem_lower = ptr[1];
		mbi.mem_upper = ptr[2];
		serial_writestring("Memory: ");
		serial_writeint10(mbi.mem_upper / 1024 + 1);
		serial_writestring(" MiB\n");
	}
	if ((flags >> 1) & 1) {
		mbi.bootdev_present = true;
		mbi.bootdev = (uint8_t*)&ptr[3];
		serial_writestring("Boot Drive: ");
		serial_writeint16(mbi.bootdev[3]);
		serial_putchar('\n');
	}
	if ((flags >> 2) & 1) {
		mbi.cmdline_present = true;
		mbi.cmdline = (char*)ptr[4];
		serial_writestring("Kernel Params: ");
		serial_writestring(mbi.cmdline);
		serial_putchar('\n');
	}
	if ((flags >> 3) & 1) {
		mbi.mods_present = true;
		mbi.mods_count = ptr[5];
		mbi.mods_addr = (uint32_t*)ptr[6];
		serial_writestring("Module info present\n");
	}
	if ((flags >> 5) & 1) {
		mbi.elf_syms_present = true;
		mbi.elf_num = ptr[7];
		mbi.elf_size = ptr[8];
		mbi.elf_addr = (uint32_t*)ptr[9];
		mbi.elf_shndx = ptr[10];
		serial_writestring("ELF syms present\n");
	}
	if ((flags >> 6) & 1) {
		mbi.mmap_present = true;
		mbi.mmap_len = ptr[11];
		mbi.mmap_addr = (uint32_t*)ptr[12];
		serial_writestring("Memory map info present\n");
	}
	if ((flags >> 7) & 1) {
		mbi.drives_present = true;
		mbi.drives_len = ptr[13];
		mbi.drives_addr = (uint32_t*)ptr[14];
		serial_writestring("Drives info present\n");
	}
	if ((flags >> 8) & 1) {
		mbi.config_present = true;
		mbi.config_addr = (uint32_t*)ptr[15];
		serial_writestring("Config table present\n");
	}
	if ((flags >> 9) & 1) {
		mbi.loader_name_present = true;
		mbi.loader_name = (char*)ptr[16];
		serial_writestring("Loader Name: ");
		serial_writestring(mbi.loader_name);
		serial_putchar('\n');
	}
	if ((flags >> 10) & 1) {
		mbi.apm_present = true;
		mbi.apm_addr = (uint32_t*)ptr[17];
		serial_writestring("APM table present\n");
	}
	if ((flags >> 11) & 1) {
		mbi.vbe_present = true;
		mbi.vbe_control_info = ptr[18];
		mbi.vbe_mode_info = ptr[19];
		uint16_t* ptr1 = (uint16_t*)&ptr[19];
		mbi.vbe_mode = ptr1[0];
		mbi.vbe_interface_seg = ptr1[1];
		mbi.vbe_interface_offs = ptr1[2];
		mbi.vbe_interface_len = ptr1[3];
		serial_writestring("VBE data present\n");
	}
}

extern char kernel_start[];
extern char kernel_end[];
extern char KERNEL_VMA_OFFS[];
extern char KERNEL_LMA[];
void kernel_main(void) {
	// save the inital eax value for later comparison
	uint32_t eax = get_eax();
	// save ebx so we can parse the multiboot structure
	uint32_t* ebx = (uint32_t*)get_ebx();

	// initialize serial first because a lot of debugging stuff uses it
	serial_init();

	// parse the multiboot structure
	read_mbi(ebx);

	serial_writestring("Kernel Size: ");
	serial_writeint10((kernel_end - kernel_start)/1024);
	serial_writestring(" KiB\n");

	serial_writestring("Kernel Loaded At: ");
	serial_writeint16((uintptr_t)KERNEL_LMA);
	serial_putchar('\n');

	serial_writestring("Kernel Mapped To: ");
	serial_writeint16((uintptr_t)KERNEL_LMA + (uintptr_t)KERNEL_VMA_OFFS);
	serial_putchar('\n');

	terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_init();
	serial_writestring("Terminal Initialized\n");

	// do some checks to make sure we can fully boot
	if (eax != 0x2BADB002) {
		abort("Multiboot magic number not found!\n");
	} else {
		serial_writestring("Multiboot magic number found\n");
	}
	if (!cpuid_supported()) {
		abort("CPUID not supported!\n");
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

	timer_init(1000);
	kb_init(&kernel_handlechar);

	enable_interrupt(KEYBOARD_INT_VEC);
	enable_interrupt(PIT_INT_VEC);

	while(true);
}
