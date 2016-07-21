/*************** Radio File (c) ***************/
/**                                          **/
/**      基于STM32开发的血液回收机           **/
/**        Email:287285415@qq.com            **/
/**                                          **/
/**--------------文件信息--------------------**/
/**文    件   名: User_dispaly.c              **/
/**创    建   人: 张峰                       **/
/**创  建  日 期: 2014年7月11日              **/
/**描         述: 界面显示函数               **/
/**                                          **/
/**********************************************/

#include "stm32f10x.h"
#include "ADC.h"
#include "..\User\user_conf.h"
void AT24CXX_Writeword(u16 DataToWrite, u16 WriteAddr);


typedef struct
{
    uint32 Prees_AD;
    uint32 Green_AD;
    uint16 Red_AD;
    uint32 Reservoir_weigh_AD;
    uint32 Dwon_weigh_AD;
    uint16 Blood_AD;

} Monitor_AD_last_type;

Monitor_AD_last_type Monitor_AD_last;




Monitor_Pump_type Monitor_Pump;



fp32 tr;
fp32 test_1;
uint8  AD_Loop_num;
uint16 Air_check_conunt;
uint8  Air_flag;
uint8  Blood_check_num;
uint16 Fill_air_time;
uint16 air_time;
uint8  Alarm_Sound_flag = 1;
uint16 Alarm_loop_time = 0;
uint8 Reservoir_able_cont;

uint8 PWOER_DWON_FLAG;
uint8 Air_time;
uint8 No_Air_time;

uint16 Monitor_Red_NO_Water;
uint16 Monitor_Green_NO_Water;


uint16 monitor_reservoir_ml_1s_av;
uint8 Reservoir_ml_Samping_time = 0;
#define SAMPING_200MS    5
uint16 monitor_reservoir_ml_temp;
uint16 monitor_reservoir_ml_buf_1s[20];
uint8 monitor_reservoir_cont;
uint8 Well_check_time_cont;
/***********************************************/
/* 函数名称: Adc_Init                          */
/* 功能描述: ADC初始化函数                     */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：2014年9月11日                      */
/***********************************************/
void Adc_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1 , ENABLE);


    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    ADC_DeInit(ADC1);


    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

/***********************************************/
/* 函数名称: ADResult                          */
/* 功能描述: AD转换启动                        */
/* 输  　入: 转换通道                          */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年09月15日                      */
/***********************************************/
uint16 ADResult(uint8 ch)
{

    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_28Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    return(ADC_GetConversionValue(ADC1));

}

/***********************************************/
/* 函数名称: AD_Loop                           */
/* 功能描述: AD循环检测                        */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void AD_Loop(void)       //该函数每5ms执行一次，所有监测参数都更新一次需要50ms 采用平均加权滤波算法
{

    uint32 Filter_Coefficient = 16;
    switch(AD_Loop_num)
    {
    case 0:
        //		     Monitor_Prees_AD=(Monitor_AD_last.Prees_AD+ ADResult(PRESS_CHK))/(Filter_Coefficient+1);
        //		     Monitor_AD_last.Prees_AD=Monitor_Prees_AD*Filter_Coefficient;
        //	        Monitor_P12_AD=ADResult(PRESS_CHK);
        //          if(Monitor_P12_AD<3000)
        //					{
        //						PWOER_DWON_FLAG=1;
        //					}
        //					if(PWOER_DWON_FLAG==1)
        //					{
        //						if(Monitor_P12_AD>2000)
        //						{
        //							PWOER_DWON_FLAG=0;
        //							PWOER_ON_FLAG=1;
        //							show_pic_flag=0;//显示一次整张图片
        //							//Show_pic_num=26;
        //						  Display_Loop=0;
        //						  show_pic_flag=0;
        //
        //						}
        //					}
        AD_Loop_num = 1;
        break;
    case 1:
        Monitor_Green_AD = ADResult(GREENLED);
        AD_Loop_num = 2;
        break;
    case 2:
        Monitor_Air_AD = ADResult(AIRCHECK);
        AD_Loop_num = 3;
        break;
    case 3:
        Monitor_Reservoir_weigh = ADResult(WEIGHTUP);
        Monitor_Reservoir_weigh = (Monitor_AD_last.Reservoir_weigh_AD + ADResult(WEIGHTUP)) / (Filter_Coefficient + 1);
        Monitor_AD_last.Reservoir_weigh_AD = Monitor_Reservoir_weigh * Filter_Coefficient;
        AD_Loop_num = 4;
        break;
    case 4:
        Monitor_Red_AD = ADResult(REDLED);
        AD_Loop_num = 5;
        break;
    case 5:
        Monitor_Blood_AD = ADResult(BLOODCHECK);
        Monitor_Blood_AD = (Monitor_AD_last.Blood_AD + ADResult(BLOODCHECK)) / (Filter_Coefficient + 1);
        Monitor_AD_last.Blood_AD = Monitor_Blood_AD * Filter_Coefficient;
        AD_Loop_num = 6;
        break;
    case 6:
        Monitor_Dwon_weigh = (Monitor_AD_last.Dwon_weigh_AD + ADResult(WEIGHTDOWN)) / (Filter_Coefficient + 1);
        Monitor_AD_last.Dwon_weigh_AD = Monitor_Dwon_weigh * Filter_Coefficient;
        AD_Loop_num = 0;
        break;
    default:
        AD_Loop_num = 0;
        break;
    }
}

