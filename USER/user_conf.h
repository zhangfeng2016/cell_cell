/********************************/
/*         用户包含头文件函数         */
/********************************/
#include "stm32f10x.h"
#include "..\Drive\Rcc.h"
#include "..\Drive\RTC.h"
//#include "..\Drive\ADC.h"
//#include "..\GUI\user_dispaly.h"





/********************************/
/*         关键字自定义         */
/********************************/
//********************************************************************************
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define TRUE  1
#define FALSE 0

#define   uint8      unsigned char       // 无符号8位整型变量
#define    int8      signed   char       // 有符号8位整型变量                     
#define  uint16     unsigned short       // 无符号16位整型变量                     
#define   int16     signed   short       // 有符号16位整型变量                     
#define  uint32       unsigned int       // 无符号32位整型变量                       
#define   int32       signed   int       // 有符号32位整型变量 
#define  uint64      unsigned long       // 无符号64位长整型变量        
#define    fp32              float       // 单精度浮点数（32位长度）                
#define    fp64             double       // 双精度浮点数（64位长度）                 

#define  Bit_set(y,x)		 	         (y) |= 1<<(x)		                     // 位设置
#define  Bit_clr(y,x)			         (y) &= ~(1<<(x))                      // 位清除
#define  Bit_check_1(y,x)		       ((y)&1<<(x)) == 1<<(x)              //位检测
#define  Bit_check_0(y,x)		       ((y)&1<<(x)) != 1<<(x)              //位检测
#define  Read_bit(y,x)		         (y)&1<<(x)) != 1<<(x)                     //位读取
#define  Data_Round(x)             (((x)/10)+((((x)%10)>4)?1:0))   //数据四舍五入
#define  lfBit_SetValue(y,x)       (y) = (((y)&0xF0) | (x))        //设置字节低4位的值
#define  hfBit_SetValue(y,x)       (y) = (((y)&0x0F) | ((x)<<4))	 //设置字节高4位的值
#define  TURE 	               1	 
#define  FALSE 	               0	 

#define  WEIGHTUP 	      ADC_Channel_8	   //C5
#define  AIRCHECK       ADC_Channel_14	   //C4
#define  GREENLED 	      ADC_Channel_4	     //A4
#define  REDLED	 	        ADC_Channel_5	   //A5
#define  WEIGHTDOWN 	    ADC_Channel_15
#define  PRESS_CHK	      ADC_Channel_1   //B0
#define  BLOODCHECK 	      ADC_Channel_9	   //B1


//管脚宏定义/////////
#define WASH_DISABLE()       GPIO_ResetBits(GPIOB, GPIO_Pin_8)          //
#define WASH_PUMP_ABLE()     GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define EMPTY_DISABLE()      GPIO_ResetBits(GPIOE , GPIO_Pin_2)
#define EMPTY_PUMP_ABLE()    GPIO_SetBits(GPIOE, GPIO_Pin_2)
#define FILL_DISABLE()       GPIO_ResetBits(GPIOD, GPIO_Pin_5)
#define FILL_PUMP_ABLE()     GPIO_SetBits(GPIOD, GPIO_Pin_5)

#define EMPTY_PUMP_DIR_FF()  GPIO_SetBits(GPIOE, GPIO_Pin_1)
#define EMPTY_PUMP_DIR_RR()  GPIO_ResetBits(GPIOE , GPIO_Pin_1)
#define FILL_PUMP_DIR_FF()   GPIO_SetBits(GPIOD, GPIO_Pin_4)
#define FILL_PUMP_DIR_RR()   GPIO_ResetBits(GPIOD , GPIO_Pin_4)

#define AIR_PULS_A_SET()     GPIOA->BSRR = GPIO_Pin_6;// GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define AIR_PULS_A_CLR()     GPIOA->BRR = GPIO_Pin_6; // GPIO_ResetBits(GPIOA , GPIO_Pin_6) 
#define AIR_PULS_B_SET()     GPIOA->BSRR = GPIO_Pin_7; //GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define AIR_PULS_B_CLR()     GPIOA->BRR = GPIO_Pin_7;// GPIO_ResetBits(GPIOA , GPIO_Pin_7)

#define CHECK_STOP()          GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)



//#define  EMPTY_PUMP_PULS_L()    GPIO_ResetBits(GPIOE , GPIO_Pin_2)
//#define  EMPTY_PUMP_PULS_H()    GPIO_SetBits(GPIOE , GPIO_Pin_2)

