#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "keyboard.h"
#include "vga.h"

#define LINE_MAX 78

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
    vga_puts("Phase 4: type a line, Backspace edits, Enter submits.\n");

    gdt_init();
    idt_init();
    keyboard_init();

    sti();

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
