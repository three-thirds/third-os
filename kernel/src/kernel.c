#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "keyboard.h"
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

void kmain(void)
{
    char line[LINE_MAX];
    size_t len;
    char c;
    struct rtc_time now;
    uint32_t calibrated;

    cli();

    vga_init();
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    vga_puts("Third OS\n");
    vga_set_color(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    vga_puts("PIT timer + RTC extras.\n");

    gdt_init();
    idt_init();
    timer_init();
    rtc_init();
    keyboard_init();
    timer_hook_register(tick_hook);

    sti();

    timer_sleep_ms(500);
    vga_puts("ticks=");
    put_u32(timer_ticks());
    vga_puts(" ms=");
    put_u32(timer_ms());
    vga_puts(" hooks=");
    put_u32(hook_hits);
    vga_putc('\n');

    calibrated = timer_calibrate();
    vga_puts("calibrated_hz=");
    put_u32(calibrated);
    vga_putc('\n');

    rtc_read(&now);
    vga_puts("rtc=");
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
