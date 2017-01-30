#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <keyboard.h>
#include <terminal.h>
#include <serial.h>
#include <string.h>

void kernel_main(void) {
	terminal_init();
	serial_init();
	serial_writestring("\n");

	idt_init();
	kb_init();

	for(int i = -10; i <= 10; ++i) {
		terminal_writeint16(i);
		terminal_putchar('\n');
	}

	while (1);
}