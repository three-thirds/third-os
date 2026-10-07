#include "vga.h"

void kmain(void)
{
    vga_init();
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    vga_puts("Third OS\n");
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    vga_puts("Phase 1: VGA console online.\n");

    for (;;) {
        __asm__ volatile("hlt");
    }
}