//#define  WASH_PUMP_PULS_L()     GPIO_ResetBits(GPIOB , GPIO_Pin_8)
//#define  WASH_PUMP_PULS_H()     GPIO_SetBits(GPIOB , GPIO_Pin_8)

//#define  FILL_PUMP_PULS_L()     GPIO_ResetBits(GPIOD , GPIO_Pin_4)
//#define  FILL_PUMP_PULS_H()     GPIO_SetBits(GPIOD , GPIO_Pin_4)


#define SPK2_1   GPIO_SetBits(GPIOE, GPIO_Pin_12)
#define SPK2_0   GPIO_ResetBits(GPIOE , GPIO_Pin_12)
#define SPK1_1   GPIO_SetBits(GPIOE, GPIO_Pin_13)
#define SPK1_0   GPIO_ResetBits(GPIOE , GPIO_Pin_13)

//#define READ_FUILT_UP_STATE()     (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0))
//#define READ_FUILT_DWON_STATE()   (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))
#define READ_FUILT_UP_STATE()     (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))
#define READ_FUILT_DWON_STATE()   (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3))
#define FLUIT_HAVE          1
#define FLUIT_NO            0

#define READ_WELL_STATE()   (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7))



////////////////////RS485_使能/////////////////////////
#define RS485_TX_EN()   GPIO_SetBits(GPIOA , GPIO_Pin_11)   //485发送允许
#define RS485_RX_EN()  GPIO_ResetBits(GPIOA , GPIO_Pin_11)  //485接收允许
/////////////////////////iIIC//////////////////////////////////BEGIN
//IO方向设置
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}

//IO操作函数
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 
/////////////////////////////////////////////////////////////////end


extern uint8 Default_Pump_set;
#define  PEMP_CHECK_ABLE        1
#define  PEMP_CHECK_DISABLE     0

extern uint8 Series_No[20];
extern uint8 Fac_Series_No[5];
extern uint8 Default_Mode_set;
extern uint8 Wash_cell_Protect;
#define  PROTECT_ABLE        0
#define  PROTECT_DISABLE     1
extern uint16 Air_check_gate;
extern uint16 Cell_check_gate;
///////////////////////////////
//变量定义
//
//
//
//////////////////////程序变量以及机器运行状态寄存器/////////////////////////
extern  u8 ver_str[50];
extern u8 ver_str_insine[4];
extern u8  hw_str[5];
extern uint32 poweroff_time;
extern uint8 SecondSign;
extern uint8 RCC_SecondSign;
extern uint8 Tend_num;
extern uint8 GUI_Tend_num;
extern uint16 Time_flag;        //位变量 //////与时间相关变量
#define TIME_5MS_FLAG        0    //bit0 5ms定时标志问
#define TIME_10MS_FLAG       1
#define TIME_1MS_FLAG        2
#define TIME_50MS_FLAG       3
extern uint8 sta;
#define STANDTBY             0
#define TEST_STA             1
#define FACTORY              2  //调试模式
#define WORK                 3
#define Patient_selection    4
extern uint8  work_sta;
#define WORK_FILL            2   //bit 0 
#define WORK_PRE_FILL        1   //BIT 1
#define WORK_STANDBY         0
#define WORK_WASH            3   //调试模式
#define WORK_EMPTY           4
#define WORK_RETURN          6
#define WORK_CONC            5  //浓缩
#define WORK_JINJI           7  //紧急
extern uint8  Pump_Sta;
#define PUMP_START           0   //bit 0 
#define PUMP_WORK            1   //BIT 1
#define PUMP_STOP            2
#define PUMP_PAUSE           3
#define PUMP_AFTER           4

extern uint8   pump_flag;
#define PUMP_START_OK           0   //bit 0 

extern uint8   red_pump_low_speed_flag;
extern uint8   red_pump_low_speed;
extern uint8 set_mode;
extern uint8  Mode;
#define MODE_AUTO    1
#define MODE_MU      2
#define MODE_YINGJI  3
#define MODE_NOWASH  4


extern uint8 Pump_choose;


