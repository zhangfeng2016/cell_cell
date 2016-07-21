#include "stm32f10x.h"
#include "..\Drive\IIC.h"
#include "..\GUI\M600_API.h"
#include  "24C256.h"
#include  "..\Drive\Uart.h"
#include  "..\User\user_conf.h"

u8 abced[] = {6, 8, 7, 6, 5};
u8 decba[] = {0};
//24C256    MAP/////////////////////////////BEGAIN
#define SAVE_SN_ADD           1
#define SAVE_SN_F_ADD         30


#define SAVE_CALI_BASE_ADD    500
#define SAVE_RES_ZERO_ADD     (SAVE_CALI_BASE_ADD+0)
#define SAVE_RES_500_ADD      (SAVE_CALI_BASE_ADD+2)
#define SAVE_WAST_7000_ADD    (SAVE_CALI_BASE_ADD+4)
#define SAVE_WAST_9000_ADD    (SAVE_CALI_BASE_ADD+6)

#define SAVE_FILL_CLI_ADD        (SAVE_CALI_BASE_ADD+10)
#define SAVE_WASH_CLI_ADD        (SAVE_CALI_BASE_ADD+12)
#define SAVE_EMPT_125_CLI_ADD    (SAVE_CALI_BASE_ADD+14)
#define SAVE_EMPT_225_CLI_ADD    (SAVE_CALI_BASE_ADD+16)
#define SAVE_DEFAULT_MODE_ADD    (SAVE_CALI_BASE_ADD+18)
#define SAVE_DEFAULT_PUMP_ADD    (SAVE_CALI_BASE_ADD + 19 )
#define SAVE_LINXIN_ADJ_ADD      (SAVE_CALI_BASE_ADD + 20 )
#define SAVE_WASH_PROT_ADD       (SAVE_CALI_BASE_ADD + 21 )
#define SAVE_BLOOD_GATE_ADD       (SAVE_CALI_BASE_ADD + 22 ) //两个字节
#define SAVE_PREFILL_ADD         (SAVE_CALI_BASE_ADD + 24 ) //两个字节

#define SAVE_ALARM_BASE_ADD    1000


#define SAVE_STATE_BASE_ADD    1500
#define SAVE_FILL_TATAL_ADD    (SAVE_STATE_BASE_ADD+0)
#define SAVE_WASH_TATAL_ADD    (SAVE_STATE_BASE_ADD+2)
#define SAVE_EMPTY_TATAL_ADD   (SAVE_STATE_BASE_ADD+4)




#define SAVE_TEND_NUM_ADD          (9999)  //????????
#define SAVE_TEND_ADD(x)           (10000+x*100)  //????
#define SAVE_P_ON_TIME_ADD(x)      (SAVE_TEND_ADD(x)+1)    //0-5
#define SAVE_FILL_TIME_ADD(x)      (SAVE_TEND_ADD(x)+7)  //7，8
#define SAVE_WASH_TIME_ADD(x)      (SAVE_TEND_ADD(x)+9)  //9，10
#define SAVE_EMPTY_TIME_ADD(x)     (SAVE_TEND_ADD(x)+11)  //11，12

#define SAVE_HUANZHE_ADD(x)        (SAVE_TEND_ADD(x)+20)

#define SAVE_FILL_ADD(x)           (SAVE_TEND_ADD(x)+30)
#define SAVE_WASH_ADD(x)           (SAVE_TEND_ADD(x)+32)
#define SAVE_EMPTY_ADD(x)          (SAVE_TEND_ADD(x)+34)


//24C256   MAP///////////////////////////////END
void PWR_BackupAccessCmd(FunctionalState NewState);
//???IIC??
void AT24CXX_Init(void)
{
    IIC_Configuration();
}

//?????     ?????
u8 I2C_ReadByte( u16 ReadAddress)
{
    u8 temp;
    IIC_Start();

    IIC_Send_Byte(0xA0);//?????
    IIC_Wait_Ack();

    IIC_Send_Byte(ReadAddress >> 8);  //???????
    IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddress % 256); //???????
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(0xA1);             //?????
    IIC_Wait_Ack();

    temp = IIC_Read_Byte();
    IIC_NAck();
    IIC_Stop();
    return temp;
}


