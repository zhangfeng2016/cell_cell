/****************************************Copyright (c)****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GPIO-LED application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-10-30
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "..\Drive\time.h"
#include "..\Drive\Uart.h"
#include "..\Drive\ADC.h"
#include "..\Drive\IIC.h"
#include "..\Drive\RTC.h"
#include "..\SD_Card\sdio_sd.h"
#include "..\FATFS_V008A\src\ff.h"
#include "..\GUI\User_dispaly.h"
#include "..\GUI\M600_API.h"
#include "..\Control\Cell_Saver_Control.h"
#include "user_conf.h"

u16 arr1;
u16 arr2;

//static u8  fac_us=0;//us延时倍乘数
uint32 blueteet_test;
unsigned char code_DataStr[11] = __DATE__;
unsigned char code_TimeStr[11] = __TIME__;
//GPIO_TypeDef *test ;
uint8 test_iic[2] = {0x12, 0x13};
uint8 test_iic_buf[2];
u8 ver_str[50] = {"BW-8200B v4.1.1 "};
u8 ver_str_insine[4] = {"94  "};
u8 hw_str[5]={"1.00 "};

const u8 TEXT_Buffer[] = {"MiniSTM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)
u8 datatemp[SIZE];
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/
//FATFS fs;         /* Work area (file system object) for logical drive */
//FIL fsrc;         /* file objects */
//FRESULT res;
//UINT br;
//FIL fsrc1;         /* file objects */
//char path[512]="0:";
//uint8_t textFileBuffer[] = "hello world \r\n";
//uint8_t textFileBuffer1[] = "test \r\n";

/* Private function prototypes -----------------------------------------------*/
int SD_TotalSize(void);
void USART_Configuration(void);
void NVIC_Configuration(void);
void TIM_Configuration(void);
void GPIO_Configuration(void);
FRESULT scan_files (char *path);
extern void SD_LowLevel_Init(void);
void File_sys_int(void);
void Write_data_to_SD(void);
/* Private functions ---------------------------------------------------------*/
void GPIO_Configuration(void);
void Step_moto_control(void);

void Save_state_and_setting(void);

void delay_init(u8 SYSCLK);
void blue_teeth_rx(void);
void Alarm_Update(void);
void Print_Report(void);
/*******************************************************************************
* Function Name  : Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  Delay (uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  Variable_Int (void)
{
    // uint8 temp_cont;
    speed_change_flag = 0;
    Pump_Pause = 0;
    Set_Fill_speed = 400;
    Set_Wash_speed = 400;
    Set_Empty_speed = 300;
    Set_Wash_Volume = 1000;
    Set_Prefill_Volume = 100;
    Set_Arlarm_ON_OFF = 1;
    //Set_Run_Volume=400;
    Set_feiye = 7000;
    Set_Bowl = 225;
    if(Set_Bowl == 225)
        Set_Run_Volume = 800;
    else
        Set_Run_Volume = 600;
    prefill_flag = 1;
    Wash_Bowl_Num = 0;
    Centrifugal_motor_speed_Set = 500;
    Mode = MODE_AUTO;
    work_sta = 25;
    GUI_State = GUI_0;
    Centrifugal_motor_orde = 2;


    ///////////////////////
    Bit_set(Save_to_EEP_flag, 0);
    //Power_Time();
    ///////////////////////报警全开
    Bit_set(Set_Alarm_flag, WELL_ALARM_SW);
    Bit_set(Set_Alarm_flag, AIR_ALARM_SW);
    Bit_set(Set_Alarm_flag, BLOOD_ALARM_SW);
    Bit_set(Set_Alarm_flag, LOUYE_ALARM_SW);
    /////////////////////////////

    Bit_clr(pump_flag, PUMP_START_OK);
    Bit_clr(Bowl_State, FILL_BLOOD);        //设置离心杯无血标志位
    Read_state_and_setting();
    Read_from_bkp();

}
/*******************************************************************************
* Function Name  : main
* Description    : Main Programme
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
int main(void)
{
    RCC_Configuration();
    GPIO_Configuration();
    TIM_Configuration();
    delay_init(72);
    IIC_Configuration();
    SD_LowLevel_Init();
    Usart_int();
    Adc_Init();
    RTC_Init();
    //EXTI_Configuration();
    NVIC_Configuration();
    PWM_Init( 360, 0);
    //PWM_Init( 800,0);
    M600_Set_Palette(0x35ff, 0xf0f0);
    //print_init();
    Delay(65534);
    Delay(65534);
    GPIO_ResetBits(GPIOE , GPIO_Pin_11);
    Variable_Int();
    Delay(0xFFFE);
    File_sys_int();


    while (1)
    {
        if(SecondSign == 1)
        {
            RTC_Get();
            LED_flash();
            Write_data_to_SD();
            if(Auto_refill_wait_flag == 1)
                Auto_refill_wait_time++;

            if(Bit_check_1(Bowl_State, 2))               //蠕动泵延时计时
                pump_dely_time--;

            SecondSign = 0;
        }
        if(RCC_SecondSign == 1)
        {
            Save_state_and_setting();
            Save_to_bkp();
            RCC_SecondSign = 0;
        }                             //通信或者接受信息控制
        Touch_Key_analysis_8200B();
        blue_teeth_rx();
        if(Bit_check_1(Time_flag, TIME_1MS_FLAG))
        {
            Sensor_Update();
            Bit_clr(Time_flag, TIME_1MS_FLAG);
        }
//\\Project\../USER/user_conf.c\Set_Wash_Volume
        if(Bit_check_1(Time_flag, TIME_5MS_FLAG)) //
        {
            //Blue_teeth();
            Blue_teeth_1();
            Bit_clr(Time_flag, TIME_5MS_FLAG);
            Alarm_Update();
            GUI_contorl();               //显示控制
            Cell_Saver_Control();
            Print_Report();
            if(Mode != MODE_YINGJI)
                Alarm_Sound();
            if(GPIO_ReadOutputDataBit(GPIOD , GPIO_Pin_10))
            {
                GPIO_ResetBits(GPIOD , GPIO_Pin_10);
            }
            else
            {
                GPIO_SetBits(GPIOD , GPIO_Pin_10);
            }
        }
    }
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
