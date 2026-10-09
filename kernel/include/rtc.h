#ifndef KERNEL_RTC_H
#define KERNEL_RTC_H

#include <stdint.h>

struct rtc_time {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year; /* years since 2000, 0–99 */
};

void rtc_init(void);
void rtc_read(struct rtc_time *out);

#endif /* KERNEL_RTC_H */
