#include "pmm.h"

#include <stdint.h>

/* Support up to 256 MiB of physical RAM (65536 × 4 KiB frames). */
#define PMM_MAX_FRAMES (256u * 1024u * 1024u / PMM_FRAME_SIZE)

extern char kernel_end[];

static uint8_t bitmap[PMM_MAX_FRAMES / 8];
static uint32_t total_frames;
static uint32_t used_frames;

static void bitmap_set(uint32_t frame)
{
    bitmap[frame / 8] |= (uint8_t)(1u << (frame % 8));
}

static void bitmap_clear(uint32_t frame)
{
    bitmap[frame / 8] &= (uint8_t)~(1u << (frame % 8));
}

static int bitmap_test(uint32_t frame)
{
    return (bitmap[frame / 8] & (uint8_t)(1u << (frame % 8))) != 0;
}

static uint32_t align_up(uint32_t value, uint32_t align)
{
    return (value + align - 1u) & ~(align - 1u);
}

void pmm_init(const struct multiboot_info *mbi)
{
    uint32_t memory_bytes;
    uint32_t first_free;
    uint32_t i;

    total_frames = 0;
    used_frames = 0;

    for (i = 0; i < sizeof(bitmap); i++) {
        bitmap[i] = 0xFF; /* mark used until we know better */
    }

    if (mbi == 0 || (mbi->flags & MULTIBOOT_INFO_MEMORY) == 0) {
        return;
    }

    /* Contiguous RAM: [0, 1MiB + mem_upper KiB). */
    memory_bytes = 0x100000u + (mbi->mem_upper * 1024u);
    total_frames = memory_bytes / PMM_FRAME_SIZE;
    if (total_frames > PMM_MAX_FRAMES) {
        total_frames = PMM_MAX_FRAMES;
    }

    used_frames = total_frames;

    first_free = align_up((uint32_t)kernel_end, PMM_FRAME_SIZE) / PMM_FRAME_SIZE;
    if (first_free >= total_frames) {
        return;
    }

    for (i = first_free; i < total_frames; i++) {
        bitmap_clear(i);
        used_frames--;
    }
}

uint32_t pmm_alloc_frame(void)
{
    uint32_t i;

    for (i = 0; i < total_frames; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            used_frames++;
            return i * PMM_FRAME_SIZE;
        }
    }

    return 0;
}

void pmm_free_frame(uint32_t frame_addr)
{
    uint32_t frame;

    if ((frame_addr % PMM_FRAME_SIZE) != 0) {
        return;
    }

    frame = frame_addr / PMM_FRAME_SIZE;
    if (frame >= total_frames) {
        return;
    }

    if (bitmap_test(frame)) {
        bitmap_clear(frame);
        used_frames--;
    }
}

uint32_t pmm_total_frames(void)
{
    return total_frames;
}

uint32_t pmm_used_frames(void)
{
    return used_frames;
}

uint32_t pmm_free_frames(void)
{
    if (used_frames > total_frames) {
        return 0;
    }
    return total_frames - used_frames;
}
