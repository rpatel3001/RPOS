#include <stddef.h>
#include <stdint.h>
#include <keyboard.h>
#include <keyboard_keymap.h>
#include <terminal.h>
#include <serial.h>
#include <string.h>
#include <kernel/util.h>
#include <kernel/interrupt.h>
#include <timer.h>
#include <kernel/isr.h>
#include <kernel/paging.h>
#include <kernel/kernel.h>

multiboot_info mbi;

void shutdown(void) {
	// this is a qemu hack
	// TODO real shutdown with APM or ACPI or whatever
	write_port(0xF4, 0);
}

void reboot(void) {
	write_port(KEYBOARD_STATUS_PORT, 0xFE);
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
	} else if (s->int_no == 0x09) {
		isr_09();
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

// translate a scancode into an ascii character
char key_to_char(key_press kp) {
	return keymap[kp.keycode][kp.shift ? 1 : 0];
}

// callback for when a key is pressed
char linebuffer[80];
size_t line_index = 0;
void kernel_handlechar(key_press kp) {
	char outchar = key_to_char(kp);
	if (outchar == 'h' && kp.control) {
		// halt on ctrl + h
		abort("Halt signal received!\n");
	} else if (outchar == 's' && kp.control) {
		// shutdown on ctrl + s
		serial_writestring("Shutdown signal received!\n");
		shutdown();
	} else if (outchar == 'r' && kp.control) {
		// reboot on ctrl + r
		serial_writestring("Reboot signal received!\n");
		reboot();
	} else if (outchar == 'c' && kp.control) {
		// clear the screen on ctrl + c
		terminal_putchar('\f');
	} else if (!outchar) {
		// don't do anything if the key isn't printable
		serial_writestring("Couldn't print character\n");
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
			serial_write(linebuffer, line_index);
			// clear the line buffer
			memset(linebuffer, 0, VGA_WIDTH);
			if (line_index == VGA_WIDTH) {
				serial_putchar('\n');
			}
			line_index = 0;
		}
	}
}

uint32_t memsize_mb(void) {
	return mbi.mem_upper / 1024 + 1;
}

void read_mbi(uint32_t* ptr) {
	uint32_t flags = ptr[0];
	serial_writestring("Multiboot Flags: ");
	serial_writeint16(flags);
	serial_putchar('\n');
	if (flags & 1) {
		mbi.mem_present = true;
		// in KiB
		mbi.mem_lower = ptr[1];
		// in MiB
		mbi.mem_upper = ptr[2];
		serial_writestring("Memory: ");
		serial_writeint10(memsize_mb());
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
		mbi.cmdline = (char*)(ptr[4] + KERNEL_VMA_OFFS);
		serial_writestring("Kernel Params: ");
		serial_writestring(mbi.cmdline);
		serial_putchar('\n');
	}
	if ((flags >> 3) & 1) {
		mbi.mods_present = true;
		mbi.mods_count = ptr[5];
		mbi.mods_addr = (uint32_t*)(ptr[6] + KERNEL_VMA_OFFS);
		serial_writestring("Module info present\n");
	}
	if ((flags >> 5) & 1) {
		mbi.elf_syms_present = true;
		mbi.elf_num = ptr[7];
		mbi.elf_size = ptr[8];
		mbi.elf_addr = (uint32_t*)(ptr[9] + KERNEL_VMA_OFFS);
		mbi.elf_shndx = ptr[10];
		serial_writestring("ELF syms present\n");
	}
	if ((flags >> 6) & 1) {
		mbi.mmap_present = true;
		mbi.mmap_len = ptr[11];
		mbi.mmap_addr = (uintptr_t)(ptr[12] + KERNEL_VMA_OFFS);
		for (uintptr_t i = mbi.mmap_addr; i < mbi.mmap_addr + mbi.mmap_len;) {
			uint32_t size = (*(uint32_t*)(i));
			uint64_t base = (*(uint64_t*)(i+4));
			uint64_t len = (*(uint64_t*)(i+12));
			uint32_t type = (*(uint32_t*)(i+20));
			if (type != 1) {
				mark_addr_range_used(base, len);
			}
			i += size+4;
		}
		serial_writestring("Loader memory map loaded\n");
	}
	if ((flags >> 7) & 1) {
		mbi.drives_present = true;
		mbi.drives_len = ptr[13];
		mbi.drives_addr = (uint32_t*)(ptr[14] + KERNEL_VMA_OFFS);
		serial_writestring("Drives info present\n");
	}
	if ((flags >> 8) & 1) {
		mbi.config_present = true;
		mbi.config_addr = (uint32_t*)(ptr[15] + KERNEL_VMA_OFFS);
		serial_writestring("Config table present\n");
	}
	if ((flags >> 9) & 1) {
		mbi.loader_name_present = true;
		mbi.loader_name = (char*)(ptr[16] + KERNEL_VMA_OFFS);
		serial_writestring("Loader Name: ");
		serial_writestring(mbi.loader_name);
		serial_putchar('\n');
	}
	if ((flags >> 10) & 1) {
		mbi.apm_present = true;
		mbi.apm_addr = (uint32_t*)(ptr[17] + KERNEL_VMA_OFFS);
		serial_writestring("APM table present\n");
	}
	if ((flags >> 11) & 1) {
		mbi.vbe_present = true;
		mbi.vbe_control_ptr = (vbe_control_info*)(ptr[18] + KERNEL_VMA_OFFS);
		mbi.vbe_mode_ptr = (vbe_mode_info*)(ptr[19] + KERNEL_VMA_OFFS);
		uint16_t* ptr1 = (uint16_t*)&ptr[19] + 2;
		mbi.vbe_mode = ptr1[0];
		mbi.vbe_interface_seg = ptr1[1];
		mbi.vbe_interface_offs = ptr1[2];
		mbi.vbe_interface_len = ptr1[3];

		VGA_WIDTH = mbi.vbe_mode_ptr->x_res;
		VGA_HEIGHT = mbi.vbe_mode_ptr->y_res;

		// the pointers we get are as segment:offset; fix them to be logical addresses
		uintptr_t ptr = (uintptr_t)mbi.vbe_control_ptr->oem_string;
		mbi.vbe_control_ptr->oem_string = ((ptr >> 12) & 0xFFFF0) + (ptr & 0xFFFF) + KERNEL_VMA_OFFS;

		ptr = (uintptr_t)mbi.vbe_control_ptr->oem_vendor_name;
		mbi.vbe_control_ptr->oem_vendor_name = ((ptr >> 12) & 0xFFFF0) + (ptr & 0xFFFF) + KERNEL_VMA_OFFS;

		ptr = (uintptr_t)mbi.vbe_control_ptr->oem_product_name;
		mbi.vbe_control_ptr->oem_product_name = ((ptr >> 12) & 0xFFFF0) + (ptr & 0xFFFF) + KERNEL_VMA_OFFS;

		ptr = (uintptr_t)mbi.vbe_control_ptr->oem_product_revision;
		mbi.vbe_control_ptr->oem_product_revision = ((ptr >> 12) & 0xFFFF0) + (ptr & 0xFFFF) + KERNEL_VMA_OFFS;

		serial_writestring("Video: ");
		serial_writestring(mbi.vbe_control_ptr->oem_product_name);
		serial_writestring(" ");
		serial_writestring(mbi.vbe_control_ptr->oem_product_revision);
		serial_putchar('\n');
		serial_writestring("Video memory: ");
		serial_writeint10(mbi.vbe_control_ptr->memory * 64 / 1024);
		serial_writestring(" MiB\n");
	}
}

