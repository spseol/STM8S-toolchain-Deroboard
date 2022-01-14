
#include "stm8s_conf.h"
#include "swspi.h"

#define CS_L GPIO_WriteLow(CS_GPIO,CS_PIN)
#define CS_H GPIO_WriteHigh(CS_GPIO,CS_PIN)
#define CLK_L GPIO_WriteLow(CLK_GPIO,CLK_PIN)
#define	CLK_H GPIO_WriteHigh(CLK_GPIO,CLK_PIN)
#define DIN_L GPIO_WriteLow(DIN_GPIO,DIN_PIN)
#define DIN_H GPIO_WriteHigh(DIN_GPIO,DIN_PIN)

void swspi_init(void){
GPIO_Init(CS_GPIO,CS_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
GPIO_Init(CLK_GPIO,CLK_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
GPIO_Init(DIN_GPIO,DIN_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
}

// Odesílá 16bit dat MSB first, SPI mode, sama provádí manipulaci s CS
void swspi_tx16(uint16_t data)
{
    uint16_t maska = 1 << 15;

    CS_L;
    while (maska) {
        if (maska & data) {
            DIN_H;
        } else {
            DIN_L;
        }
        CLK_H;
        maska = maska >> 1;
        CLK_L;
    }
    CS_H;
}

