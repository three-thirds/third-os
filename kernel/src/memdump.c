#include "memdump.h"
#include "kprintf.h"
#include "multiboot.h"
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

void print_mmap(const struct multiboot_info *mbi)
{
    if ((mbi->flags & MULTIBOOT_INFO_MEM_MAP) == 0) {
        kprintf("mmap: no memory map provided by bootloader\n");
        return;
    }

    uint32_t mmap_addr = mbi->mmap_addr;
    uint32_t mmap_end = mbi->mmap_addr + mbi->mmap_length;

    kprintf("BIOS E820 Physical Memory Mapp: \n");

    while (mmap_addr < mmap_end) {
        struct multiboot_mmap_entry *entry =
            (struct multiboot_mmap_entry *)mmap_addr;

        uint32_t start = entry->addr_low;
        uint32_t end = entry->addr_low + entry->len_low;
        uint32_t kb = entry->len_low / 1024;

        const char *type_str = "Unknown";
        switch (entry->type) {
        case MULTIBOOT_MEMORY_AVAILABLE:
            type_str = "Usable RAM";
            break;
        case MULTIBOOT_MEMORY_RESERVED:
            type_str = "Reserved";
            break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            type_str = "ACPI Reclaim";
            break;
        case MULTIBOOT_MEMORY_MEMORY_NVS:
            type_str = "ACPI NVS";
            break;
        case MULTIBOOT_MEMORY_BADRAM:
            type_str = "Bad RAM";
            break;
        }

        kprintf("   [0x%x - 0x%x] %s (%d KB)\n", start, end, type_str, kb);

        mmap_addr += entry->size + sizeof(entry->size);
    }
}
