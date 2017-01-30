#ifndef _SERIAL_H
#define _SERIAL_H

void serial_init(void);
void serial_write(const char* data, size_t len);
void serial_writestring(const char* data);
void serial_putchar(char c); 

#endif