#include <stddef.h>

size_t strlen(const char *s)
{
    size_t len = 0;
    while (*s) {
        len++;
        s++;
    }
    return len;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    return (int)(*(unsigned char *)s1 - *(unsigned char *)s2);
}

// fill the memory address data from dest to dest + len with val
void *memset(void *dest, int val, size_t len)
{

    unsigned char *p = (unsigned char *)dest;
    for (size_t i = 0; i < len; i++) {
        p[i] = val;
    }

    return dest;
}

// copy from memory address src to dest till dest + len
void *memcpy(void *dest, void *src, size_t len)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (unsigned char *)src;

    for (size_t i = 0; i < len; i++) {
        d[i] = s[i];
    }

    return dest;
}