/***********************************************/
/* 函数名称: Cleaner_check                     */
/* 功能描述: 清洗清洁度检测函数                */
/* 输  　入: 无                                */
/* 输  　出:                                   */
/* 全局变量: 无                                */
/* 调用模块:                                   */
/* 修改时间：14年12月26日                      */
/***********************************************/
void Cleaner_check(void)
{

    Red_Tend[0] = Red_Tend[1];
    Red_Tend[1] = Red_Tend[2];
    Red_Tend[2] = Red_Tend[3];
    Red_Tend[3] = Red_Tend[4];
    Red_Tend[4] = Monitor_Red_AD;

}
/***********************************************/
/* 函数名称: Cleaner_check                     */
/* 功能描述: 清洗清洁度检测函数                */
/* 输  　入: 无                                */
/* 输  　出:                                   */
/* 全局变量: 无                                */
/* 调用模块:                                   */
/* 修改时间：14年12月26日                      */
/***********************************************/
void Auto_Bwol_check(void)
{
    if(work_sta == WORK_PRE_FILL)
    {
        if(Bit_check_1(Auto_check_bowl_falg, 1))
        {
            Monitor_Red_NO_Water = Monitor_Red_AD;
            Monitor_Green_NO_Water = Monitor_Green_AD;
            Bit_set(Auto_check_bowl_falg, 2);
            Bit_clr(Auto_check_bowl_falg, 1);
        }
        if(Monitor_Pre_fill_Volume > 80)
        {
            if(Bit_check_1(Auto_check_bowl_falg, 2))
            {
                if(Monitor_Green_AD < (Monitor_Green_NO_Water / 2)) //离心杯已经充满
                {
                    if(Monitor_Pre_fill_Volume < 200)
                    {
                        Set_Bowl	= 125;
                        Pre_fill_Volume = 200;
                        Set_Run_Volume = 600;

                    }
                    else
                    {
                        Set_Bowl	= 225;
                        Pre_fill_Volume = 300;
                        Set_Run_Volume = 800;
                    }
                }

            }
        }
    }


}
/***********************************************/
/* 函数名称: Air_check                   */
/* 功能描述: 气泡检测函数                     */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void Air_check(void)
{
#define NO_AIR   0
#define HAVE_AIR 1
#define AIR_CONTIUNE_TIME      10
#define NO_AIR_CONTIUNE_TIME   10


    if(Air_flag == HAVE_AIR) //air_now
    {
        if(Monitor_Air_AD > Air_check_gate)
        {
            Air_time++;
            if(Air_time > NO_AIR_CONTIUNE_TIME)
            {
                Air_flag = NO_AIR;
                Air_time = 0;
                No_Air_time = 0;
            }
        }
        else
        {
            Air_time = 0;
            No_Air_time = 0;
        }
    }
    else if(Air_flag == NO_AIR)
    {
        if(Monitor_Air_AD < Air_check_gate)
        {
            No_Air_time++;
            if(No_Air_time > NO_AIR_CONTIUNE_TIME)
            {
                Air_flag = HAVE_AIR;
                Air_time = 0;
                No_Air_time = 0;
            }
        }
        else
        {
            Air_time = 0;
            No_Air_time = 0;
        }
    }

    if(Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW)) //  气泡报警开
    {
        if(Air_flag == HAVE_AIR)
        {
            Air_check_conunt++;
            if(Air_check_conunt > 30)
            {
                switch(work_sta)
                {
                case WORK_FILL:
                    //自动模式上添加充血控制，检测到气泡进行停机等待
                    if(Monitor_Pump.Fill_Pump_count < 120)  //37.5ml
                    {
                        air_time = 0;
                    }
                    else if(Monitor_Pump.Fill_Pump_count < 200) // 50ml
                    {
                        if(air_time > 200)
                        {
                            Bit_set(Alarm_sta, FILL_AIR_ALARM);
                            if(Mode != MODE_AUTO)
                                Pump_Pause = 1;
                        }
                        else
                            air_time++;
                    }
                    else
                    {
                        if(air_time > 300)
                        {
                            Bit_set(Alarm_sta, FILL_AIR_ALARM);
                            if(Mode != MODE_AUTO)
                                Pump_Pause = 1;
                        }
                        else
                            air_time++;
                    }
                    break;
                case WORK_WASH:
                    if(Monitor_Pump.Wash_Pump_count < 80)  //37.5ml
                    {
                        air_time = 0;;
                    }
                    else
                    {
                        if(Pump_Sta != PUMP_AFTER)
                        {
                            if(air_time > 200)
                            {
                                if(Bit_check_1(Pump_state_flag, WASH_OK) )
                                {
                                    Bit_clr(Alarm_sta, WASH_AIR_ALARM);
                                    Pump_Pause = 1;
                                }
                                else
                                {
                                    Bit_set(Alarm_sta, WASH_AIR_ALARM);
                                    Pump_Pause = 1;
                                }
                            }
                            else
                                air_time++;
                        }
                        else
                        {
                            Bit_clr(Alarm_sta, WASH_AIR_ALARM);
                        }
                    }
                    break;
                case WORK_EMPTY:
                    if(Bit_check_1(Pump_state_flag, EMPTY_OK))
                    {
                        if(Monitor_Pump.Empty_Pump_count > 180)
                        {
                            Bit_clr(Pump_state_flag, FILL_OK);
                            Bit_clr(Pump_state_flag, WASH_OK);
                            Bit_clr(Pump_state_flag, CONC_OK);
                            Auto_run_step = 0;
                            Pump_Sta = PUMP_STOP;
                            Auto_refill_wait_flag = 0;
                            Auto_run_step_cont = 0;
                        }
                    }
                    else//正常情况
                    {
                        if(Monitor_Pump.Empty_Pump_count < 80)  //18ml
                        {
                            air_time = 0;;
                        }
                        else if(Monitor_Pump.Empty_Pump_count < 180) // 50ml
                        {
                            if(air_time > 100)
                            {
                                Bit_set(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
                                Pump_Pause = 1;
                            }
                            else
                                air_time++;
                        }
                        else
                        {
                            if(Monitor_Pump.Empty_Pump_count * 375 / 1000 > (Set_Bowl - 40))
                            {
                                if(air_time > 50)
                                {
                                    //Bit_set(Alarm_sta,EMPTY_AIR_ALARM);
                                    //Standby();
                                    Bit_clr(Pump_state_flag, FILL_OK);
                                    Bit_clr(Pump_state_flag, WASH_OK);
                                    Bit_clr(Pump_state_flag, CONC_OK);
                                    Auto_run_step = 0;
                                    Pump_Sta = PUMP_STOP;
                                    Auto_refill_wait_flag = 0;
                                    Auto_run_step_cont = 0;

                                }
                                else
                                    air_time++;
                            }
                            else
                            {
                                if(air_time > 50)
                                {
                                    Bit_set(Alarm_sta, EMPTY_AIR_ALARM);
                                    Pump_Pause = 1;
                                }
                                else
                                    air_time++;
                            }
                        }
                    }

                    break;
                case WORK_RETURN:
                    if(Monitor_Pump.Fill_Pump_count < 100)  //37.5ml
                    {
                        air_time = 0;;
                    }
                    else
                    {
                        if(air_time > 500)
                        {
                            // Bit_set(Alarm_sta,AIR_ALARM);
                            work_sta = WORK_STANDBY;
                            Bit_set(Pump_state_flag, RETURN_OK);
                            // Pump_Pause=1;
                        }
                        else
                            air_time++;
                    }
                    break;
                case WORK_CONC:
                    if(Monitor_Pump.Empty_Pump_count < 200)  //37.5ml
                    {
                        air_time = 0;;
                    }
                    else
                    {
                        if(air_time > 200)
                        {
                            Bit_set(Alarm_sta, AIR_ALARM);
                            Pump_Pause = 1;
                        }
                        else
                            air_time++;
                    }
                    break;
                case WORK_PRE_FILL:
                    if(Monitor_Pump.Wash_Pump_count < 80)  //37.5ml
                    {
                        air_time = 0;;
                    }
                    else
                    {
                        if(air_time > 300)
                        {
                            Bit_set(Alarm_sta, PRE_FILL_ALARM);
                            Pump_Pause = 1;
                        }
                        else
                            air_time++;
                    }
                    break;
                case WORK_JINJI:
                    air_time = 0;
                    Bit_clr(Alarm_sta, AIR_ALARM);
                    Bit_clr(Alarm_sta, FILL_AIR_ALARM);
                    Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
                    Bit_clr(Alarm_sta, WASH_AIR_ALARM);
                    Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
                    Bit_clr(Alarm_sta, PRE_FILL_ALARM);
                    break;
                case WORK_STANDBY:
                    air_time = 0;
                    Bit_clr(Alarm_sta, AIR_ALARM);
                    Bit_clr(Alarm_sta, FILL_AIR_ALARM);
                    Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
                    Bit_clr(Alarm_sta, WASH_AIR_ALARM);
                    Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
                    Bit_clr(Alarm_sta, PRE_FILL_ALARM);
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            Air_check_conunt = 0;
            Fill_air_time = 0;
            air_time = 0;
            Bit_clr(Alarm_sta, AIR_ALARM);
            Bit_clr(Alarm_sta, FILL_AIR_ALARM);
            Bit_clr(Alarm_sta, WASH_AIR_ALARM);
            Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
            Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
        }
    }
    else //气泡报警关
    {
        Air_check_conunt = 0;
        Fill_air_time = 0;
        air_time = 0;
        Bit_clr(Alarm_sta, AIR_ALARM);
        Bit_clr(Alarm_sta, FILL_AIR_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
        Bit_clr(Alarm_sta, WASH_AIR_ALARM);
    }
}

/***********************************************/
/* 函数名称: Weight_check                   */
/* 功能描述: 称重计算函数                    */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void Weight_check(void)
{
    //uint16 monitor_reservoir_ml;
#define RES_MAX    4095
#define RES_MIN    1963
    uint16 temp;
    if(Monitor_Reservoir_weigh == 0)
    {
        Reservoir_able_cont++;
        //Delay(20);
        if(Reservoir_able_cont > 20)
        {
            if(Monitor_Reservoir_weigh == 0)
                reservoir_state = 0;
            else
                reservoir_state = 1;
        }
    }
    else
    {
        reservoir_state = 1;
    }
    if(Monitor_Reservoir_weigh > Reservoir_zero) //2221  500   // 2518   1000   //2820     1500
    {
        monitor_reservoir_ml = (Monitor_Reservoir_weigh - Reservoir_zero) * 500 / (Reservoir_500 - Reservoir_zero); //600/(RES_MAX-RES_MIN);
    }
    else
        monitor_reservoir_ml = 0;

    if(Reservoir_ml_Samping_time >	SAMPING_200MS)
    {
        Reservoir_ml_Samping_time = 0;
        if(monitor_reservoir_cont < 5)
        {
            monitor_reservoir_ml_temp += monitor_reservoir_ml;
            monitor_reservoir_cont++;

        }
        else
        {
            monitor_reservoir_ml_buf_1s[0] = monitor_reservoir_ml_buf_1s[1];
            monitor_reservoir_ml_buf_1s[1] = monitor_reservoir_ml_buf_1s[2];
            monitor_reservoir_ml_buf_1s[2] = monitor_reservoir_ml_buf_1s[3];
            monitor_reservoir_ml_buf_1s[3] = monitor_reservoir_ml_buf_1s[4];
            monitor_reservoir_ml_buf_1s[4] = monitor_reservoir_ml_buf_1s[5];
            monitor_reservoir_ml_buf_1s[5] = monitor_reservoir_ml_buf_1s[6];
            monitor_reservoir_ml_buf_1s[6] = monitor_reservoir_ml_buf_1s[7];
            monitor_reservoir_ml_buf_1s[7] = monitor_reservoir_ml_buf_1s[8];
            monitor_reservoir_ml_buf_1s[8] = monitor_reservoir_ml_buf_1s[9];
            monitor_reservoir_ml_buf_1s[9] = monitor_reservoir_ml_buf_1s[10];
            monitor_reservoir_ml_buf_1s[10] = monitor_reservoir_ml_buf_1s[11];
            monitor_reservoir_ml_buf_1s[11] = monitor_reservoir_ml_buf_1s[12];
            monitor_reservoir_ml_buf_1s[12] = monitor_reservoir_ml_buf_1s[13];
            monitor_reservoir_ml_buf_1s[13] = monitor_reservoir_ml_buf_1s[14];
            monitor_reservoir_ml_buf_1s[14] = monitor_reservoir_ml_buf_1s[15];
            monitor_reservoir_ml_buf_1s[15] = monitor_reservoir_ml_buf_1s[16];
            monitor_reservoir_ml_buf_1s[16] = monitor_reservoir_ml_buf_1s[17];
            monitor_reservoir_ml_buf_1s[17] = monitor_reservoir_ml_buf_1s[18];
            monitor_reservoir_ml_buf_1s[18] = monitor_reservoir_ml_buf_1s[19];
            monitor_reservoir_ml_buf_1s[19] = monitor_reservoir_ml_temp / 5;
            monitor_reservoir_ml_temp = 0;
            monitor_reservoir_ml_temp += monitor_reservoir_ml;
            reservoir_delay_check_cont++;
            monitor_reservoir_cont = 1;
        }
    }
    else
    {
        Reservoir_ml_Samping_time++;
    }



    if(Monitor_Dwon_weigh > Dwon_weigh_7000) //7000
    {
        temp = (Dwon_weigh_9000 - Dwon_weigh_7000) / 20; //100mlAD
        temp = (Monitor_Dwon_weigh - Dwon_weigh_7000) / temp;
        monitor_weast_ml = 7000 + temp * 100;
        //monitor_weast_ml=(Monitor_Dwon_weigh-Dwon_weigh_7000)*350/100;//600/(RES_MAX-RES_MIN);
    }
    else
    {
        temp = (Dwon_weigh_9000 - Dwon_weigh_7000) / 20; //100mlAD
        temp = (Dwon_weigh_7000 - Monitor_Dwon_weigh) / temp;
        if(7000 > (temp * 100))
            monitor_weast_ml = 7000 - temp * 100;
        else
            monitor_weast_ml = 0;
    }
    //	monitor_weast_ml=0;
    if(Set_feiye == 9500)
    {
        Bit_clr(Alarm_sta, FEIYE_ALARM);
    }
    else
    {
        if((sta == WORK))
        {
            if(monitor_weast_ml > Set_feiye)
                Bit_set(Alarm_sta, FEIYE_ALARM);
            else
                Bit_clr(Alarm_sta, FEIYE_ALARM);
        }
        else
        {
            Bit_clr(Alarm_sta, FEIYE_ALARM);
        }

    }

}
/***********************************************/
/* 函数名称: Pump_speed_check                   */
/* 功能描述:  泵速测定函数                     */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void Pump_speed_check(void)       //该函数每5ms执行一次，所有监测参数都更新一次需要50ms
{

    Air_check();
    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1) != Monitor_Fill_buf_Flag)
    {
        Monitor_Pump.Fill_Pump_count++;

        if(sta == TEST_STA)
        {
            Monitor_Fill_count++;
        }
        else
        {
            if(Set_Arlarm_ON_OFF == 1) //  气泡报警开
            {
                if(Air_flag == NO_AIR)
                {
                    if(work_sta == WORK_FILL)
                    {
                        Monitor_Fill_count++;
                        Monitor_Bowl_Fill_Count++;
                    }
                    else if(work_sta == WORK_RETURN)
                    {
                        if(Monitor_Fill_count > 0)
                            Monitor_Fill_count--;
                    }
                }
            }
            else
            {
                if(work_sta == WORK_FILL)
                {
                    Monitor_Fill_count++;
                    Monitor_Bowl_Fill_Count++;
                }
                else if(work_sta == WORK_RETURN)
                {
                    if(Monitor_Fill_count > 0)
                        Monitor_Fill_count--;
                }
            }
        }
        Monitor_Fill_buf_Flag = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
    }
    ///////////////////////////////////////////////////////////////////////////
    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) != Monitor_Wash_buf_Flag)
    {

        Monitor_Pump.Wash_Pump_count++;
        if(sta == TEST_STA)
        {
            Monitor_Wash_count++;
        }
        else
        {
            if(Set_Arlarm_ON_OFF == 1) //  气泡报警开
            {
                if(Air_flag == NO_AIR)
                {
                    if(work_sta == WORK_WASH)
                    {
                        Monitor_Wash_count++;
                        Monitor_Bowl_Wash_Count++;
                    }
                    else if(work_sta == WORK_PRE_FILL)
                    {
                        Monitor_Pre_fill_count++;
                    }
                }
            }
            else
            {
                if(work_sta == WORK_WASH)
                {
                    Monitor_Wash_count++;
                    Monitor_Bowl_Wash_Count++;
                }
                else if(work_sta == WORK_PRE_FILL)
                {
                    Monitor_Pre_fill_count++;
                }
            }
        }
        Monitor_Wash_buf_Flag = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6);
    }

    //////////////////////////////////////////////////////////////////////////
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) != Monitor_Empty_buf_Flag)
    {
        Monitor_Pump.Empty_Pump_count++;

        if(sta == TEST_STA)
        {
            Monitor_Empty_count++;
        }
        else
        {
            if(Set_Arlarm_ON_OFF == 1) //  气泡报警开
            {
                if(Air_flag == NO_AIR)
                {
                    if(work_sta == WORK_EMPTY)
                    {
                        Monitor_Empty_count++;
                        Monitor_Bowl_Empty_Count++;
                        Monitor_Empty_Force_Count++;
                    }
                    else if(work_sta == WORK_CONC)
                    {
                        if(Monitor_Empty_Volume > 0)
                            Monitor_Empty_count--;
                    }
                }
            }
            else
            {
                if(work_sta == WORK_WASH && Pump_Sta == PUMP_AFTER)
                {
                    Monitor_Aujst_count++;
                }
                if(work_sta == WORK_EMPTY)
                {
                    Monitor_Empty_count++;
                    Monitor_Bowl_Empty_Count++;
                    Monitor_Empty_Force_Count++;
                }
                else if(work_sta == WORK_CONC)
                {
                    if(Monitor_Empty_Volume > 0)
                        Monitor_Empty_count--;
                }
            }
        }
        Monitor_Empty_buf_Flag = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
    }
}
/***********************************************/
/* 函数名称: Pump_alarm_check                   */
/* 功能描述: 蠕动泵报警检测函数                    */
/* 输  　入: 无                                */
/* 输  　出:  报警内容                   */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：16年02月24日                      */
/***********************************************/
void Pump_alarm_check(void)       //该函数每5ms执行一次，所有监测参数都更新一次需要50ms
{
    static uint16 Err_red_pump_cont;
    static uint8  Err_red_pump_restart_falg;
    static uint8  Err_red_pump_state;
//#define ERR_PUMP_BEGEIN            0
//#define ERR_PUMP_RESTART           1
//#define ERR_PUMP_OK                 2
    if(Default_Pump_set == PEMP_CHECK_DISABLE)
    {
			if(sta== WORK)
			{
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0) //进血泵
        {
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            Pump_Pause = 1;
            Bit_set(Alarm_sta, FILL_PUMP_ALARM);
        }
        else
        {

        }
        if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == 0)
        {
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            Pump_Pause = 1;
            Bit_set(Alarm_sta, WASH_PUMP_ALARM);
        }

        if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1) == 0)
        {
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            Pump_Pause = 1;
            Bit_set(Alarm_sta, EMPTY_PUMP_ALARM);

        }
			}
    }
		//#undef
}
/***********************************************/
/* 函数名称: Pump_speed_check                   */
/* 功能描述:  泵速测定函数                     */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void Pump_speed_check_inside(void)       //该函数每5ms执行一次，所有监测参数都更新一次需要50ms
{
    Air_check();
    switch(work_sta)
    {
    case WORK_FILL:
    case WORK_RETURN:
        if(Pump_puls_count > 3200) //添加到计数，并清零脉冲计数
        {
            Pump_puls_count = Pump_puls_count - 3200;
            Monitor_Pump.Fill_Pump_count += 2;
            if(Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW)) //  气泡报警开
            {
                if(Air_flag == NO_AIR)
                {
                    Monitor_Fill_count += 2;
                    Monitor_Bowl_Fill_Count += 2;
                    //			         if(work_sta==WORK_FILL)
                    //					     {
                    //						     Monitor_Fill_count+=2;
                    //						     Monitor_Bowl_Fill_Count+=2;
                    //					     }
                    //			         else if(work_sta==WORK_RETURN)
                    //			         {
                    //			  	       if(Monitor_Fill_count>0)
                    //			  	         Monitor_Fill_count-=2;
                    //			         }
                }
            }
            else
            {
                Monitor_Fill_count += 2;
                Monitor_Bowl_Fill_Count += 2;
                //			       if(work_sta==WORK_FILL)
                //				     {
                //			          Monitor_Fill_count+=2;
                //					      Monitor_Bowl_Fill_Count+=2;
                //				     }
                //			       else if(work_sta==WORK_RETURN)
                //			       {
                //			  	     if(Monitor_Fill_count>2)
                //			  	       Monitor_Fill_count-=2;
                //			       }
            }
        }

        break;
    case WORK_WASH:
    case WORK_PRE_FILL:
        if(Pump_puls_count > 6400) //添加到计数，并清零脉冲计数
        {
            Pump_puls_count = Pump_puls_count - 6400;
            Monitor_Pump.Wash_Pump_count += 4;
            if(Pump_Sta == PUMP_AFTER)
            {
                Monitor_Pump.Fill_Pump_count += 4;
            }
            if(Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW)) //  气泡报警开
            {
                if(Air_flag == NO_AIR)
                {
                    if(work_sta == WORK_WASH)
                    {
                        Monitor_Wash_count += 4;
                        Monitor_Bowl_Wash_Count += 4;
                        if(Pump_Sta == PUMP_AFTER)
                        {
                            Monitor_Pump.Fill_Pump_count += 4;
                        }
                    }
                    else if(work_sta == WORK_PRE_FILL)
                    {
                        Monitor_Pre_fill_count += 4;
                    }
                }
            }
            else
            {
                if(work_sta == WORK_WASH)
                {
                    Monitor_Wash_count += 4;
                    Monitor_Bowl_Wash_Count += 4;
                    if(Pump_Sta == PUMP_AFTER)
                    {
                        Monitor_Pump.Fill_Pump_count += 4;
                    }
                }
                else if(work_sta == WORK_PRE_FILL)
                {
                    Monitor_Pre_fill_count += 4;
                }
            }
        }
        break;
    case WORK_EMPTY:
    case WORK_CONC:
        if(Pump_puls_count > 6400) //添加到计数，并清零脉冲计数
        {
            Pump_puls_count = Pump_puls_count - 6400;
            Monitor_Pump.Empty_Pump_count += 4;
            if(Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW)) //  气泡报警开
            {
                if(Air_flag == NO_AIR)
                {
                    Monitor_Empty_count += 4;
                    Monitor_Bowl_Empty_Count += 4;
                    Monitor_Empty_Force_Count += 4;
                }
            }
            else
            {
                if(work_sta == WORK_WASH && Pump_Sta == PUMP_AFTER)
                {
                    Monitor_Aujst_count += 4;
                }
                Monitor_Empty_count += 4;
                Monitor_Bowl_Empty_Count += 4;
                Monitor_Empty_Force_Count += 4;
            }
        }
        break;
    case WORK_JINJI:
        if(Pump_puls_count > 3200) //添加到计数，并清零脉冲计数
        {
            Pump_puls_count = Pump_puls_count - 3200;
            Monitor_Empty_count += 2;
            Monitor_Fill_count += 2;
        }

        break;
    default:
        break;
    }
}
/***********************************************/
/* 函数名称: Pump_Volume_check                 */
/* 功能描述:                                   */
/* 输  　入: 无                                */
/* 输  　出: 流量计算                          */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void Pump_Volume_check(void)       //该函数每5ms执行一次，所有监测参数都更新一次需要50ms
{
    //uint16 Monitor_Waste_Volume;
    fp32 te = 0.375;
    if(work_sta == WORK_RETURN)
    {
        if(Monitor_Fill_Volume > (fp32) Monitor_Fill_count * 0.375f * fill_pump_adjust / 100)
            Monitor_Fill_Volume = Monitor_Fill_Volume - (fp32) Monitor_Fill_count * 0.375f * fill_pump_adjust / 100;
        else
            Monitor_Fill_Volume = 0;
    }
    else if(work_sta == WORK_FILL)
        Monitor_Fill_Volume = Monitor_Fill_Volume + (fp32) Monitor_Fill_count * 0.375f * fill_pump_adjust / 100;
    else if(work_sta == WORK_JINJI)
        Monitor_Fill_Volume = Monitor_Fill_Volume + (fp32) Monitor_Fill_count * 0.375f * fill_pump_adjust / 100;
    Monitor_Wash_Volume  = Monitor_Wash_Volume + (fp32) Monitor_Wash_count * 0.375f * wash_pump_adjust / 100;
    if(work_sta == WORK_CONC)
    {
        if(Set_Bowl == 125)
        {
            if(Monitor_Empty_Volume > (fp32) Monitor_Empty_count * 0.375 * empty_pump_adjust_125 / 100)
                Monitor_Empty_Volume  = Monitor_Empty_Volume - (fp32) Monitor_Empty_count * 0.375 * empty_pump_adjust_125 / 100;
            else
                Monitor_Empty_Volume = 0;
        }
        else
        {
            if(Monitor_Empty_Volume > (fp32) Monitor_Empty_count * 0.375 * empty_pump_adjust_225 / 100)
                Monitor_Empty_Volume  = Monitor_Empty_Volume - (fp32) Monitor_Empty_count * 0.375 * empty_pump_adjust_225 / 100;
            else
                Monitor_Empty_Volume = 0;
        }
    }
    else if(work_sta == WORK_EMPTY)
    {
        if(Set_Bowl == 125)
            Monitor_Empty_Volume  = Monitor_Empty_Volume + (fp32) Monitor_Empty_count * 0.375 * empty_pump_adjust_125 / 100;
        else
            Monitor_Empty_Volume  = Monitor_Empty_Volume + (fp32) Monitor_Empty_count * 0.375 * empty_pump_adjust_225 / 100;
    }

    Monitor_Pre_fill_Volume = Monitor_Pre_fill_Volume + (fp32) Monitor_Pre_fill_count * 0.375f * fill_pump_adjust / 100;

    Monitor_Wash_count = 0;
    Monitor_Empty_count = 0;
    Monitor_Fill_count = 0;
    Monitor_Pre_fill_count = 0;


    Monitor_Bowl_Fill_Volume = Monitor_Bowl_Fill_Volume + (fp32) Monitor_Bowl_Fill_Count * 0.375f * fill_pump_adjust / 100;
    Monitor_Bowl_Wash_Volume = Monitor_Bowl_Wash_Volume + (fp32) Monitor_Bowl_Wash_Count * 0.375f * wash_pump_adjust / 100;
    if(Set_Bowl == 125)
        Monitor_Empty_Volume  = Monitor_Empty_Volume + (fp32) Monitor_Empty_count * 0.375f * empty_pump_adjust_125 / 100;
    else
        Monitor_Empty_Volume  = Monitor_Empty_Volume + (fp32) Monitor_Empty_count * 0.375f * empty_pump_adjust_225 / 100;
    Monitor_Bowl_Fill_Count = 0;
    Monitor_Bowl_Wash_Count = 0;
    Monitor_Bowl_Empty_Count = 0;
    //	Monitor_Bowl_Fill_Volume=Monitor_Bowl_Fill_Count * fill_pump_adjust/1000;
    //	Monitor_Bowl_Wash_Volume=Monitor_Bowl_Wash_Count*wash_pump_adjust/1000;
    //	Monitor_Bowl_Empty_Volume=Monitor_Bowl_Empty_Count*empty_pump_adjust/1000;

    //Monitor_Aujst_count
    //Monitor_Bowl_Empty_Count
}
/***********************************************/
/* 函数名称:                        */
/* 功能描述: 漏液传感器检测                    */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void  Fluid(void)
{
    if(Bit_check_1(Set_Alarm_flag, LOUYE_ALARM_SW)) //井盖报警开
    {
        if(sta == WORK)
        {
            if(READ_FUILT_UP_STATE() == 1)
            {
                Bit_set(Alarm_sta, louye_1_ALARM);
            }
            else
            {
                Bit_clr(Alarm_sta, louye_1_ALARM);
            }
            if(READ_FUILT_DWON_STATE() == 1)
            {
                Bit_set(Alarm_sta, louye_2_ALARM);
            }
            else
            {
                Bit_clr(Alarm_sta, louye_2_ALARM);
            }
        }

    }
    else
    {
        Bit_clr(Alarm_sta, louye_2_ALARM);
        Bit_clr(Alarm_sta, louye_1_ALARM);
    }
}
/***********************************************/
/* 函数名称: 井盖传感器                     */
/* 功能描述: 漏液传感器检测                    */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void Well_state_Check(void)             //增加离心井盖软件防抖
{
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 1) //检测井盖
    {
        Well_state_flag = 1; //井盖开启

    }
    else
    {
        Well_state_flag = 0; //井盖关闭
        Bit_clr(Alarm_sta, WELL_ALARM);
    }
}
/***********************************************/
/* 函数名称: 井盖传感器                     */
/* 功能描述: 漏液传感器检测                    */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void Well_Check(void)
{
    if(Bit_check_1(Set_Alarm_flag, WELL_ALARM_SW)) //井盖报警开
    {
        if((work_sta == WORK_PRE_FILL) || (work_sta == WORK_FILL) || (work_sta == WORK_WASH) || (work_sta == WORK_CONC) || (Auto_refill_wait_flag == 1))
        {
            if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 1) //检测井盖
            {
                Well_check_time_cont++;
                if(Well_check_time_cont > 20)
                {
                    Well_check_time_cont = 0;
                    Bit_set(Alarm_sta, WELL_ALARM);
                    WASH_DISABLE();
                    FILL_DISABLE();
                    EMPTY_DISABLE();
                    //if(mode==MODE_AUTO)
                    Work_Stop_Flag = 1;
                    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
                    if(Bit_check_1(Bowl_State, FILL_BLOOD))  //设置蠕动泵延时标志
                    {
                        Bit_set(Bowl_State, 1);               //再次启动时需要延时蠕动泵
                        Bit_clr(Bowl_State, 2);               //清除正在延时标志位
                        pump_dely_time = 50;
                    }
                }
            }
            else
            {
                Well_check_time_cont = 0;
                //Bit_clr(Alarm_sta,WELL_ALARM);
            }
        }
        else
        {
            Well_check_time_cont = 0;
            Bit_clr(Alarm_sta, WELL_ALARM);
        }
    }
    else
    {
        Well_check_time_cont = 0;
        Bit_clr(Alarm_sta, WELL_ALARM);
    }

}


