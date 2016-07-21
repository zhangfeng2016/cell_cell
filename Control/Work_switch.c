/*************** Radio File (c) ***************/
/**                                          **/
/**      ����STM32������ѪҺ���ջ�           **/
/**        Email:287285415@qq.com            **/
/**                                          **/
/**--------------�ļ���Ϣ--------------------**/
/**��    ��   ��: Work_control.c              **/
/**��    ��   ��: �ŷ�                       **/
/**��  ��  �� ��: 2014��7��11��              **/
/**��         ��: ����״̬������             **/
/**               �л�����Ѫͨ��             **/
/**********************************************/
#include "stm32f10x.h"
#include "..\User\user_conf.h"

//define///////////////////////////////////////////////////////////////////////
#define STEP_MOTOR_START()   GPIO_SetBits(GPIOB , GPIO_Pin_0)
#define STEP_MOTOR_RUN_FF()  GPIO_SetBits(GPIOB , GPIO_Pin_0)      //δ���壬����ʵ���������20140626�ŷ�
#define STEP_MOTOR_RUN_RR()  GPIO_SetBits(GPIOB , GPIO_Pin_0)      //δ���壬����ʵ���������20140626
#define STEP_MOTOR_STOP()    GPIO_ResetBits(GPIOB , GPIO_Pin_0)
#define CHECK_REST()         (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)==0&&GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)==0)
#define REST    0
#define FILL    1
#define WASH    2
#define EMPTY   3
uint8_t Step_moto_state;     //�����������״̬
uint8_t Step_moto_state_Expect;
uint8_t step_moto_start_time;
uint8_t step_moto_check_flag;
uint16_t step_moto_flag;
#define TASK    0
//bit0 1����������ж�������   0��������
//bit1  1:����������ڶ���     0����ɶ���

/*******************************************************************************
* Function Name  : Step_moto
* Description    : ���������������
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void Step_moto_Run_FF(void)
{
    //STEP_MOTOR_START();   //����������λ
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
* Description    : ���������λ
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Step_moto_reset(void)
{
    if(1)                     //δ���ڸ�λ״̬�����и�λ����
    {
        switch(Step_moto_state) //�ж��ƶ������ƶ�������С��
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
    //����Ƿ�λ����λ��ֹͣ����
    //����ʱ�����״̬
    if(step_moto_start_time < 10)
        step_moto_start_time++;
    else
        step_moto_check_flag = 1;

    if(CHECK_REST() && step_moto_check_flag == 1)
    {
        Step_moto_stop();
        Step_moto_state = RESET;
        step_moto_start_time = 0; //�����´ζ�ʱ��λ
        Bit_clr(step_moto_flag, TASK); //����������״̬

        //��ɱ��ζ���
    }
}
/*******************************************************************************
* Function Name  : Step_moto_Fill
* Description    : �����л�����Ѫ״̬
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Step_moto_Fill(void)
{
    if(1)                     //δ���ڸ�λ״̬�����и�λ����
    {
        switch(Step_moto_state) //�ж��ƶ������ƶ�������С��
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
                step_moto_start_time = 0; //�����´ζ�ʱ��λ
                Bit_clr(step_moto_flag, TASK); //����������״̬
                //��ɱ��ζ���
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
                step_moto_start_time = 0; //�����´ζ�ʱ��λ
                Bit_clr(step_moto_flag, TASK); //����������״̬
                //��ɱ��ζ���
            }
            break;
        case EMPTY:              //��Ҫ��ת����RESET״̬��
            Step_moto_Run_FF();
            if(step_moto_start_time < 10)
                step_moto_start_time++;
            else
                step_moto_check_flag = 1;
            if(CHECK_REST() && step_moto_check_flag == 1)
            {
                Step_moto_stop();
                Step_moto_state = RESET;
                step_moto_start_time = 0; //�����´ζ�ʱ��λ
            }
            break;

        }
    }

}
/*******************************************************************************
* Function Name  : Step_moto_Empty
* Description    : �����л�����Ѫ״̬
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Step_moto_Empty(void)
{
    if(1)                     //δ���ڸ�λ״̬�����и�λ����
    {
        switch(Step_moto_state) //�ж��ƶ������ƶ�������С��
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
                step_moto_start_time = 0; //�����´ζ�ʱ��λ
                Bit_clr(step_moto_flag, TASK); //����������״̬
                //��ɱ��ζ���
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
                step_moto_start_time = 0; //�����´ζ�ʱ��λ
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
                step_moto_start_time = 0; //�����´ζ�ʱ��λ
                Bit_clr(step_moto_flag, TASK); //����������״̬
                //��ɱ��ζ���
            }
            break;
        case EMPTY:
            break;

        }
    }

}
/*******************************************************************************
* Function Name  : Step_moto_Wash
* Description    : �����л�����Ѫ״̬
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Step_moto_Wash(void)
{
    if(1)                     //δ���ڸ�λ״̬�����и�λ����
    {
        switch(Step_moto_state) //�ж��ƶ������ƶ�������С��
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
                step_moto_start_time = 0; //�����´ζ�ʱ��λ

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
                step_moto_start_time = 0; //�����´ζ�ʱ��λ
                Bit_clr(step_moto_flag, TASK); //����������״̬
                //��ɱ��ζ���
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
                step_moto_start_time = 0; //�����´ζ�ʱ��λ
                Bit_clr(step_moto_flag, TASK); //����������״̬
                //��ɱ��ζ���
            }
            break;

        }
    }

}
/*******************************************************************************
* Function Name  : Step_moto_control
* Description    : work state switch����״̬�л�����
* Input          : work_state
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Step_moto_control(void)
{
    if(Bit_check_1(step_moto_flag, TASK)) //���л�����
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


