#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <stdint.h>

#define TIMER_HZ 100

void timer_init(void);
void timer_irq_handler(void);

uint32_t timer_ticks(void);
void timer_sleep_ms(uint32_t ms);

#endif /* KERNEL_TIMER_H */
