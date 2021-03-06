#include <stddef.h>
#include <stdint.h>

#include "string.h"

//return the length of a null terminated string
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len]) {
		++len;
	}
	return len;
}

//copy a chunk of memory from one place to another
void memcpy(void* dest, const void* orig, size_t len) {
	for (size_t col = 0; col < len; ++col) {
		((char*)dest)[col] = ((char*)orig)[col];
	}
}

//set a chunk of memory to a single value
void memset(void* dest, char val, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		((char*)dest)[i] = val;
	}
}

//compare two chunks of memory
uint8_t memcmp(const void* buf1, const void* buf2, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		if (((uint8_t*)buf1)[i] != ((uint8_t*)buf2)[i]) {
			return ((uint8_t*)buf1)[i] - ((uint8_t*)buf2)[i];
		}
	}
	return 0;
}