#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <keyboard.h>
#include <terminal.h>
#include <serial.h>

size_t strlen(const char* str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

void linecpy(uint16_t* to, const uint16_t* from, size_t len) {
  for (size_t col = 0; col < len; ++col) {
    to[col] = from[col];
  }
}

void kernel_main(void) {
	terminal_init();
	serial_init();
	serial_writestring("\n\n");

	for (int i = 'A'; i <= 'Z'; ++i) {
		terminal_writestring("Hello, kernel World! ");
		terminal_putchar((char)i);
		terminal_putchar('\n');
		serial_putchar((char)i);
		serial_putchar('\n');
	}
	terminal_writestring("Hello, kernel World!\r");
	terminal_writestring("Blech\bk\nbye\tkern\na\tb\tc\nasdfagfdsfgvd\tfdfeac");

	idt_init();
	kb_init();

	while (1);
}