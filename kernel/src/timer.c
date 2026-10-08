#include "timer.h"
#include "io.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND 0x43
#define PIT_FREQUENCY 1193182u

static volatile uint32_t ticks;

void timer_init(void)
{
    uint32_t divisor = PIT_FREQUENCY / TIMER_HZ;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    ticks = 0;

    /* Channel 0, lobyte/hibyte, mode 3 (square wave), binary. */
    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, low);
    outb(PIT_CHANNEL0, high);
}

void timer_irq_handler(void)
{
    ticks++;
}

uint32_t timer_ticks(void)
{
    return ticks;
}

void timer_sleep_ms(uint32_t ms)
{
    uint32_t start = ticks;
    uint32_t need = (ms * TIMER_HZ + 999u) / 1000u;

    if (need == 0) {
        return;
    }

    while ((ticks - start) < need) {
        hlt();
    }
}
