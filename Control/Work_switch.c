/*************** Radio File (c) ***************/
/**                                          **/
/**      基于STM32开发的血液回收机           **/
/**        Email:287285415@qq.com            **/
/**                                          **/
/**--------------文件信息--------------------**/
/**文    件   名: Work_control.c              **/
/**创    建   人: 张峰                       **/
/**创  建  日 期: 2014年7月11日              **/
/**描         述: 控制状态函数，             **/
/**               切换进出血通道             **/
/**********************************************/
#include "stm32f10x.h"
#include "..\User\user_conf.h"

//define///////////////////////////////////////////////////////////////////////
#define STEP_MOTOR_START()   GPIO_SetBits(GPIOB , GPIO_Pin_0)
#define STEP_MOTOR_RUN_FF()  GPIO_SetBits(GPIOB , GPIO_Pin_0)      //未定义，根据实际情况定义20140626张峰
#define STEP_MOTOR_RUN_RR()  GPIO_SetBits(GPIOB , GPIO_Pin_0)      //未定义，根据实际情况定义20140626
#define STEP_MOTOR_STOP()    GPIO_ResetBits(GPIOB , GPIO_Pin_0)
#define CHECK_REST()         (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)==0&&GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)==0)
#define REST    0
#define FILL    1
#define WASH    2
#define EMPTY   3
uint8_t Step_moto_state;     //步进电机现在状态
uint8_t Step_moto_state_Expect;
uint8_t step_moto_start_time;
uint8_t step_moto_check_flag;
uint16_t step_moto_flag;
#define TASK    0
//bit0 1：步进电机有动作命令   0：无命令
//bit1  1:步进电机正在动作     0：完成动作