/***********************************************/
/* 函数名称: 血层传感器                     */
/* 功能描述: 漏液传感器检测                    */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void Blood_Check(void)
{
	
    if(Bit_check_1(Set_Alarm_flag, BLOOD_ALARM_SW)) //血层检测报警开
    {
        if((work_sta == WORK_FILL) || (work_sta == WORK_CONC))
        {
            if(Monitor_Blood_AD > Cell_check_gate)
            {
                //if( pump_dely_time>1)
                {
                    Blood_check_num++;
                    if(Blood_check_num == 50)
                    {
                        Cell_line = TURE;
                        if(Mode == MODE_MU)
                            Bit_set(Alarm_sta, CELL_ALARM);
                        else
                        {
                            Bit_clr(Alarm_sta, CELL_ALARM);
                        }
                    }
                }
            }
            else if (Monitor_Blood_AD > Cell_check_gate) //血层值在250-1300时判断红绿灯
            {
                //		 if(Monitor_Green_AD<300)
                //		 {
                ////			 Cell_line=1;
                ////			 if(mode==MODE_MU) //手动状态下，血层报警。
                ////			  Bit_set(Alarm_sta,CELL_ALARM);
                ////			 else
                ////			 {
                ////				Bit_clr(Alarm_sta,CELL_ALARM);
                //			 }
                //
                //		 }
                //		 else
                //		{
                //		  Blood_check_num=0;
                //		  Cell_line=0;
                //		  Bit_clr(Alarm_sta,CELL_ALARM);
                //		}
            }
            else
            {
                Blood_check_num = 0;
                Cell_line = 0;
                Bit_clr(Alarm_sta, CELL_ALARM);
            }
        }
        else
        {
            Bit_clr(Alarm_sta, CELL_ALARM);
            Cell_line = 0;
        }
    }
    else
    {
        Bit_clr(Alarm_sta, CELL_ALARM);
        Cell_line = 0;
    }
}
/***********************************************/
/* 函数名称: Sensor_Update                     */
/* 功能描述: 传感器数据更新                    */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void Sensor_Update(void)       //该函数每5ms执行一次，所有监测参数都更新一次需要50ms
{
    AD_Loop();
    if(sta == TEST_STA)
    {
        Pump_speed_check();
    }
    else
    {
        Pump_speed_check_inside();

    }
    Pump_Volume_check();
    //Auto_Bwol_check();
    Well_state_Check();
}

