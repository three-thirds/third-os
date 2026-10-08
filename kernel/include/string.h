#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H
#include <stddef.h>

size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
void *memset(void *dest, int val, size_t len);
void *memcpy(void *dest, void *src, size_t len);

#endif // !KERNEL_STRING_H
