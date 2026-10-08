#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H

#include <stdint.h>

/* PS/2 controller ports (Scan Code Set 1; QEMU default). */
#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64
#define KBD_STATUS_OUTPUT_FULL 0x01

void keyboard_init(void);

/* Called from IRQ1 stub. */
void keyboard_irq_handler(void);

/*
 * Non-blocking: returns 1 and writes a scancode if the ring buffer
 * has data, otherwise returns 0.
 */
int keyboard_read_scancode(uint8_t *out);

#endif /* KERNEL_KEYBOARD_H */