///???????      ?????   ?????
void I2C_WriteByte(u8 SendByte, u16 WriteAddress)
{
    u16 j;
    IIC_Start();

    IIC_Send_Byte(0xA0);//?????
    IIC_Wait_Ack();

    IIC_Send_Byte(WriteAddress >> 8);  //???????
    IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddress % 256); //???????
    IIC_Wait_Ack();
    IIC_Send_Byte(SendByte);           //???
    IIC_Wait_Ack();
    IIC_Stop();
    //??(10ms)
    //  for(j=0;j<3000;j++)
    for(j = 0; j < 800; j++)
    {
        IIC_delay();
    }

}


void AT24CXX_Writeword(u16 DataToWrite, u16 WriteAddr)
{
    u8 t;
    for(t = 0; t < 2; t++)
    {
        I2C_WriteByte(DataToWrite >> (8 * t), WriteAddr + t);
    }
}



u16 AT24CXX_Readword(u16 ReadAddr)
{
    u8 t;
    u16 temp = 0;
    for(t = 0; t < 2; t++)
    {
        temp <<= 8;
        temp += I2C_ReadByte(ReadAddr + 1 - t);
    }
    return temp;
}



void I2C_24CXX_Write(u16 Addr, u8 *pDat, u16 Len)
{
    u16 i;
    for(i = 0; i < Len; i++)
    {
        I2C_WriteByte(*(pDat + i), Addr + i);

    }
}


void I2C_24CXX_Read(u16 Addr, u8 *pDat, u16 Len)
{
    u16 i;
    for(i = 0; i < Len; i++)
        *(pDat + i) = I2C_ReadByte(Addr + i);
}




//void iic_test(void)
//{