extern uint8 GUI_State;
#define GUI_FACTORY   0
#define GUI_TEST      1
#define GUI_STANDBY   2
#define GUI_WORK      3
#define GUI_SET       4
#define GUI_MODE_SET  5
#define GUI_TIME_SET  6
#define GUI_NEW_OLD   7
#define GUI_JINJI     8
#define GUI_HELP      9
#define GUI_ARLAM_SET      10
#define GUI_VIDIO_HELP      11
#define GUI_FACTORY_1   20
#define GUI_0        0xff

extern uint8 GUI_Set_State;
#define GUI_SET_MODE      0
#define GUI_SET_PRA      1
#define GUI_SET_ARLAM     2
#define GUI_SET_HUANZHE      3
#define GUI_SET_TEND     4
#define GUI_SET_TIME      5
//#define GUI_SET      6
//#define GUI_SET      7
//#define GUI_SET      8


extern uint8 RS_485_flag;    //位变量
#define RS_485_ERR    7

extern uint8 Show_pic_num;
extern uint8 Show_donghua_num;
extern uint8 Set_person[15];
extern uint8  Set_person_name[15] ;
extern uint8  Set_person_age[3] ;
extern uint8 Tend_Set_person[10];
extern uint8 Set_person_Num;
extern uint8  Set_person_name_Num;
extern uint8  Set_person_age_Num;
extern uint8   line_select;
extern uint8  person_sex;
extern uint8  Set_keshi;

extern uint16  Save_to_EEP_flag;//bit
#define       SAVE_NUM           0
#define       SAVE_FILL_TIME     1
#define       SAVE_WASH_TIME     2
#define       SAVE_EMPTY_TIME    3
#define       SAVE_PUMP_ADJ      4
#define       SAVE_MODE_SET      5
#define       SAVE_PUMP_SET      6
#define       SAVE_LINXI_ADJ     7
#define       WWWWWWWWWWWWWW     8
#define       WWWWWWWWWWW        9
#define       WWWWWWWWWW        10
#define       SAVE_PREFILL_SET   12

extern uint16 Auto_run_step_cont;//用于记录自动启动延时
extern uint8 Auto_run_step;     //非位变量
#define       AUTO_RUN_START    0  //
#define       AUTO_FILL         1  //
#define       AUTO_WASH         2  //
#define       AUTO_EMPTY        3  //
#define       AUTO_RETURN        4  //
//#define       AUTO_FILL           4
//#define       AUTO_FILL           5
extern uint8   Set_pra_now_buf;
extern uint8 Set_pra_now;
#define       SET_FILL_FLOW          0  //
#define       SET_WASH_FLOW          1  //
#define       SET_EMPTY_FLOW         2  //
#define       SET_WASH_V             3  // 
#define        SET_AUTO_ML           4
#define       SET_BOWL               5  //
#define       SET_YEAR               6  //
#define       SET_MONTH              7  //
#define       SET_DAY                8
#define       SET_HOUR               9
#define       SET_MIN               10
#define       SET_SEC               11
#define       SET_FEIYE_ALARM       12
#define       SET_BLOOD_ALARM       13
#define       SET_AIR_ALARM         14
#define       SET_WELL_ALARM        15
#define       SET_LOUYE_ALARM       16
#define       SET_patients          17
#define       SET_NAME              18
#define       SET_AGE               19
#define       SET_KESHI             20
#define       SET_SEX               21

//extern uint8 Set_per_now;
extern uint8  work_percheck;
#define       FILL_ALLOW_CHECK   0

extern uint16 Pump_state_flag;
#define       FILLING           0  //0 非进血状态    1 进血状态
#define       FILL_OK           1  //0 进血未完成    1 进血完成
#define       PRE_FILLING       2  //0 非预充状态    1 预充状态
#define       PRE_FILL_OK       3  //0 预充未完成    1 预充完成
#define       WASHING           4
#define       WASH_OK           5
#define       EMPTYING          6
#define       EMPTY_OK          7
#define       RETURNING         8
#define       RETURN_OK         9
#define       CONCING          10
#define       CONC_OK          11

extern uint8  Bowl_State;
#define       FILL_BLOOD            0  //0 clr  离心杯内无血液    1 set  离心杯有血液
#define       PUMP_DELY_ABLE        1  //0 clr  离心杯内无血液    1 set  离心杯有血液
#define       PUMP_DELYING          2  //0 clr  no dely    1 set  delying
#define       PROC_ING              3  //处理中
#define       PROC_END              4   // 本杯处理完成，重新计数


