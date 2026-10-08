#include "keyboard.h"
#include "io.h"

#define KBD_RING_SIZE 32

static volatile uint8_t ring[KBD_RING_SIZE];
static volatile uint8_t head;
static volatile uint8_t tail;

static void keyboard_drain(void)
{
    int i;

    for (i = 0; i < 256; i++) {
        if (!(inb(KBD_STATUS_PORT) & KBD_STATUS_OUTPUT_FULL)) {
            break;
        }
        (void)inb(KBD_DATA_PORT);
    }
}

void keyboard_init(void)
{
    head = 0;
    tail = 0;
    keyboard_drain();
}

void keyboard_irq_handler(void)
{
    uint8_t scancode;
    uint8_t next;

    if (!(inb(KBD_STATUS_PORT) & KBD_STATUS_OUTPUT_FULL)) {
        return;
    }

    scancode = inb(KBD_DATA_PORT);
    next = (uint8_t)((head + 1) % KBD_RING_SIZE);

    /* Drop newest if the ring is full. */
    if (next == tail) {
        return;
    }

    ring[head] = scancode;
    head = next;
}

int keyboard_read_scancode(uint8_t *out)
{
    int available;

    cli();
    if (tail == head) {
        available = 0;
    } else {
        *out = ring[tail];
        tail = (uint8_t)((tail + 1) % KBD_RING_SIZE);
        available = 1;
    }
    sti();

    return available;
}
