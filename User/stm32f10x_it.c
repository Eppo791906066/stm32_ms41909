/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdio.h>

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */

}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */

}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */

}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */

}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
/*******************************************************/
extern void VD_FZ(void);
//void TIM2_IRQHandler(void)
//{
//	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
//	{	
//		VD_FZ();
//		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
//	}		 	
//}
/*******************************************************/
extern void Variable_A(unsigned int *aa,char *bb);
extern void Variable_B(unsigned int *aa,char *bb);
extern void Spi_Write(u8 addr, unsigned int data);
//unsigned int Speed_AB = 0x248d;
//unsigned int Speed_CD;
unsigned int Speed[10]={0xff00,0x248d,0x0b16,0x091b,0x07f8,0x0600,0x0500,0x0300,0x0250,0x0180};
char Circle_number[10]={0x01,0x07,0x17,0x1c,0x20,0x2b,0x33,0x7f,0x6e,0xaa};
unsigned int LED[2]={0x0500,0x0d00};
char Aperture_A=4,Aperture_B=4;
extern void LED_Change_A(char LED_addr,char *cc,unsigned int *dd);
extern void Delay(int b);
extern void MS_Int1(void);
extern void MS_Rest(void);

//char led_1,led_2=0;
//EXTI_InitTypeDef EXTI_InitStructure;
//void EXTI15_10_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line14) != RESET) //确保是否产生了EXTI Line中断
//	{	
//		Delay(60);
//		Delay(60);
//		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==0){
////		 while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14))
////		 {
////     }
////     led_2=!led_2;
////     LED_Change_A(0x29,&Circle_number[Aperture_B],&LED[led_2]);


//	 }
//		EXTI_ClearITPendingBit(EXTI_Line14);
//	} 
///********************************************************************************/
//	if(EXTI_GetITStatus(EXTI_Line13) != RESET) //确保是否产生了EXTI Line中断
//	{	
//		Delay(60);
//		Delay(60);
//		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0){
////		 while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
////		 {
////     }
////     led_1=!led_1;
////      LED_Change_A(0x24,&Circle_number[Aperture_A],&LED[led_1]);
////			Spi_Write(0x25,Speed[Aperture_A]);

//			
//	 }
//		EXTI_ClearITPendingBit(EXTI_Line13);
//	}  
///********************************************************************************/
//	if(EXTI_GetITStatus(EXTI_Line15) != RESET) //确保是否产生了EXTI Line中断
//	{	
//		Delay(60);
//		Delay(60);
//		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)==0)
//	 {
//		 MS_Rest();
//		 MS_Int1();
//		 printf("\r\n ************MS_41908@9复位初始化***********\r\n");	
//	 }
//		EXTI_ClearITPendingBit(EXTI_Line15);
//	}  
///***********************************************************************************/
//}
/***********************************************************************************/
//extern void Variable_A(unsigned int *aa,char *bb);
//extern void Variable_B(unsigned int *aa,char *bb);
//unsigned int Speed[10]={0xffff,0x248d,0x0aff,0x08ff,0x07ff,0x0600,0x0500,0x0300,0x0150,0x0100};
//char Circle_number[10]={0x01,0x07,0x17,0x1c,0x20,0x2a,0x33,0x55,0xc2,0xff};
//char Aperture_A=4,Aperture_B=4;

//void EXTI1_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line1) != RESET) //确保是否产生了EXTI Line中断
//	{
//		Delay(60);
//		Delay(60);
//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
//	  {
//		 Aperture_A=Aperture_A+0x01;
//		 if(Aperture_A>=9)
//			 Aperture_A=9;	 
//		 Variable_A(&Speed[Aperture_A],&Circle_number[Aperture_A]);
//	  }
//		EXTI_ClearITPendingBit(EXTI_Line1);     //清除中断标志位
//	}  
//}
/***********************************************************************************/
//void EXTI3_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line3) != RESET) //确保是否产生了EXTI Line中断
//	{
//		Delay(60);
//		Delay(60);
//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0)
//	  {
//		 if(Aperture_A==0)
//			 Aperture_A=1;
//		 Aperture_A=Aperture_A-0x01;	 
//		 Variable_A(&Speed[Aperture_A],&Circle_number[Aperture_A]);
//	  }
//		EXTI_ClearITPendingBit(EXTI_Line3);     //清除中断标志位
//	}  
//}
/***********************************************************************************/
//void EXTI2_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //确保是否产生了EXTI Line中断
//	{
//		Delay(60);
//		Delay(60);
//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
//	  {
//		 Aperture_B=Aperture_B+0x01;	
//		 if(Aperture_B>=9)
//			 Aperture_B=9;			 			
//		 Variable_B(&Speed[Aperture_B],&Circle_number[Aperture_B]);
//	  }
//		EXTI_ClearITPendingBit(EXTI_Line2);     //清除中断标志位
//	}  
//}
/***********************************************************************************/
//void EXTI0_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //确保是否产生了EXTI Line中断
//	{
//		Delay(60);
//		Delay(60);
//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)
//	  {
//		 if(Aperture_B==0)
//			 Aperture_B=0;
//		 else
//		   Aperture_B=Aperture_B-0x01;			 
//		 Variable_B(&Speed[Aperture_B],&Circle_number[Aperture_B]);
//		 printf("\r\n%d\r\n",Circle_number[Aperture_B]);
//	  }
//		EXTI_ClearITPendingBit(EXTI_Line0);     //清除中断标志位
//	}  
//}
/***********************************************************************************/
/***********************************************************************************/
//void EXTI4_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //确保是否产生了EXTI Line中断
//	{
//		Delay(60);
//		Delay(60);
//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0)
//	  {
//      Spi_Write(0x24,0x0500);
//			Spi_Write(0x29,0x0500);
//	  }
//		EXTI_ClearITPendingBit(EXTI_Line4);     //清除中断标志位
//	}  
//}
/***********************************************************************************/
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void USART1_IRQHandler(void)
{
//	uint8_t ch;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{ 	
//	    //ch = USART1->DR;
//			ch = USART_ReceiveData(USART1);
//	  	printf( "%c", ch );    //将接受到的数据直接返回打印
//	} 
	 
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
