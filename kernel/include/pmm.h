#ifndef KERNEL_PMM_H
#define KERNEL_PMM_H

#include "multiboot.h"

#include <stdint.h>

#define PMM_FRAME_SIZE 4096u

void pmm_init(const struct multiboot_info *mbi);

/* Returns physical frame address, or 0 on failure. */
uint32_t pmm_alloc_frame(void);
void pmm_free_frame(uint32_t frame_addr);

uint32_t pmm_total_frames(void);
uint32_t pmm_used_frames(void);
uint32_t pmm_free_frames(void);

#endif /* KERNEL_PMM_H */
