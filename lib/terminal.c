#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <serial.h>
#include <util.h>
#include "terminal.h"

#define VGA_PORT 0x3D4

//give a vga buffer value for a character and color combo
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

//update the VGA cursor position
static void update_cursor(int row, int col) {
	uint16_t index=(row * VGA_WIDTH) + col;
	// cursor LOW port to vga INDEX register
	write_port(VGA_PORT, 0x0F);
	write_port(VGA_PORT + 1, index & 0xFF);
	// cursor HIGH port to vga INDEX register
	write_port(VGA_PORT, 0x0E);
	write_port(VGA_PORT + 1, (index >> 8) & 0xFF);
}

//initialize the terminal
void terminal_init(void) {
	update_cursor(0, 0);
	terminal_row = 0;
	terminal_column = 0;
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; ++y) {
		for (size_t x = 0; x < VGA_WIDTH; ++x) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

//set the background and foreground color
void terminal_setcolor(enum vga_color fg, enum vga_color bg) {
	terminal_color = fg | bg << 4;
}

//set the foreground color
void terminal_setfgcolor(enum vga_color fg) {
	terminal_color &= 0xF0;
	terminal_color |= fg;
}

//set the background color
void terminal_setbgcolor(enum vga_color bg) {
	terminal_color &= 0x0F;
	terminal_color |= bg << 4;
}

//get the foreground color
enum vga_color terminal_getfgcolor(void) {
	return terminal_color & 0x0F;
}

//get the background color
enum vga_color terminal_getbgcolor(void) {
	return (terminal_color >> 4) & 0x0F;
}

//put a character at a specific location onscreen
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

//scroll up the terminal by one line
void terminal_scroll() {
	memcpy(terminal_buffer, terminal_buffer + VGA_WIDTH, 2 * VGA_WIDTH * (VGA_HEIGHT - 1));
	for (size_t col = 0; col < VGA_WIDTH; ++col) {
		terminal_putentryat(' ', terminal_color, col, VGA_HEIGHT - 1);
	}
}

//put a character at the current cursor position
void terminal_putchar(char c) {
	const size_t index = terminal_row * VGA_WIDTH + terminal_column;
	if ((c >= 0 && c <= 6) || (c == 11) || (c >= 14 && c <= 31)) {
		//nonprintable
	} else if (c == 7) {
		//bell
	} else if (c == 8) {
		//backspace
		if (terminal_column != 0) {
			--terminal_column;
			memcpy(&terminal_buffer[index-1], &terminal_buffer[index], index - index / VGA_WIDTH * terminal_row);
		}
	} else if (c == 9) {
		//tab
		do {
			terminal_putchar(' ');
		} while (terminal_column % 8 != 0);
	} else if (c == 10) {
		//newline
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT) {
			--terminal_row;
			terminal_scroll();
		}
	} else if (c == 12) {
		//form feed
		terminal_init();
	} else if (c == 13) {
		//carriage return
		terminal_column = 0;
	} else if (c == 127) {
		//delete
		if (terminal_column != 0) {
			memcpy(&terminal_buffer[index], &terminal_buffer[index+1], index - index / VGA_WIDTH * terminal_row);
		}
	} else {
		//printable character
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT) {
				--terminal_row;
				terminal_scroll();
			}
		}
	}
	update_cursor(terminal_row, terminal_column);
}

//write a given number of characters
void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; ++i)
		terminal_putchar(data[i]);
}

//write a null terminated string
void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

//write an integer with radix 10
void terminal_writeint10(int32_t data) {
	if (!data) {
		terminal_putchar('0');
	} else {
		bool neg = data < 0;
		if (neg) {
			data = -data;
		}
		char buf[12];
		size_t index = 11;
		buf[index] = 0;
		while (data) {
			buf[--index] = data % 10 + '0';
			data /= 10;
		}
		if (neg) {
			buf[--index] = '-';
		}
		terminal_writestring(buf + index);
	}
}

static char nibbleToHex(uint8_t c) {
	switch (c) {
	case 0: return '0';
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	case 10: return 'A';
	case 11: return 'B';
	case 12: return 'C';
	case 13: return 'D';
	case 14: return 'E';
	case 15: return 'F';
	default: return '?';
	}
}

//write an integer with radix 16
void terminal_writeint16(uint64_t data) {
	if (!data) {
		terminal_writestring("0x00");
	} else {
		char buf[11];
		size_t index = 10;
		buf[index] = 0;
		for (int i = 0; i < 8 && (data || index % 2 == 1); ++i) {
			buf[--index] = nibbleToHex(data & 0xF);
			data >>= 4;
		}
		buf[--index] = 'x';
		buf[--index] = '0';
		terminal_writestring(buf + index);
	}
}
