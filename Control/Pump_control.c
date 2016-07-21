#include "Pump_control.h"
#include "../User/user_conf.h"
void Pump_set_ARR(uint16 set_speed_mlpm); //redefine

void Yellow_Pump_Test(uint8 dir, uint16 Set_Pump_Test_speed)
{
    WASH_DISABLE();
    EMPTY_DISABLE();
    FILL_PUMP_ABLE();
    Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
    GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_3;
    TIM2->ARR = Pump_speed_change_TO_ARR(Set_Pump_Test_speed);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    if(dir == 1)
        FILL_PUMP_DIR_FF();
    else
        FILL_PUMP_DIR_RR();
    //TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    //Pump_set_ARR(Set_Pump_Test_speed);
}


void Green_Pump_Test(uint8 dir, uint16 Set_Pump_Test_speed)
{
    FILL_DISABLE();
    EMPTY_DISABLE();
    WASH_PUMP_ABLE();
    Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
    GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_7;
    Pump_set_ARR(Set_Pump_Test_speed);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}


void Red_Pump_Test(uint8 dir, uint16 Set_Pump_Test_speed)
{
    FILL_DISABLE();
    WASH_DISABLE();
    EMPTY_PUMP_ABLE();
    Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOE;
    GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_0;
    // TIM2->ARR=Pump_speed_change_TO_ARR(100);
    Pump_set_ARR(Set_Pump_Test_speed);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    if(dir == 1)
        EMPTY_PUMP_DIR_FF();
    else
        EMPTY_PUMP_DIR_RR();
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

}

void Pump_Stop_Test(void)
{
    FILL_DISABLE();
    WASH_DISABLE();
    EMPTY_DISABLE();
    //Pump_choose=WORK_EMPTY;
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
}

void Red_Pump_Alarm_check(void)
{

}
void Yellow_Pump_Alarm_check(void)
{

}
