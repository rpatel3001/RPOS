#ifndef _ASM_H
#define _ASM_H

#include <stdint.h>

// retrieve CPU registers
uint32_t get_eax(void);
uint32_t get_ebx(void);
uint32_t get_ecx(void);
uint32_t get_edx(void);
uint32_t get_esi(void);
uint32_t get_edi(void);
uint32_t get_ebp(void);
uint32_t get_esp(void);
uint32_t get_flags(void);
uint16_t get_ss(void);
uint16_t get_cs(void);
uint16_t get_ds(void);
uint16_t get_es(void);
uint16_t get_fs(void);
uint16_t get_gs(void);

// read/write bytes from/to cpu ports
uint8_t read_port(uint16_t port);
void write_port(uint16_t port, uint8_t data);

//print a message and halt
void abort(char* msg);
void abort_code(char* msg, uint32_t err);

#endif