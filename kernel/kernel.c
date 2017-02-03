#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <keyboard.h>
#include <terminal.h>
#include <serial.h>
#include <string.h>

extern int get_eax(void);
extern int get_ebx(void);
extern int get_ecx(void);
extern int get_edx(void);
extern int get_esi(void);
extern int get_edi(void);
extern int get_ebp(void);
extern int get_esp(void);
extern int get_flags(void);
extern int get_ss(void);
extern int get_cs(void);
extern int get_ds(void);
extern int get_es(void);
extern int get_fs(void);
extern int get_gs(void);

void kernel_printregisters(void) {
	serial_writestring("eax:\t");
	serial_writeint16(get_eax());
	serial_writestring("\nebx:\t");
	serial_writeint16(get_ebx());
	serial_writestring("\necx:\t");
	serial_writeint16(get_ecx());
	serial_writestring("\nedx:\t");
	serial_writeint16(get_edx());
	serial_writestring("\nesi:\t");
	serial_writeint16(get_esi());
	serial_writestring("\nedi:\t");
	serial_writeint16(get_edi());
	serial_writestring("\nebp:\t");
	serial_writeint16(get_ebp());
	serial_writestring("\nesp:\t");
	serial_writeint16(get_esp());
	serial_writestring("\nflags:\t");
	serial_writeint16(get_flags());
	serial_writestring("\nss:\t");
	serial_writeint16(get_ss());
	serial_writestring("\ncs:\t");
	serial_writeint16(get_cs());
	serial_writestring("\nds:\t");
	serial_writeint16(get_ds());
	serial_writestring("\nes:\t");
	serial_writeint16(get_es());
	serial_writestring("\nfs:\t");
	serial_writeint16(get_fs());
	serial_writestring("\ngs:\t");
	serial_writeint16(get_gs());
	serial_writestring("\n");
}

char linebuffer[VGA_WIDTH+1];
size_t line_index = 0;
void kernel_handlechar(uint8_t c) {
	if (c == 0) return;

	terminal_putchar(c);
	linebuffer[line_index++] = c;
	if (c == '\n' || line_index == VGA_WIDTH) {
		serial_writestring(linebuffer);
		memset(linebuffer, 0, VGA_WIDTH);
		if (line_index == VGA_WIDTH) {
			serial_putchar('\n');
		}
		line_index = 0;
	}
}

void kernel_main(void) {
	serial_init();
	serial_writestring("\n");
	terminal_init();

	idt_init();
	kb_init(&kernel_handlechar);

	for (int i = -10; i <= 10; ++i) {
		terminal_writeint16(i);
		terminal_putchar('\n');
	}
	kernel_printregisters();
	serial_putchar('\n');
	while (1);
}