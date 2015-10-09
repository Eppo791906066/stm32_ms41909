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

#define DIR_FORWARD					(unsigned short)(0x0000)
#define DIR_REVERSE					(unsigned short)(0x0100)

#define SPEED_INC						0
#define SPEED_DEC						1

#define TRUE								1
#define FALSE								0

/* STEP_time = (INTCTXX[15:0]*192)/27M
** 0.5cycle/s,200step/s,STEP_time = 5ms   --> INTCTXX[15:0] = 0x02BF
** 1cycle/s,  400step/s,STEP_time = 2.5ms --> INTCTXX[15:0] = 0x015F
** 1.5cycle/s,600step/s,STEP_time = 1.7ms --> INTCTXX[15:0] = 0x00EF(目标速度，大圈 1圈/2s，减速比16:50)
** 2cycle/s,  800step/s,STEP_time = 1.25ms--> INTCTXX[15:0] = 0x00AF//该速度下共振严重
** 3cycle/s, 1200step/s,STEP_time = 0.8ms --> INTCTXX[15:0] = 0x0070//该速度下已带不动传送带
 */
#define SPEED_EXPECT				0x00EF//1bigCycle/2s
#define SPEED_STOP				  0x6FFF//大于0x5000即可
#define SPEED_MAX_VALUE			0x00EF//0x00AF//0x02BF//(unsigned short)(0x0070)
#define SPEED_MIN_VALUE			0x4AB5

u8 Flag_AB1 = 0;
u8 Flag_AB2 = 0;
u8 PSUMAB_num = 0xff;
u8 PSUMCD_num = 0xff;

u8 flag_spd_change = SPEED_INC;

unsigned int Speed_AB = 0x248d;

unsigned int INCTAB_num;

unsigned int INCTCD_num;

unsigned short dir = DIR_FORWARD;
unsigned short val_bttnC = SPEED_EXPECT;

volatile unsigned char flag_reverse = FALSE;

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
				 摁B键固定在某一速度，且改变运动方向。
 */
int main(void)
{
	SystemInit();
	
	//1ms systick
	SysTick_Config(SystemCoreClock / 1000);
	
	USART1_Config();
  SPI_SPI2_Config();
	MS_Config();
	EXTI_IO_Config();
	TIM2_Configuration();
	digitalHi(GPIOC,GPIO_Pin_13);
	digitalHi(GPIOC,GPIO_Pin_14);
	digitalHi(GPIOC,GPIO_Pin_15);
		
	MS_Rest();//41908复位
	MS_Int1();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	/*
	 * VD_time  = INTCTXX[15:0] X SUMXX[7:0] X 24 / 27M   (单位：秒)  
	 * 那么VD_FZ两个上升沿之间的时间间隔至少为 VD_FZ = VD_time + DT1 + DT2;
	 * 一般DT2时间设置为0， 所以  VD_FZ = VD_time + DT1;
	 * 比如设定 Freq(VD_time) = 60 HZ 即 VD_time = 0.017 秒,
	 * 那么代入上式可得 INTCTXX[15:0] X SUMXX[7:0] = 19125 (0x4AB5)
	 * 只要在调速期间 INTCTXX[15:0] X SUMXX[7:0] 的积保持不变，那么 VD_FZ的频率也不用变
	 * INTCTXX[15:0] 的值是决定转速的，值越大，转速越慢，反之则越快
	 * 所以如果要对加减速进行曲线的拟合，那么只需要对 INCTXX_num的变量代入表达式
	 * 加,减速可以放在中断处理
	 */
	
	INCTCD_num = SPEED_MAX_VALUE;
	PSUMCD_num = 19125/INCTCD_num;  //INCTAB_num * PSUMAB_num;			  //17ms PSUMCD_num的值由 INCTCD_num决定

	INCTAB_num = SPEED_MAX_VALUE;
  PSUMAB_num = 19125/INCTAB_num;  //INCTAB_num * PSUMAB_num;			  //17ms	PSUMCD_num的值由 INCTCD_num决定
	
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
    if(flag_reverse == TRUE)
		{
			flag_reverse = FALSE;
			/* change direction */
			dir = (dir==DIR_FORWARD)?DIR_REVERSE:DIR_FORWARD;
			if(dir==DIR_FORWARD)
			{
				Spi_Write(0x24,0x0400 | PSUMAB_num);//&0xf7ff - ledB off
			}
			else //if(dir==DIR_REVERSE)
			{
				Spi_Write(0x24,0x0d00 | PSUMAB_num);
			}
		}
  }
}


