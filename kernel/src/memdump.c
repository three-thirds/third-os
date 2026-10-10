#include "memdump.h"
#include "vga.h"
#include <stddef.h>
#include <stdint.h>

static void print_hex_byte(uint8_t byte)
{
    static const char hex_chars[] = "0123456789abcdef";
    vga_putc(hex_chars[(byte >> 4) & 0x0F]);
    vga_putc(hex_chars[byte & 0x0F]);
}

static void print_hex_addr(uint32_t addr)
{
    static const char hex_chars[] = "0123456789abcdef";
    int shift;
    vga_puts("0x");
    for (shift = 28; shift >= 0; shift -= 4) {
        vga_putc(hex_chars[(addr >> shift) & 0x0F]);
    }
}

void dump_memory(uint32_t addr, size_t length)
{
    const uint8_t *ptr = (const uint8_t *)addr;
    size_t i;
    size_t j;

    for (i = 0; i < length; i += 16) {
        print_hex_addr(addr + i);
        vga_puts(": ");

        for (j = 0; j < 16; j++) {
            if (i + j < length) {
                print_hex_byte(ptr[i + j]);
                vga_putc(' ');
            } else {
                vga_puts("   ");
            }

            if (j == 7) {
                vga_putc(' '); // Spacer between 8-byte blocks
            }
        }

        // 3. Print ASCII sidebar
        vga_puts(" |");
        for (j = 0; j < 16 && (i + j) < length; j++) {
            uint8_t b = ptr[i + j];
            if (b >= 32 && b <= 126) {
                vga_putc((char)b);
            } else {
                vga_putc('.');
            }
        }
        vga_puts("|\n");
    }
}
