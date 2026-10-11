#ifndef KERNEL_MEMDUMP_H
#define KERNEL_MEMDUMP_H

#include "multiboot.h"
#include <stddef.h>
#include <stdint.h>

void print_mmap(const struct multiboot_info *mbi);
void dump_memory(uint32_t addr, size_t length);
#endif // !KERNEL_MEMDUMP_H
