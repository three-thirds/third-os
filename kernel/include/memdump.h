#ifndef KERNEL_MEMDUMP_H
#define KERNEL_MEMDUMP_H

#include <stddef.h>
#include <stdint.h>

void dump_memory(uint32_t addr, size_t length);
#endif // !KERNEL_MEMDUMP_H
