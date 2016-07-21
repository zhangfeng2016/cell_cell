/*************** Radio File (c) ***************/
/**                                          **/
/**      基于STM32开发的血液回收机           **/
/**        Email:287285415@qq.com            **/
/**                                          **/
/**--------------文件信息--------------------**/
/**文    件   名: User_dispaly.c              **/
/**创    建   人: 张峰                       **/
/**创  建  日 期: 2014年7月11日              **/
/**描         述:  所有函数都能用到的全局变量**/
/**                或者函数                  **/
/**********************************************/


#include "user_conf.h"
time_type timer;
time_type Time_Set;
time_type prt_Time;
time_type power_on_Time;
uint32 power_on_Time_sec;

////////////////////////////////////////
uint8 Default_Pump_set = PEMP_CHECK_DISABLE; //没有测速版
uint8 Series_No[20];
uint8 Fac_Series_No[5];
uint8 Default_Mode_set;
uint8 Wash_cell_Protect;
uint16 Air_check_gate = 1700;
uint16 Cell_check_gate;


///////////////////////////////////////

//位变量
uint16 Time_flag;    //bit valu
uint8 SecondSign;
uint32 poweroff_time = 0;
uint8 RCC_SecondSign;
uint8 Tend_num;
uint8 GUI_Tend_num;
uint8 set_mode;
//全局变量 允许整机的函数调用
uint8  sta;          //整机状态寄存器//
//0  自检
//1  工厂模式
//3  待机
//4

uint8   Mode;
uint8   work_sta;     //整机工作状态
uint8   Pump_Sta;
uint8   pump_flag;
uint8   red_pump_low_speed_flag = 0;
uint8   red_pump_low_speed;
uint8   Pump_choose;
uint32  Alarm_sta;
uint32  Alarm_switch; //报警开关标志位，可以控制
uint16  Moto_step_cont;
uint16  step_speed_set;
uint8   Auto_run_step;
uint16  Auto_run_step_cont;
uint8   Set_pra_now;
uint8   Set_pra_now_buf;

uint8  work_percheck;
uint16 Pump_state_flag;
uint8  Bowl_State;
uint8  pump_dely_time;
uint8  Auto_refill_wait_flag;
uint16 Auto_refill_wait_time;
uint8  Auto_check_bowl_falg;
uint8  Well_state_flag;

uint8  GUI_State;
uint8  GUI_Set_State;
uint8  Show_pic_num;
uint8  Show_donghua_num;
uint8  Set_person[15] = {'_', '\0', '-', '-', '-', '-', '-', '-', '-', '\0'};
uint8  Set_person_name[15] = {'-', '\0', '-', '-', '-', '-', '-', '-', '-', '\0'};
uint8  Set_person_age[3] = {'-', '\0', '-'};
uint8  Tend_Set_person[10] = {'-', '-', '-', '-', '-', '-', '-', '-', '-', '\0'};
uint8  person_sex;
uint8  Set_keshi;
uint8  Set_person_Num;
uint8  Set_person_name_Num;
uint8  Set_person_age_Num;
uint8   line_select = 7;
uint16 Save_to_EEP_flag;

uint8  Centrifugal_motor_orde;
uint8  Centrifugal_motor_stop_flag;
uint8  Centrifugal_motor_stop_orde;
uint16 Centrifugal_motor_speed_Set;
uint8  Centrifugal_motor_speed_Adj;
uint16 motor_speed_step;
uint8  RS_485_flag;    //位变量


uint8  Test_Step = 1;

uint8 Test_step_retest;
uint8 Test_step_next;
uint8 test_time;
uint16 Test_Flag;
uint16 Mon_Pump_rpm;

uint8 Factory_Auto_step;
uint16 Factory_test_time;
uint8 Factory_autorun_flag;