/***********************************************/
/* 函数名称: Alarm_Update                     */
/* 功能描述: 传感器数据更新                    */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void 	Alarm_Update(void)       //该函数每10ms执行一次，所有监测参数都更新一次需要50ms
{
    if(Work_Stop_Flag == 1)
    {
        Bit_clr(Alarm_sta, LIXIN_ALARM);
        Bit_clr(Alarm_sta, CELL_ALARM);
        Bit_clr(Alarm_sta, WASH_AIR_ALARM);
        Bit_clr(Alarm_sta, FILL_AIR_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
        Bit_clr(Alarm_sta, PRE_FILL_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
        Bit_clr(Alarm_sta, AIR_ALARM);

    }
    else
    {

        Blood_Check();
        Well_Check();

    }
    Pump_alarm_check();
    Weight_check();
    Fluid();
}
/***********************************************/
/* 函数名称: Alarm_Update                     */
/* 功能描述: 传感器数据更新                    */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
void 	Alarm_Sound(void)       //该函数每10ms执行一次，
{
    if(((Alarm_sta >> 16) & 0xffFF) == 0)
    {
        if(((Alarm_sta & 0xffff) != 0))
        {
            if(Alarm_Sound_flag == 1)
            {
                GPIO_ResetBits(GPIOE , GPIO_Pin_13);
                if(Alarm_loop_time == 150)
                    Alarm_Sound_flag = 0;
            }
            else
            {
                GPIO_SetBits(GPIOE , GPIO_Pin_13);
            }

            Alarm_loop_time++;
            if(Alarm_loop_time > 300)
            {
                Alarm_Sound_flag = 1;
                Alarm_loop_time = 0;
            }


        }
        else
        {
            GPIO_SetBits(GPIOE , GPIO_Pin_13);
            Alarm_Sound_flag = 1;
            Alarm_loop_time = 0;
        }

    }
    if(((Alarm_sta >> 16) & 0xffFF) != 0)
    {
        if(Alarm_Sound_flag == 1)
        {
            GPIO_ResetBits(GPIOE , GPIO_Pin_12);
            if(Alarm_loop_time == 250)
                Alarm_Sound_flag = 0;
        }
        else
        {
            GPIO_SetBits(GPIOE , GPIO_Pin_12);
        }

        Alarm_loop_time++;
        if(Alarm_loop_time > 350)
        {
            Alarm_Sound_flag = 1;
            Alarm_loop_time = 0;
        }


    }
    else
    {
        GPIO_SetBits(GPIOE , GPIO_Pin_12);
        //		Alarm_Sound_flag=1;
        //		Alarm_loop_time=0;

    }

}