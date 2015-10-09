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
#include "bsp_41908.h"
#include "stm32f10x_spi.h"
#include "bsp_SPI.h"
//u8 PSUMAB;
//u8 PSUMCD;

void MS_Config(void);
void MS_Rest(void);
void MS_Int1(void);
void Variable_A(unsigned int *aa,char *bb);
void Variable_B(unsigned int *cc,char *dd);
void LED_Change_A(char LED_addr,char *cc,unsigned int *dd);
//unsigned int Speed[10]={0xffff,0x2000,0x0aff,0x08ff,0x07ff,0x0600,0x0500,0x0300,0x0150,0x0100};
//char Circle_number[10]={0x01,0x07,0x17,0x1 c,0x20,0x2a,0x33,0x55,0xc2,0xff};

/*********************************************/
void MS_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*41908��λ��-PA8*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
}
/****************************************************************************
* ��    �ƣ�void MS_Rest(void)
* ��    �ܣ�MS41908��λ
* ��ڲ���: ��
* ���ڲ�������
* ˵    ����41908_REST �ߡ��͡����߸�λ
* ���÷�����
****************************************************************************/  
void MS_Rest(void)
{
  MS_Hihg();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	MS_Low();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
	MS_Hihg();	
}
/****************************************************************************
* ��    �ƣ�void MS_Int1(void)
* ��    �ܣ�MS41908��ʼ��
* ��ڲ���: ��
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
void MS_Int1(void)
{
//	u8 temp;
	Spi_Write(0x0b,0x0080);   //      ʹ�� TESTEN1 
	
	
  Spi_Write(0x20,0x0E03);	 //�趨Ƶ��Ϊ	   DT1��ʱ��Ϊ 3ms   0x1e0a
	Spi_Write(0x22,0x0002);	 //DT2��ʱ��Ϊ 0.6ms   0x0002
	Spi_Write(0x23,0x6b6b);	 //����ABռ�ձ�Ϊ 90%  0xd8d8		�趨ռ�ձ�				0xf0f0 100%	  , 0x7878 50%
	Spi_Write(0x24,0x0dff);	 //AB 256ϸ��	�趨��������  0x0520
	Spi_Write(0x25,0x07ff);  //����STEPΪ3.64ms  0x07ff
	Spi_Write(0x27,0x0002);	 //DT2��ʱ��Ϊ 0.6ms  0x0002
	Spi_Write(0x28,0x6b6b);	 //����CDռ�ձ�Ϊ 90%  0xd8d8		�趨ռ�ձ�				0xf0f0 100%	  , 0x7878 50%
	Spi_Write(0x29,0x0dff);	 //AB 256ϸ��	�趨��������   0x0520
	Spi_Write(0x2a,0x0dbb);  //����STEPΪ3.64ms  0x07ff
	
	
	Spi_Write(0x21,0x0095);   //      ���ź�����PLS1��PLS2  
//	temp =  SPI_SendByte(0x23);
	
	Spi_Write(0x2c,0x0005);   // ����IRCUT
	
	__nop();
//	Write_word(0x20,0x0E0E);	 //             �趨Ƶ��
//	Write_word(0x22,0x0003);	 //0x0002
//	Write_word(0x23,0xd8d8);	 //0xd8d8		�趨ռ�ձ�				0xf0f0 100%	  , 0x7878 50%
//	Write_word(0x24,0x0dff);	 //0x0dff		�趨��������
//	Write_word(0x25,0x0fff);
//	Write_word(0x27,0x0003);	//0x0002
//	Write_word(0x28,0xd8d8);	//0xd8d8		�趨ռ�ձ�				0xf0f0 100%	   , 0x7878 50%
//	Write_word(0x29,0x0dff);	//0x0dff		�趨��������
//	Write_word(0x2a,0x0dbb);
}
/****************************************************************************
* ��    �ƣ�void Variable_A(unsigned int *aa,char *bb)
* ��    �ܣ����A����
* ��ڲ���: ��
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void Variable_A(unsigned int *aa,char *bb)
{
	unsigned int addr24,addr25;
	addr24=(0x0500+*bb);
	addr25=*aa; 
	Spi_Write(0x24,addr24);
  Spi_Write(0x25,addr25);		
}
/****************************************************************************
* ��    �ƣ�void Variable_B(unsigned int *cc,char *dd)
* ��    �ܣ����B����
* ��ڲ���: ��
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void Variable_B(unsigned int *cc,char *dd)
{
	unsigned int addr29,addr2a;
	addr29=(0x0500+*dd);
	addr2a=*cc; 
	Spi_Write(0x29,addr29);
  Spi_Write(0x2a,addr2a);	
}
/****************************************************************************
* ��    ��: LED_Change_A(char LED_addr,char *cc,unsigned int *dd)
* ��    �ܣ�����LED_A&B
* ��ڲ���: ��
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void LED_Change_A(char LED_addr,char *cc,unsigned int *dd)
{
	unsigned int LED_data;
	LED_data=(*dd+*cc);
	Spi_Write(LED_addr,LED_data);
}