/*******************************************************************************
* Function Name  : Step_moto
* Description    : 步进电机动作控制
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void Step_moto_Run_FF(void)
{
    //STEP_MOTOR_START();   //启动引脚置位
    //STEP_MOTOR_RUN_FF();

}
void Step_moto_Run_RR(void)
{
    STEP_MOTOR_START();
    STEP_MOTOR_RUN_RR();
}
void Step_moto_stop(void)
{
    STEP_MOTOR_STOP();
}
/*******************************************************************************
* Function Name  : Step_moto_rest
* Description    : 步进电机复位
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Step_moto_reset(void)
{
    if(1)                     //未处于复位状态，进行复位操作
    {
        switch(Step_moto_state) //判断移动方向，移动方向最小化
        {
        case REST:
            break;
        case FILL:
            Step_moto_Run_RR();
            break;
        case WASH:
            Step_moto_Run_FF();
            break;
        case EMPTY:
            Step_moto_Run_FF();
            break;
        default:
            break;
        }
    }
    //检测是否到位，到位后停止工作
    //启动时不检测状态
    if(step_moto_start_time < 10)
        step_moto_start_time++;
    else
        step_moto_check_flag = 1;

    if(CHECK_REST() && step_moto_check_flag == 1)
    {
        Step_moto_stop();
        Step_moto_state = RESET;
        step_moto_start_time = 0; //进行下次定时复位
        Bit_clr(step_moto_flag, TASK); //进入无任务状态

        //完成本次动作
    }
}
/*******************************************************************************
* Function Name  : Step_moto_Fill
* Description    : 功能切换到进血状态
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Step_moto_Fill(void)
{
    if(1)                     //未处于复位状态，进行复位操作
    {
        switch(Step_moto_state) //判断移动方向，移动方向最小化
        {
        case REST:
            Step_moto_Run_FF();
            if(step_moto_start_time < 10)
                step_moto_start_time++;
            else
                step_moto_check_flag = 1;
            if(CHECK_STOP() && step_moto_check_flag == 1)
            {
                Step_moto_stop();
                Step_moto_state = FILL;
                step_moto_start_time = 0; //进行下次定时复位
                Bit_clr(step_moto_flag, TASK); //进入无任务状态
                //完成本次动作
            }
            break;
        case FILL:
            break;
        case WASH:
            Step_moto_Run_RR();
            if(step_moto_start_time < 10)
                step_moto_start_time++;
            else
                step_moto_check_flag = 1;
            if(CHECK_STOP() && step_moto_check_flag == 1)
            {
                Step_moto_stop();
                Step_moto_state = FILL;
                step_moto_start_time = 0; //进行下次定时复位
                Bit_clr(step_moto_flag, TASK); //进入无任务状态
                //完成本次动作
            }
            break;
        case EMPTY:              //需要先转动到RESET状态。
            Step_moto_Run_FF();
            if(step_moto_start_time < 10)
                step_moto_start_time++;
            else
                step_moto_check_flag = 1;
            if(CHECK_REST() && step_moto_check_flag == 1)
            {
                Step_moto_stop();
                Step_moto_state = RESET;
                step_moto_start_time = 0; //进行下次定时复位
            }
            break;

        }
    }

}
/*******************************************************************************
* Function Name  : Step_moto_Empty
* Description    : 功能切换到进血状态
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Step_moto_Empty(void)
{
    if(1)                     //未处于复位状态，进行复位操作
    {
        switch(Step_moto_state) //判断移动方向，移动方向最小化
        {
        case RESET:
            Step_moto_Run_RR();
            if(step_moto_start_time < 10)
                step_moto_start_time++;
            else
                step_moto_check_flag = 1;
            if(CHECK_STOP() && step_moto_check_flag == 1)
            {
                Step_moto_stop();
                Step_moto_state = EMPTY;
                step_moto_start_time = 0; //进行下次定时复位
                Bit_clr(step_moto_flag, TASK); //进入无任务状态
                //完成本次动作
            }
            break;
        case FILL:
            Step_moto_Run_RR();
            if(step_moto_start_time < 10)
                step_moto_start_time++;
            else
                step_moto_check_flag = 1;
            if(CHECK_STOP() && step_moto_check_flag == 1)
            {
                Step_moto_stop();
                Step_moto_state = RESET;
                step_moto_start_time = 0; //进行下次定时复位
            }
            break;
        case WASH:
            Step_moto_Run_FF();
            if(step_moto_start_time < 10)
                step_moto_start_time++;
            else
                step_moto_check_flag = 1;
            if(CHECK_STOP() && step_moto_check_flag == 1)
            {
                Step_moto_stop();
                Step_moto_state = EMPTY;
                step_moto_start_time = 0; //进行下次定时复位
                Bit_clr(step_moto_flag, TASK); //进入无任务状态
                //完成本次动作
            }
            break;
        case EMPTY:
            break;

        }
    }

}
/*******************************************************************************
* Function Name  : Step_moto_Wash
* Description    : 功能切换到进血状态
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Step_moto_Wash(void)
{
    if(1)                     //未处于复位状态，进行复位操作
    {
        switch(Step_moto_state) //判断移动方向，移动方向最小化
        {
        case RESET:
            Step_moto_Run_FF();
            if(step_moto_start_time < 10)
                step_moto_start_time++;
            else
                step_moto_check_flag = 1;
            if(CHECK_STOP() && step_moto_check_flag == 1)
            {
                Step_moto_stop();
                Step_moto_state = FILL;
                step_moto_start_time = 0; //进行下次定时复位

            }
            break;
        case FILL:
            Step_moto_Run_FF();
            if(step_moto_start_time < 10)
                step_moto_start_time++;
            else
                step_moto_check_flag = 1;
            if(CHECK_STOP() && step_moto_check_flag == 1)
            {
                Step_moto_stop();
                Step_moto_state = WASH;
                step_moto_start_time = 0; //进行下次定时复位
                Bit_clr(step_moto_flag, TASK); //进入无任务状态
                //完成本次动作
            }
            break;
        case WASH:

            break;
        case EMPTY:
            Step_moto_Run_RR();
            if(step_moto_start_time < 10)
                step_moto_start_time++;
            else
                step_moto_check_flag = 1;
            if(CHECK_STOP() && step_moto_check_flag == 1)
            {
                Step_moto_stop();
                Step_moto_state = WASH;
                step_moto_start_time = 0; //进行下次定时复位
                Bit_clr(step_moto_flag, TASK); //进入无任务状态
                //完成本次动作
            }
            break;

        }
    }

}
/*******************************************************************************
* Function Name  : Step_moto_control
* Description    : work state switch工作状态切换函数
* Input          : work_state
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Step_moto_control(void)
{
    if(Bit_check_1(step_moto_flag, TASK)) //有切换任务
    {
        switch(Step_moto_state_Expect)
        {
        case RESET:
            Step_moto_reset();
            break;
        case FILL:
            Step_moto_Fill();
            break;
        case WASH:
            Step_moto_Wash();
            break;
        case EMPTY:
            Step_moto_Empty();
            break;
        default:
            break;
        }
    }
}


