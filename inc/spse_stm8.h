#ifndef _SPSE_STM8_H_
#define _SPSE_STM8_H_ 1

#ifndef F_CPU
#warning F_CPU is not defined!
#endif

#include "stm8s_conf.h"
#include "delay.h"

#define ADC_TIMEOUT 1000
#define ADC_TSTAB 7 // èas pro stabilizaci ADC po zapnutí (7 us)

#ifdef STM8S208
uint16_t ADC_get(ADC2_Channel_TypeDef ADC2_Channel); // perform conversion on selected channel (ADC must be enabled prior calling that function)
void ADC2_Select_Channel(ADC2_Channel_TypeDef ADC2_Channel); // corrected function for selecting ADC channel
void ADC2_AlignConfig(ADC2_Align_TypeDef ADC2_Align); // configure ADC2 data align
void ADC2_Startup_Wait(void);
#endif

#ifdef STM8S103
uint16_t ADC_get(ADC1_Channel_TypeDef ADC1_Channel); // perform conversion on selected channel (ADC must be enabled prior calling that function)
void ADC1_Select_Channel(ADC1_Channel_TypeDef ADC1_Channel); // corrected function for selecting ADC channel
void ADC1_AlignConfig(ADC1_Align_TypeDef ADC1_Align); // configure ADC1 data align
void ADC1_Startup_Wait(void);
#endif



#endif