#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "keyboard.h"
#include "kmalloc.h"
#include "multiboot.h"
#include "pmm.h"
#include "rtc.h"
#include "timer.h"
#include "vga.h"

#define LINE_MAX 78

static volatile uint32_t hook_hits;

static void tick_hook(void)
{
    hook_hits++;
}

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

static void put_u8_2(uint8_t value)
{
    vga_putc((char)('0' + (value / 10)));
    vga_putc((char)('0' + (value % 10)));
}

static void prompt(void)
{
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    vga_puts("> ");
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void kmain(uint32_t magic, struct multiboot_info *mbi)
{
    char line[LINE_MAX];
    size_t len;
    char c;
    struct rtc_time now;
    uint32_t calibrated;
    uint8_t *probe;
    int i;

    cli();

    vga_init();
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    vga_puts("Third OS\n");
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        vga_puts("bad multiboot magic\n");
        for (;;) {
            hlt();
        }
    }

    gdt_init();
    idt_init();
    timer_init();
    rtc_init();
    pmm_init(mbi);
    kmalloc_init();
    keyboard_init();
    timer_hook_register(tick_hook);

    sti();

    vga_puts("mem: free_frames=");
    put_u32(pmm_free_frames());
    vga_puts(" heap=");
    put_u32(kmalloc_heap_size());
    vga_putc('\n');

    probe = (uint8_t *)kmalloc(64);
    if (probe == 0) {
        vga_puts("kmalloc failed\n");
    } else {
        for (i = 0; i < 64; i++) {
            probe[i] = (uint8_t)(0xA0 + i);
        }
        vga_puts("kmalloc ok used=");
        put_u32(kmalloc_used_bytes());
        vga_putc('\n');
    }

    timer_sleep_ms(200);
    calibrated = timer_calibrate();
    rtc_read(&now);

    vga_puts("ticks=");
    put_u32(timer_ticks());
    vga_puts(" hz=");
    put_u32(calibrated);
    vga_puts(" rtc=");
    put_u8_2(now.year);
    vga_putc('-');
    put_u8_2(now.month);
    vga_putc('-');
    put_u8_2(now.day);
    vga_putc(' ');
    put_u8_2(now.hour);
    vga_putc(':');
    put_u8_2(now.minute);
    vga_putc(':');
    put_u8_2(now.second);
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
