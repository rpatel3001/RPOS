#ifndef _PAGING_H
#define _PAGING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

//kernel page tables
extern char kernel_PDP[];
extern char kernel_PD0[];
extern char kernel_PD1[];
extern char kernel_PD2[];
extern char kernel_PD3[];

#define PAGE_SIZE (2*1024*1024)
#define PAGE_BITMAP_LEN 64

size_t addr_to_page(uintptr_t addr);

uintptr_t page_to_addr(size_t page);

void mark_page_used(size_t page);

bool is_page_used(size_t page);

void mark_addr_range_used(uintptr_t addr, size_t len);

void mark_page_available(size_t page);

uintptr_t allocate_page();

void free_page(uintptr_t addr);

#endif