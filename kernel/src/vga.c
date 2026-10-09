#include "vga.h"
#include "io.h"
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000
#define VGA_TAB_WIDTH 8

#define VGA_CRT_INDEX 0x3D4
#define VGA_CRT_DATA 0x3D5
#define VGA_CRT_CURSOR_START 0x0A
#define VGA_CRT_CURSOR_END 0x0B
#define VGA_CRT_CURSOR_HIGH 0x0E
#define VGA_CRT_CURSOR_LOW 0x0F

#define VGA_CURSOR_DISABLE_BIT 0x20

static size_t cursor_row;
static size_t cursor_col;
static uint8_t terminal_color;
static volatile uint16_t *terminal_buffer;

static uint8_t cursor_start = 14; /* underline by default */
static uint8_t cursor_end = 15;
static int cursor_enabled = 1;

static uint16_t vga_entry(unsigned char c, uint8_t color)
{
    return (uint16_t)c | ((uint16_t)color << 8);
}

static void vga_crt_write(uint8_t reg, uint8_t value)
{
    outb(VGA_CRT_INDEX, reg);
    outb(VGA_CRT_DATA, value);
}

static void vga_apply_cursor_shape(void)
{
    uint8_t start = (uint8_t)(cursor_start & 0x1F);

    if (!cursor_enabled) {
        start |= VGA_CURSOR_DISABLE_BIT;
    }

    vga_crt_write(VGA_CRT_CURSOR_START, start);
    vga_crt_write(VGA_CRT_CURSOR_END, (uint8_t)(cursor_end & 0x1F));
}

static void vga_update_cursor(void)
{
    uint16_t pos = (uint16_t)(cursor_row * VGA_WIDTH + cursor_col);

    vga_crt_write(VGA_CRT_CURSOR_HIGH, (uint8_t)((pos >> 8) & 0xFF));
    vga_crt_write(VGA_CRT_CURSOR_LOW, (uint8_t)(pos & 0xFF));
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
    cursor_enabled = 1;
    cursor_start = 14;
    cursor_end = 15;
    vga_apply_cursor_shape();
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

    vga_update_cursor();
}

void vga_get_cursor(size_t *row, size_t *col)
{
    if (row != NULL) {
        *row = cursor_row;
    }
    if (col != NULL) {
        *col = cursor_col;
    }
}

void vga_enable_cursor(void)
{
    cursor_enabled = 1;
    vga_apply_cursor_shape();
}

void vga_disable_cursor(void)
{
    cursor_enabled = 0;
    vga_apply_cursor_shape();
}

void vga_set_cursor_shape(uint8_t start, uint8_t end)
{
    if (start > 15) {
        start = 15;
    }
    if (end > 15) {
        end = 15;
    }
    if (start > end) {
        start = end;
    }

    cursor_start = start;
    cursor_end = end;
    vga_apply_cursor_shape();
}

void vga_putc(char c)
{
    size_t next_tab;

    if (c == '\n') {
        cursor_col = 0;
        if (++cursor_row == VGA_HEIGHT) {
            vga_scroll();
        }
        vga_update_cursor();
        return;
    }

    if (c == '\r') {
        cursor_col = 0;
        vga_update_cursor();
        return;
    }

    if (c == '\t') {
        next_tab = (cursor_col + VGA_TAB_WIDTH) & ~(size_t)(VGA_TAB_WIDTH - 1);
        if (next_tab >= VGA_WIDTH) {
            cursor_col = 0;
            if (++cursor_row == VGA_HEIGHT) {
                vga_scroll();
            }
        } else {
            while (cursor_col < next_tab) {
                terminal_buffer[cursor_row * VGA_WIDTH + cursor_col] =
                    vga_entry(' ', terminal_color);
                cursor_col++;
            }
        }
        vga_update_cursor();
        return;
    }

    if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
        } else if (cursor_row > 0) {
            cursor_row--;
            cursor_col = VGA_WIDTH - 1;
        } else {
            vga_update_cursor();
            return;
        }
        terminal_buffer[cursor_row * VGA_WIDTH + cursor_col] =
            vga_entry(' ', terminal_color);
        vga_update_cursor();
        return;
    }

    if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            terminal_buffer[cursor_row * VGA_WIDTH + cursor_col] =
                vga_entry(' ', terminal_color);
            vga_update_cursor();
        }
    }

    terminal_buffer[cursor_row * VGA_WIDTH + cursor_col] =
        vga_entry((unsigned char)c, terminal_color);

    if (++cursor_col == VGA_WIDTH) {
        cursor_col = 0;
        if (++cursor_row == VGA_HEIGHT) {
            vga_scroll();
        }
    }

    vga_update_cursor();
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
