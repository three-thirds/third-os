#include "keyboard.h"
#include "io.h"

#define KBD_RING_SIZE 32

static volatile uint8_t ring[KBD_RING_SIZE];
static volatile uint8_t head;
static volatile uint8_t tail;

static int shift_left;
static int shift_right;
static int caps_lock;
static int e0_prefix;

/* US QWERTY Set 1 make codes → ASCII (index = make code & 0x7F). */
static const char keymap_normal[128] = {
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4', [0x06] = '5',
    [0x07] = '6', [0x08] = '7', [0x09] = '8', [0x0A] = '9', [0x0B] = '0',
    [0x0C] = '-', [0x0D] = '=',
    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r', [0x14] = 't',
    [0x15] = 'y', [0x16] = 'u', [0x17] = 'i', [0x18] = 'o', [0x19] = 'p',
    [0x1A] = '[', [0x1B] = ']',
    [0x1E] = 'a', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f', [0x22] = 'g',
    [0x23] = 'h', [0x24] = 'j', [0x25] = 'k', [0x26] = 'l', [0x27] = ';',
    [0x28] = '\'', [0x29] = '`',
    [0x2B] = '\\',
    [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v', [0x30] = 'b',
    [0x31] = 'n', [0x32] = 'm', [0x33] = ',', [0x34] = '.', [0x35] = '/',
    [0x39] = ' ',
};

static const char keymap_shift[128] = {
    [0x02] = '!', [0x03] = '@', [0x04] = '#', [0x05] = '$', [0x06] = '%',
    [0x07] = '^', [0x08] = '&', [0x09] = '*', [0x0A] = '(', [0x0B] = ')',
    [0x0C] = '_', [0x0D] = '+',
    [0x10] = 'Q', [0x11] = 'W', [0x12] = 'E', [0x13] = 'R', [0x14] = 'T',
    [0x15] = 'Y', [0x16] = 'U', [0x17] = 'I', [0x18] = 'O', [0x19] = 'P',
    [0x1A] = '{', [0x1B] = '}',
    [0x1E] = 'A', [0x1F] = 'S', [0x20] = 'D', [0x21] = 'F', [0x22] = 'G',
    [0x23] = 'H', [0x24] = 'J', [0x25] = 'K', [0x26] = 'L', [0x27] = ':',
    [0x28] = '"', [0x29] = '~',
    [0x2B] = '|',
    [0x2C] = 'Z', [0x2D] = 'X', [0x2E] = 'C', [0x2F] = 'V', [0x30] = 'B',
    [0x31] = 'N', [0x32] = 'M', [0x33] = '<', [0x34] = '>', [0x35] = '?',
    [0x39] = ' ',
};

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

static int is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static char apply_caps(char c)
{
    if (!caps_lock || !is_letter(c)) {
        return c;
    }

    if (c >= 'a' && c <= 'z') {
        return (char)(c - 'a' + 'A');
    }

    return (char)(c - 'A' + 'a');
}

void keyboard_init(void)
{
    head = 0;
    tail = 0;
    shift_left = 0;
    shift_right = 0;
    caps_lock = 0;
    e0_prefix = 0;
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

int keyboard_read_char(char *out)
{
    uint8_t sc;
    uint8_t code;
    int release;
    int shifted;
    char c;

    if (!keyboard_read_scancode(&sc)) {
        return 0;
    }

    if (sc == 0xE0) {
        e0_prefix = 1;
        return 0;
    }

    if (e0_prefix) {
        e0_prefix = 0;
        /* Ignore extended keys (arrows, etc.) for now. */
        return 0;
    }

    release = (sc & 0x80) != 0;
    code = (uint8_t)(sc & 0x7F);

    if (code == 0x2A) {
        shift_left = !release;
        return 0;
    }
    if (code == 0x36) {
        shift_right = !release;
        return 0;
    }
    if (code == 0x3A) {
        if (!release) {
            caps_lock = !caps_lock;
        }
        return 0;
    }

    if (release) {
        return 0;
    }

    if (code == 0x1C) {
        *out = '\n';
        return 1;
    }
    if (code == 0x0E) {
        *out = '\b';
        return 1;
    }

    if (code >= 128) {
        return 0;
    }

    shifted = shift_left || shift_right;
    c = shifted ? keymap_shift[code] : keymap_normal[code];
    if (c == '\0') {
        return 0;
    }

    *out = apply_caps(c);
    return 1;
}
