#include "spse_stm8.h"  


// blok funkcí pro STM s instancí ADC2
#ifdef STM8S208

/* funkce provede pøevod a vrátí výsledek pøevodu ze zvoleného kanálu.
Argument mùže být buï makro: ADC2_CHANNEL_x ... kde x mùže být 0 až 15 nebo èíselná hodnota 0 až 15 (odpovídá èíslu kanálu)
*/
uint16_t ADC_get(ADC2_Channel_TypeDef ADC2_Channel){
ADC2_Select_Channel(ADC2_Channel); // vybere kanál / nastavuje analogový multiplexer
ADC2->CR1 |= ADC2_CR1_ADON; // Start Conversion (ADON must be SET before => ADC must be enabled !)
while(!(ADC2->CSR & ADC2_CSR_EOC)); // èeká na dokonèení pøevodu (End Of Conversion)
ADC2->CSR &=~ADC2_CSR_EOC; // maže vlajku 
return ADC2_GetConversionValue(); // vrací výsledek
}

/* Funkce šetrnì pøepne multiplexer (náhrada za mizerné øešení ze SPL knihoven)
Argument mùže být buï makro: ADC2_CHANNEL_x ... kde x mùže být 0 až 15 nebo èíselná hodnota 0 až 15 (odpovídá èíslu kanálu)
*/
void ADC2_Select_Channel(ADC2_Channel_TypeDef ADC2_Channel){
    uint8_t tmp = (ADC2->CSR) & (~ADC2_CSR_CH);
    tmp |= ADC2_Channel | ADC2_CSR_EOC;
    ADC2->CSR = tmp;
}

/* Nastaví zarovnání výsledku pøevodu vlevo nebo vpravo
Argument mùže být makro ADC2_ALIGN_LEFT nebo ADC2_ALIGN_RIGHT
*/
void ADC2_AlignConfig(ADC2_Align_TypeDef ADC2_Align){
	if(ADC2_Align){
		ADC2->CR2 |= (uint8_t)(ADC2_Align);
	}else{
		ADC2->CR2 &= (uint8_t)(~ADC2_CR2_ALIGN);
	}
}

/* Funkce poèká definovaný èas na stabilizaci ADC (vhodné zaøadit po spuštìní ADC k zajištìní že se první pøevod neprovede pøed stabilizací)
*/
void ADC2_Startup_Wait(void){
	_delay_us(ADC_TSTAB);
}

#endif


// blok funkcí pro STM s instancí ADC1
#ifdef STM8S103

uint16_t ADC_get(ADC1_Channel_TypeDef ADC1_Channel){
ADC1_Select_Channel(ADC1_Channel);
ADC1->CR1 |= ADC1_CR1_ADON; // Start Conversion (ADON must be SET before => ADC must be enabled !)
while(!(ADC1->CSR & ADC1_CSR_EOC));
ADC1->CSR &=~ADC1_CSR_EOC;
return ADC1_GetConversionValue();
}

void ADC1_Select_Channel(ADC1_Channel_TypeDef ADC1_Channel){
    uint8_t tmp = (ADC1->CSR) & (~ADC1_CSR_CH);
    tmp |= ADC1_Channel | ADC1_CSR_EOC;
    ADC1->CSR = tmp;
}

void ADC1_AlignConfig(ADC1_Align_TypeDef ADC1_Align){
	if(ADC1_Align){
		ADC1->CR2 |= (uint8_t)(ADC1_Align);
	}else{
		ADC1->CR2 &= (uint8_t)(~ADC1_CR2_ALIGN);
	}
}

void ADC1_Startup_Wait(void){
	_delay_us(ADC_TSTAB);
}

#endif

