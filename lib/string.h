#ifndef _STRING_H
#define _STRING_H

//return the length of a null terminated string
size_t strlen(const char* str);

//copy a chunk of memory from one place to another
void memcpy(void* dest, const void* orig, size_t len);

#endif