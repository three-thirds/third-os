#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H
#include <stddef.h>

size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, int n);
void *memset(void *dest, int val, size_t len);
void *memcpy(void *dest, const void *src, size_t len);
char *itoa(int value, char *buf, int base);

#endif // !KERNEL_STRING_H
