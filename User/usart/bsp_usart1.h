#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define USART1_DR_Base  0x40013804		// 0x40013800 + 0x04 = 0x40013804
#define SENDBUFF_SIZE   100
void USART1_Config(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

#endif /* __USART1_H */
