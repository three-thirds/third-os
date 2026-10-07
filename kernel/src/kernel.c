#include "keyboard.h"
#include "vga.h"

static void vga_put_hex8(uint8_t value)
{
    static const char hex[] = "0123456789ABCDEF";

    vga_putc(hex[(value >> 4) & 0x0F]);
    vga_putc(hex[value & 0x0F]);
}

void kmain(void)
{
    uint8_t scancode;

    vga_init();
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    vga_puts("Third OS\n");
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    vga_puts("Phase 2: polling PS/2 keyboard (scancodes).\n");

    keyboard_poll_init();

    for (;;) {
        scancode = keyboard_poll();
        vga_puts("sc: 0x");
        vga_put_hex8(scancode);
        vga_putc('\n');
    }
}
