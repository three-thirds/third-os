#include "rtc.h"
#include "io.h"

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

static uint8_t cmos_read(uint8_t reg)
{
    outb(CMOS_ADDRESS, (uint8_t)(0x80 | reg)); /* keep NMI disabled during access */
    return inb(CMOS_DATA);
}

static int rtc_updating(void)
{
    return (cmos_read(0x0A) & 0x80) != 0;
}

static uint8_t bcd_to_bin(uint8_t value)
{
    return (uint8_t)(((value >> 4) * 10) + (value & 0x0F));
}

void rtc_init(void)
{
    /* CMOS needs no programming for basic reads. */
}

void rtc_read(struct rtc_time *out)
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t reg_b;
    uint8_t second2;
    uint8_t minute2;
    uint8_t hour2;
    uint8_t day2;
    uint8_t month2;
    uint8_t year2;

    if (out == 0) {
        return;
    }

    /* Wait for a stable read (no update in progress; two identical samples). */
    for (;;) {
        while (rtc_updating()) {
            /* spin */
        }

        second = cmos_read(0x00);
        minute = cmos_read(0x02);
        hour = cmos_read(0x04);
        day = cmos_read(0x07);
        month = cmos_read(0x08);
        year = cmos_read(0x09);

        while (rtc_updating()) {
            /* spin */
        }

        second2 = cmos_read(0x00);
        minute2 = cmos_read(0x02);
        hour2 = cmos_read(0x04);
        day2 = cmos_read(0x07);
        month2 = cmos_read(0x08);
        year2 = cmos_read(0x09);

        if (second == second2 && minute == minute2 && hour == hour2 &&
            day == day2 && month == month2 && year == year2) {
            break;
        }
    }

    reg_b = cmos_read(0x0B);

    if ((reg_b & 0x04) == 0) {
        second = bcd_to_bin(second);
        minute = bcd_to_bin(minute);
        day = bcd_to_bin(day);
        month = bcd_to_bin(month);
        year = bcd_to_bin(year);
        hour = bcd_to_bin((uint8_t)(hour & 0x7F));
    } else {
        hour = (uint8_t)(hour & 0x7F);
    }

    out->second = second;
    out->minute = minute;
    out->hour = hour;
    out->day = day;
    out->month = month;
    out->year = year;
}
