#include "..\User\user_conf.h"
#include "..\Drive\Uart.h"
#include "User_dispaly.h"
#include "..\Drive\ADC.h"
uint8 Fac_set_num = 0;
uint8 Help_key_show = 1;
uint8 vido_run_falg = 0;
uint8 vido_stop_flag = 1;
//uint8 key_buf[3];
void Stop(void);
void Pump_Stop_Test(void );
uint8 ComReadCmd(void);
void Yellow_Pump_Test(uint8 dir, uint16 Set_Pump_Test_speed);
void Green_Pump_Test(uint8 dir, uint16 Set_Pump_Test_speed);
void Red_Pump_Test(uint8 dir, uint16 Set_Pump_Test_speed);
void AT24CXX_Writeword(u16 DataToWrite, u16 WriteAddr);
void  Tx_Byte(uint8 data);
void delay_ms(u16 nms);

u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec);

#define SAVE_CALI_BASE_ADD    500
#define SAVE_RES_ZERO_ADD     (SAVE_CALI_BASE_ADD+0)
#define SAVE_RES_500_ADD      (SAVE_CALI_BASE_ADD+2)
#define SAVE_WAST_7000_ADD    (SAVE_CALI_BASE_ADD+4)
#define SAVE_WAST_9000_ADD    (SAVE_CALI_BASE_ADD+6)

/**********************************************/
/* 函数名称: Clr_Alarm_flag                   */
/* 功能描述: 复位所有报警                     */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void Clr_Alarm_flag(void)
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




/**********************************************/
/* 函数名称: Touch_Key_Factory                 */
/* 功能描述: 工厂模式按键处理                */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void Touch_Key_Factory(void)
{
#define PIC_TIME_SET  17
#define TIME_SET  0X02
#define FILL_KEY  0X04
#define WASH_KEY  0X05
#define EMPTY_KEY  0X06
#define PUMP_SPEED_INC  0X24
#define PUMP_SPEED_DEC  0X25
#define MOTO_SPEED_1_KEY  0X21
#define MOTO_SPEED_2_KEY  0X22
#define MOTO_SPEED_3_KEY  0X23
#define STOP_KEY          0X01
    static uint8 return_cont;
    static uint8 seting_cont;
    //#define FILL_KEY  0X02
    switch(key_buf[2])
    {

    case STOP_KEY:
        Pump_Stop_Test();
        Centrifugal_motor_orde = 2;
        Factory_autorun_flag = 0;
        if(return_cont < 10)
        {
            return_cont++;
        }
        else
        {
            return_cont = 0;
            GUI_State = GUI_0;
            Show_pic_num = 0;
            Display_Loop = 0;
            show_pic_flag = 0;
        }
        break;
    case 0x11:
        Yellow_Pump_Test(1, 300);
        break;
    case 0x12:
        Green_Pump_Test(1, 300);
        break;
    case 0x13:
        Red_Pump_Test(0, 300);
        break;
    case 0x09:
        if(Centrifugal_motor_speed_Set < 5650)
        {
            Centrifugal_motor_orde = 1;
            Centrifugal_motor_speed_Set += 50;
        }
        break;
    case 0x10:
        if(Centrifugal_motor_speed_Set > 2000)
        {
            Centrifugal_motor_orde = 1;
            Centrifugal_motor_speed_Set -= 50;
        }
        break;
    case 0x06:
        Centrifugal_motor_orde = 1;
        Centrifugal_motor_speed_Set = 2000;
        break;
    case 0x07:
        Centrifugal_motor_orde = 1;
        Centrifugal_motor_speed_Set = 4500;
        break;
    case 0x08:
        Centrifugal_motor_orde = 1;
        Centrifugal_motor_speed_Set = 5650;
        break;
    case 0x20:
        if(seting_flag == 1)
        {
            Reservoir_zero = Monitor_Reservoir_weigh;
            AT24CXX_Writeword(Reservoir_zero, SAVE_RES_ZERO_ADD);
        }
        break;
    case 0x21:
        if(seting_flag == 1)
        {
            Reservoir_500 = Monitor_Reservoir_weigh;
            AT24CXX_Writeword(Reservoir_500, SAVE_RES_500_ADD);
        }
        break;
    case 0x22:
        if(seting_flag == 1)
        {
            Dwon_weigh_7000 = Monitor_Dwon_weigh;
            AT24CXX_Writeword(Dwon_weigh_7000, SAVE_WAST_7000_ADD);
        }
        break;
    case 0x23:
        if(seting_flag == 1)
        {
            Dwon_weigh_9000 = Monitor_Dwon_weigh;
            AT24CXX_Writeword(Dwon_weigh_9000, SAVE_WAST_9000_ADD);
        }
        break;
    case 0x24:
        print_flag = 4;
        break;
    case 0x25:
        GUI_State = GUI_FACTORY_1;
        Show_pic_num = 19;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    case 0x14:	//sheding
        seting_cont++;
        if(seting_cont < 10)
        {
            seting_flag = 0;
        }
        else
        {
            seting_flag = 1;
            if(seting_cont > 15)
                seting_cont = 0;
        }

        break;
    case 0x15:	//zidonglaohua
        Factory_autorun_flag = 1;
        Yellow_Pump_Test(1, 300);
        break;
    case 0x55:
        GUI_State = GUI_FACTORY;
        Show_pic_num = 9;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
        break;
    case 0x56:
        if(line_select > 6)
            line_select--;
        break;
    case 0x57:
        if(line_select < 16)
            line_select++;
        break;
    case 0x59:
        switch(line_select)
        {
        case 7:
            if(Default_Pump_set == 0)
                Default_Pump_set = 1;
            else
                Default_Pump_set = 0;
            break;
        case 8:
            if(fill_pump_adjust > 60)
                fill_pump_adjust--;
            break;
        case 9:
            if(wash_pump_adjust > 60)
                wash_pump_adjust--;
            break;
        case 10:
            if(empty_pump_adjust_125 > 60)
                empty_pump_adjust_125--;
            break;
        case 11:
            if(empty_pump_adjust_225 > 60)
                empty_pump_adjust_225--;
            break;
        case 12:
            if(Set_Prefill_Volume > 50)
                Set_Prefill_Volume -= 50;
            break;
        case 13:
            if(Default_Mode_set == MODE_AUTO)
                Default_Mode_set = MODE_MU;
            else
                Default_Mode_set = MODE_AUTO;
            break;	//
        case 14: //离心电机调整
            if(Centrifugal_motor_speed_Adj > 50)
                Centrifugal_motor_speed_Adj--;
            break;	//
        case 15: //
            if(Wash_cell_Protect == PROTECT_ABLE)
                Wash_cell_Protect = PROTECT_DISABLE;
            else
                Wash_cell_Protect = PROTECT_ABLE;
            break;	//
        case 16: //血层阈值调整
            if(Cell_check_gate > 1000)
                Cell_check_gate = Cell_check_gate - 20;
            break;	//

        default:
            break;

        }
        break;
    case 0x58:
        switch(line_select)
        {
        case 7:
            if(Default_Pump_set == 0)
                Default_Pump_set = 1;
            else
                Default_Pump_set = 0;

            break;
        case 8:
            if(fill_pump_adjust < 140)
                fill_pump_adjust++;
            break;
        case 9:
            if(wash_pump_adjust < 140)
                wash_pump_adjust++;
            break;
        case 10:
            if(empty_pump_adjust_125 < 140)
                empty_pump_adjust_125++;
            break;
        case 11:
            if(empty_pump_adjust_225 < 140)
                empty_pump_adjust_225++;
            break;
        case 13:
            if(Default_Mode_set == MODE_AUTO)
                Default_Mode_set = MODE_MU;
            else
                Default_Mode_set = MODE_AUTO;
            break;
        case 12:
            if(Set_Prefill_Volume < 300)
                Set_Prefill_Volume += 50;
            break;
        case 14: //离心电机调整
            if(Centrifugal_motor_speed_Adj < 150)
                Centrifugal_motor_speed_Adj++;
            break;	//
        case 15: //离心电机调整
            if(Wash_cell_Protect == PROTECT_ABLE)
                Wash_cell_Protect = PROTECT_DISABLE;
            else
                Wash_cell_Protect = PROTECT_ABLE;
            break;
        case 16: //血层阈值调整
            if(Cell_check_gate < 1700)
                Cell_check_gate = Cell_check_gate + 20;
            break;	//
        default:
            break;

        }
        break;
    case 0x60:
        switch(line_select)
        {
        case 7:
            Bit_set(Save_to_EEP_flag, SAVE_PUMP_SET);
            break;
        case 8:
            Bit_set(Save_to_EEP_flag, SAVE_PUMP_ADJ);
            break;
        case 9:
            Bit_set(Save_to_EEP_flag, SAVE_PUMP_ADJ);
            break;
        case 10:
        case 11:
            Bit_set(Save_to_EEP_flag, SAVE_PUMP_ADJ);
            break;
        case 12:
            Bit_set(Save_to_EEP_flag, SAVE_PREFILL_SET);
            break;

        case 13:
            Bit_set(Save_to_EEP_flag, SAVE_MODE_SET);
            break;
        case 14:
            Bit_set(Save_to_EEP_flag, SAVE_LINXI_ADJ);
            break;
        case 15:
            Bit_set(Save_to_EEP_flag, 10);
            break;
        case 16:
            Bit_set(Save_to_EEP_flag, 11);
            break;
        default:
            break;

        }
        break;
    default:
        break;
    }

}
/**********************************************/
/* 函数名称: Touch_Key_Time_Set                 */
/* 功能描述: 时间处理按键函数                */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void Touch_Key_Set_Time_8200b(void )
{
    switch(key_buf[2])
    {
    case 0x01: //
        Set_pra_now = SET_YEAR;
        break;
    case 0x02:
        Set_pra_now = SET_MONTH;
        break;
    case 0x03:
        Set_pra_now = SET_DAY;
        break;
    case 0x04:

        Set_pra_now = SET_HOUR;
        break;
    case 0x06:
        Set_pra_now = SET_SEC;

        break;
    case 0x05:
        Set_pra_now = SET_MIN;
        break;
    case 0X07://变量+
        switch(Set_pra_now)
        {
        case SET_YEAR:
            if(Time_Set.year < 2078)
                Time_Set.year++;
            break;
            break;
        case SET_MONTH:
            if(Time_Set.month < 12)
                Time_Set.month++;
            break;
        case SET_DAY:
            if(Time_Set.month == 2)
            {
                if(Time_Set.day < 28)
                    Time_Set.day++;
            }
            else
            {
                if(Time_Set.day < 31)
                    Time_Set.day++;
            }
            break;
        case SET_HOUR:
            if(Time_Set.hour < 23)
                Time_Set.hour++;
            break;
        case SET_MIN:
            if(Time_Set.min < 59)
                Time_Set.min++;
            break;
        case SET_SEC:
            if(Time_Set.sec < 59)
                Time_Set.sec++;
            break;
        default:
            break;

        }
        break;
    case 0x08:
        switch(Set_pra_now)
        {
        case SET_YEAR:
            if(Time_Set.year > 1970)
                Time_Set.year--;
            break;
        case SET_MONTH:
            if(Time_Set.month > 1)
                Time_Set.month--;
            break;
        case SET_DAY:

            if(Time_Set.day > 1)
                Time_Set.day--;
            break;
        case SET_HOUR:
            if(Time_Set.hour > 0)
                Time_Set.hour--;
            break;
        case SET_MIN:
            if(Time_Set.min > 0)
                Time_Set.min--;
            break;
        case SET_SEC:
            if(Time_Set.sec > 0)
                Time_Set.sec--;
            break;
        default:
            break;
        }
        break;
    case 0x09:
        RTC_Set(Time_Set.year, Time_Set.month, Time_Set.day, Time_Set.hour, Time_Set.min, Time_Set.sec);
        break;
    default:
        break;
    }

}

