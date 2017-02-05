#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <keyboard.h>
#include <keyboard_keymap.h>
#include <terminal.h>
#include <serial.h>
#include <string.h>
#include <kernel/kernel.h>
#include <kernel/asm.h>

void kernel_printregisters(void) {
	serial_writestring("  eax:\t");
	serial_writeint16(get_eax());
	serial_writestring("\n  ebx:\t");
	serial_writeint16(get_ebx());
	serial_writestring("\n  ecx:\t");
	serial_writeint16(get_ecx());
	serial_writestring("\n  edx:\t");
	serial_writeint16(get_edx());
	serial_writestring("\n  esi:\t");
	serial_writeint16(get_esi());
	serial_writestring("\n  edi:\t");
	serial_writeint16(get_edi());
	serial_writestring("\n  ebp:\t");
	serial_writeint16(get_ebp());
	serial_writestring("\n  esp:\t");
	serial_writeint16(get_esp());
	serial_writestring("\nflags:\t");
	serial_writeint16(get_flags());
	serial_writestring("\n   ss:\t");
	serial_writeint16(get_ss());
	serial_writestring("\n   cs:\t");
	serial_writeint16(get_cs());
	serial_writestring("\n   ds:\t");
	serial_writeint16(get_ds());
	serial_writestring("\n   es:\t");
	serial_writeint16(get_es());
	serial_writestring("\n   fs:\t");
	serial_writeint16(get_fs());
	serial_writestring("\n   gs:\t");
	serial_writeint16(get_gs());
	serial_writestring("\n");
}

void abort(char* msg) {
	kernel_printregisters();
	serial_writestring(msg);
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
	terminal_putchar('\f');
	terminal_writestring("ERROR: ");
	terminal_writestring(msg);
	while(1);
}

char key_to_char(key_press kp) {
	return keymap[kp.keycode][kp.shift ? 1 : 0];
}

char linebuffer[VGA_WIDTH+1];
size_t line_index = 0;
void kernel_handlechar(key_press kp) {
	char outchar = key_to_char(kp);

	if(outchar == 'h' && kp.control) {
		abort("HALT SIGNAL RECEIVED!\n");
	}

	if (!outchar) {
		return;
	}

	terminal_putchar(outchar);
	linebuffer[line_index++] = outchar;
	if (outchar == '\n' || line_index == VGA_WIDTH) {
		serial_writestring(linebuffer);
		memset(linebuffer, 0, VGA_WIDTH);
		if (line_index == VGA_WIDTH) {
			serial_putchar('\n');
		}
		line_index = 0;
	}
}

void kernel_main(void) {
	uint32_t eax = get_eax();
	serial_init();
	terminal_init();
	terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	serial_writestring("terminal initialized\n");

	if(eax != 0x2BADB002) {
		abort("multiboot magic number not found\n");
	}

	idt_init();
	kb_init(&kernel_handlechar);

	for (int i = -10; i <= 10; ++i) {
		terminal_writeint16(i);
		terminal_putchar('\n');
	}
	while (1);
}
