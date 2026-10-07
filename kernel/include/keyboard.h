#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H

#include <stdint.h>

/* PS/2 controller ports (Scan Code Set 1; QEMU default). */
#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64
#define KBD_STATUS_OUTPUT_FULL 0x01

void keyboard_poll_init(void);

/* Busy-wait until a scancode is available, then return it. */
uint8_t keyboard_poll(void);

#endif /* KERNEL_KEYBOARD_H */
