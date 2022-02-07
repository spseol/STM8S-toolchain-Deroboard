#include "delay.h"


void delay_us(uint8_t us)
{
    _delay_us(us);
}

void delay_ms(uint16_t ms)
{
    for (int16_t i = 0; i < ms; i++) {
        _delay_us(250);
        _delay_us(250);
        _delay_us(250);
        _delay_us(243);
    }
}
