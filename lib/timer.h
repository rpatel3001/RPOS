#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>

void timer_init(uint32_t f);

uint32_t millis(void);

void timer_isr(void);

void sleep(uint32_t m);

#endif