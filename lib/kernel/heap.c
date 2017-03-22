#include "heap.h"
#include <kernel/paging.h>
#include <kernel/util.h>
#include <stdbool.h>

#define HEAD_MAGIC 0x4EADBEEF
#define FOOT_MAGIC 0xF007BEEF

typedef struct heap_header {
	uint32_t magic;
	uint32_t size;
	bool is_hole;
	struct heap_header* next;
} heap_header;

typedef struct heap_footer {
	uint32_t magic;
	heap_header* header;
} heap_footer;

// right now this is a singly linked list
// eventually, we can add sorting, then turn it into a self balancing tree
heap_header* holes;

void remove_index(heap_header* val) {
	if (val == holes) {
		holes = val->next;
	} else {
		heap_header* ptr = holes;
		while (ptr->next) {
			if (ptr->next == val) {
				ptr->next = val->next;
				break;
			} else {
				ptr = ptr->next;
			}
		}
	}
}

void init_hole(heap_header* addr, uint32_t size) {
	addr->magic = HEAD_MAGIC;
	addr->size = size;
	addr->is_hole = true;
	addr->next = 0;

	heap_footer* footer = (heap_footer*)(addr + size - sizeof(heap_footer));
	footer->magic = FOOT_MAGIC;
	footer->header = addr;
}

void* kmalloc(uint32_t size) {
	if (size > PAGE_SIZE) {
		abort_code("tried to malloc too much memory", size);
	}
	heap_header* ptr = holes;
	while(ptr) {
		if (ptr->size - (sizeof(heap_header) + sizeof(heap_footer)) >= size) {
			remove_index(ptr);
			ptr->is_hole = false;
			return ptr + sizeof(heap_header);
		} else {
			ptr = ptr->next;
		}
	}
	ptr = (heap_header*)allocate_page();
	init_hole(ptr, PAGE_SIZE);
	ptr->next = holes;
	holes = ptr;
	return ptr + sizeof(heap_header);
}

void kfree(void* ptr) {
	ptr -= sizeof(heap_header);

	bool add_to_index = true;

	heap_header* header = (heap_header*)ptr;
	if (header->magic != HEAD_MAGIC) {
		abort_code("tried to free invalid address", (uint32_t)ptr);
	} else if (header->is_hole) {
		abort_code("tried to free a freed address", (uint32_t)ptr);
	}
	header->is_hole = true;

	// unify left
	heap_footer* left_footer = (heap_footer*)(ptr - sizeof(heap_footer));
	if (left_footer->magic == FOOT_MAGIC) {
		heap_header* left_header = (heap_header*)left_footer->header;
		if (left_header->is_hole) {
			left_header->size += header->size;
			((heap_footer*)(ptr + header->size - sizeof(heap_footer)))->header = left_header;
			header = left_header;
			add_to_index = false;
		}
	}

	// unify right
	heap_header* right_header = header + header->size;
	if (right_header->magic == HEAD_MAGIC && right_header->is_hole) {
		remove_index(right_header);
		heap_footer* right_footer = (heap_footer*)(right_header + right_header->size - sizeof(heap_footer));
		right_footer->header = header;
		header->size += right_header->size;
	}

	if (add_to_index) {
		header->next = holes;
		holes = header;
	}
}