//定时器中断 -  per 17ms -- Freq(VD_TIME) = 60HZ
//在定时器中断中处理加减速控制
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		if(flag_spd_change==SPEED_INC) 
		{
			// A 电机加速控制
			INCTAB_num = INCTAB_num - 2;   //加速,只需要把改值按照速度曲线变化就可以改变速度，现在演示的是线性的速度变化
			if(INCTAB_num<SPEED_MAX_VALUE)
			{
				INCTAB_num = SPEED_MAX_VALUE;
			}
			PSUMAB_num = 19125/INCTAB_num;
			
			if(dir==DIR_FORWARD)
			{
				Spi_Write(0x24,0x0400 | PSUMAB_num);//&0xf7ff - ledB off
			}
			else //if(dir==DIR_REVERSE)
			{
				Spi_Write(0x24,0x0d00 | PSUMAB_num);
			}	 //0x0dff     
			Spi_Write(0x25,INCTAB_num); 

			// B 电机减速控制
			INCTCD_num = INCTCD_num + 2;
			if(INCTCD_num>SPEED_MIN_VALUE)//19125
			{
				INCTCD_num = SPEED_MIN_VALUE;
			}
			PSUMCD_num = 19125/INCTCD_num;

			Spi_Write(0x29,0x0d00 | PSUMCD_num);	 //0x0dff    
			Spi_Write(0x2a,INCTCD_num); 
		}
		else if(flag_spd_change==SPEED_DEC)
		{
			// A 电机减速控制
			INCTAB_num = INCTAB_num + 2;   
			if(INCTAB_num > SPEED_MIN_VALUE)
			{
				INCTAB_num = SPEED_MIN_VALUE;
			}
			PSUMAB_num = 19125/INCTAB_num;
			
			if(dir==DIR_FORWARD)
			{
				Spi_Write(0x24,0x0400 | PSUMAB_num);//&0xf7ff - ledB off
			}
			else //if(dir==DIR_REVERSE)
			{
				Spi_Write(0x24,0x0d00 | PSUMAB_num);
			}	 //0x0dff   
			Spi_Write(0x25,INCTAB_num); 

			//B 电机加速控制
			INCTCD_num  = INCTCD_num - 2;
			if(INCTCD_num < SPEED_MAX_VALUE) 
			{
				INCTCD_num = SPEED_MAX_VALUE;
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

EXTI_InitTypeDef EXTI_InitStructure;
void EXTI15_10_IRQHandler(void)   //  复位中断
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) //确保是否产生了EXTI Line中断
	{	
		Delay(60);
		Delay(60);
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0)
		{
			flag_spd_change = (flag_spd_change+1)%2; //0-->1,1-->0
		}
		
		EXTI_ClearITPendingBit(EXTI_Line13);
	} 
	
	if(EXTI_GetITStatus(EXTI_Line14) != RESET) //确保是否产生了EXTI Line中断
	{	
		Delay(60);
		Delay(60);
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==0)
		{
			flag_spd_change = 2; //maintain current speed
			
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
	
	if(EXTI_GetITStatus(EXTI_Line15) != RESET) //确保是否产生了EXTI Line中断
	{	
		Delay(60);
		Delay(60);
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)==0)
		{
			MS_Rest();
			MS_Int1();

			INCTCD_num = SPEED_MAX_VALUE;
			PSUMCD_num = 19125/INCTCD_num;  //INCTAB_num * PSUMAB_num;

			/* 当摁下C键时要么停止要么按照1大圈/2s的速度转动 */
			flag_spd_change = 2; //maintain current speed
			val_bttnC = (val_bttnC==SPEED_EXPECT)?SPEED_STOP:SPEED_EXPECT;
			INCTAB_num = val_bttnC;
			PSUMAB_num = 19125/INCTAB_num;  //INCTAB_num * PSUMAB_num;
			
			if(dir==DIR_FORWARD)
			{
				Spi_Write(0x24,0x0400 | PSUMAB_num);//&0xf7ff - ledB off
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
