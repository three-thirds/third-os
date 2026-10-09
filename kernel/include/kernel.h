#ifndef KERNEL_H
#define KERNEL_H

#include "multiboot.h"

#include <stdint.h>

void kmain(uint32_t magic, struct multiboot_info *mbi);

#endif /* KERNEL_H */