//	 AT24CXX_Writeword(Monitor_Fill_count,SAVE_FILL_TATAL_ADD);
//   I2C_WriteByte(iic_save, 0);
//	 AT24CXX_Writeword(iic_save1,900);
//   I2C_24CXX_Write(500, abced, 5);
//
//
//
//	 back1=I2C_ReadByte(0);
//   back2=AT24CXX_Readword(SAVE_FILL_TATAL_ADD);
//   I2C_24CXX_Read(500, decba, 5);
//
//}
/***********************************************/
/* 函数名称: Clr_save_add                     */
/* 功能描述: 清除上次数据                      */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：15年10月19日                      */
/***********************************************/
void Clr_save_add(uint8 save_tend)
{
    uint8 temp_i;
    for(temp_i = 0; temp_i < 100; temp_i++)
        I2C_WriteByte(0xff, (SAVE_TEND_ADD(save_tend) + temp_i));
}
/***********************************************/
/* ????: Save_Poweron_time                */
/* ????: ????                         */
/* ?   ?: No                                */
/* ?   ?: No                                */
/* ????: ?                                */
/* ????: ?                                */
/* ????:15?06?24?  //                  */
/***********************************************/
void Save_Poweron_time(uint8 save_tend )
{
    AT24CXX_Writeword(timer.year , SAVE_P_ON_TIME_ADD(save_tend));
    I2C_WriteByte(timer.month, (SAVE_P_ON_TIME_ADD(save_tend) + 2));
    I2C_WriteByte(timer.day, (SAVE_P_ON_TIME_ADD(save_tend) + 3));
    I2C_WriteByte(timer.hour, (SAVE_P_ON_TIME_ADD(save_tend) + 4));
    I2C_WriteByte(timer.min, (SAVE_P_ON_TIME_ADD(save_tend) + 5));
    //	  AT24CXX_Writeword(1983 ,SAVE_P_ON_TIME_ADD(Tend_num));
    //	I2C_WriteByte(11,(SAVE_P_ON_TIME_ADD(Tend_num)+2));
    //	I2C_WriteByte(22,(SAVE_P_ON_TIME_ADD(Tend_num)+3));
    //	I2C_WriteByte(33,(SAVE_P_ON_TIME_ADD(Tend_num)+4));
    //	I2C_WriteByte(44,(SAVE_P_ON_TIME_ADD(Tend_num)+5));

}
/***********************************************/
/* 函数名称: Read_Poweron_time                 */
/* 功能描述:                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
time_type Read_Poweron_time(uint8 tend_num)
{

    time_type temp;
    temp.year = AT24CXX_Readword(SAVE_P_ON_TIME_ADD(tend_num));
    temp.month = I2C_ReadByte((SAVE_P_ON_TIME_ADD(tend_num) + 2));
    temp.day = I2C_ReadByte((SAVE_P_ON_TIME_ADD(tend_num) + 3));
    temp.hour = I2C_ReadByte((SAVE_P_ON_TIME_ADD(tend_num) + 4));
    temp.min = I2C_ReadByte((SAVE_P_ON_TIME_ADD(tend_num) + 5));
    return temp;

}
/***********************************************/
/* 函数名称: Read_Poweron_time                 */
/* 功能描述:                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
time_type Read_fill_time(uint8 tend_num)
{

    time_type temp;

    temp.hour = I2C_ReadByte((SAVE_FILL_TIME_ADD(tend_num)));
    temp.min = I2C_ReadByte((SAVE_FILL_TIME_ADD(tend_num) + 1));
    return temp;

}
/***********************************************/
/* 函数名称: Read_Poweron_time                 */
/* 功能描述:                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
time_type Read_wash_time(uint8 tend_num)
{

    time_type temp;

    temp.hour = I2C_ReadByte((SAVE_WASH_TIME_ADD(tend_num)));
    temp.min = I2C_ReadByte((SAVE_WASH_TIME_ADD(tend_num) + 1));
    return temp;

}
/***********************************************/
/* 函数名称: Read_Poweron_time                 */
/* 功能描述:                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
time_type Read_empty_time(uint8 tend_num)
{

    time_type temp;

    temp.hour = I2C_ReadByte((SAVE_EMPTY_TIME_ADD(tend_num)));
    temp.min = I2C_ReadByte((SAVE_EMPTY_TIME_ADD(tend_num) + 1));
    return temp;

}
/***********************************************/
/* 函数名称: Read_sta                 */
/* 功能描述: 泵速显示                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Read_sta(uint8 tend_num )
{


    Tend_Fill_Volume = AT24CXX_Readword(SAVE_FILL_ADD(tend_num));
    Tend_Wash_Volume = AT24CXX_Readword(SAVE_WASH_ADD(tend_num));
    Tend_Empty_Volume = AT24CXX_Readword(SAVE_EMPTY_ADD(tend_num));


}
/***********************************************/
/* 函数名称: Read_huanzhe                      */
/* 功能描述: 患者信息读取                      */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Read_huanzhe(uint8 tend_num )
{
    uint8 temp_i = 0;
    //uint8 str[10];
    for(temp_i = 0; temp_i < 9; temp_i++)
        Tend_Set_person[temp_i] = I2C_ReadByte((SAVE_HUANZHE_ADD(GUI_Tend_num) + temp_i));
    //return str;
}
/***********************************************/
/* ????: Save_Poweron_time                */
/* ????: ????                         */
/* ?   ?: No                                */
/* ?   ?: No                                */
/* ????: ?                                */
/* ????: ?                                */
/* ????:15?06?24?  //                  */
/***********************************************/
void Save_Run_time(void)
{


    AT24CXX_Writeword(RTC->CNTH, SAVE_P_ON_TIME_ADD(Tend_num));
    AT24CXX_Writeword(RTC->CNTL, (SAVE_P_ON_TIME_ADD(Tend_num) + 1));
}
/***********************************************/
/* ????: Save_state_and_setting           */
/* ????: ????                         */
/* ?   ?: No                                */
/* ?   ?: No                                */
/* ????: ?                                */
/* ????: ?                                */
/* ????:15?02?9?  //150205??       */
/***********************************************/
void Save_state_and_setting(void)
{
    //
    //	AT24CXX_Writeword(Monitor_Fill_count,SAVE_FILL_TATAL_ADD);
    //	AT24CXX_Writeword(Monitor_Wash_count,SAVE_WASH_TATAL_ADD);
    //	AT24CXX_Writeword(Monitor_Empty_count,SAVE_EMPTY_TATAL_ADD);

    AT24CXX_Writeword(Monitor_Fill_Volume, SAVE_FILL_ADD(Tend_num));
    AT24CXX_Writeword(Monitor_Wash_Volume, SAVE_WASH_ADD(Tend_num));
    AT24CXX_Writeword(Monitor_Empty_Volume, SAVE_EMPTY_ADD(Tend_num));
    if(Bit_check_1(Save_to_EEP_flag, 0))
    {
        Bit_clr(Save_to_EEP_flag, 0);
        I2C_WriteByte(Set_person[0], SAVE_HUANZHE_ADD(Tend_num));
        I2C_WriteByte(Set_person[1], (SAVE_HUANZHE_ADD(Tend_num) + 1));
        I2C_WriteByte(Set_person[2], (SAVE_HUANZHE_ADD(Tend_num) + 2));
        I2C_WriteByte(Set_person[3], (SAVE_HUANZHE_ADD(Tend_num) + 3));
        I2C_WriteByte(Set_person[4], (SAVE_HUANZHE_ADD(Tend_num) + 4));
        I2C_WriteByte(Set_person[5], (SAVE_HUANZHE_ADD(Tend_num) + 5));
        I2C_WriteByte(Set_person[6], (SAVE_HUANZHE_ADD(Tend_num) + 6));
        I2C_WriteByte(Set_person[7], (SAVE_HUANZHE_ADD(Tend_num) + 7));
        I2C_WriteByte(Set_person[8], (SAVE_HUANZHE_ADD(Tend_num) + 8));
        I2C_WriteByte(Set_person[9], (SAVE_HUANZHE_ADD(Tend_num) + 9));
    }
    if(Bit_check_1(Save_to_EEP_flag, SAVE_FILL_TIME)) //FILL_TIME SAVE
    {
        Bit_clr(Save_to_EEP_flag, SAVE_FILL_TIME);
        I2C_WriteByte(timer.hour, SAVE_FILL_TIME_ADD(Tend_num)); //SAVE_FILL_TIME_ADD(x)
        I2C_WriteByte(timer.min, (SAVE_FILL_TIME_ADD(Tend_num) + 1));
    }
    if(Bit_check_1(Save_to_EEP_flag, SAVE_WASH_TIME)) //FILL_TIME SAVE
    {
        Bit_clr(Save_to_EEP_flag, SAVE_WASH_TIME);
        I2C_WriteByte(timer.hour, SAVE_WASH_TIME_ADD(Tend_num)); //SAVE_FILL_TIME_ADD(x)
        I2C_WriteByte(timer.min, (SAVE_WASH_TIME_ADD(Tend_num) + 1));
    }
    if(Bit_check_1(Save_to_EEP_flag, SAVE_EMPTY_TIME)) //FILL_TIME SAVE
    {
        Bit_clr(Save_to_EEP_flag, SAVE_EMPTY_TIME);
        I2C_WriteByte(timer.hour, SAVE_EMPTY_TIME_ADD(Tend_num)); //SAVE_FILL_TIME_ADD(x)
        I2C_WriteByte(timer.min, (SAVE_EMPTY_TIME_ADD(Tend_num) + 1));
    }
    if(Bit_check_1(Save_to_EEP_flag, SAVE_PUMP_ADJ))
    {
        Bit_clr(Save_to_EEP_flag, SAVE_PUMP_ADJ);
        AT24CXX_Writeword(fill_pump_adjust, SAVE_FILL_CLI_ADD);
        AT24CXX_Writeword(wash_pump_adjust, SAVE_WASH_CLI_ADD);
        AT24CXX_Writeword(empty_pump_adjust_225, SAVE_EMPT_225_CLI_ADD);
        AT24CXX_Writeword(empty_pump_adjust_125, SAVE_EMPT_125_CLI_ADD);
    }
    if(Bit_check_1(Save_to_EEP_flag, SAVE_MODE_SET))
    {
        Bit_clr(Save_to_EEP_flag, SAVE_MODE_SET);
        I2C_WriteByte(Default_Mode_set, SAVE_DEFAULT_MODE_ADD);
    }
    if(Bit_check_1(Save_to_EEP_flag, SAVE_PUMP_SET))
    {
        Bit_clr(Save_to_EEP_flag, SAVE_PUMP_SET);
        I2C_WriteByte(Default_Pump_set, SAVE_DEFAULT_PUMP_ADD);
    }
    //SAVE_LINXI_ADJ
    if(Bit_check_1(Save_to_EEP_flag, SAVE_LINXI_ADJ))
    {
        Bit_clr(Save_to_EEP_flag, SAVE_LINXI_ADJ);
        I2C_WriteByte(Centrifugal_motor_speed_Adj, SAVE_LINXIN_ADJ_ADD);
    }
    if(Bit_check_1(Save_to_EEP_flag, 8))
    {
        Bit_clr(Save_to_EEP_flag, 8);
        I2C_WriteByte(Series_No[0], SAVE_SN_ADD);
        I2C_WriteByte(Series_No[1], SAVE_SN_ADD + 1);
        I2C_WriteByte(Series_No[2], SAVE_SN_ADD + 2);
        I2C_WriteByte(Series_No[3], SAVE_SN_ADD + 3);
        I2C_WriteByte(Series_No[4], SAVE_SN_ADD + 4);
        I2C_WriteByte(Series_No[5], SAVE_SN_ADD + 5);
        I2C_WriteByte(Series_No[6], SAVE_SN_ADD + 6);
        I2C_WriteByte(Series_No[7], SAVE_SN_ADD + 7);
        I2C_WriteByte(Series_No[8], SAVE_SN_ADD + 8);
        I2C_WriteByte(Series_No[9], SAVE_SN_ADD + 9);
    }
    if(Bit_check_1(Save_to_EEP_flag, 9))
    {
        Bit_clr(Save_to_EEP_flag, 9);
        I2C_WriteByte(Fac_Series_No[0], SAVE_SN_F_ADD);
        I2C_WriteByte(Fac_Series_No[1], SAVE_SN_F_ADD + 1);
        I2C_WriteByte(Fac_Series_No[2], SAVE_SN_F_ADD + 2);
        I2C_WriteByte(Fac_Series_No[3], SAVE_SN_F_ADD + 3);

    }
    if(Bit_check_1(Save_to_EEP_flag, 10))
    {
        Bit_clr(Save_to_EEP_flag, 10);
        I2C_WriteByte(Wash_cell_Protect, SAVE_WASH_PROT_ADD);

    }
    //SAVE_BLOOD_GATE_ADD
    if(Bit_check_1(Save_to_EEP_flag, 11))
    {
        Bit_clr(Save_to_EEP_flag, 11);
        AT24CXX_Writeword(Cell_check_gate, SAVE_BLOOD_GATE_ADD);

    }
    if(Bit_check_1(Save_to_EEP_flag, SAVE_PREFILL_SET))
    {
        Bit_clr(Save_to_EEP_flag, SAVE_PREFILL_SET);
        AT24CXX_Writeword(Set_Prefill_Volume, SAVE_PREFILL_ADD);

    }
}
/***********************************************/
/* ????: Read_state_and_setting            */
/* ????: ????                          */
/* ?   ?: No                                */
/* ?   ?: No                                */
/* ????: ?                                */
/* ????: ?                                */
/* ????:15?2?5?                        */
/***********************************************/
void Read_state_and_setting(void)
{
    // AT24CXX_Writeword(0xaa,SAVE_TEND_ADD(1));
    //	Monitor_Fill_count=AT24CXX_Readword(SAVE_FILL_TATAL_ADD);
    //	Monitor_Wash_count=AT24CXX_Readword(SAVE_WASH_TATAL_ADD);
    //	Monitor_Empty_count=AT24CXX_Readword(SAVE_EMPTY_TATAL_ADD);
    Reservoir_zero = AT24CXX_Readword(SAVE_RES_ZERO_ADD);
    Reservoir_500 = AT24CXX_Readword(SAVE_RES_500_ADD);
    Dwon_weigh_7000 = AT24CXX_Readword(SAVE_WAST_7000_ADD);
    Dwon_weigh_9000 = AT24CXX_Readword(SAVE_WAST_9000_ADD);

    //AT24CXX_Writeword(Series_No[0],SAVE_SN_ADD);
    Series_No[0] = AT24CXX_Readword(SAVE_SN_ADD);
    Series_No[1] = AT24CXX_Readword(SAVE_SN_ADD + 1);
    Series_No[2] = AT24CXX_Readword(SAVE_SN_ADD + 2);
    Series_No[3] = AT24CXX_Readword(SAVE_SN_ADD + 3);
    Series_No[4] = AT24CXX_Readword(SAVE_SN_ADD + 4);
    Series_No[5] = AT24CXX_Readword(SAVE_SN_ADD + 5);
    Series_No[6] = AT24CXX_Readword(SAVE_SN_ADD + 6);
    Series_No[7] = AT24CXX_Readword(SAVE_SN_ADD + 7);
    Series_No[8] = AT24CXX_Readword(SAVE_SN_ADD + 8);
    Series_No[9] = AT24CXX_Readword(SAVE_SN_ADD + 9);

    Fac_Series_No[0] = AT24CXX_Readword(SAVE_SN_F_ADD + 0);
    Fac_Series_No[1] = AT24CXX_Readword(SAVE_SN_F_ADD + 1);
    Fac_Series_No[2] = AT24CXX_Readword(SAVE_SN_F_ADD + 2);
    Fac_Series_No[3] = AT24CXX_Readword(SAVE_SN_F_ADD + 3);

    Wash_cell_Protect = AT24CXX_Readword(SAVE_WASH_PROT_ADD);
    if(Wash_cell_Protect == 0 || Wash_cell_Protect == 1)
        ;
    else
    {
        Wash_cell_Protect = 0;
        I2C_WriteByte(Wash_cell_Protect, SAVE_WASH_PROT_ADD);
    }
    Default_Pump_set = AT24CXX_Readword(SAVE_DEFAULT_PUMP_ADD);
    if(Default_Pump_set == 0 || Default_Pump_set == 1)
        ;
    else
    {
        Default_Pump_set = 0;
        AT24CXX_Writeword(Default_Pump_set, SAVE_DEFAULT_PUMP_ADD);
    }

    Default_Mode_set = AT24CXX_Readword(SAVE_DEFAULT_MODE_ADD);
    if(Default_Mode_set == 1 || Default_Mode_set == 2)
        Mode = Default_Mode_set;
    else
    {
        Default_Mode_set = Mode = 1;
        I2C_WriteByte(Default_Mode_set, SAVE_DEFAULT_MODE_ADD);
    }

    fill_pump_adjust = AT24CXX_Readword(SAVE_FILL_CLI_ADD);
    wash_pump_adjust = AT24CXX_Readword(SAVE_WASH_CLI_ADD);
    empty_pump_adjust_225 = AT24CXX_Readword(SAVE_EMPT_225_CLI_ADD);
    empty_pump_adjust_125 = AT24CXX_Readword(SAVE_EMPT_125_CLI_ADD);
    Centrifugal_motor_speed_Adj = AT24CXX_Readword(SAVE_LINXIN_ADJ_ADD);
    //AT24CXX_Writeword(Centrifugal_motor_speed_Adj,SAVE_LINXIN_ADJ_ADD);

    if(fill_pump_adjust > 150 || fill_pump_adjust < 60)
    {
        fill_pump_adjust = 100	;
        I2C_WriteByte(fill_pump_adjust, SAVE_FILL_CLI_ADD);
    }

    if(wash_pump_adjust > 150 || wash_pump_adjust < 60)
    {
        wash_pump_adjust = 100;
        I2C_WriteByte(wash_pump_adjust, SAVE_WASH_CLI_ADD);
    }
    if(empty_pump_adjust_125 > 150 || empty_pump_adjust_125 < 60)
    {

        empty_pump_adjust_125 = 100;
        empty_pump_adjust_225 = 100;
        I2C_WriteByte(empty_pump_adjust_125, SAVE_EMPT_125_CLI_ADD);
        I2C_WriteByte(empty_pump_adjust_225, SAVE_EMPT_225_CLI_ADD);

    }
    if(Centrifugal_motor_speed_Adj < 50 || Centrifugal_motor_speed_Adj > 150)
    {
        Centrifugal_motor_speed_Adj = 100;
        I2C_WriteByte(Centrifugal_motor_speed_Adj, SAVE_LINXIN_ADJ_ADD);
    }
    ///////////////////////////////////////////////////
    Cell_check_gate = AT24CXX_Readword(SAVE_BLOOD_GATE_ADD);
    if(Cell_check_gate < 1000 || Cell_check_gate > 1700)
    {
        Cell_check_gate = 1600;
        AT24CXX_Writeword(Cell_check_gate, SAVE_BLOOD_GATE_ADD);
    }
    ///////////////////////////////////////////////////
    Set_Prefill_Volume = AT24CXX_Readword(SAVE_PREFILL_ADD);
    if(Set_Prefill_Volume < 50 || Set_Prefill_Volume > 300)
    {
        Set_Prefill_Volume = 300;
        Pre_fill_Volume = Set_Prefill_Volume;
        AT24CXX_Writeword(Set_Prefill_Volume, SAVE_PREFILL_ADD);
    }
    else
    {
        Pre_fill_Volume = Set_Prefill_Volume;
    }

    Tend_num = I2C_ReadByte(SAVE_TEND_NUM_ADD);
    if(Tend_num < 100)
    {
        GUI_Tend_num = Tend_num = I2C_ReadByte(SAVE_TEND_NUM_ADD) + 1;
        I2C_WriteByte(Tend_num, SAVE_TEND_NUM_ADD);
    }
    else
    {
        Tend_num = 0;
        I2C_WriteByte(Tend_num, SAVE_TEND_NUM_ADD);
    }
    Clr_save_add(Tend_num);      //clr ever data
    Save_Poweron_time(Tend_num);                //??????
}

