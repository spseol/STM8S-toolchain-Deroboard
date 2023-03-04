#ifndef _MAIN_H_
#define _MAIN_H_ 1

#define LOW(BAGR) GPIO_WriteLow(BAGR##_PORT, BAGR##_PIN)
#define HIGH(BAGR) GPIO_WriteHigh(BAGR##_PORT, BAGR##_PIN)
#define REVERSE(BAGR) GPIO_WriteReverse(BAGR##_PORT, BAGR##_PIN)

#define READ(BAGR) GPIO_ReadInputPin(BAGR##_PORT, BAGR##_PIN) 
#define PUSH(BAGR) (GPIO_ReadInputPin(BAGR##_PORT, BAGR##_PIN)==RESET) 

#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_4

// functions
void setup(void);

#endif // !_MAIN_H_
