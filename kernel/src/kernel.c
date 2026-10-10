#include "fs.h"
#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "keyboard.h"
#include "kmalloc.h"
#include "kprintf.h"
#include "memdump.h"
#include "multiboot.h"
#include "pmm.h"
#include "rtc.h"
#include "timer.h"
#include "vga.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

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
    vga_puts("Phase 1: VGA console online.\n");
    kprintf("Kernel: %s | Status: %c | Memory: %d KB | Magic: 0x%x | 100%%\n",
            "Third OS", 'A', 1024, 0xCAFEBABE);

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
    fs_init();
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
                    if (strcmp(line, "ls") == 0) {
                        fs_list();
                    } else if (strncmp(line, "cat ", 4) == 0) {
                        const char *content = fs_read(line + 4);
                        if (content != NULL) {
                            kprintf("%s\n", content);
                        } else {
                            kprintf("cat: %s: No such file\n", line + 4);
                        }
                    } else if (strncmp(line, "write ", 6) == 0) {
                        const char *args = line + 6;
                        char filename[FS_MAX_FILENAME];
                        size_t fn_len = 0;

                        // this one just skips accidental spaces
                        while (*args == ' ') {
                            args++;
                        }

                        while (*args != '\0' && *args != ' ' &&
                               fn_len < FS_MAX_FILENAME - 1) {

                            filename[fn_len++] = *args++;
                        }
                        filename[fn_len] = '\0';

                        // this one skips the space between filename and content
                        while (*args == ' ') {
                            args++;
                        }

                        const char *content = args;
                        if (fn_len == 0 || *content == '\0') {
                            kprintf("write: usage: write <fillename> <text>\n");
                        } else {
                            if (fs_create(filename, content) == 0) {
                                kprintf("fs: created %s (%d bytes)\n", filename,
                                        strlen(content));
                            }
                        }
                    } else if (strncmp(line, "dump ", 5) == 0) {
                        const char *args = line + 5;
                        while (*args == ' ')
                            args++;

                        uint32_t addr = parse_hex(args);

                        while (*args != '\0' && *args != ' ')
                            args++;

                        while (*args == ' ')
                            args++;

                        uint32_t len = 64;
                        if (*args != '\0') {
                            len = parse_hex(args);
                            if (len == 0) {
                                len = 64;
                            }
                        }
                        kprintf("Memory dump: 0x%x (%d bytes)\n", addr, len);
                        dump_memory(addr, len);
                    } else {
                        kprintf("you: %s\n", line);
                    }
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
