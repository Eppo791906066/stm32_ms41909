/**
  ******************************************************************************
  * @file    
  * @author 
  * @version 
  * @date    
  * @brief  
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  *
  ******************************************************************************
  */ 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_SPI.h"
#include "bsp_41908.h"
#include "bsp_exti.h"
#include "bsp_TIMbase.h"

#define	digitalHi(p,i)			{p->BSRR=i;}			//����Ϊ�ߵ�ƽ		

#define DIR_FORWARD					(unsigned short)(0x0000)
#define DIR_REVERSE					(unsigned short)(0x0100)

#define SPEED_INC						0
#define SPEED_DEC						1

u8 Flag_AB1 = 0;
u8 Flag_AB2 = 0;
u8 PSUMAB_num = 0xff;
u8 PSUMCD_num = 0xff;

u8 flag1 = SPEED_INC;

u8 flag2 = 0;

u8 flag3 = 0;

unsigned int Speed_AB = 0x248d;

unsigned int INCTAB_num;

unsigned int INCTCD_num;

unsigned short dir = DIR_FORWARD;

u8 led_1,led_2=0;
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
void Delay(int b)
{ 
	unsigned int a,c;
	for(c=0;c<b;c++)
	 for(a=0;a<3060;a++);	
}
/*
 * ˵�����ϵ����٣���A��ť����٣��ٴΰ�A��ť���٣����ν���...
				 ��C��ť�ظ�Ĭ��Ĭ��ֵ�������ı���ټ���״̬;
				 ��B���̶���ĳһ�ٶȡ�
 */
int main(void)
{
	SystemInit();
	USART1_Config();
  SPI_SPI2_Config();
	MS_Config();
	EXTI_IO_Config();
	TIM2_Configuration();
	digitalHi(GPIOC,GPIO_Pin_13);
	digitalHi(GPIOC,GPIO_Pin_14);
	digitalHi(GPIOC,GPIO_Pin_15);
	
//  printf("\r\n ************MS41908@9Demo**************\r\n");		
	MS_Rest();//41908��λ
	MS_Int1();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	/*
	 * VD_time  = INTCTXX[15:0] X SUMXX[7:0] X 24 / 27M   (��λ����)  
	 * ��ôVD_FZ����������֮���ʱ��������Ϊ VD_FZ = VD_time + DT1 + DT2;
	 * һ��DT2ʱ������Ϊ0�� ����  VD_FZ = VD_time + DT1;
	 * �����趨 Freq(VD_time) = 60 HZ �� VD_time = 0.017 ��,
	 * ��ô������ʽ�ɵ� INTCTXX[15:0] X SUMXX[7:0] = 19125 (0x4AB5)
	 * ֻҪ�ڵ����ڼ� INTCTXX[15:0] X SUMXX[7:0] �Ļ����ֲ��䣬��ô VD_FZ��Ƶ��Ҳ���ñ�
	 * INTCTXX[15:0] ��ֵ�Ǿ���ת�ٵģ�ֵԽ��ת��Խ������֮��Խ��
	 * �������Ҫ�ԼӼ��ٽ������ߵ���ϣ���ôֻ��Ҫ�� INCTXX_num�ı���������ʽ
	 * ��,���ٿ��Է����жϴ���
	 */
	
	INCTCD_num = 0x0140;
	PSUMCD_num = 19125/INCTCD_num;  //INCTAB_num * PSUMAB_num;			  //17ms PSUMCD_num��ֵ�� INCTCD_num����

//	INCTAB_num = 0x4AB5;
//	INCTAB_num = 0x0140;
	INCTAB_num = 0x0140;
  PSUMAB_num = 19125/INCTAB_num;  //INCTAB_num * PSUMAB_num;			  //17ms	PSUMCD_num��ֵ�� INCTCD_num����
	
	if(dir==DIR_FORWARD)
	{
		Spi_Write(0x24,0x0c00 | PSUMAB_num);
	}
	else //if(dir==DIR_REVERSE)
	{
		Spi_Write(0x24,0x0d00 | PSUMAB_num);
	}	 //0x0dff    
	Spi_Write(0x25,INCTAB_num); 
		 
	Spi_Write(0x29,0x0d00 | PSUMCD_num);	 //0x0dff    
	Spi_Write(0x2a,INCTCD_num); 

	while(1)
	{	
    ;
  }
}


