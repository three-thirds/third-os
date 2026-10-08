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

static void reverse(char *str, int len)
{
    int start = 0;
    int end = len - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Integer to ASCII
char *itoa(int value, char *buf, int base)
{
    int i = 0;
    int is_negative = 0;

    // 0 is null escape code terminator
    if (value == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return buf;
    }

    if (value < 0 && base == 10) {
        is_negative = 1;
        value = -value;
    }

    while (value != 0) {
        // get the base of value.
        int rem = value % base;
        if (rem > 9) {
            buf[i++] =
                (char)((rem - 10) + 'a'); // hex as remainder is greater than 9,
        } else {
            buf[i++] = (char)(rem + '0');
        }
        value = value / base;
    }

    if (is_negative) {
        buf[i++] = '-';
    }

    buf[i] = '\0';

    reverse(buf, i);
    return buf;
}
