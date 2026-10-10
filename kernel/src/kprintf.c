#include "kprintf.h"
#include "string.h"
#include "vga.h"
#include <stdarg.h>
void kprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char num_buf[32];
    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;

            if (*fmt == '\0') {
                break;
            }
            switch (*fmt) {
            case 'c': {
                char c = (char)va_arg(args, int);
                vga_putc(c);
                break;
            }
            case 's': {
                char *temp = va_arg(args, char *);
                char *s = (temp == NULL) ? "[null]" : temp;
                vga_puts(s);
                break;
            }
            case 'd': {
                int d = va_arg(args, int);
                itoa(d, num_buf, 10);
                vga_puts(num_buf);
                break;
            }
            case 'x': {
                int x = va_arg(args, int);
                itoa((int)x, num_buf, 16);
                vga_puts(num_buf);
                break;
            }
            case '%':
                vga_putc('%');
                break;
            }
        } else {
            vga_putc(*fmt);
        }
        fmt++;
    }
    va_end(args);
}
