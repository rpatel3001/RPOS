#include <kernel/kernel.h>
#include <serial.h>
#include "isr.h"

// divide by zero
void isr_00(void) {
	char* msg = "divide by zero\n";
	serial_writestring(msg);
	abort(msg);
}

// debug interrupt
void isr_01(void) {
	char* msg = "debug interrupt\n";
	serial_writestring(msg);
	abort(msg);
}

// non maskable interrupt
void isr_02(void) {
	char* msg = "non maskable interrupt\n";
	serial_writestring(msg);
	abort(msg);
}

// breakpoint
void isr_03(void) {
	char* msg = "breakpoint\n";
	serial_writestring(msg);
	abort(msg);
}

// overflow
void isr_04(void) {
	char* msg = "overflow\n";
	serial_writestring(msg);
	abort(msg);
}

// bound range exceeded
void isr_05(void) {
	char* msg = "bound range exceeded\n";
	serial_writestring(msg);
	abort(msg);
}

// invalid opcode
void isr_06(void) {
	char* msg = "invalid opcode\n";
	serial_writestring(msg);
	abort(msg);
}

// device not available
void isr_07(void) {
	char* msg = "device not available\n";
	serial_writestring(msg);
	abort(msg);
}

// double fault
void isr_08(void) {
	char* msg = "double fault\n";
	serial_writestring(msg);
	abort(msg);
}

// invalid TSS
void isr_0a(void) {
	char* msg = "invalid TSS\n";
	serial_writestring(msg);
	abort(msg);
}

// segment not present
void isr_0b(void) {
	char* msg = "segment not present\n";
	serial_writestring(msg);
	abort(msg);
}

// stack segment fault
void isr_0c(void) {
	char* msg = "stack segment fault\n";
	serial_writestring(msg);
	abort(msg);
}

// general protection fault
void isr_0d(void) {
	char* msg = "general protection fault\n";
	serial_writestring(msg);
	abort(msg);
}

// page fault
void isr_0e(void) {
	char* msg = "page fault\n";
	serial_writestring(msg);
	abort(msg);
}

// x87 floating point exception
void isr_10(void) {
	char* msg = "x87 floating point exception\n";
	serial_writestring(msg);
	abort(msg);
}

// alignment check
void isr_11(void) {
	char* msg = "alignment check\n";
	serial_writestring(msg);
	abort(msg);
}

// machine check
void isr_12(void) {
	char* msg = "machine check\n";
	serial_writestring(msg);
	abort(msg);
}

// SIMD floating point exception
void isr_13(void) {
	char* msg = "SIMD floating point exception\n";
	serial_writestring(msg);
	abort(msg);
}

// virtualization exception
void isr_14(void) {
	char* msg = "virtualization exception\n";
	serial_writestring(msg);
	abort(msg);
}

// security exception
void isr_1e(void) {
	char* msg = "security exception\n";
	serial_writestring(msg);
	abort(msg);
}