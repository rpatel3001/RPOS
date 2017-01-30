#include <stddef.h>

#include "string.h"

size_t strlen(const char* str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

void memcpy(void* to, const void* from, size_t len) {
  for (size_t col = 0; col < len; ++col) {
    ((char*)to)[col] = ((char*)from)[col];
  }
}