extern uint8  pump_dely_time;
extern uint8  Auto_refill_wait_flag; //自动状态下使用
extern uint16 Auto_refill_wait_time;  //自动状态下使用，用于记录等待时间
extern uint8  Auto_check_bowl_falg;
#define BEGAIN_BIT  1
#define CHECKING_BIT  2


extern uint8 Well_state_flag;

extern uint8 Centrifugal_motor_orde;
#define MOTOR_STOP       2
#define MOTOR_START      1
extern uint8 Centrifugal_motor_stop_flag;
extern uint16 Centrifugal_motor_speed_Set;
extern uint8 Centrifugal_motor_speed_Adj;
extern uint16 motor_speed_step;
extern uint8 Centrifugal_motor_stop_orde;

extern uint8 Test_step_retest;
extern uint8 Test_step_next;
extern uint8 test_time;
extern uint8 Test_Step;
extern uint16 Test_Flag;
#define  FILL_TEST_OK      0  //1 OK    0 NG
#define  EMPTY_TEST_OK     2  //1 OK    0 NG
#define  WASH_TEST_OK      1  //1 OK    0 NG
#define  WELL_TEST_OK      3
#define  MOTO_TEST_OK      4
#define  LOUYE_TEST_OK      5



///////////////////设定参数_开始///////////////////////////////
extern uint8 speed_change_flag;
extern uint8 Pump_Pause;
extern uint8  Work_Stop_Flag;
extern uint16 Set_Pump_Test_speed;
extern uint16 Set_Fill_speed;
extern uint16 Set_Wash_speed;
extern uint16 Set_Wash_speed_adj;
extern uint16 Set_Empty_speed;
extern uint16 Set_Wash_Volume;
extern uint16 Set_Wash_ext_Volume;
extern uint16 Set_Prefill_Volume;
extern uint8  Set_Arlarm_ON_OFF;

#define ARLARM_ON    1
#define ARLARM_OFF   0


extern	uint16 wash_pump_adjust;
extern	uint16 fill_pump_adjust;
//extern	uint16 empty_pump_adjust;
extern uint16 empty_pump_adjust_125;
extern uint16 empty_pump_adjust_225;

extern uint8 seting_flag;
extern uint16 Set_Run_Volume;;
extern uint16  Set_feiye;
extern uint8  Set_Bowl;
extern uint8  prefill_flag;
extern uint16 speed_now;
extern uint16 Pre_fill_Volume;
extern uint16 Set_Alarm_flag;
#define       AIR_ALARM_SW         0     //气泡报警开关
#define       BLOOD_ALARM_SW       1     //血层报警开关
#define       LOUYE_ALARM_SW       2     //漏液报警开关
#define       WELL_ALARM_SW        3     //井盖报警开关
#define       FEIYE_ALARM_SW        4     //井盖报警开关
//#define  AIR_ALARM       4     //气泡报警开关
//#define  AIR_ALARM       5     //气泡报警开关
//#define  AIR_ALARM       6     //气泡报警开关

///////////////////设定参数_结束///////////////////////////////

/////////////////监测函数//////////////////////////////////
extern uint8 Monitor_Fill_buf_Flag;
extern uint8 Monitor_Wash_buf_Flag;
extern uint8 Monitor_Empty_buf_Flag;

extern uint16 Monitor_Fill_speed;
extern uint16 Monitor_Wash_speed;
extern uint16 Monitor_Empty_speed;
extern fp32 Monitor_Fill_Volume;
extern fp32 Monitor_Wash_Volume;
extern fp32 Monitor_Empty_Volume;
extern fp32 Monitor_Waste_Volume;
extern fp32 Monitor_Pre_fill_Volume;

extern uint16 Monitor_Fill_count;
extern uint16 Monitor_Wash_count;
extern uint16 Monitor_Empty_count;
extern uint16 Monitor_Pre_fill_count;
extern uint16 Monitor_Aujst_count;
extern uint16 Pump_puls_count;

extern fp32 Monitor_Bowl_Fill_Volume;
extern fp32 Monitor_Bowl_Wash_Volume;
extern fp32 Monitor_Bowl_Empty_Volume;

extern uint16 Monitor_Bowl_Fill_Count;
extern uint16 Monitor_Bowl_Wash_Count;
extern uint16 Monitor_Bowl_Empty_Count;

