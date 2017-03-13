#include <kernel/kernel.h>
#include <kernel/util.h>
#include "paging.h"

uint32_t physical_page_bitmap[PAGE_BITMAP_LEN];

size_t addr_to_page(uintptr_t addr) {
	return addr >> 21;
}

uintptr_t page_to_addr(size_t page) {
	return page << 21;
}

void mark_page_used(size_t page) {
	physical_page_bitmap[page / PAGE_BITMAP_LEN] |= 1 << (page & (PAGE_BITMAP_LEN - 1));
}

bool is_page_used(size_t page) {
	return physical_page_bitmap[page / PAGE_BITMAP_LEN] & (1 << ((page & (PAGE_BITMAP_LEN - 1))));
}

void mark_addr_range_used(uintptr_t addr, size_t len) {
	while (addr < (uint64_t)addr + len) {
		mark_page_used(addr_to_page(addr));
		if (addr + PAGE_SIZE < addr) {
			break;
		} else {
			addr += PAGE_SIZE;
		}
	}
}

void mark_page_available(size_t page) {
	physical_page_bitmap[page / PAGE_BITMAP_LEN] &= ~(1 << (page & (PAGE_BITMAP_LEN - 1)));
}

uintptr_t allocate_page() {
	size_t page = 0;
	while (!~physical_page_bitmap[page / PAGE_BITMAP_LEN]) {
		page += PAGE_BITMAP_LEN;
	}
	for (size_t i = 0; i < 32; ++i) {
		if (physical_page_bitmap[page / PAGE_BITMAP_LEN] & (1 << i)) {
			++page;
		} else {
			break;
		}
	}
	if (page_to_addr(page) > memsize_mb() * 1024 * 1024) {
		abort("out of memory\n");
	}
	mark_page_used(page);
	return page_to_addr(page);
}

void free_page(uintptr_t addr) {
	mark_page_available(addr_to_page(addr));
}