/**********************************************/
/* 函数名称: Touch_Key_Time_Set                 */
/* 功能描述: 时间处理按键函数                */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/

void Touch_Key_Time_Set(void)
{
#define PIC_TIME_SET  17
#define TIME_SET  0X02
    switch(CommRecvBuf[2])
    {
    case 0X53:
        if(Time_Set.year < 2078)
            Time_Set.year++;
        break;
    case 0X52:
        if(Time_Set.year > 1970)
            Time_Set.year--;
        if(Time_Set.year < 2010)
            Time_Set.year = 2010;
        break;
    case 0X55:
        if(Time_Set.month < 12)
            Time_Set.month++;
        break;
    case 0X54:
        if(Time_Set.month > 1)
            Time_Set.month--;
        break;
    case 0X57:
        if(Time_Set.day < 31)
            Time_Set.day++;
        break;
    case 0X56:
        if(Time_Set.day > 1)
            Time_Set.day--;
        break;
    case 0X59:
        if(Time_Set.hour < 24)
            Time_Set.hour++;
        break;
    case 0X58:
        if(Time_Set.hour > 1)
            Time_Set.hour--;
        break;
    case 0X61:
        if(Time_Set.min < 60)
            Time_Set.min++;
        break;
    case 0X60:
        if(Time_Set.min > 1)
            Time_Set.min--;
        break;
    case 0X63:
        if(Time_Set.sec < 60)
            Time_Set.sec++;
        break;
    case 0X62:
        if(Time_Set.sec > 1)
            Time_Set.sec--;
        break;
    case 0x01://停止
        work_sta = 25;
        Pump_Pause = 0;
        Centrifugal_motor_orde = 2;
        if(Mode == MODE_AUTO)
            Work_Stop_Flag = 1;

        if(Bit_check_1(Bowl_State, FILL_BLOOD))  //设置蠕动泵延时标志
        {
            Bit_set(Bowl_State, 1);               //再次启动时需要延时蠕动泵
            Bit_clr(Bowl_State, 2);               //清除正在延时标志位
            pump_dely_time = 50;
        }
        break;
    case	0x03: //fanhui
        GUI_State = GUI_FACTORY;
        Show_pic_num = 11;
        RTC_Set(Time_Set.year, Time_Set.month, Time_Set.day, Time_Set.hour, Time_Set.min, Time_Set.sec);
        //Time_Set=timer;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;

    }

}
/**********************************************/
/* 函数名称: Touch_Key_Help()                 */
/* 功能描述: 帮助界面按键处理函数               */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/

void Touch_Key_Help(void)
{
    switch(CommRecvBuf[2])
    {
    case 0x03:    //返回
        GUI_State = GUI_STANDBY;
        Show_pic_num = Mode;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    }

}
/**********************************************/
/* 函数名称: Touch_Key_Help()                 */
/* 功能描述: 帮助界面按键处理函数               */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/

void Touch_Key_Test(void)
{
    switch(	key_buf[2])
    {
    case 0x02:
        if(test_time > 200)
        {
            Test_step_next = 1;
        }
        break;
    case 0x01:
        if(test_time > 200)
        {
            Test_step_retest = 1;
        }
        break;
    default:
        break;

    }

}
/**********************************************/
/* 函数名称: Touch_Key_jINGJI()                 */
/* 功能描述: 时间处理按键函数                */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/

void Touch_Key_jingji(void)
{
    switch(CommRecvBuf[2])
    {
    case 0x11:
        break;
    default:
        break;
    }

}
/**********************************************/
/* 函数名称: Touch_Key_jINGJI()                 */
/* 功能描述: 时间处理按键函数                */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/

