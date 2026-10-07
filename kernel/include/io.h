#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#include <stdint.h>

/* always_inline: clang freestanding may otherwise emit out-of-line copies
 * that overwrite the caller's stack frame when passing port args. */
static inline __attribute__((always_inline)) uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline __attribute__((always_inline)) void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

#endif /* KERNEL_IO_H */