extern uint16 Monitor_Empty_Force_Count;

extern uint16 Monitor_Reservoir_weigh;
extern uint16 Monitor_Dwon_weigh;
extern uint16 Monitor_Green_AD;
extern uint16 Monitor_Red_AD;
extern uint16 Monitor_Air_AD;
extern uint16 Monitor_Prees_AD;
extern uint16 Monitor_Blood_AD;
extern uint16 Monitor_P12_AD;


extern uint16 Monitor_Fill_time;
extern uint16 Monitor_Pre_Fill_time;
extern uint16 Monitor_Wash_time;
extern uint16 Monitor_Empty_time;
extern uint16 Monitor_Conc_time;
extern uint16 Monitor_Return_time;
extern uint16 monitor_reservoir_ml_buf_1s[20];
extern uint8 reservoir_state;
//#define  RESERVOIR_ABLE      1
//#define  RESERVOIR_DISABLE    0
extern uint8 Wash_Bowl_Num;

extern uint16 monitor_reservoir_ml;
extern uint32 monitor_weast_ml;
extern uint16 reservoir_delay_check_cont;
extern uint16 Red_Tend[5];
extern uint16 Green_Tend[5];
extern uint16 Centrifugal_motor_speed;
extern uint16 Reservoir_zero;
extern uint16 Reservoir_500;
extern uint16 Reservoir_leftover;
extern uint16 Dwon_weigh_7000;
extern uint16 Dwon_weigh_9000;
extern uint8 Cell_line;
extern uint8 Alarm_dip_now;

extern uint8 print_able_flag;

extern GPIO_TypeDef  *Pump_GPIOx_1;
extern uint16  GPIOx_1_Pin;
extern GPIO_TypeDef  *Pump_GPIOx_2;
extern uint16  GPIOx_2_Pin;
///////////////报警变量//////////////////





extern uint32 Alarm_sta;
#define AIR_ALARM         0
#define WELL_ALARM        1
#define CELL_ALARM        2
#define FEIYE_ALARM       3
#define LIXIN_ALARM       4
#define FILL_AIR_ALARM    5
#define EMPTY_AIR_ALARM   6
#define WASH_AIR_ALARM    7
#define EMPTY_AIR_EARLY_ALARM    8
#define PRE_FILL_ALARM           9
#define FILL_PUMP_ALARM           10
#define WASH_PUMP_ALARM           11
#define EMPTY_PUMP_ALARM           12
//#define EMPTY_PUMP_ALARM           12
//#define EMPTY_PUMP_ALARM           12



#define louye_1_ALARM    16
#define louye_2_ALARM    17

extern uint32 Alarm_switch;

#define AIR_ALARM           0
#define WELL_ALARM          1
#define CELL_ALARM          2
#define FEIYE_ALARM         3
#define LIXIN_ALARM         4
//#define FILL_AIR_ALARM           5
//#define EMPTY_AIR_ALARM          6
//#define WASH_AIR_ALARM           7
//#define EMPTY_AIR_EARLY_ALARM    8
#define PRE_FILL_ALARM           9

#define louye_1_ALARM    16
#define louye_2_ALARM    17








extern uint16 Moto_step_cont;
extern uint16 step_speed_set;
extern uint16 Mon_Pump_rpm;
extern uint8 Factory_Auto_step;
extern uint16 Factory_test_time;
extern uint8 Factory_autorun_flag;

extern time_type timer;
extern time_type Time_Set;
extern time_type prt_Time;
extern time_type power_on_Time;
extern uint32 power_on_Time_sec;
extern uint8 main_dbug;
extern uint8 main_dbug_cont;
extern uint8 print_flag;
extern unsigned char code_DataStr[11];
extern unsigned char code_TimeStr[11];



extern uint8 iic_save;
extern uint16 iic_save1;
extern uint8 back1;
extern uint16 back2;
/////////////////////////////////趋势查看参数、、、
extern	uint16 Tend_Fill_Volume;
extern  uint16 Tend_Wash_Volume;
extern	uint16 Tend_Empty_Volume;

extern uint8 show_pic_flag;
extern uint8 Display_Loop;
extern uint8 PWOER_ON_FLAG;
extern uint8 PWOER_ON_cont;
extern	uint8 Key_save_flag;
extern	uint16 Key_num;
extern uint8 key_buf[3];
extern uint8 Key_flag;