void Touch_Key_Set(void)
{

    switch(key_buf[2])
    {
    case 0x07:    //返回
        GUI_State = GUI_STANDBY;
        Show_pic_num = 2;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    case 0x01:
        GUI_State = GUI_SET;
        GUI_Set_State = GUI_SET_MODE;
        set_mode = 0;
        Show_pic_num = 3;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    case 0x02:
        GUI_State = GUI_SET;
        GUI_Set_State = GUI_SET_PRA;
        Show_pic_num = 4;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    case 0x03:
        GUI_State = GUI_SET;
        GUI_Set_State = GUI_SET_ARLAM ;
        Show_pic_num = 5;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    case 0x04:
        GUI_State = GUI_SET;
        GUI_Set_State = GUI_SET_HUANZHE ;
        Set_pra_now = 0;
        Show_pic_num = 6;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    case 0x05://baojing
        GUI_State = GUI_SET;
        GUI_Set_State = GUI_SET_TEND ;
        Show_pic_num = 7;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    case 0x06:
        GUI_State = GUI_SET;
        GUI_Set_State = GUI_SET_TIME ;
        Show_pic_num = 8;
        Display_Loop = 0;
        show_pic_flag = 0;
        Time_Set = timer;
        break;
    case 0x08:
        switch(set_mode)
        {
        case MODE_MU:
            if(Mode == MODE_YINGJI)
            {
                Bit_set(Set_Alarm_flag, 0);
                Bit_set(Set_Alarm_flag, 1);
                Bit_set(Set_Alarm_flag, 2);
                Bit_set(Set_Alarm_flag, 3);
            }
            if(Mode == MODE_NOWASH)
            {
                work_sta = WORK_STANDBY;
            }
            break;
        case MODE_AUTO:
            Bit_set(Set_Alarm_flag, 0);
            Bit_set(Set_Alarm_flag, 1);
            Bit_set(Set_Alarm_flag, 2);
            Bit_set(Set_Alarm_flag, 3);
            Bit_set(Set_Alarm_flag, 4);
            if(Mode == MODE_YINGJI)
            {
                Bit_set(Set_Alarm_flag, 0);
                Bit_set(Set_Alarm_flag, 1);
                Bit_set(Set_Alarm_flag, 2);
                Bit_set(Set_Alarm_flag, 3);
                Bit_set(Set_Alarm_flag, 4);
            }
            if(Mode == MODE_NOWASH)
            {
                work_sta = WORK_STANDBY;
            }
            if	(Mode == MODE_MU)	//手动----自动
            {
                switch(work_sta)
                {
                case WORK_STANDBY:
                case WORK_RETURN:
                case WORK_JINJI:
                    WASH_DISABLE();
                    FILL_DISABLE();
                    EMPTY_DISABLE();
                    Work_Stop_Flag = 0;
                    Auto_run_step = 0;
                    work_sta = WORK_STANDBY;
                    break;
                case WORK_FILL:
                case WORK_CONC:
                    Auto_run_step = AUTO_FILL;
                    break;
                case WORK_WASH:
                    Auto_run_step = AUTO_WASH;
                    break;
                case WORK_EMPTY:
                    Auto_run_step = AUTO_EMPTY;
                    break;
                default:
                    break;
                }
            }
            break;
        case MODE_NOWASH:
            WASH_DISABLE();
            FILL_DISABLE();
            EMPTY_DISABLE();
            Centrifugal_motor_orde = 2;
            work_sta = WORK_STANDBY;
            work_sta = WORK_STANDBY;
            Bit_clr(Set_Alarm_flag, 0);
            Bit_clr(Set_Alarm_flag, 1);
            Bit_clr(Set_Alarm_flag, 2);
            Bit_clr(Set_Alarm_flag, 3);
            Bit_clr(Set_Alarm_flag, 4);
            Bit_clr(Set_Alarm_flag, LOUYE_ALARM_SW);
            break;
        case MODE_YINGJI:
            WASH_DISABLE();
            FILL_DISABLE();
            EMPTY_DISABLE();
            Centrifugal_motor_orde = 2;
            work_sta = WORK_STANDBY;
            Centrifugal_motor_orde = 2;
            Bit_clr(Set_Alarm_flag, 0);
            Bit_clr(Set_Alarm_flag, 1);
            Bit_clr(Set_Alarm_flag, 2);
            Bit_clr(Set_Alarm_flag, 3);
            Bit_clr(Set_Alarm_flag, 4);
            Bit_clr(Set_Alarm_flag, LOUYE_ALARM_SW);
            Set_Arlarm_ON_OFF = 0; //报警都关闭

            break;
        default:
            break;
        }
        if(set_mode != 0)
            Mode = set_mode;
        set_mode = 0;
        GUI_State = GUI_STANDBY;
        Show_pic_num = 2;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    case 0x09:
        set_mode = 0;
        break;
    case 0x13:    //不清洗
        Auto_run_step = 0;
        set_mode = MODE_NOWASH;
        break;
    case 0x10:    //自动
        //			         GUI_State=GUI_STANDBY;
        // Centrifugal_motor_orde=2;
        //						     Set_Arlarm_ON_OFF=1;
        //						     if(mode!=MODE_AUTO)
        //									 Auto_run_step=0;
        set_mode = MODE_AUTO;
        break;
    case 0x11:    //手动
        // Centrifugal_motor_orde=2;
        Set_Arlarm_ON_OFF = 1;
        set_mode = MODE_MU;

        break;
    case 0x12:    //应急模式
        //Centrifugal_motor_orde=2;
        //  Set_Arlarm_ON_OFF=0;//报警都关闭
        //Bit_clr(Set_Alarm_flag,LOUYE_ALARM_SW);
        set_mode = MODE_YINGJI;

        break;
    default:
        break;
    }
}

/**********************************************/
/* 函数名称: Touch_Key_Set_Alarm()                 */
/* 功能描述: 时间处理按键函数                */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/

void Touch_Key_Set_Pra(void)
{
    switch(key_buf[2])
    {
    case 0x01: //进血流速设定
        Set_pra_now = SET_FILL_FLOW;
        break;
    case 0x02:
        Set_pra_now = SET_WASH_FLOW;
        break;
    case 0x03:
        Set_pra_now = SET_EMPTY_FLOW;
        break;
    case 0x04:
        Set_pra_now = SET_WASH_V;
        break;
    case 0x05:
        Set_pra_now = SET_AUTO_ML;
        break;
    case 0x06:
        Set_pra_now = SET_BOWL;
        break;
    case 0X07://变量+
        switch(Set_pra_now)
        {
        case SET_FILL_FLOW:
            if(Set_Fill_speed < 1000)
                Set_Fill_speed += 10;
            break;
        case SET_WASH_FLOW:
            if(Set_Wash_speed < 1000)
                Set_Wash_speed += 10;
            break;
        case SET_EMPTY_FLOW:
            if(Set_Empty_speed < 1000)
                Set_Empty_speed += 10;
            break;
        case SET_WASH_V:
            if(Set_Wash_Volume < 5000)
                Set_Wash_Volume += 50;
            break;
        case SET_AUTO_ML:
            if(Set_Run_Volume < 1500)
                Set_Run_Volume += 10;
            break;
        case SET_BOWL:
            if(Set_Bowl == 225)
            {
                Set_Bowl = 125;
                Set_Wash_Volume = 800;
            }
            else
            {
                Set_Bowl = 225;
                Set_Wash_Volume = 1000;
            }
            break;
        default:
            break;

        }
        break;
    case 0x08:
        switch(Set_pra_now)
        {
        case SET_FILL_FLOW:
            if(Set_Fill_speed > 50)
                Set_Fill_speed -= 10;
            break;
        case SET_WASH_FLOW:
            if(Set_Wash_speed > 50)
                Set_Wash_speed -= 10;
            break;
        case SET_EMPTY_FLOW:
            if(Set_Empty_speed > 50)
                Set_Empty_speed -= 10;
            break;
        case SET_WASH_V:
            if(Set_Wash_Volume > 500)
                Set_Wash_Volume -= 50;
            break;
        case SET_AUTO_ML:
            if(Set_Run_Volume > 400)
                Set_Run_Volume -= 10;
            break;
        case SET_BOWL:
            if(Set_Bowl == 225)
            {
                Set_Bowl = 125;
                Pre_fill_Volume = 200;
                Set_Wash_Volume = 800;
            }
            else
            {
                Set_Bowl = 225;
                Pre_fill_Volume = 300;
                Set_Wash_Volume = 1000;
            }
            break;
        default:
            break;

        }
        break;
    case 0x09:
        break;//确定
    case 0x10:	//复位
        main_dbug_cont++;
        if(main_dbug_cont % 10 == 0)
        {
            main_dbug == 1 ? (main_dbug = 0) : (main_dbug = 1);
        }
        Set_Fill_speed = 300;
        Set_Wash_speed = 400;
        Set_Run_Volume = 800;
        Set_Empty_speed = 300;
        Set_Wash_Volume = 1000;
        Set_Bowl = 225;
        break;
    default:
        break;
    }


}
/**********************************************/
/* 函数名称: Touch_Key_Set_Alarm()                 */
/* 功能描述: 时间处理按键函数                */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/

