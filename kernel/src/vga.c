#include "vga.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

static size_t cursor_row;
static size_t cursor_col;
static uint8_t terminal_color;
static volatile uint16_t *terminal_buffer;

static uint16_t vga_entry(unsigned char c, uint8_t color)
{
    return (uint16_t)c | ((uint16_t)color << 8);
}

static void vga_scroll(void)
{
    size_t x;
    size_t y;

    for (y = 1; y < VGA_HEIGHT; y++) {
        for (x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(y - 1) * VGA_WIDTH + x] =
                terminal_buffer[y * VGA_WIDTH + x];
        }
    }

    for (x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
            vga_entry(' ', terminal_color);
    }

    cursor_row = VGA_HEIGHT - 1;
}

void vga_init(void)
{
    terminal_buffer = (volatile uint16_t *)VGA_MEMORY;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_clear();
}

void vga_set_color(uint8_t color)
{
    terminal_color = color;
}

void vga_clear(void)
{
    size_t x;
    size_t y;

    cursor_row = 0;
    cursor_col = 0;

    for (y = 0; y < VGA_HEIGHT; y++) {
        for (x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }
}

void vga_putc(char c)
{
    if (c == '\n') {
        cursor_col = 0;
        if (++cursor_row == VGA_HEIGHT) {
            vga_scroll();
        }
        return;
    }

    if (c == '\r') {
        cursor_col = 0;
        return;
    }

    terminal_buffer[cursor_row * VGA_WIDTH + cursor_col] =
        vga_entry((unsigned char)c, terminal_color);

    if (++cursor_col == VGA_WIDTH) {
        cursor_col = 0;
        if (++cursor_row == VGA_HEIGHT) {
            vga_scroll();
        }
    }
}

void vga_write(const char *data, size_t length)
{
    size_t i;

    for (i = 0; i < length; i++) {
        vga_putc(data[i]);
    }
}

void vga_puts(const char *s)
{
    while (*s != '\0') {
        vga_putc(*s++);
    }
}
