#include <stddef.h>
#include <stdint.h>

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

int strncmp(const char *s1, const char *s2, int n)
{
    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }

    if (n == 0) {
        return 0;
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
void *memcpy(void *dest, const void *src, size_t len)
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
    unsigned int uvalue;

    // 0 is null escape code terminator
    if (value == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
        return buf;
    }

    if (value < 0 && base == 10) {
        is_negative = 1;
        uvalue = (unsigned int)(-value);
    } else {
        uvalue = (unsigned int)value;
    }

    while (uvalue != 0) {
        // get the base of value.
        int rem = uvalue % base;
        if (rem > 9) {
            buf[i++] =
                (char)((rem - 10) + 'a'); // hex as remainder is greater than 9,
        } else {
            buf[i++] = (char)(rem + '0');
        }
        uvalue = uvalue / base;
    }

    if (is_negative) {
        buf[i++] = '-';
    }

    buf[i] = '\0';

    reverse(buf, i);
    return buf;
}

uint32_t parse_hex(const char *s)
{
    uint32_t result = 0;

    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        s += 2;
    }

    while (*s != '\0') {
        char c = *s;
        uint32_t digit = 0;

        if (c >= '0' && c <= '9') {
            digit = (uint32_t)(c - '0');
        } else if (c >= 'a' && c <= 'f') {
            digit = (uint32_t)(c - 'a' + 10);
        } else if (c >= 'A' && c <= 'F') {
            digit = (uint32_t)(c - 'A' + 10);
        } else {
            break;
        }

        result = (result << 4) | digit;
        s++;
    }
    return result;
}
