#include "stm8s.h"
#include "main.h"
#include "milis.h"

//#include "delay.h"
//#include <stdio.h>
//#include "uart1.h"

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();
    //init_uart1();
}


int main(void)
{
  
    uint32_t time = 0;

    setup();

    while (1) {
        if (milis() - time > 333 ) {
            REVERSE(LED); 
            time = milis();
            //printf("%ld\n", time);
        }

        //delay_ms(333);
        //printf("Funguje to!!!\n");
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
