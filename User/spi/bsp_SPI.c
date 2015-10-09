/****************************************************************************
* 
*
*          
* 
*  
*
* 
* 
*    
*
* 
* 
* 
*
*/

#include "stm32f10x.h"
#include "bsp_SPI.h"
#include "stm32f10x_spi.h"	

void Spi_Write(u8 addr, unsigned int data);
void SPI_SPI2_Config(void);
u8 SPI_SendByte(u8 byte);
void VD_FZ(void);
extern void Delay(int b);

/****************************************************************************
* ��    �ƣ�void Spi_Write(u8 addr, unsigned int data)
* ��    �ܣ�MS41908@9����д��
* ��ڲ�����u8 addr����8λ��ַ  unsigned int *data-16λSPI����
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  
void Spi_Write(u8 addr, unsigned int data)
{	
	CS_Hihg();
	SPI_SendByte(addr);
	SPI_SendByte (data%256);
	SPI_SendByte(data/256);
	CS_Low();
}
/****************************************************************************
* ��    �ƣ�void VD_FZ(void)
* ��    �ܣ�����VD�ź�
* ��ڲ���: ��
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
void VD_FZ(void)
{

	VD_Hihg();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
//  Delay(2);
	VD_Low();	
}

void SPI_SPI2_Config(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
//  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2  ,ENABLE);	   //ʹ��SPI2����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);



//  /* ���� SPI2 ����: SCK, MISO and MOSI��PB13, PB14, PB15) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //���ù��ܣ����죩���  SPI2
  GPIO_Init(GPIOB, &GPIO_InitStructure);

//  /* ����SPI2 Ƭѡ PB12*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_0;			  					 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //���ģʽ����ٶ�50MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //ͨ���������ģʽ
  GPIO_Init(GPIOB, &GPIO_InitStructure);

//  /* SPI2 ���� */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
  /* ʹ��SPI2  */
  SPI_Cmd(SPI2, ENABLE);   
}
/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  //NotSelect_Flash();  while(1);
  /* Send byte through the SPI2 peripheral */;
  SPI_I2S_SendData(SPI2, byte);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/
