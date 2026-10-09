#include "kmalloc.h"
#include "pmm.h"

#define KMALLOC_HEAP_FRAMES 256u /* 1 MiB bump heap */

static uint8_t *heap_base;
static uint32_t heap_size;
static uint32_t heap_used;

void kmalloc_init(void)
{
    uint32_t i;
    uint32_t first;
    uint32_t next;

    heap_base = 0;
    heap_size = 0;
    heap_used = 0;

    first = pmm_alloc_frame();
    if (first == 0) {
        return;
    }

    for (i = 1; i < KMALLOC_HEAP_FRAMES; i++) {
        next = pmm_alloc_frame();
        if (next != first + i * PMM_FRAME_SIZE) {
            /* Non-contiguous: free what we took and abort. */
            uint32_t j;
            for (j = 0; j < i; j++) {
                pmm_free_frame(first + j * PMM_FRAME_SIZE);
            }
            if (next != 0) {
                pmm_free_frame(next);
            }
            return;
        }
    }

    heap_base = (uint8_t *)first;
    heap_size = KMALLOC_HEAP_FRAMES * PMM_FRAME_SIZE;
    heap_used = 0;
}

void *kmalloc(size_t size)
{
    uint32_t aligned;
    void *ptr;

    if (heap_base == 0 || size == 0) {
        return 0;
    }

    aligned = (uint32_t)((size + 7u) & ~7u);
    if (heap_used + aligned > heap_size) {
        return 0;
    }

    ptr = heap_base + heap_used;
    heap_used += aligned;
    return ptr;
}

void kfree(void *ptr)
{
    (void)ptr;
    /* Bump allocator: frees are ignored until a real heap exists. */
}

uint32_t kmalloc_used_bytes(void)
{
    return heap_used;
}

uint32_t kmalloc_heap_size(void)
{
    return heap_size;
}