/***********************************************/
/* 函数名称: Save_to_bkp                       */
/* 功能描述: 将信息存储于备份寄存器            */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：15年10月9日                       */
/***********************************************/
void Save_to_bkp(void )
{
    PWR_BackupAccessCmd(ENABLE);
    BKP_WriteBackupRegister(BKP_DR30, 0x5850);
    BKP_WriteBackupRegister(BKP_DR4, (uint16)Monitor_Fill_Volume);
    BKP_WriteBackupRegister(BKP_DR5, (uint16)Monitor_Wash_Volume);
    BKP_WriteBackupRegister(BKP_DR6, (uint16)Monitor_Empty_Volume);
    BKP_WriteBackupRegister(BKP_DR7, Set_Wash_Volume);
    BKP_WriteBackupRegister(BKP_DR8, prefill_flag);
    BKP_WriteBackupRegister(BKP_DR9, Monitor_Empty_count);
    BKP_WriteBackupRegister(BKP_DR10, (power_on_Time_sec >> 16) & 0xffff);
    BKP_WriteBackupRegister(BKP_DR11, (power_on_Time_sec) & 0x0000ffff);
    PWR_BackupAccessCmd(DISABLE);


}
/***********************************************/
/* 函数名称: Read_from_bkp                     */
/* 功能描述: 将信息从备份寄存器中读出          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：15年10月9日                       */
/***********************************************/
void Read_from_bkp(void )
{
    uint16 power_on_Time_sec_h;
    uint16 power_on_Time_sec_l;
    if(BKP_ReadBackupRegister(BKP_DR30) == 0x5850)
    {
        Monitor_Fill_Volume = BKP_ReadBackupRegister(BKP_DR4);
        Monitor_Wash_Volume = BKP_ReadBackupRegister(BKP_DR5);
        Monitor_Empty_Volume = BKP_ReadBackupRegister(BKP_DR6);
        Set_Wash_Volume = BKP_ReadBackupRegister(BKP_DR7);
        prefill_flag = BKP_ReadBackupRegister(BKP_DR8);
        power_on_Time_sec_l = BKP_ReadBackupRegister(BKP_DR11);
        power_on_Time_sec_h = BKP_ReadBackupRegister(BKP_DR10);
        power_on_Time_sec = (power_on_Time_sec_h << 16) + power_on_Time_sec_l;
        if(Set_Wash_Volume < 400 || Set_Wash_Volume > 3000)
        {
            if(Set_Bowl == 225)
                Set_Wash_Volume = 1000;
            else if (Set_Bowl == 125)
            {
                Set_Wash_Volume = 800;
            }
        }
    }
    else
    {
        Monitor_Fill_count = 0;
        Monitor_Wash_count = 0;
        Monitor_Empty_count = 0;
        if(Set_Bowl == 225)
            Set_Wash_Volume = 1000;
        else if (Set_Bowl == 125)
        {
            Set_Wash_Volume = 800;
        }
    }
}
