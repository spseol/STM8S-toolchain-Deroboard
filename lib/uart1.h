#ifndef __STM8_UART1_H
#define __STM8_UART1_H

#include "stm8s.h"

#define _ISOC99_SOURCE
#define _GNU_SOURCE

/* ***************    Makra    ******************** */
#ifdef _RAISONANCE_
#define PUTCHAR_PROTOTYPE int putchar (char c)
#define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
#define PUTCHAR_PROTOTYPE char putchar (char c)
#define GETCHAR_PROTOTYPE char getchar (void)
#elif defined (_SDCC_)          /* SDCC patch: ensure same types as stdio.h */
#define PUTCHAR_PROTOTYPE int putchar(int c)
#define GETCHAR_PROTOTYPE int getchar(void)
#else                           /* _IAR_ */
#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)
#endif                          /* _RAISONANCE_ */

#endif /* __STM8_UART1_H*/


void init_uart1(void);

