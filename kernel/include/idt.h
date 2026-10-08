#ifndef KERNEL_IDT_H
#define KERNEL_IDT_H

#include <stdint.h>

void idt_init(void);

/* Called from IRQ stubs in arch/x86/isr.s (PIC line 0–15). */
void irq_dispatch(uint32_t irq);

/* Send End Of Interrupt to the PIC(s) for IRQ line 0–15. */
void pic_send_eoi(uint8_t irq);

#endif /* KERNEL_IDT_H */