///////////////////设定参数_开始///////////////////////////////
uint8  speed_change_flag;
uint8  Pump_Pause;
uint8  Work_Stop_Flag;
uint16 Set_Pump_Test_speed = 300;
uint16 Set_Fill_speed;
uint16 Set_Wash_speed;
uint16 Set_Wash_speed_adj = 50;
uint16 Set_Empty_speed;
uint16 Set_Wash_Volume = 1500;
uint16 Set_Wash_ext_Volume = 0;
uint16 Set_Prefill_Volume;
uint8  Set_Arlarm_ON_OFF = 1;
uint16 Set_Run_Volume;
uint16 Set_feiye;;
uint8  Set_Bowl;
uint8  prefill_flag = 1;
uint16 speed_now = 100;
uint16 Pre_fill_Volume;
uint16 Set_Alarm_flag;
uint8  seting_flag;
uint16 wash_pump_adjust = 100;
uint16 fill_pump_adjust = 100;
uint16 empty_pump_adjust_125;
uint16 empty_pump_adjust_225;


/////////////////监测参适?/////////////////////////////////uint
uint8  Monitor_Fill_buf_Flag;
uint8  Monitor_Wash_buf_Flag;
uint8  Monitor_Empty_buf_Flag;
uint16 Monitor_Fill_speed;
uint16 Monitor_Wash_speed;
uint16 Monitor_Empty_speed;

fp32 Monitor_Fill_Volume;
fp32 Monitor_Wash_Volume;
fp32 Monitor_Empty_Volume;
fp32 Monitor_Waste_Volume;
fp32 Monitor_Pre_fill_Volume;


fp32 Monitor_Bowl_Fill_Volume;
fp32 Monitor_Bowl_Wash_Volume;
fp32 Monitor_Bowl_Empty_Volume;

uint16 Monitor_Bowl_Fill_Count;
uint16 Monitor_Bowl_Wash_Count;
uint16 Monitor_Bowl_Empty_Count;

uint16 Monitor_Empty_Force_Count;

uint16 Monitor_Fill_count;
uint16 Monitor_Wash_count;
uint16 Monitor_Empty_count;
uint16 Monitor_Pre_fill_count;
uint16 Monitor_Aujst_count;


uint16 Monitor_Fill_time;
uint16 Monitor_Pre_Fill_time;
uint16 Monitor_Wash_time;
uint16 Monitor_Empty_time;
uint16 Monitor_Conc_time;
uint16 Monitor_Return_time;

uint16 Pump_puls_count;

uint16 Monitor_Reservoir_weigh;
uint16 Monitor_Green_AD;
uint16 Monitor_Red_AD;
uint16 Monitor_Air_AD;
uint16 Monitor_Prees_AD;
uint16 Monitor_Blood_AD;
uint16 Monitor_Dwon_weigh;
uint16 Monitor_P12_AD;

uint8  Wash_Bowl_Num;
uint8  reservoir_state;
uint16 monitor_reservoir_ml;
uint32 monitor_weast_ml;

uint16 reservoir_delay_check_cont = 0;
uint16 Red_Tend[5];
uint16 Green_Tend[5];
uint16 Centrifugal_motor_speed;


uint16 Reservoir_zero;
uint16 Reservoir_500;
uint16 Reservoir_leftover;

uint16 Dwon_weigh_7000;
uint16 Dwon_weigh_9000;

uint8 Cell_line;
uint8 print_flag;
uint8 main_dbug;
uint8 main_dbug_cont;
uint8 Alarm_dip_now = 1;

GPIO_TypeDef  *Pump_GPIOx_1;
uint16  GPIOx_1_Pin;
GPIO_TypeDef  *Pump_GPIOx_2;
uint16  GPIOx_2_Pin;
//unsigned char code_DataStr[11];
//unsigned char code_TimeStr[11];


uint8 iic_save;
uint16 iic_save1;
uint8 back1;
uint16 back2;
//Set_par_type Set_par;

//////////////////////趋势查看参数/////////////////////////
uint16 Tend_Fill_Volume;
uint16 Tend_Wash_Volume;
uint16 Tend_Empty_Volume;

uint8 show_pic_flag;
uint8 Display_Loop;
uint8 PWOER_ON_FLAG = 0;
uint8 PWOER_ON_cont;

uint8 Key_save_flag;
uint16 Key_num;
uint8 key_buf[3];
uint8 Key_flag;

