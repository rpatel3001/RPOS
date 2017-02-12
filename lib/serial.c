#include <stddef.h>
#include <stdint.h>
#include <serial.h>
#include <stdbool.h>
#include <string.h>
#include <util.h>

#define PORT 0x3f8

//initialize serial output
void serial_init(void) {
	write_port(PORT + 1, 0x00);    // Disable all interrupts
	write_port(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	write_port(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	write_port(PORT + 1, 0x00);    //                  (hi byte)
	write_port(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	write_port(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	write_port(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	serial_writestring("serial initialized\n");
}

//write a number of characters to serial
void serial_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; ++i)
		serial_putchar(data[i]);
}

//write a null terminated string to serial
void serial_writestring(const char* data) {
	serial_write(data, strlen(data));
}

//write one characetr to serial
void serial_putchar(char c) {
	write_port(PORT, c);
}

//serial an integer with radix 10
void serial_writeint10(int32_t data) {
	if (!data) {
		serial_putchar('0');
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
		serial_writestring(buf + index);
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
void serial_writeint16(uint64_t data) {
	if (!data) {
		serial_writestring("0x0");
	} else {
		char buf[11];
		size_t index = 10;
		buf[index] = 0;
		for (int i = 0; i < 8 && data; ++i) {
			buf[--index] = nibbleToHex(data & 0xF);
			data >>= 4;
		}
		buf[--index] = 'x';
		buf[--index] = '0';
		serial_writestring(buf + index);
	}
}