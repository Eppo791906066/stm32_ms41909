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

#define	digitalHi(p,i)			{p->BSRR=i;}			//设置为高电平		

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

u8 led_1,led_2=0;
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
void Delay(int b)
{ 
	unsigned int a,c;
	for(c=0;c<b;c++)
	 for(a=0;a<3060;a++);	
}
/*
 * 说明：上电后减速，按A按钮后加速，再次按A按钮减速，依次交替...
				 摁C按钮回复默认默认值，但不改变加速减速状态;
				 摁B键固定在某一速度。
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
	
//	printf("\r\n ***************************************\r\n");	
//	printf("\r\n **********杭州瑞盟科技有限公司*********\r\n");	
//  printf("\r\n ************MS41908@9Demo**************\r\n");	
//	printf("\r\n 用于网络摄像机.监控摄像机用机头驱动芯片\r\n");	
	MS_Rest();//41908复位
	MS_Int1();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	 
	//——————————————————————————————说明----------------------------------//
	// VD_time  = INTCTXX[15:0] X SUMXX[7:0] X 24 / 27M   (单位：秒)  
	//那么VD_FZ两个上升沿之间的时间间隔至少为 VD_FZ = VD_time + DT1 + DT2;
	// 一般DT2时间设置为0， 所以  VD_FZ = VD_time + DT1;
	// 比如 VD_time = 0.017 秒,那么代入上式可得 INTCTXX[15:0] X SUMXX[7:0] = 19125
	//只要在调速期间 INTCTXX[15:0] X SUMXX[7:0] 的积保持不变，那么 VD_FZ的频率也不用变
	// INTCTXX[15:0] 的值是决定转速的，值越大，转速越慢，反之则越快
	//所以如果要对加减速进行曲线的拟合，那么只需要对 INCTXX_num的变量代入表达式
	// 加 减速可以放在中断处理
  // 以下就是步进电机加减速调速的例子
	//----------------------------说明结束--------------------------------//
	
	INCTCD_num = 0x0140;
	PSUMCD_num = 19125/INCTCD_num;  //INCTAB_num * PSUMAB_num;			  //17ms PSUMCD_num的值由 INCTCD_num决定

//	INCTAB_num = 0x4AB5;
//	INCTAB_num = 0x0140;
	INCTAB_num = 0x0140;
  PSUMAB_num = 19125/INCTAB_num;  //INCTAB_num * PSUMAB_num;			  //17ms	PSUMCD_num的值由 INCTCD_num决定
	
 Spi_Write(0x24,0x0d00 | PSUMAB_num);	 //0x0dff    
 Spi_Write(0x25,INCTAB_num); 
			 
 Spi_Write(0x29,0x0d00 | PSUMCD_num);	 //0x0dff    
 Spi_Write(0x2a,INCTCD_num); 

	while(1)
	{	
    ;
  }
}


//定时器中断 -  per 17ms -- VD_TIME = 60HZ
//在定时器中断中处理加减速控制
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		if(flag1==SPEED_INC) 
		{
			// A 电机加速控制
			INCTAB_num = INCTAB_num - 2;   //加速,只需要把改值按照速度曲线变化就可以改变速度，现在演示的是线性的速度变化
			if(INCTAB_num<0x0140)//320
			{
				INCTAB_num = 0x0140;
			}
			PSUMAB_num = 19125/INCTAB_num;
			Spi_Write(0x24,0x0d00 | PSUMAB_num);	 //0x0dff    
			Spi_Write(0x25,INCTAB_num); 

			// B 电机减速控制
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
			// A 电机减速控制
			INCTAB_num = INCTAB_num + 2;   
			if(INCTAB_num > 0x4AB5)
			{
				INCTAB_num = 0x4AB5;
			}
			PSUMAB_num = 19125/INCTAB_num;
			Spi_Write(0x24,0x0d00 | PSUMAB_num);	 //0x0dff    
			Spi_Write(0x25,INCTAB_num); 

			//B 电机加速控制
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
//		VD_Low();           //上升沿更新寄存器值
//		
//		if(flag1==0)  // 
//		{
//			// A 电机加速控制
//		   INCTAB_num = INCTAB_num - 50;   //加速  只需要把改值按照 速度曲线变化就可以改变速度，现在演示的是线性的速度变化
//		   if(INCTAB_num<345)
//			 {
//			//	  flag1 = 1;
//					INCTAB_num = 205;
//			 }
//		   PSUMAB_num = 19125/INCTAB_num;
//		   Spi_Write(0x24,0x0d00 | PSUMAB_num);	 //0x0dff    
//		   Spi_Write(0x25,INCTAB_num); 
//			 
//			 // B 电机减速控制
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
//			// A 电机减速控制
//		   INCTAB_num = INCTAB_num + 30;    //减速
//		   if(INCTAB_num > 0x4AB5)
//			 {
//			//	 flag1 = 0;
//				 INCTAB_num = 0x4AB5;
//			 }
//		   PSUMAB_num = 19125/INCTAB_num;
//		   Spi_Write(0x24,0x0d00 | PSUMAB_num);	 //0x0dff    
//		   Spi_Write(0x25,INCTAB_num); 
//			 
//			 //B 电机加速控制
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
//			//VD_Low();           //上升沿更新寄存器值

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
void EXTI15_10_IRQHandler(void)   //  复位中断
{

	if(EXTI_GetITStatus(EXTI_Line13) != RESET) //确保是否产生了EXTI Line中断
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
	
	if(EXTI_GetITStatus(EXTI_Line14) != RESET) //确保是否产生了EXTI Line中断
	{	
		Delay(60);
		Delay(60);
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==0)
		{
			flag1 = 2; //0-->1,1-->0
		}
		
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	
	if(EXTI_GetITStatus(EXTI_Line15) != RESET) //确保是否产生了EXTI Line中断
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
			//printf("\r\n ************MS_41908@9复位初始化***********\r\n");	
		 }
		EXTI_ClearITPendingBit(EXTI_Line15);
	}  
/***********************************************************************************/
}

/*********************************************END OF FILE**********************/
