/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "user_conf.h"
#include "..\SD_Card\sdio_sd.h"
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
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */


uint8 Pulse_Turn_Flag;
#define TURN    0

uint32   OS_Time;

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
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
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
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 Handler.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Pump_Chose(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_TypeDef *GPIOx_1, uint16_t GPIO_Pin1)        //
{
    if(GPIO_ReadOutputDataBit(GPIOx , GPIO_Pin))                  //������������������弫�Ա任
    {
        Pump_puls_count++;
        GPIO_ResetBits(GPIOx , GPIO_Pin);
        //if(GPIOx!=GPIOx_1)
        GPIO_ResetBits(GPIOx_1 , GPIO_Pin1);
    }
    else
    {
        GPIO_SetBits(GPIOx , GPIO_Pin);
        //if(GPIOx!=GPIOx_1)
        GPIO_SetBits(GPIOx_1 , GPIO_Pin1);
    }
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 Handler.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TIM2_IRQHandler(void)         //0.5US             //���ڲ��������ź����������������������
{
    if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
        Pump_Chose(Pump_GPIOx_1, GPIOx_1_Pin, Pump_GPIOx_2, GPIOx_2_Pin);
    }
}


/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM2 Handler.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TIM4_IRQHandler(void)       //0.125ms����һ���ж� //���ڲ���ϵͳʱ��Ƭ//���������õ�������ʱ�Ĳ���
{

    if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
        OS_Time++;
        if(OS_Time % 20 == 0)
        {
            Bit_set(Time_flag, TIME_1MS_FLAG); //
        }
        if(OS_Time % 160 == 0)
        {
            Bit_set(Time_flag, TIME_5MS_FLAG); //��������ʱ��Ƭ ʱ��δ���е���

        }
        if(OS_Time % 1000 == 0)
        {
            if(Show_donghua_num < 4)
                Show_donghua_num++;
            if(Show_donghua_num == 4)
                Show_donghua_num = 0;

        }
        if(OS_Time % 4000 == 0)
        {
            Bit_set(Time_flag, TIME_50MS_FLAG); //���ָ��ʱ�䡣
            RCC_SecondSign = 1;
        }
        if(OS_Time == 80000)
        {
            OS_Time = 0;

        }
    }
}



/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM2 Handler.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TIM3_IRQHandler(void)       //0.125ms����һ���ж�
{

}
void UART0_IRQHandler(void)
{


}


/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{
    /* Process All SDIO Interrupt Sources */
    SD_ProcessIRQSrc();
}

//void USART1_IRQHandler(void)
//{
// 	u8 m;
//	USART_ClearITPendingBit(USART1,USART_IT_TXE);
//	USART_ClearITPendingBit(USART1,USART_IT_TC);
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//		m=USART_ReceiveData(USART1);
//		USART_SendData(USART1, m);
//	}
//}
void RTC_IRQHandler(void)
{
    if(RTC->CRL & 0x0001) //�����ж�
    {
        SecondSign = 1;
    }
    if(RTC->CRL & 0x0002)         //�����ж�
    {
        RTC->CRL &= ~(0x0002);      //�������ж�
    }
    RTC->CRL &= 0X0FFA;         //�������������жϱ�־
    while(!(RTC->CRL & (1 << 5))); //�ȴ�RTC�Ĵ����������
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

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


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
