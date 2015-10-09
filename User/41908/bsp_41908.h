
#include "stm32f10x.h"
#include <stdio.h>


#define MS_Low()     GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define MS_Hihg()    GPIO_SetBits(GPIOA, GPIO_Pin_8)
void MS_Config(void);
void MS_Rest(void);
void MS_Int1(void);