void kernel_main(uint32_t eax, uint32_t ebx, uint16_t ecx) {
	// initialize serial first because a lot of debugging stuff uses it
	serial_init();
	serial_writestring("\nBooting RPOS\n");

	// mark the kernel's page as used
	mark_addr_range_used((uintptr_t)KERNEL_LMA, KERNEL_END - KERNEL_VMA);

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

	// parse the multiboot structure
	read_mbi((uint32_t*)(ebx + KERNEL_VMA_OFFS));

	serial_writestring("Kernel Size: ");
	serial_writeint10((KERNEL_END - KERNEL_VMA) / 1024);
	serial_writestring(" KiB\n");

	serial_writestring("Kernel LMA: ");
	serial_writeint16((uintptr_t)KERNEL_LMA);
	serial_putchar('\n');

	serial_writestring("Kernel VMA: ");
	serial_writeint16((uintptr_t)KERNEL_VMA);
	serial_putchar('\n');

	terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_init();
	serial_writestring("Terminal Initialized\n");


	serial_writestring("EBDA: ");
	serial_writeint10(*(int16_t*)(ecx<<4));
	serial_putchar('\n');

	// enable ISRs
	add_isr(0x00, (uintptr_t)asm_isr_00);
	add_isr(0x01, (uintptr_t)asm_isr_01);
	add_isr(0x02, (uintptr_t)asm_isr_02);
	add_isr(0x03, (uintptr_t)asm_isr_03);
	add_isr(0x04, (uintptr_t)asm_isr_04);
	add_isr(0x05, (uintptr_t)asm_isr_05);
	add_isr(0x06, (uintptr_t)asm_isr_06);
	add_isr(0x07, (uintptr_t)asm_isr_07);
	add_isr(0x08, (uintptr_t)asm_isr_08);
	add_isr(0x09, (uintptr_t)asm_isr_09);
	add_isr(0x0a, (uintptr_t)asm_isr_0a);
	add_isr(0x0b, (uintptr_t)asm_isr_0b);
	add_isr(0x0c, (uintptr_t)asm_isr_0c);
	add_isr(0x0d, (uintptr_t)asm_isr_0d);
	add_isr(0x0e, (uintptr_t)asm_isr_0e);
	add_isr(0x10, (uintptr_t)asm_isr_10);
	add_isr(0x11, (uintptr_t)asm_isr_11);
	add_isr(0x12, (uintptr_t)asm_isr_12);
	add_isr(0x13, (uintptr_t)asm_isr_13);
	add_isr(0x14, (uintptr_t)asm_isr_14);
	add_isr(0x1e, (uintptr_t)asm_isr_1e);

	add_isr(PIT_INT_VEC, (uintptr_t)asm_isr_20);
	add_isr(KEYBOARD_INT_VEC, (uintptr_t)asm_isr_21);

	idt_init();

	timer_init(1000);
	kb_init(&kernel_handlechar);

	enable_interrupt(KEYBOARD_INT_VEC);
	enable_interrupt(PIT_INT_VEC);

	while (true);
}
