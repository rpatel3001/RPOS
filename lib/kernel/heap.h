#ifndef _HEAP_H
#define _HEAP_H

#include <stdint.h>

void heap_init(void);

void* kmalloc(uint32_t size);

void kfree(void* ptr);

#endif