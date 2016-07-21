#include "stm32f10x.h"
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GPIO_LOUYE_Configuration(void)//
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //??
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4
                                  | GPIO_Pin_7; //??????
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GPIO_PUMP_Configuration(void)//
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
    GPIO_InitStructure.GPIO_Pin =

        GPIO_Pin_8 ;  //step1 free
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD , ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |
                                  GPIO_Pin_4 |
                                  GPIO_Pin_5 |

                                  GPIO_Pin_7 ;  //step1 free
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOD, &GPIO_InitStructure);


    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |
                                  GPIO_Pin_1 |
                                  GPIO_Pin_2 ;  //step1 free
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    ////////////////????????????////////////////
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_6 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GPIO_AIR_PULS_Configuration(void)//
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ///GPIO PA
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);
    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_6 |
        GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void GPIO_LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ///GPIO Pd
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD , ENABLE);
    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_8 |
        GPIO_Pin_9 |
        GPIO_Pin_10 |
        GPIO_Pin_11             ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void GPIO_TEST_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ///GPIO Pd
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE);
    GPIO_InitStructure.GPIO_Pin =
//        GPIO_Pin_8 |
//        GPIO_Pin_9 |
//        GPIO_Pin_10 |
        GPIO_Pin_8             ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE , GPIO_Pin_8);
}
void GPIO_Alarm_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ///GPIO Pd
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE);
    GPIO_InitStructure.GPIO_Pin = //GPIO_Pin_11 |
        GPIO_Pin_12 |
        GPIO_Pin_13             ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE , GPIO_Pin_12);
    GPIO_SetBits(GPIOE , GPIO_Pin_13);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE , GPIO_Pin_11);

}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GPIO_Configuration(void)//
{

    GPIO_AIR_PULS_Configuration();
    GPIO_PUMP_Configuration();
    GPIO_LOUYE_Configuration();
    GPIO_LED_Init();
    GPIO_Alarm_Init();
	GPIO_TEST_Init();


}





