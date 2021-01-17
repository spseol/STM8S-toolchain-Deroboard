#include "stm8s.h"
#include "milis.h"

/*#include "delay.h"*/
/*#include <stdio.h>*/
/*#include "../lib/uart.c"*/

#define _ISOC99_SOURCE
#define _GNU_SOURCE

#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_5
#define LED_ON   GPIO_WriteHigh(LED_PORT, LED_PIN);
#define LED_OFF  GPIO_WriteLow(LED_PORT, LED_PIN);
#define LED_FLIP GPIO_WriteReverse(LED_PORT, LED_PIN);


void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    init_milis();
}


void main(void)
{
    uint32_t time = 0;
    init();
    /*init_uart();*/

    while (1) {

        if (milis() - time > 577) {
            GPIO_WriteReverse(GPIOC, GPIO_PIN_5);
            time = milis();
        }

        /*LED_FLIP; */
        /*_delay_ms(333);*/
        /*printf("Funguje to!!!\n");*/
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
