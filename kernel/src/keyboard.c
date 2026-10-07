#include "keyboard.h"
#include "io.h"

void keyboard_poll_init(void)
{
    int i;

    /* Drain leftover bytes; bound the loop in case status stays set. */
    for (i = 0; i < 256; i++) {
        if (!(inb(KBD_STATUS_PORT) & KBD_STATUS_OUTPUT_FULL)) {
            break;
        }
        (void)inb(KBD_DATA_PORT);
    }
}

uint8_t keyboard_poll(void)
{
    while (!(inb(KBD_STATUS_PORT) & KBD_STATUS_OUTPUT_FULL)) {
        /* spin */
    }

    return inb(KBD_DATA_PORT);
}