//��ʱ���ж� -  per 17ms -- Freq(VD_TIME) = 60HZ
//�ڶ�ʱ���ж��д���Ӽ��ٿ���
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		if(flag1==SPEED_INC) 
		{
			// A ������ٿ���
			INCTAB_num = INCTAB_num - 2;   //����,ֻ��Ҫ�Ѹ�ֵ�����ٶ����߱仯�Ϳ��Ըı��ٶȣ�������ʾ�������Ե��ٶȱ仯
			if(INCTAB_num<0x0140)//320
			{
				INCTAB_num = 0x0140;
			}
			PSUMAB_num = 19125/INCTAB_num;
			
			if(dir==DIR_FORWARD)
			{
				Spi_Write(0x24,0x0c00 | PSUMAB_num);
			}
			else //if(dir==DIR_REVERSE)
			{
				Spi_Write(0x24,0x0d00 | PSUMAB_num);
			}	 //0x0dff     
			Spi_Write(0x25,INCTAB_num); 

			// B ������ٿ���
			INCTCD_num = INCTCD_num + 2;
			if(INCTCD_num>0x4AB5)//19125
			{
				INCTCD_num = 0x4AB5;
			}
			PSUMCD_num = 19125/INCTCD_num;

			Spi_Write(0x29,0x0d00 | PSUMCD_num);	 //0x0dff    
			Spi_Write(0x2a,INCTCD_num); 
		}
		else if(flag1==SPEED_DEC)    // 
		{
			// A ������ٿ���
			INCTAB_num = INCTAB_num + 2;   
			if(INCTAB_num > 0x4AB5)
			{
				INCTAB_num = 0x4AB5;
			}
			PSUMAB_num = 19125/INCTAB_num;
			
			if(dir==DIR_FORWARD)
			{
				Spi_Write(0x24,0x0c00 | PSUMAB_num);
			}
			else //if(dir==DIR_REVERSE)
			{
				Spi_Write(0x24,0x0d00 | PSUMAB_num);
			}	 //0x0dff   
			Spi_Write(0x25,INCTAB_num); 

			//B ������ٿ���
			INCTCD_num  = INCTCD_num - 2;
			if(INCTCD_num < 0x0140) 
			{
				INCTCD_num = 0x0140;
			}
			PSUMCD_num = 19125/INCTCD_num;

			Spi_Write(0x29,0x0d00 | PSUMCD_num);	 //0x0dff    
			Spi_Write(0x2a,INCTCD_num); 
		}
		
		VD_Hihg();

		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();

		VD_Low(); 
		
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}		 	
}
//void TIM2_IRQHandler(void)
//{
//	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
//	{	
//		VD_Low();           //�����ظ��¼Ĵ���ֵ
//		
//		if(flag1==0)  // 
//		{
//			// A ������ٿ���
//		   INCTAB_num = INCTAB_num - 50;   //����  ֻ��Ҫ�Ѹ�ֵ���� �ٶ����߱仯�Ϳ��Ըı��ٶȣ�������ʾ�������Ե��ٶȱ仯
//		   if(INCTAB_num<345)
//			 {
//			//	  flag1 = 1;
//					INCTAB_num = 205;
//			 }
//		   PSUMAB_num = 19125/INCTAB_num;
//		   Spi_Write(0x24,0x0d00 | PSUMAB_num);	 //0x0dff    
//		   Spi_Write(0x25,INCTAB_num); 
//			 
//			 // B ������ٿ���
//				 INCTCD_num = INCTCD_num+ 30;
//				 if(INCTCD_num>19125)
//				 {
//					 INCTCD_num = 19125;
//				 }
//				 PSUMCD_num = 19125/INCTCD_num;
//				 
//				 Spi_Write(0x29,0x0d00 | PSUMCD_num);	 //0x0dff    
//				 Spi_Write(0x2a,INCTCD_num); 
//			 
//			
//			VD_Hihg();
//				 
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//				 
//			VD_Low(); 
//		}

