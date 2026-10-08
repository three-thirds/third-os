#include "timer.h"
#include "io.h"

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL2 0x42
#define PIT_COMMAND 0x43
#define PIT_FREQUENCY 1193182u
#define PC_SPEAKER_PORT 0x61

static volatile uint32_t ticks;
static uint32_t hz_measured = TIMER_HZ;
static timer_hook_fn hooks[TIMER_MAX_HOOKS];

static void pit_channel0_periodic(uint32_t frequency)
{
    uint32_t divisor = PIT_FREQUENCY / frequency;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    /* Channel 0, lobyte/hibyte, mode 3 (square wave), binary. */
    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, low);
    outb(PIT_CHANNEL0, high);
}

static void pit_channel2_oneshot(uint32_t count)
{
    uint8_t low = (uint8_t)(count & 0xFF);
    uint8_t high = (uint8_t)((count >> 8) & 0xFF);
    uint8_t speaker;

    /* Channel 2, lobyte/hibyte, mode 0 (interrupt on terminal count), binary. */
    outb(PIT_COMMAND, 0xB0);
    outb(PIT_CHANNEL2, low);
    outb(PIT_CHANNEL2, high);

    /* Gate channel 2 on; keep speaker data bit clear. */
    speaker = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, (uint8_t)((speaker & ~0x02u) | 0x01u));

    /* Bit 5 of port 0x61 mirrors channel-2 OUT; wait until count completes. */
    while ((inb(PC_SPEAKER_PORT) & 0x20) == 0) {
        /* spin */
    }
}

void timer_init(void)
{
    int i;

    ticks = 0;
    hz_measured = TIMER_HZ;

    for (i = 0; i < TIMER_MAX_HOOKS; i++) {
        hooks[i] = 0;
    }

    pit_channel0_periodic(TIMER_HZ);
}

void timer_irq_handler(void)
{
    int i;

    ticks++;

    for (i = 0; i < TIMER_MAX_HOOKS; i++) {
        if (hooks[i] != 0) {
            hooks[i]();
        }
    }
}

uint32_t timer_ticks(void)
{
    return ticks;
}

uint32_t timer_ms(void)
{
    return (ticks * 1000u) / hz_measured;
}

uint32_t timer_hz(void)
{
    return hz_measured;
}

void timer_sleep_ms(uint32_t ms)
{
    uint32_t start = ticks;
    uint32_t need = (ms * hz_measured + 999u) / 1000u;

    if (need == 0) {
        return;
    }

    while ((ticks - start) < need) {
        hlt();
    }
}

void timer_busywait_ms(uint32_t ms)
{
    uint32_t remaining = ms;

    while (remaining > 0) {
        uint32_t slice = remaining;
        uint32_t count;

        /* Channel 2 is 16-bit; cap each oneshot under ~55 ms at PIT freq. */
        if (slice > 50) {
            slice = 50;
        }

        count = (PIT_FREQUENCY * slice) / 1000u;
        if (count == 0) {
            count = 1;
        }
        if (count > 0xFFFFu) {
            count = 0xFFFFu;
        }

        pit_channel2_oneshot(count);
        remaining -= slice;
    }
}

uint32_t timer_calibrate(void)
{
    uint32_t start;
    uint32_t elapsed;
    uint32_t estimated;

    /*
     * Run a 100 ms channel-2 oneshot while IRQ0 keeps ticking.
     * estimated_hz ≈ elapsed_ticks * (1000 / 100).
     */
    start = ticks;
    timer_busywait_ms(100);
    elapsed = ticks - start;

    if (elapsed == 0) {
        hz_measured = TIMER_HZ;
        return hz_measured;
    }

    estimated = elapsed * 10u;
    if (estimated < 50u) {
        estimated = 50u;
    }
    if (estimated > 200u) {
        estimated = 200u;
    }

    hz_measured = estimated;
    return hz_measured;
}

int timer_hook_register(timer_hook_fn fn)
{
    int i;

    if (fn == 0) {
        return -1;
    }

    for (i = 0; i < TIMER_MAX_HOOKS; i++) {
        if (hooks[i] == fn) {
            return 0;
        }
    }

    for (i = 0; i < TIMER_MAX_HOOKS; i++) {
        if (hooks[i] == 0) {
            hooks[i] = fn;
            return 0;
        }
    }

    return -1;
}

void timer_hook_unregister(timer_hook_fn fn)
{
    int i;

    if (fn == 0) {
        return;
    }

    for (i = 0; i < TIMER_MAX_HOOKS; i++) {
        if (hooks[i] == fn) {
            hooks[i] = 0;
            return;
        }
    }
}
