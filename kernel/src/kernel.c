#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "keyboard.h"
#include "timer.h"
#include "vga.h"

#define LINE_MAX 78

static void put_u32(uint32_t value)
{
    char buf[10];
    int i = 0;

    if (value == 0) {
        vga_putc('0');
        return;
    }

    while (value > 0) {
        buf[i++] = (char)('0' + (value % 10));
        value /= 10;
    }

    while (i > 0) {
        vga_putc(buf[--i]);
    }
}

static void prompt(void)
{
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    vga_puts("> ");
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void kmain(void)
{
    char line[LINE_MAX];
    size_t len;
    char c;

    cli();

    vga_init();
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    vga_puts("Third OS\n");
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    vga_puts("PIT timer online.\n");

    gdt_init();
    idt_init();
    timer_init();
    keyboard_init();

    sti();

    timer_sleep_ms(500);
    vga_puts("ticks after 500ms: ");
    put_u32(timer_ticks());
    vga_putc('\n');

    len = 0;
    prompt();

    for (;;) {
        while (keyboard_read_char(&c)) {
            if (c == '\n') {
                line[len] = '\0';
                vga_putc('\n');
                if (len > 0) {
                    vga_puts("you: ");
                    vga_puts(line);
                    vga_putc('\n');
                }
                len = 0;
                prompt();
            } else if (c == '\b') {
                if (len > 0) {
                    len--;
                    vga_putc('\b');
                }
            } else if (c >= 32 && c < 127 && len + 1 < LINE_MAX) {
                line[len++] = c;
                vga_putc(c);
            }
        }
        hlt();
    }
}
