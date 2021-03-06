#include <kernel/util.h>
#include <serial.h>
#include "isr.h"

// divide by zero
void isr_00(void) {
	char* msg = "divide by zero\n";
	abort(msg);
}

// debug interrupt
void isr_01(void) {
	char* msg = "debug interrupt\n";
	abort(msg);
}

// non maskable interrupt
void isr_02(void) {
	char* msg = "non maskable interrupt\n";
	abort(msg);
}

// breakpoint
void isr_03(void) {
	char* msg = "breakpoint\n";
	abort(msg);
}

// overflow
void isr_04(void) {
	char* msg = "overflow\n";
	abort(msg);
}

// bound range exceeded
void isr_05(void) {
	char* msg = "bound range exceeded\n";
	abort(msg);
}

// invalid opcode
void isr_06(void) {
	char* msg = "invalid opcode\n";
	abort(msg);
}

// device not available
void isr_07(void) {
	char* msg = "device not available\n";
	abort(msg);
}

// double fault
void isr_08(uint32_t err) {
	char* msg = "double fault\n";
	abort_code(msg, err);
}

// coprocessor segment overrun
void isr_09(void) {
	char* msg = "coprocessor segment overrun\n";
	abort(msg);
}

// invalid TSS
void isr_0a(uint32_t err) {
	char* msg = "invalid TSS\n";
	abort_code(msg, err);
}

// segment not present
void isr_0b(uint32_t err) {
	char* msg = "segment not present\n";
	abort_code(msg, err);
}

// stack segment fault
void isr_0c(uint32_t err) {
	char* msg = "stack segment fault\n";
	abort_code(msg, err);
}

// general protection fault
void isr_0d(uint32_t err) {
	char* msg = "general protection fault\n";
	abort_code(msg, err);
}

// page fault
void isr_0e(uint32_t err) {
	char* msg = "page fault\n";
	serial_writeint16(get_cr2());
	serial_writestring(" address\n");
	abort_code(msg, err);
}

// x87 floating point exception
void isr_10(void) {
	char* msg = "x87 floating point exception\n";
	abort(msg);
}

// alignment check
void isr_11(uint32_t err) {
	char* msg = "alignment check\n";
	abort_code(msg, err);
}

// machine check
void isr_12(void) {
	char* msg = "machine check\n";
	abort(msg);
}

// SIMD floating point exception
void isr_13(void) {
	char* msg = "SIMD floating point exception\n";
	abort(msg);
}

// virtualization exception
void isr_14(void) {
	char* msg = "virtualization exception\n";
	abort(msg);
}

// security exception
void isr_1e(uint32_t err) {
	char* msg = "security exception\nerr code: ";
	abort_code(msg, err);
}