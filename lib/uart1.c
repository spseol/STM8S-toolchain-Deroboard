/*
 * Soubor:  uart.c
 * Datum:   15.01.2021 13:24
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha: 
 * Popis:   
 ****************************************************/

#include "uart1.h"


void init_uart1(void)
{
    UART1_DeInit();
    /* UART1 configuration ------------------------------------------------------ */
    UART1_Init((uint32_t) 115200,       // - BaudRate = 115200 baud  
               UART1_WORDLENGTH_8D,     // - Word Length = 8 Bits
               UART1_STOPBITS_1,        // - One Stop Bit
               UART1_PARITY_NO, // - No parity
               UART1_SYNCMODE_CLOCK_DISABLE,    // - UART1 Clock disabled
               UART1_MODE_TXRX_ENABLE   //  - Receive and transmit enabled
        );
    UART1_Cmd(ENABLE);
}


PUTCHAR_PROTOTYPE {
    /* Write a character to the UART1 */
    UART1_SendData8(c);
    /* Loop until the end of transmission */
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET){
        ;
    }
    return (c);
}

/**
  * @brief Retargets the C library scanf function to the USART.
  * @param None
  * @retval char Character to Read
  */
GETCHAR_PROTOTYPE {
#ifdef _COSMIC_
    char c = 0;
#else
    int c = 0;
#endif
    /* Loop until the Read data register flag is SET */
    while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET) {
        ;
    }
    c = UART1_ReceiveData8();
    return (c);
}
