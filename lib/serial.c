#include <stddef.h>
#include <stdint.h>
#include <serial.h>

extern size_t strlen(const char* str);
extern uint8_t read_port(uint16_t port);
extern void write_port(uint16_t port, uint8_t data);

#define PORT 0x3f8

void serial_init(void) {
   write_port(PORT + 1, 0x00);    // Disable all interrupts
   write_port(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   write_port(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   write_port(PORT + 1, 0x00);    //                  (hi byte)
   write_port(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   write_port(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   write_port(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

void serial_write(const char* data, size_t size) {
  for (size_t i = 0; i < size; ++i)
    serial_putchar(data[i]);
}

void serial_writestring(const char* data) {
  serial_write(data, strlen(data));
}

void serial_putchar(char c) {
  write_port(PORT, c);
}