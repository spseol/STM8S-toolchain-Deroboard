#include "swspi.h"

#define CS_L GPIO_WriteLow(CS_PORT,CS_PIN)
#define CS_H GPIO_WriteHigh(CS_PORT,CS_PIN)
#define CLK_L GPIO_WriteLow(CLK_PORT,CLK_PIN)
#define	CLK_H GPIO_WriteHigh(CLK_PORT,CLK_PIN)
#define DIN_L GPIO_WriteLow(DIN_PORT,DIN_PIN)
#define DIN_H GPIO_WriteHigh(DIN_PORT,DIN_PIN)

#define SET(BAGR) GPIO_WriteHigh(BAGR##_PORT, BAGR##_PIN)
#define CLR(BAGR)  GPIO_WriteLow(BAGR##_PORT, BAGR##_PIN)

void swspi_init(void){
GPIO_Init(CS_PORT,CS_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
GPIO_Init(CLK_PORT,CLK_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
GPIO_Init(DIN_PORT,DIN_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
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


void swspi_tx2x8(uint8_t address, uint8_t data)
{
    uint16_t mask ;

    CLR(CS);                    // začánám visílat

    // adresa
    mask = 1 << 7;
    while (mask) {
        CLR(CLK);
        if (address & mask) {
            SET(DIN);
        } else {
            CLR(DIN);
        }
        SET(CLK);
        mask >>= 1;
        CLR(CLK);
    }

    // data 
    mask = 1 << 7;
    while (mask) {
        CLR(CLK);
        if (data & mask) {
            SET(DIN);
        } else {
            CLR(DIN);
        }
        SET(CLK);
        mask >>= 1;
        CLR(CLK);
    }

    SET(CS);                    // končím vysílán
}
