#ifndef _SERIAL_H
#define _SERIAL_H

//initialize serial output
void serial_init(void);

//write a number of characters to serial
void serial_write(const char* data, size_t len);

//write a null terminated string to serial
void serial_writestring(const char* data);

//write one character to serial
void serial_putchar(char c);

//write an integer with radix 10
void serial_writeint10(int32_t data);

//write an integer with radix 16
void serial_writeint16(uint64_t data);

#endif