void Touch_Key_Set_Alarm(void)
{

    switch(key_buf[2])
    {
    case 0x01:
        Set_pra_now = SET_FEIYE_ALARM;

        break;
    case 0x02:
        if( Mode == MODE_AUTO)
        {
            Bit_set(Set_Alarm_flag, AIR_ALARM_SW);
        }
        else if( Mode == MODE_YINGJI)
        {
            Bit_clr(Set_Alarm_flag, AIR_ALARM_SW);
        }
        else
        {
            if(Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW))
            {
                Bit_clr(Set_Alarm_flag, AIR_ALARM_SW);
                Set_Arlarm_ON_OFF = 0;
            }
            else
            {
                Bit_set(Set_Alarm_flag, AIR_ALARM_SW);
                Set_Arlarm_ON_OFF = 1;
            }
        }
        Set_pra_now = SET_AIR_ALARM;
        break;
    case 0x03:
        if( Mode == MODE_AUTO)
        {
            Bit_set(Set_Alarm_flag, BLOOD_ALARM_SW);
        }
        else if( Mode == MODE_YINGJI)
        {
            Bit_clr(Set_Alarm_flag, BLOOD_ALARM_SW);
        }
        else
        {

            if(Bit_check_1(Set_Alarm_flag, BLOOD_ALARM_SW))
                Bit_clr(Set_Alarm_flag, BLOOD_ALARM_SW);
            else
                Bit_set(Set_Alarm_flag, BLOOD_ALARM_SW);
        }
        Set_pra_now = SET_BLOOD_ALARM;
        break;
    case 0x04:
        if( Mode == MODE_AUTO)
        {
            Bit_set(Set_Alarm_flag, LOUYE_ALARM_SW);
        }
        else if( Mode == MODE_YINGJI)
        {
            Bit_clr(Set_Alarm_flag, LOUYE_ALARM_SW);
        }
        else
        {
            if(Bit_check_1(Set_Alarm_flag, LOUYE_ALARM_SW))
                Bit_clr(Set_Alarm_flag, LOUYE_ALARM_SW);
            else
                Bit_set(Set_Alarm_flag, LOUYE_ALARM_SW);
        }
        Set_pra_now = SET_LOUYE_ALARM;
        break;

    case 0x05:
        if( Mode == MODE_AUTO)
        {
            Bit_set(Set_Alarm_flag, WELL_ALARM_SW);
        }
        else if( Mode == MODE_YINGJI)
        {
            Bit_clr(Set_Alarm_flag, WELL_ALARM_SW);
        }
        Set_pra_now = SET_WELL_ALARM;
        break;

    case 0x06:
        //					   Set_pra_now_buf=Set_pra_now;
        //							    Set_pra_now=SET_BOWL;
        break;
    case 0X07://变量+
        switch(Set_pra_now)
        {
        case SET_FEIYE_ALARM:
            if( Mode == MODE_AUTO)
            {
                if(Set_feiye < 9000)
                    Set_feiye += 500;
            }
            else if( Mode == MODE_MU)
            {
                if(Set_feiye < 9500)
                    Set_feiye += 500;
            }
            else if( Mode == MODE_YINGJI)
            {
                Set_feiye = 9500;
            }
            if(Set_feiye == 9500)
                Bit_clr(Set_Alarm_flag, FEIYE_ALARM_SW);
            else
                Bit_set(Set_Alarm_flag, FEIYE_ALARM_SW);
            break;
        case SET_AIR_ALARM:
            //						 			       if(Bit_check_1(Set_Alarm_flag,AIR_ALARM_SW))
            //										       Bit_clr(Set_Alarm_flag,AIR_ALARM_SW);
            //									       else
            //										       Bit_set(Set_Alarm_flag,AIR_ALARM_SW);
            break;
        case SET_BLOOD_ALARM:
            break;
        case SET_WELL_ALARM:
            break;
        case SET_BOWL:
            break;
        default:
            break;

        }
        break;
    case 0x08:
        switch(Set_pra_now)
        {
        case SET_FEIYE_ALARM:
            if(Set_feiye > 7000)
                Set_feiye -= 500;
            break;
        case SET_AIR_ALARM:
            //						 			       if(Bit_check_1(Set_Alarm_flag,AIR_ALARM_SW))
            //										       Bit_clr(Set_Alarm_flag,AIR_ALARM_SW);
            //									       else
            //										       Bit_set(Set_Alarm_flag,AIR_ALARM_SW);
            break;
        case SET_BLOOD_ALARM:
            break;
        case SET_WELL_ALARM:
            break;
        case SET_BOWL:
            break;
        default:
            break;

        }
        break;
    case 0x10://复位
        if( Mode != MODE_YINGJI)
        {
            Set_feiye = 7000;
            Bit_set(Set_Alarm_flag, FEIYE_ALARM_SW);
            Bit_set(Set_Alarm_flag, WELL_ALARM_SW);
            Bit_set(Set_Alarm_flag, LOUYE_ALARM_SW);
            Bit_set(Set_Alarm_flag, BLOOD_ALARM_SW);
            Bit_set(Set_Alarm_flag, AIR_ALARM_SW);
        }
        break;

    default:
        break;
    }
}
/**********************************************/
/* 函数名称: Touch_Key_Main()                 */
/* 功能描述: 时间处理按键函数                */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/

