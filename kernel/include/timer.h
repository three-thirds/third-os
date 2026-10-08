#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <stdint.h>

#define TIMER_HZ 100
#define TIMER_MAX_HOOKS 8

typedef void (*timer_hook_fn)(void);

void timer_init(void);
void timer_irq_handler(void);

uint32_t timer_ticks(void);
uint32_t timer_ms(void);
uint32_t timer_hz(void);

void timer_sleep_ms(uint32_t ms);

/* Busy-wait using PIT channel 2 in mode 0 (does not disturb IRQ0). */
void timer_busywait_ms(uint32_t ms);

/*
 * Measure IRQ0 rate against a channel-2 one-shot reference.
 * Updates the value returned by timer_hz(); returns that Hz estimate.
 */
uint32_t timer_calibrate(void);

/* Invoked from IRQ0 after the tick counter advances. Keep handlers short. */
int timer_hook_register(timer_hook_fn fn);
void timer_hook_unregister(timer_hook_fn fn);

#endif /* KERNEL_TIMER_H */
