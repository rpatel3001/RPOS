#include <util.h>
#include <serial.h>
#include <kernel/interrupt.h>
#include "timer.h"
#define COMMAND_PORT 0x43
#define CHAN0_PORT 0x40

uint32_t freq;
uint32_t count;

void timer_init(uint32_t f) {
	freq = f;
	uint32_t divisor = 1193180 / freq;
	write_port(COMMAND_PORT, 0x36);
	write_port(CHAN0_PORT, divisor & 0xFF);
	write_port(CHAN0_PORT, (divisor >> 8) & 0xFF);
	serial_writestring("PIT initialized\n");
}

void timer_isr(void) {
	count++;
}

uint32_t millis(void) {
	return count * 1000 / freq;
}

void sleep(uint32_t m) {
	uint32_t start = millis();
	while(millis() < start + m);
}