void Touch_Key_Main(void)
{
    switch(key_buf[2])
    {
    case 0x09:    //fill
        Centrifugal_motor_stop_orde = 0;
        Bit_clr(Alarm_sta, FILL_PUMP_ALARM);
        Bit_clr(Alarm_sta, WASH_PUMP_ALARM);
        Bit_clr(Alarm_sta, EMPTY_PUMP_ALARM);
        Bit_clr(Pump_state_flag, EMPTY_OK);
        if( Mode == MODE_NOWASH)
        {
            sta = WORK;
            Pump_Pause = 0;     //取消暂停
            work_sta = WORK_JINJI;
            Centrifugal_motor_orde = 2;
            Bit_set(work_percheck, FILL_ALLOW_CHECK);
        }
        else
        {
            if(prefill_flag == 1)
            {
                if(work_sta == WORK_PRE_FILL)
                {
                    if(Monitor_Pump.Wash_Pump_count > 50)
                    {
                        prefill_flag = 0;
                        sta = WORK;
                        work_sta = WORK_FILL;
                        Monitor_Pump.Fill_Pump_count = 0;
                        Bit_clr(Pump_state_flag, FILL_OK);
                        Bit_clr(Pump_state_flag, CONC_OK);
                        Pump_Pause = 0;
                        Bit_set(work_percheck, FILL_ALLOW_CHECK);
                        if(Mode == MODE_YINGJI)
                        {
                            Centrifugal_motor_orde = 1;
                            Centrifugal_motor_speed_Set = 5650;
                        }
                        else if(READ_WELL_STATE() == 1)
                        {
                            Centrifugal_motor_orde = 1;
                            Centrifugal_motor_speed_Set = 5650;
                        }
                    }
                }
                else
                {
                    sta = WORK;
                    WASH_DISABLE();
                    FILL_DISABLE();
                    EMPTY_DISABLE();
                    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
                    Bit_set(Auto_check_bowl_falg, 1);
                    work_sta = WORK_PRE_FILL;
                    Monitor_Pump.Wash_Pump_count = 0;
                    Bit_clr(Pump_state_flag, FILL_OK);
                    Bit_clr(Pump_state_flag, CONC_OK);
                    Auto_run_step = AUTO_FILL;
                    if(Mode == MODE_YINGJI)
                    {
                        Centrifugal_motor_orde = 1;
                        Centrifugal_motor_speed_Set = 5650;
                    }
                    else if(READ_WELL_STATE() == 0) //
                    {
                        Centrifugal_motor_orde = 1;
                        Centrifugal_motor_speed_Set = 5650;
                    }
                    Bit_set(work_percheck, FILL_ALLOW_CHECK);
                }
            }
            else
            {
                if((work_sta != WORK_FILL) || (Work_Stop_Flag == 1) || (Pump_Pause == 1))
                {
                    WASH_DISABLE();
                    FILL_DISABLE();
                    EMPTY_DISABLE();
                    Work_Stop_Flag = 0;
                    sta = WORK;
                    work_sta = WORK_FILL;
                    Bit_clr(Pump_state_flag, FILL_OK);
                    Bit_clr(Pump_state_flag, CONC_OK);
                    Monitor_Pump.Fill_Pump_count = 0;
                    Pump_Pause = 0;
                    reservoir_delay_check_cont = 0;
                    Auto_refill_wait_flag = 0;
                    if(Mode == MODE_YINGJI)
                    {
                        Centrifugal_motor_orde = 1;
                        Centrifugal_motor_speed_Set = 5650;
                    }
                    else if(READ_WELL_STATE() == 0)
                    {
                        Centrifugal_motor_orde = 1;
                        Centrifugal_motor_speed_Set = 5650;
                    }
                    Bit_set(work_percheck, FILL_ALLOW_CHECK);
                }
            }
        }
        break;
    case 0x0a:    //WASH
			 if( Mode != MODE_NOWASH)
			 {
        Bit_clr(Alarm_sta, FILL_PUMP_ALARM);
        Bit_clr(Alarm_sta, WASH_PUMP_ALARM);
        Bit_clr(Alarm_sta, EMPTY_PUMP_ALARM);
        Bit_clr(Pump_state_flag, EMPTY_OK);
        Bit_clr(Alarm_sta, CELL_ALARM);
        Auto_refill_wait_flag = 0;
        Centrifugal_motor_stop_orde = 0 ; //允许继续启动
        if((work_sta != WORK_WASH) || (Work_Stop_Flag == 1) || (Pump_Pause == 1))
        {
            if((Work_Stop_Flag == 1) || (Pump_Pause == 1))
            {
            }
            else
            {
                Monitor_Bowl_Wash_Count = 0;
            }

            Work_Stop_Flag = 0;
            Pump_Pause = 0;
            sta = WORK;
            work_sta = WORK_WASH;
            Auto_run_step = AUTO_EMPTY;
            //Bit_clr(Pump_state_flag,WASH_OK);
            Monitor_Pump.Wash_Pump_count = 0;
            if(Bit_check_1(Pump_state_flag, WASH_OK))
                Set_Wash_ext_Volume = Set_Wash_ext_Volume + 300;
            else
                Set_Wash_ext_Volume = 0;
            Bit_clr(Pump_state_flag, WASH_OK);
            if(Mode == MODE_YINGJI)
            {
                Centrifugal_motor_orde = 1;
                Centrifugal_motor_speed_Set = 5650;
            }
            else if(READ_WELL_STATE() == 0)
            {
                Centrifugal_motor_orde = 1;
                Centrifugal_motor_speed_Set = 5650;
            }
            Bit_set(work_percheck, FILL_ALLOW_CHECK);
        }
			}
        break;
    case 0x0b:    //EMPTY
						 if( Mode != MODE_NOWASH)
			 {
        Bit_clr(Alarm_sta, FILL_PUMP_ALARM);
        Bit_clr(Alarm_sta, WASH_PUMP_ALARM);
        Bit_clr(Alarm_sta, EMPTY_PUMP_ALARM);
        Auto_refill_wait_flag = 0;
        Centrifugal_motor_stop_orde = 0;
        if((work_sta != WORK_EMPTY) || (Work_Stop_Flag == 1) || (Pump_Pause == 1))
        {
            Work_Stop_Flag = 0;
            sta = WORK;
            Pump_Pause = 0;
            Auto_run_step = AUTO_EMPTY;
            WASH_DISABLE();
            FILL_DISABLE();
            EMPTY_DISABLE();
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            work_sta = WORK_EMPTY;
            Monitor_Bowl_Empty_Volume = 0;
            Monitor_Pump.Empty_Pump_count = 0; //清零蠕动泵强制计数
            //Monitor_Empty_Force_Count=0;
            Centrifugal_motor_orde = 2;
            Bit_set(work_percheck, FILL_ALLOW_CHECK);
        }
        break;
    case 0x0c:    //conc 浓缩
						 if( Mode != MODE_NOWASH)
			 {
        Bit_clr(Alarm_sta, FILL_PUMP_ALARM);
        Bit_clr(Alarm_sta, WASH_PUMP_ALARM);
        Bit_clr(Alarm_sta, EMPTY_PUMP_ALARM);
        Bit_clr(Pump_state_flag, EMPTY_OK);
        Centrifugal_motor_stop_orde = 0;
        GPIO_ResetBits(GPIOE , GPIO_Pin_11);
        if((work_sta != WORK_CONC) || (Work_Stop_Flag == 1) || (Pump_Pause == 1))
        {
            sta = WORK;
            Auto_run_step = AUTO_FILL;
            Work_Stop_Flag = 0;
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            WASH_DISABLE();
            FILL_DISABLE();
            EMPTY_DISABLE();
            work_sta = WORK_CONC;
            Pump_Pause = 0;
            if(Mode == MODE_YINGJI)
            {
                Centrifugal_motor_orde = 1;
                Centrifugal_motor_speed_Set = 5650;
            }
            else if(READ_WELL_STATE() == 0)
            {
                Centrifugal_motor_orde = 1;
                Centrifugal_motor_speed_Set = 5650;
            }
            Bit_set(work_percheck, FILL_ALLOW_CHECK);
        }
			}
		}
        break;
    case 0x0d:    //回血//紧急
						 if( Mode != MODE_NOWASH)
			 {
        Bit_clr(Alarm_sta, FILL_PUMP_ALARM);
        Bit_clr(Alarm_sta, WASH_PUMP_ALARM);
        Bit_clr(Alarm_sta, EMPTY_PUMP_ALARM);
        Bit_clr(Pump_state_flag, EMPTY_OK);
        Auto_refill_wait_flag = 0;
        Auto_run_step = AUTO_RETURN;
        Centrifugal_motor_stop_orde = 0;
        Bit_clr(Pump_state_flag, RETURN_OK);
        if((Mode != MODE_NOWASH) || (Work_Stop_Flag == 1) || (Pump_Pause == 1))
        {
            if(work_sta != WORK_RETURN)
            {
                Work_Stop_Flag = 0;
                sta = WORK;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
                WASH_DISABLE();
                FILL_DISABLE();
                EMPTY_DISABLE();
                work_sta = WORK_RETURN;
                Pump_Pause = 0;
                Centrifugal_motor_orde = 2;
                Bit_set(work_percheck, FILL_ALLOW_CHECK);
            }
        }
			}
        break;
    case 0x10:    //SETING
        GUI_State = GUI_SET;
        set_mode = 0;
        GUI_Set_State = GUI_SET_MODE;
        Show_pic_num = 3;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    case 0x05:    //
        if(Set_Bowl == 225)
        {
            Set_Bowl = 125;
            if(Set_Prefill_Volume == 300)
            {
                Pre_fill_Volume = 300;
            }
            else
            {
                Pre_fill_Volume = Set_Prefill_Volume;
            }
            Set_Wash_Volume = 800;
            Set_Run_Volume = 600;
        }
        else
        {
            Set_Bowl = 225;
            if(Set_Prefill_Volume == 300)
            {
                Pre_fill_Volume = 200;
            }
            else
            {
                Pre_fill_Volume = Set_Prefill_Volume;
            }
            Set_Wash_Volume = 1000;
            Set_Run_Volume = 800;
        }
        break;
    case 0xfd:    //MODE
        GUI_State = GUI_MODE_SET;
        Show_pic_num = 3;
        Display_Loop = 0;
        show_pic_flag = 0;
        break;
    case 0x07:    //
        switch(work_sta)
        {
        case WORK_PRE_FILL:
            if(Set_Wash_speed < 1000)
                Set_Wash_speed += 10;
            break;
        case WORK_WASH:
            if(Set_Wash_speed < 1000)
            {
                Set_Wash_speed += 10;
                if(Set_Wash_speed > 50)
                    Set_Wash_speed_adj = Set_Wash_speed - 50;
            }
            break;
        case WORK_FILL:
        case WORK_RETURN:
            if(Set_Fill_speed < 1000)
                Set_Fill_speed += 10;
            break;
        case WORK_EMPTY:
        case WORK_CONC:
            if(Set_Empty_speed < 1000)
                Set_Empty_speed += 10;
            break;
        default:
            break;
        }
        speed_change_flag = 1;
        break;
    case 0x08:    //MODE
        switch(work_sta)
        {
        case WORK_PRE_FILL:
            if(Set_Wash_speed > 50)
                Set_Wash_speed -= 10;
            break;
        case WORK_WASH:
            if(Set_Wash_speed > 50)
                Set_Wash_speed -= 10;
            if(Set_Wash_speed > 50)
                Set_Wash_speed_adj = Set_Wash_speed - 50;
            break;
        case WORK_FILL:
        case WORK_RETURN:
            if(Set_Fill_speed > 50)
                Set_Fill_speed -= 10;
            break;
        case WORK_EMPTY:
        case WORK_CONC:
            if(Set_Empty_speed > 50)
                Set_Empty_speed -= 10;
            break;
        default:
            break;
        }
        speed_change_flag = 1;
        break;
    case 0x03:        //停止
        if(Mode != MODE_AUTO)
            work_sta = WORK_STANDBY;
        if(Mode == MODE_AUTO)
            Work_Stop_Flag = 1;
        Pump_Pause = 0;
        Stop();
        Centrifugal_motor_orde = 2; //离心电机停止
        Bit_clr(Alarm_sta, LIXIN_ALARM);
        Bit_clr(Alarm_sta, AIR_ALARM);
        Bit_clr(Alarm_sta, CELL_ALARM);
        Bit_clr(Alarm_sta, WASH_AIR_ALARM);
        Bit_clr(Alarm_sta, FILL_AIR_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
        Bit_clr(Alarm_sta, PRE_FILL_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
        Bit_clr(Alarm_sta, AIR_ALARM);
        if(Bit_check_1(Bowl_State, FILL_BLOOD))  //设置蠕动泵延时标志
        {
            Bit_set(Bowl_State, 1);               //再次启动时需要延时蠕动泵
            Bit_clr(Bowl_State, 2);               //清除正在延时标志位
            pump_dely_time = 50;
        }
        break;
    case 0x02:    //暂停 继续
        if(Mode == MODE_AUTO)
        {
            if(Work_Stop_Flag == 1) //stoping
            {
                Work_Stop_Flag = 0;

                reservoir_delay_check_cont = 0;
                if(Bit_check_1(Bowl_State, 2)) //启动泵延时中，按继续案件取消延时
                {
                    pump_dely_time = 0;
                }
                Pump_Pause = 0;
                Bit_set(work_percheck, FILL_ALLOW_CHECK);
                Monitor_Pump.Empty_Pump_count = 0;
                Monitor_Pump.Fill_Pump_count = 0;
                Monitor_Pump.Wash_Pump_count = 0;
                Bit_clr(Alarm_sta, WASH_AIR_ALARM);
                Bit_clr(Alarm_sta, FILL_AIR_ALARM);
                Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
                Bit_clr(Alarm_sta, PRE_FILL_ALARM);
                Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
                Bit_clr(Alarm_sta, FILL_PUMP_ALARM);

                if(work_sta == WORK_PRE_FILL || work_sta == WORK_FILL || work_sta == WORK_WASH || work_sta == WORK_CONC)
                {
                    if( READ_WELL_STATE() == FLUIT_NO)
                    {
                        Centrifugal_motor_orde = 1;
                        Centrifugal_motor_speed_Set = 5650;
                    }
                }
            }
            else
            {
                if(Pump_Pause == 1)         //当前为暂停状态
                {

                    if(Bit_check_1(Bowl_State, 2)) //启动泵延时中，按继续案件取消延时
                    {
                        pump_dely_time = 0;
                    }
                    Pump_Pause = 0;
                    reservoir_delay_check_cont = 0;
                    Bit_set(work_percheck, FILL_ALLOW_CHECK);
                    Monitor_Pump.Empty_Pump_count = 0;
                    Monitor_Pump.Fill_Pump_count = 0;
                    Monitor_Pump.Wash_Pump_count = 0;
                    Bit_clr(Alarm_sta, WASH_AIR_ALARM);
                    Bit_clr(Alarm_sta, FILL_AIR_ALARM);
                    Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
                    Bit_clr(Alarm_sta, PRE_FILL_ALARM);
                    Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
                    Bit_clr(Alarm_sta, FILL_PUMP_ALARM);

                    if(work_sta == WORK_PRE_FILL || work_sta == WORK_FILL || work_sta == WORK_WASH || work_sta == WORK_CONC)
                    {
                        if( READ_WELL_STATE() == FLUIT_NO)
                        {
                            Centrifugal_motor_orde = 1;
                            Centrifugal_motor_speed_Set = 5650;
                        }
                    }
                }
                else
                    Pump_Pause = 1;
            }
        }
        else
        {
            if(Work_Stop_Flag == 1) //stoping
            {
                Work_Stop_Flag = 0;
            }
            if(work_sta != WORK_STANDBY)
            {

                if(Pump_Pause == 1)         //当前为暂停状态
                {
                    if(Bit_check_1(Bowl_State, 2)) //启动泵延时中，按继续案件取消延时
                    {
                        pump_dely_time = 0;
                    }
                    Pump_Pause = 0;
                    reservoir_delay_check_cont = 0;
                    Bit_set(work_percheck, FILL_ALLOW_CHECK);
                    Monitor_Pump.Empty_Pump_count = 0;
                    Monitor_Pump.Fill_Pump_count = 0;
                    Monitor_Pump.Wash_Pump_count = 0;
                    Bit_clr(Alarm_sta, WASH_AIR_ALARM);
                    Bit_clr(Alarm_sta, FILL_AIR_ALARM);
                    Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
                    Bit_clr(Alarm_sta, PRE_FILL_ALARM);
                    Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
                    Bit_clr(Alarm_sta, FILL_PUMP_ALARM);
                    Bit_clr(Alarm_sta, WASH_PUMP_ALARM);
                    Bit_clr(Alarm_sta, EMPTY_PUMP_ALARM);

                    if(work_sta == WORK_PRE_FILL || work_sta == WORK_FILL || work_sta == WORK_WASH || work_sta == WORK_CONC)
                    {
                        //if( READ_WELL_STATE()==FLUIT_NO)
                        {
                            Centrifugal_motor_orde = 1;
                            Centrifugal_motor_speed_Set = 5650;
                        }
                    }
                }
                else
                {
                    Pump_Pause = 1;
                }
            }
        }
        break;
    case 0x0e:
        if(print_flag == 0)
            print_flag = 1;
        break;
    case 0x06:
        if(Mode == MODE_AUTO)
        {
            Mode = MODE_MU;
            if(Work_Stop_Flag == 1)
            {
                Work_Stop_Flag = 0;
                work_sta = WORK_STANDBY;
            }
            if(work_sta == WORK_STANDBY)
            {
                Centrifugal_motor_orde = MOTOR_STOP;
            }
            Auto_run_step_cont = 0;
        }
        else if(Mode == MODE_MU)
        {
            Mode = MODE_AUTO;
            Auto_run_step_cont = 0;
            Bit_set(Set_Alarm_flag, AIR_ALARM_SW);
            Bit_set(Set_Alarm_flag, BLOOD_ALARM_SW);
            Bit_set(Set_Alarm_flag, LOUYE_ALARM_SW);
            Bit_set(Set_Alarm_flag, WELL_ALARM_SW);
            // Bit_Set(Set_Alarm_flag,WELL_ALARM_SW);
            switch(work_sta)
            {
            case WORK_STANDBY:
            case WORK_RETURN:
            // case WORK_CONC:
            case WORK_JINJI:
                Auto_run_step = 0;

                work_sta = WORK_STANDBY;
                break;
            case WORK_FILL:
            case WORK_CONC:
                Auto_run_step = AUTO_FILL;
                break;
            case WORK_WASH:
                Auto_run_step = AUTO_WASH;
                break;
            case WORK_EMPTY:
                Auto_run_step = AUTO_EMPTY;
                break;
            default:
                break;
            }
        }
        break;
    case 0x04:
        Show_pic_num = 41;
        Display_Loop = 0;
        show_pic_flag = 0;
        GUI_State = GUI_HELP;
        break;
    case 0xee:
        main_dbug = 1;
        break;
    default:
        break;
    }
}
/**********************************************/
/* 函数名称: UART3_Normal_Test_Check         */
/* 功能描述: 串口3 读接收到的指令            */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void Touch_Key_Welcome(void)
{
    switch(key_buf[2])
    {
    case 0x03:
        Fac_set_num++;
        if(Fac_set_num == 15)
        {
            Show_pic_num = 9;
            Display_Loop = 0;
            show_pic_flag = 0;
            GUI_State = GUI_FACTORY;
            sta = FACTORY;
        }
        break;
    case 0x01:
        if(poweroff_time < 100)
        {
            Monitor_Empty_count = 0;
            Monitor_Wash_count = 0;
            Monitor_Fill_count = 0;
            prefill_flag = 1;
            sta = TEST_STA;
            GUI_State = GUI_TEST;
            Show_pic_num = 1;
            Display_Loop = 0;
            show_pic_flag = 0;
        }
        else
        {
            GUI_State = GUI_TEST;
            sta = TEST_STA;
            Show_pic_num = 1;
            Display_Loop = 0;
            show_pic_flag = 0;
        }
        break;
    case 0x02:
        if(poweroff_time < 100)
        {
            sta = WORK;
            //界面控制相关
            work_sta = WORK_STANDBY;
            GUI_State = GUI_STANDBY;
            Show_pic_num = 2;
            show_pic_flag = 0;
            Display_Loop = 0;
        }
        break;
    default:
        break;
    }

}
/**********************************************/
/* 函数名称: UART3_Normal_Test_Check         */
/* 功能描述: 串口3 读接收到的指令            */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void Touch_Key_Huanzhe(void)
{

    switch(key_buf[2])
    {
    case 0x00:    //清除
        switch(Set_pra_now)
        {
        case SET_patients:
            Set_person[0] = '\0';
            Set_person_Num = 0;
            break;
        case SET_NAME:
            Set_person_name[0] = '\0';
            Set_person_name_Num = 0;
            break;
        case SET_AGE:
            Set_person_age[0] = '\0';
            Set_person_age_Num = 0;
            break;
        }
        break;


    case 0x01:  //退格
        switch(Set_pra_now)
        {
        case SET_patients:
            if(Set_person_Num > 0)
            {
                Set_person_Num--;
                if(Set_person_Num == 0)
                    Set_person[Set_person_Num] = '\0';
            }
            Set_person[Set_person_Num] = '\0';
            break;
        case SET_NAME:
            if(Set_person_name_Num > 0)
            {
                Set_person_name_Num--;
                if(Set_person_name_Num == 0)
                    Set_person_name[Set_person_name_Num] = '\0';
            }
            Set_person_name[Set_person_name_Num] = '\0';
            break;
        case SET_AGE:
            if(Set_person_age_Num > 0)
            {
                Set_person_age_Num--;
                if(Set_person_age_Num == 0)
                    Set_person_age[Set_person_age_Num] = '\0';
            }
            Set_person_age[Set_person_age_Num] = '\0';
            break;
        }
        break;
    //		case 0x02://确定
    //
    //          break;
    case 0x03:
        Set_pra_now = SET_patients;
        break;
    case 0x04:
        Set_pra_now = SET_KESHI;
        break;
    case 0x05:
        Set_pra_now = SET_NAME;
        break;
    case 0x07:
        Set_pra_now = SET_AGE;
        break;
    case 0x06:
        Set_pra_now = SET_SEX;
        break;
    case 0x02:
        switch(Set_pra_now)
        {
        case SET_patients:
            Bit_set(Save_to_EEP_flag, 0);
            break;
        }
        break;
    default:
        if(CommRecvBuf[2] > 0x2f)
        {
            switch(Set_pra_now)
            {
            case SET_patients:
                Set_person[Set_person_Num] = CommRecvBuf[2];
                if(Set_person_Num < 10)
                    Set_person_Num++;
                Set_person[Set_person_Num] = '\0';
                break;
            case  SET_NAME:
                Set_person_name[Set_person_name_Num] = CommRecvBuf[2];
                if(Set_person_name_Num < 10)
                    Set_person_name_Num++;
                Set_person_name[Set_person_name_Num] = '\0';
                break;
            case  SET_AGE://
                if((CommRecvBuf[2] > 0x30) && (CommRecvBuf[2] < 0x40))
                {
                    Set_person_age[Set_person_age_Num] = CommRecvBuf[2];
                    if(Set_person_age_Num < 3)
                        Set_person_age_Num++;
                    Set_person_age[Set_person_age_Num] = '\0';
                }
                break;
            case  SET_SEX://
                if(CommRecvBuf[2] == 0x31)
                    person_sex = 2;
                else if(CommRecvBuf[2] == 0x32)
                    person_sex = 1;

                break;
            case SET_KESHI:
                if((CommRecvBuf[2] > 0x30) && (CommRecvBuf[2] < 0x36))
                    Set_keshi = CommRecvBuf[2];
                break;
            default:
                break;
            }
        }
        break;
    }

}
/**********************************************/
/* 函数名称: UART3_Normal_Test_Check         */
/* 功能描述: 串口3 读接收到的指令            */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void Touch_Key_Tend(void)
{
    switch(key_buf[2])
    {
    case 0x03:
        print_flag = 2;
        break;
    case 0x01:
        if(GUI_Tend_num > 0)
            GUI_Tend_num--;

        break;
    case 0x02:
        if(GUI_Tend_num < Tend_num)
            GUI_Tend_num++;
        break;
    default:
        break;
    }

}

/**********************************************/
/* 函数名称: UART3_Normal_Test_Check         */
/* 功能描述: 串口3 读接收到的指令            */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void Touch_Key_Vidio(void)
{
    switch(key_buf[2])
    {

    case 0x10:
        GUI_State = GUI_VIDIO_HELP;
        Display_Loop = 0;
        show_pic_flag = 0;
        Show_pic_num = 45;
        break;
    case 0x01:
        if(vido_run_falg == 0)
        {
            Tx_Byte(0xAA);
            Tx_Byte(0xBB);
            Tx_Byte(0X05);
            Tx_Byte(0X11);
            Tx_Byte(0x00);
            Tx_Byte(0x00);
            Tx_Byte(0X00);
            Tx_Byte(0X00);
            delay_ms(20);
            Tx_Byte(0xAA);
            Tx_Byte(0xBB);
            Tx_Byte(0X01);
            Tx_Byte(0X16);
            vido_run_falg = 1;
            vido_stop_flag = 1;
        }
        else
        {
            Tx_Byte(0xAA);
            Tx_Byte(0xBB);
            Tx_Byte(0X01);
            Tx_Byte(0X18);
            vido_stop_flag = 1;

        }
        break;
    case 0x02:
        Tx_Byte(0xAA);
        Tx_Byte(0xBB);
        Tx_Byte(0X01);
        Tx_Byte(0X14);


        break;
    case 0x03:
        Tx_Byte(0xAA);
        Tx_Byte(0xBB);
        Tx_Byte(0X01);
        Tx_Byte(0X15);
        break;
    case 0x04:
        Tx_Byte(0xAA);
        Tx_Byte(0xBB);
        Tx_Byte(0X01);
        Tx_Byte(0X1A);
        break;
    case 0x05:
        Tx_Byte(0xAA);
        Tx_Byte(0xBB);
        Tx_Byte(0X01);
        Tx_Byte(0X1B);
        break;
    case 0x06:
        if(vido_stop_flag == 1)
        {
            Tx_Byte(0xAA);
            Tx_Byte(0xBB);
            Tx_Byte(0X01);
            Tx_Byte(0X19);
            delay_ms(20);
            Tx_Byte(0xAA);
            Tx_Byte(0xBB);
            Tx_Byte(0X01);
            Tx_Byte(0X01);
            delay_ms(300);
            delay_ms(30);
            vido_run_falg = 0;
            vido_stop_flag = 0;
        }
        //	else
        {
            //vido_stop_flag=1;
            GUI_State = GUI_HELP;
            Display_Loop = 0;
            show_pic_flag = 0;
            Show_pic_num = 41;

        }
        break;
    case 0x07:
        if(Help_key_show == 1)
        {
            Tx_Byte(0xAA);
            Tx_Byte(0xBB);
            Tx_Byte(0X01);
            Tx_Byte(0X08);

            Help_key_show = 0;
        }
        else
        {
            Tx_Byte(0xAA);
            Tx_Byte(0xBB);
            Tx_Byte(0X03);
            Tx_Byte(0X07);
            Tx_Byte(0xff);
            Tx_Byte(0xff);
            Tx_Byte(0Xff);
            Tx_Byte(0Xff);
            Help_key_show = 1;
        }
        break;
    case 0x08:
        Tx_Byte(0xAA);
        Tx_Byte(0xBB);
        Tx_Byte(0X03);
        Tx_Byte(0X07);
        Tx_Byte(0xff);
        Tx_Byte(0xff);
        Tx_Byte(0Xff);
        Tx_Byte(0Xff);

        break;
    case 0x09:
        if(vido_stop_flag == 0)
        {
            GUI_State = GUI_HELP;
            Display_Loop = 0;
            show_pic_flag = 0;
            Show_pic_num = 41;
        }
        break;

    default:
        break;
    }
}
//switch(CommRecvBuf[2])
//	     {
//		      case 0x11:    //停止
//                work_sta=25;
//					      Pump_Pause=0;
//						    Centrifugal_motor_orde=2;
//					 		  if(Bit_check_1(Bowl_State,FILL_BLOOD))   //设置蠕动泵延时标志
//								{
//									Bit_set(Bowl_State,1);                //再次启动时需要延时蠕动泵
//									Bit_clr(Bowl_State,2);                //清除正在延时标志位
//									pump_dely_time=50;
//								}
//						    break;
//		       case 0x12:    //
//			          GUI_State=GUI_HELP;
//						    Show_pic_num=33;
//						    Display_Loop=0;
//						    show_pic_flag=0;
//						    break;
//					 case 0x20:    //返回
//			          GUI_State=GUI_STANDBY;
//						    Show_pic_num=1;
//						    Display_Loop=0;
//						    show_pic_flag=0;
//			           break;
//					 case 0x21:
//						 		GUI_State=GUI_SET;
//					      GUI_Set_State=GUI_SET_MODE;
//						    Show_pic_num=3;
//						    Display_Loop=0;
//						    show_pic_flag=0;
//			           break;
//					 case 0x22:
//						 		GUI_State=GUI_SET;
//					      GUI_Set_State=GUI_SET_PRA;
//						    Show_pic_num=4;
//						    Display_Loop=0;
//						    show_pic_flag=0;
//			           break;
//					 case 0x23:
//						 		GUI_State=GUI_SET;
//					 			GUI_Set_State=GUI_SET_HUANZHE;
//						    Show_pic_num=6;
//						    Display_Loop=0;
//						    show_pic_flag=0;
//			           break;
//					 case 0x24:
//						 		GUI_State=GUI_SET;
//					      GUI_Set_State=GUI_SET_TEND;
//						    Show_pic_num=7;
//						    Display_Loop=0;
//						    show_pic_flag=0;
//			           break;
//					 case 0x26://baojing
//						 		GUI_State=GUI_SET;
//					      GUI_Set_State=GUI_SET_ARLAM;
//						    Show_pic_num=5;
//						    Display_Loop=0;
//						    show_pic_flag=0;
//			           break;
//					 case 0x27:
//						 		GUI_State=GUI_SET;
//						    Show_pic_num=8;
//						    Display_Loop=0;
//						    show_pic_flag=0;
//			           break;
//					 default:break;
//				 }
/**********************************************/
/* 函数名称: Touch_Key_analysis_8200B         */
/* 功能描述: 串口3 读接收到的指令            */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void Touch_Key_analysis_8200B(void)
{
    //uint8 Key_flag;
    uint16 Touch_Key_delay_cont = 0;

    if(ComReadCmd() == RECV_CMD_ENDDATA)
    {
        Key_flag = 1;
        key_buf[1] = CommRecvBuf[1];
        key_buf[2] = CommRecvBuf[2];
    }

    if(Touch_Key_delay_cont > 0)
    {
        Touch_Key_delay_cont--;
        ComReadCmd();
    }
    else
    {
        if(Key_flag == 1)
        {
            Key_flag = 0;
            Key_save_flag = 1;
            Key_num = (CommRecvBuf[1] << 8) + CommRecvBuf[2];
            switch(key_buf[1])
            {
            case 0x00://欢迎界面
                Touch_Key_Welcome();
                break;
            case 0x01: //自检界面
                Touch_Key_Test();
                break;
            case 0x02:
                Touch_Key_Main();
                break;
            case 0x03:
                Touch_Key_Set();
                break;
            case 0x04:
                //Touch_Key_Set_Alarm();
                Touch_Key_Set_Pra();
                break;
            case 0x05:
                Touch_Key_Set_Alarm();
                break;
            case 0x06:     //huanzhe
                Touch_Key_Huanzhe();
                break;
            case 0x07:     //huanzhe
                Touch_Key_Tend();
                break;
            case 0x08:
                Touch_Key_Set_Time_8200b();
                break;
            case 0x09:
                Touch_Key_Factory();
                break;
            case 0x21:
                switch(key_buf[2])
                {
                case 0x01:
                    GUI_State = GUI_STANDBY;
                    Show_pic_num = 2;
                    Display_Loop = 0;
                    show_pic_flag = 0;

                    break;
                default:
                    break;
                }
                break;
            case 0x45:  //shipin
                Touch_Key_Vidio();
                break;
            default:
                break;
            }
        }
    }
}


void Touch_Key_analysis(void)
{
    ;;;;

}

