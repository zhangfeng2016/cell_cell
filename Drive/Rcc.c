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
    RCC_DeInit();             //ʱ�Ӹ�λ
    RCC_HSEConfig(RCC_HSE_ON);//ʹ���ⲿʱ��

    if(SUCCESS == RCC_WaitForHSEStartUp())
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);//

        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //9��Ƶ��PLL���Ϊ���72Mhz
        RCC_PLLCmd(ENABLE);

        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
            ;
        }
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //ϵͳʱ��ѡ��ʱ��Դ
    }
}

