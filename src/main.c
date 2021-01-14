#include "stm8s.h"
#include "__assert__.h"
#include "delay.h"
/*#include "milis.h"*/

void delay_ms(uint16_t ms)
{
    for (uint16_t d = 0; d < ms; d++) {
        _delay_us(250);
        _delay_us(250);
        _delay_us(250);
        _delay_us(2);
    }
}

void main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
    /*init_milis();*/
    while (1) {
        GPIO_WriteReverse(GPIOC, GPIO_PIN_5);
        delay_ms(777);
    }
}
