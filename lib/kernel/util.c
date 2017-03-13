#include <serial.h>
#include <terminal.h>
#include <kernel/kernel.h>
#include <kernel/util.h>

// print the raw cpu registers
void print_registers(void) {
	uint32_t eax = get_eax();
	uint32_t ebx = get_ebx();
	uint32_t ecx = get_ecx();
	uint32_t edx = get_edx();
	uint32_t esi = get_esi();
	uint32_t edi = get_edi();
	uint32_t ebp = get_ebp();
	uint32_t esp = get_esp();
	uint32_t flags = get_flags();
	uint16_t ss = get_ss();
	uint16_t cs = get_cs();
	uint16_t ds = get_ds();
	uint16_t es = get_es();
	uint16_t fs = get_fs();
	uint16_t gs = get_gs();
	serial_writestring("  eax:\t");
	serial_writeint16(eax);
	serial_writestring("\n  ebx:\t");
	serial_writeint16(ebx);
	serial_writestring("\n  ecx:\t");
	serial_writeint16(ecx);
	serial_writestring("\n  edx:\t");
	serial_writeint16(edx);
	serial_writestring("\n  esi:\t");
	serial_writeint16(esi);
	serial_writestring("\n  edi:\t");
	serial_writeint16(edi);
	serial_writestring("\n  ebp:\t");
	serial_writeint16(ebp);
	serial_writestring("\n  esp:\t");
	serial_writeint16(esp);
	serial_writestring("\nflags:\t");
	serial_writeint16(flags);
	serial_writestring("\n   ss:\t");
	serial_writeint16(ss);
	serial_writestring("\n   cs:\t");
	serial_writeint16(cs);
	serial_writestring("\n   ds:\t");
	serial_writeint16(ds);
	serial_writestring("\n   es:\t");
	serial_writeint16(es);
	serial_writestring("\n   fs:\t");
	serial_writeint16(fs);
	serial_writestring("\n   gs:\t");
	serial_writeint16(gs);
	serial_writestring("\n");
}

// print a message and hang the machine
void abort(char* msg) {
	print_registers();
	serial_writestring("ERROR: ");
	serial_writestring(msg);
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
	terminal_putchar('\f');
	terminal_writestring("ERROR: ");
	terminal_writestring(msg);
	asm_halt();
}

void abort_code(char* msg, uint32_t err) {
	print_registers();
	serial_writestring("ERROR: ");
	serial_writestring(msg);
	serial_writestring("CODE: ");
	serial_writeint16(err);
	serial_putchar('\n');
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
	terminal_putchar('\f');
	terminal_writestring("ERROR: ");
	terminal_writestring(msg);
	terminal_writestring("CODE: ");
	terminal_writeint16(err);
	terminal_putchar('\n');
	asm_halt();
}