//		if(flag1==1)    // 
//		{
//			// A ������ٿ���
//		   INCTAB_num = INCTAB_num + 30;    //����
//		   if(INCTAB_num > 0x4AB5)
//			 {
//			//	 flag1 = 0;
//				 INCTAB_num = 0x4AB5;
//			 }
//		   PSUMAB_num = 19125/INCTAB_num;
//		   Spi_Write(0x24,0x0d00 | PSUMAB_num);	 //0x0dff    
//		   Spi_Write(0x25,INCTAB_num); 
//			 
//			 //B ������ٿ���
//				 INCTCD_num  = INCTCD_num - 50;
//				 if(INCTCD_num < 0x0140) 
//				 {
//					 INCTCD_num = 0x0140;
//				 }
//				 PSUMCD_num = 19125/INCTCD_num;
//				 
//				 Spi_Write(0x29,0x0d00 | PSUMCD_num);	 //0x0dff    
//				 Spi_Write(0x2a,INCTCD_num); 
//			
//			//VD_Low();           //�����ظ��¼Ĵ���ֵ

//			VD_Hihg();
//				 
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//			__nop();__nop();__nop();__nop();
//				 
//			VD_Low(); 
//		}
//		

//		
//		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
//	}		 	
//}


EXTI_InitTypeDef EXTI_InitStructure;
void EXTI15_10_IRQHandler(void)   //  ��λ�ж�
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{	
		Delay(60);
		Delay(60);
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0)
		{
			//flag1 = !flag1;
			flag1 = (flag1+1)%2; //0-->1,1-->0
		}
		
		EXTI_ClearITPendingBit(EXTI_Line13);
	} 
	
	if(EXTI_GetITStatus(EXTI_Line14) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{	
		Delay(60);
		Delay(60);
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==0)
		{
			flag1 = 2; //0-->1,1-->0
			
			/* change direction */
			dir = (dir==DIR_FORWARD)?DIR_REVERSE:DIR_FORWARD;
			if(dir==DIR_FORWARD)
			{
				Spi_Write(0x24,0x0c00 | PSUMAB_num);
			}
			else //if(dir==DIR_REVERSE)
			{
				Spi_Write(0x24,0x0d00 | PSUMAB_num);
			}
		}
		
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	
	if(EXTI_GetITStatus(EXTI_Line15) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{	
		Delay(60);
		Delay(60);
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)==0)
		{
			MS_Rest();
			MS_Int1();


			INCTCD_num = 0x0120;
			PSUMCD_num = 19125/INCTCD_num;  //INCTAB_num * PSUMAB_num;			  //5ms

			//			INCTAB_num = 0x4AB5;
			INCTAB_num = 0x0120;
			PSUMAB_num = 19125/INCTAB_num;  //INCTAB_num * PSUMAB_num;			  //5ms	
			//printf("\r\n ************MS_41908@9��λ��ʼ��***********\r\n");	
			
			if(dir==DIR_FORWARD)
			{
				Spi_Write(0x24,0x0c00 | PSUMAB_num);
			}
			else //if(dir==DIR_REVERSE)
			{
				Spi_Write(0x24,0x0d00 | PSUMAB_num);
			}	 //0x0dff   
			Spi_Write(0x25,INCTAB_num); 
		}
		EXTI_ClearITPendingBit(EXTI_Line15);
	}  
/***********************************************************************************/
}

/*********************************************END OF FILE**********************/
