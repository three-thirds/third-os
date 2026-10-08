#ifndef KERNEL_MULTIBOOT_H
#define KERNEL_MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002u
#define MULTIBOOT_INFO_MEMORY (1u << 0)

struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower; /* KiB of memory below 1 MiB */
    uint32_t mem_upper; /* KiB of memory above 1 MiB */
} __attribute__((packed));

#endif /* KERNEL_MULTIBOOT_H */
