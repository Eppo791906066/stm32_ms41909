#ifndef __Spi_H
#define	__Spi_H

#include "stm32f10x.h"
#include <stdio.h>

#define VD_Low()   GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define VD_Hihg()    GPIO_SetBits(GPIOB,GPIO_Pin_0)


#define CS_Low()   GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define CS_Hihg()    GPIO_SetBits(GPIOB,GPIO_Pin_12)

void Spi_Write(u8 addr, unsigned int data);
void SPI_SPI2_Config(void);
u8 SPI_Flash_ReadByte(void);
u8 SPI_SendByte(u8 byte);
void VD_FZ(void);

#endif /* ___H */

