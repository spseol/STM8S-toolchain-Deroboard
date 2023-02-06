#include "stm8s.h"
#include "milis.h"

//#include "delay.h"
//#include <stdio.h>
//#include "uart1.h"

#ifdef STM8S003
#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_0
#define BTN_PORT GPIOB
#define BTN_PIN  GPIO_PIN_7
#endif
#ifdef STM8S103
#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_4
#endif
#ifdef STM8S105
#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_0
#endif
#ifdef STM8S208
#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_5
#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#endif

#define LOW(BAGR) GPIO_WriteLow(BAGR##_PORT, BAGR##_PIN)
#define HIGH(BAGR) GPIO_WriteHigh(BAGR##_PORT, BAGR##_PIN)
#define REVERSE(BAGR) GPIO_WriteReverse(BAGR##_PORT, BAGR##_PIN)

#define READ(BAGR) GPIO_ReadInputPin(BAGR##_PORT, BAGR##_PIN) 
#define PUSH(BAGR) (GPIO_ReadInputPin(BAGR##_PORT, BAGR##_PIN)==RESET) 


void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
#if defined (BTN_PORT) || defined (BTN_PIN)
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);
#endif /* 
     */

    init_milis();
    //init_uart1();
}


int main(void)
{
  
    uint32_t time = 0;

    setup();

    while (1) {
#if defined (BTN_PORT) || defined (BTN_PIN)
        if (milis() - time > 333 && !PUSH(BTN)) {
#else
        if (milis() - time > 333 ) {
#endif
            REVERSE(LED); 
            time = milis();
            //printf("%ld\n", time);
        }

        //LED_REVERSE; 
        //delay_ms(333);
        //printf("Funguje to!!!\n");
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
