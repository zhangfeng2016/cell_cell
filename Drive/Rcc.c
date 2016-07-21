/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "Rcc.h"
//#include "stm32f10x_tim.h"

/*******************************************************************************
* Function Name  : Rcc_Configuration
* Description    : Rcc_Configuration program.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void RCC_Configuration(void)
{
    RCC_DeInit();             //时钟复位
    RCC_HSEConfig(RCC_HSE_ON);//使用外部时钟

    if(SUCCESS == RCC_WaitForHSEStartUp())
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);//

        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //9倍频，PLL输出为最大72Mhz
        RCC_PLLCmd(ENABLE);

        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
            ;
        }
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //系统时钟选择时钟源
    }
}

