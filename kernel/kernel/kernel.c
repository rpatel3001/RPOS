#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();

	for(int i = 'A'; i <= 'Z'; ++i) {
		terminal_writestring("Hello, kernel World! ");
		terminal_putchar((char)i);
		terminal_putchar('\n');
	}
}
