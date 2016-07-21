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
#include "User_dispaly.h"
#include "..\User\user_conf.h"
#include "M600_API.h"
#include "..\Drive\Uart.h"

time_type Tend_Time;
time_type Tend_fill_Time;
time_type Tend_wash_Time;
time_type Tend_empty_Time;

/***********************************************/
/* 函数名称: Save_Poweron_time                */
/* 功能描述: 存储数据                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：15年06月24日  //                  */
/***********************************************/
time_type Read_Poweron_time(uint8 tend_num);
/***********************************************/
/* 函数名称: Read_Poweron_time                 */
/* 功能描述:                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
time_type Read_fill_time(uint8 tend_num);
time_type Read_wash_time(uint8 tend_num);
time_type Read_empty_time(uint8 tend_num);
/***********************************************/
/* 函数名称: Read_sta                 */
/* 功能描述: 泵速显示                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Read_sta(uint8 tend_num );

/***********************************************/
/* 函数名称: Read_huanzhe                      */
/* 功能描述: 患者信息读取                      */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
uint8 *Read_huanzhe(uint8 tend_num );


///////////////////本文件用的变量//////////////////
/*************************************  字库序号   **************************************************/
//#define  Show_Font_12X12      0x20   //12X12点阵
//#define  Show_Font_16X16			0x21   //16X16点阵
//#define  Show_Font_24X24			0x22   //24X24点阵
//#define  Show_Font_32X32			0x23   //32X32点阵

#define  Show_8X8             0x53   //12X12点阵
#define  Show_12X12           0x54   //12X12点阵
#define  Show_16X16			      0x6F   //16X16点阵
#define  Show_Font_24X24			0x22   //24X24点阵
#define  Show_32X32			      0x55   //32X32点阵

#define  Color_White                     0xffff
#define  Color_RED                        0xf800

/*************************************  参数显示   **************************************************/
#define  Show_6X12_ASCII			0x00	   //显示6X12ASCII码
#define	 Show_8X16_ASCII			0x01    //显示8X16ASCII码
#define	 Show_12X24_ASCII			0x02    //显示12X24ASCII码  
#define  Show_GBK_Code			  0x82    //文字显示前景色，不显示背景色，采用GBK内码
#define  Background_Color		  0x0000	//背景颜色为黑色
#define  Date_Time_color      0xE673	  //时间日期颜色——淡黄色
/*************************************  显示位置定义   **************************************************/
#define Monitor_R_WEIGHT_Address()			 160,  119		   //显示X轴坐标，显示Y轴坐标
#define Monitor_R_WEIGHT_ML_Address()		 490,  119		   //显示X轴坐标，显示Y轴坐标
#define Monitor_R_0_Address()		         240,  119		   //显示X轴坐标，显示Y轴坐标
#define Monitor_R_500_Address()		       300,  119		   //显示X轴坐标，显示Y轴坐标
#define Monitor_D_WEIGHT_Address()			 150,  159		   //显示X轴坐标，显示Y轴坐标


#define Monitor_D_WEIGHT_ML_Address()		 500,  159		   //显示X轴坐标，显示Y轴坐标
#define Monitor_RED_Address()			       190,  194		   //显示X轴坐标，显示Y轴坐标
#define Monitor_GRE_Address()			       190,  225		   //显示X轴坐标，显示Y轴坐标
#define Monitor_AIR_Address()			       190,  260		   //显示X轴坐标，显示Y轴坐标
#define Monitor_CELL_Address()			     190,  290		   //显示X轴坐标，显示Y轴坐标
#define Monitor_WELL_Address()			     190,  325
#define Monitor_CELL_LINE_Address()			 190,  357
#define Monitor_S_LOUYE_Address()			   190,  360
#define Monitor_X_LOUYE_Address()			   190,  397

#define Monitor_FILL_Address()			 250,  175		   //显示X轴坐标，显示Y轴坐标
#define Monitor_WASH_Address()			 450,  175		   //显示X轴坐标，显示Y轴坐标
#define Monitor_EMPTY_Address()			 660,  175		   //显示X轴坐标，显示Y轴坐标
#define Monitor_SPEED_Address()      287,  413
#define Monitor_RPM_Address()        550,  380
#define Monitor_WASTE_Address()			 675,  432		   //显示X轴坐标，显示Y轴坐标
#define DELYTIME_Address()           20,   255
#define DELYTIME_SEC_Address()       134,  308
#define INFO_1_Address()             15,   280
#define INFO_2_Address()             15,   305
#define INFO_3_Address()             15,   330
#define INFO_4_Address()             15,   355
#define INFO_5_Address()             15,   380
#define INFO_6_Address()             15,   405
#define INFO_7_Address()             15,   430
#define INFO_8_Address()             15,   455
#define DELYTIME_Address_1()         230,  380


#define SET_PRA_Address()            610,245
#define SET_TIME_Address()           600,275
#define SET_FILLSPEED_Address()      50,207
#define SET_WASHSPEED_Address()      205,207
#define SET_EMPTYSPEED_Address()     360,207
#define SET_WASH_MAX_Address()       40,355

#define POWEROFF_TIME_Address()      290,516

#define SET_AUTO_Address()           205,355

#define SET_FEIYE_Address()           30,207
#define SET_FEIYE_ON_Address()           90,207
#define SET_AIR_Address()            240,207
#define SET_BLOOD_Address()          400,207
#define SET_LOUYE_Address()          90,355
#define SET_WELL_Address()           240,355
//#define SET_FEIYE_Address()          50,207
//#define SET_FEIYE_Address()          50,207


#define SET_ARLRM_ON_Address()       435,350
#define SET_BOWL_Address()           381,355
#define SET_BOWL_WORK_Address()      40,65
#define Alarm_num_Address()			     50, 350

#define SET_MODE_Address()          100,140
#define  MODE_Address()             100,90


//显示X轴坐标，显示Y轴坐标
#define SET_YEAR_Address()         45, 225  // 130,170
#define SET_MONTH_Address()       200, 225  //  370,170
#define SET_DAY_Address()         365, 225  //  610,170
#define SET_HOUR_Address()         35, 330  // 130,370
#define SET_SEC_Address()         365, 330  // 610,370
#define SET_MIN_Address()         200, 330  //  370,370


#define PIC_TEST          28
#define PIC_CUT_KEY       31




uint8 show_pic_test;
//uint8 Display_Loop;
//uint8 show_pic_flag;
uint8 Alarm_switch_num = 0;

/***********************************************/
/* 函数名称: M600_DISPLAY_Soft_ver              */
/* 功能描述: 显示该程序编译日期                */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月26日                      */
/***********************************************/
void M600_DISPLAY_Soft_ver(void)
{
    Dwin_Text_Display(cmd_Text_0x55, 323, 553, code_DataStr);
    Dwin_Text_Display(cmd_Text_0x55, 290, 553, "VER");

}


/***********************************************/
/* 函数名称: M600_DISPLAY_Date                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Date(void)
{

    uint8 str[11] = {0, 0, 0, 0, '-', 0, 0, '-', 0, 0, '\0'};
    str[0] = ((timer.year)) / 1000 + '0';             //年的第1位
    str[1] = (((timer.year)) % 1000) / 100 + '0';     //年的第2位
    str[2] = ((timer.year)) % 100 / 10 + '0';         //年的第3位
    str[3] = (((timer.year) & 0x0FFF) % 100) % 10 + '0'; //年的第4位
    str[5] = ((timer.month)) / 10 + '0';              //月的第1位
    str[6] = ((timer.month)) % 10 + '0';              //月的第2位
    str[8] = (timer.day) / 10 + '0';                  //日的第1位
    str[9] = (timer.day) % 10 + '0';                  //日的第2位
    M600_Set_Palette(0xE623, 0xf6ef);
    M600_Set_background(565, 80);
    Dwin_Text_Display(cmd_Text_0x6f, 565, 80, str);

}
/***********************************************/
/* 函数名称: M600_DISPLAY_time                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Time(void)
{
    uint8 str[9] = {0, 0, ':', 0, 0, ':', 0, 0, '\0'};
    str[0] = timer.hour / 10 + '0';
    str[1] = timer.hour % 10 + '0';
    str[3] = timer.min / 10 + '0';
    str[4] = timer.min % 10 + '0';
    str[6] = timer.sec / 10 + '0';
    str[7] = timer.sec % 10 + '0';
    Dwin_Text_Display(cmd_Text_0x6f, 700, 80, str);
    M600_Set_Palette(0x389f, 0xf6ef);
}
/***********************************************/
/* 函数名称: M600_DISPLAY_Date                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Date_time(void)
{
    M600_DISPLAY_Date();
    M600_DISPLAY_Time();

}
/***********************************************/
/* 函数名称: M600_DISPLAY_Tend_time                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Tend_time(time_type Show_Tend_Time, uint16 show_x, uint16 show_y)
{
    uint8 str1[9] = {'-', '-', ':', '-', '-', '\0', '-', '-', '\0'};
    uint8 str[11] = {'-', '-', '-', '-', '.', '-', '-', '.', '-', '-', '\0'};
    if(Show_Tend_Time.hour != 0xff)
    {
        str[0] = ((Show_Tend_Time.year)) / 1000 + '0';             //年的第1位
        str[1] = (((Show_Tend_Time.year)) % 1000) / 100 + '0';     //年的第2位
        str[2] = ((Show_Tend_Time.year)) % 100 / 10 + '0';         //年的第3位
        str[3] = (((Show_Tend_Time.year) & 0x0FFF) % 100) % 10 + '0'; //年的第4位
        str[5] = ((Show_Tend_Time.month)) / 10 + '0';              //月的第1位
        str[6] = ((Show_Tend_Time.month)) % 10 + '0';              //月的第2位
        str[8] = (Show_Tend_Time.day) / 10 + '0';                  //日的第1位
        str[9] = (Show_Tend_Time.day) % 10 + '0';                  //日的第2位
        Dwin_Text_Display(cmd_Text_0x6f, show_x, show_y, str);
        str1[0] = Show_Tend_Time.hour / 10 + '0';
        str1[1] = Show_Tend_Time.hour % 10 + '0';
        str1[3] = Show_Tend_Time.min / 10 + '0';
        str1[4] = Show_Tend_Time.min % 10 + '0';

        Dwin_Text_Display(cmd_Text_0x6f, (show_x + 130), show_y, str1);
    }
    else
    {
        Dwin_Text_Display(cmd_Text_0x6f, show_x, show_y, str);
        Dwin_Text_Display(cmd_Text_0x6f, (show_x + 130), show_y, str1);
    }


}
/***********************************************/
/* 函数名称: M600_DISPLAY_Tend_time                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Tend_fill_time(void)
{

    uint8 str1[9] = {'-', '-', ':', '-', '-', '\0', '-', '-', '\0'};
    uint8 str[11] = {'-', '-', '-', '-', '.', '-', '-', '.', '-', '-', '\0'};
    M600_Set_Palette(0x0000, 0xffff);
    if(Tend_fill_Time.hour != 0xff)
    {
        str[0] = ((Tend_Time.year)) / 1000 + '0';             //年的第1位
        str[1] = (((Tend_Time.year)) % 1000) / 100 + '0';     //年的第2位
        str[2] = ((Tend_Time.year)) % 100 / 10 + '0';         //年的第3位
        str[3] = (((Tend_Time.year) & 0x0FFF) % 100) % 10 + '0'; //年的第4位
        str[5] = ((Tend_Time.month)) / 10 + '0';              //月的第1位
        str[6] = ((Tend_Time.month)) % 10 + '0';              //月的第2位
        str[8] = (Tend_Time.day) / 10 + '0';                  //日的第1位
        str[9] = (Tend_Time.day) % 10 + '0';                  //日的第2位
        Dwin_Text_Display(cmd_Text_0x6f, 20, 208, str);
        str1[0] = Tend_fill_Time.hour / 10 + '0';
        str1[1] = Tend_fill_Time.hour % 10 + '0';
        str1[3] = Tend_fill_Time.min / 10 + '0';
        str1[4] = Tend_fill_Time.min % 10 + '0';

        Dwin_Text_Display(cmd_Text_0x6f, 149, 208, str1);
    }
    else
    {
        Dwin_Text_Display(cmd_Text_0x6f, 20, 208, str);
        Dwin_Text_Display(cmd_Text_0x6f, 149, 208, str1);
    }
    //M600_Set_Palette(0x389f,0xf6ef);
}
/***********************************************/
/* 函数名称: M600_DISPLAY_Tend_time                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Tend_huanzhe(void)
{
    //	uint8 str[10];
    Read_huanzhe(GUI_Tend_num);
    M600_Set_Palette(0x0000, 0xffff);
    Dwin_Text_Display(cmd_Text_0x6f, 569, 170, Tend_Set_person);
    //M600_Set_Palette(0x389f,0xf6ef);


}
/***********************************************/
/* 函数名称: M600_DISPLAY_Tend_page                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Tend_page(void)
{
    //uint8 str1[4]={0,0,0,'\0'};//,0,'\0',0,0,'\0'};
    uint8 str[8] = {0, 0, 0, '/', 0, 0, 0, '\0'}; //,0,0,'.',0,0,'\0'};
    M600_Set_Palette(0x0000, 0xffff);
    str[0] = (GUI_Tend_num) / 100 + '0';
    str[1] = (GUI_Tend_num % 100) / 10 + '0';
    str[2] = (GUI_Tend_num) % 10 + '0';
    str[4] = (Tend_num) / 100 + '0';
    str[5] = (Tend_num % 100) / 10 + '0';
    str[6] = (Tend_num) % 10 + '0';
    Dwin_Text_Display(cmd_Text_0x6f, 379, 458, str);


    // str1[0] = Tend_Time.hour/10+ '0';
    // str1[1] = Tend_Time.hour%10+ '0';
    // str1[3] = Tend_Time.min/10 + '0';
    // str1[4] = Tend_Time.min%10 + '0';
    //// str[6] = Tend_Time.sec/10 + '0';
    //// str[7] = timer.sec%10 + '0';
    // Dwin_Text_Display(cmd_Text_0x54,147,170,str1);
    // M600_Set_Palette(0x389f,0xf6ef);
    //	M600_DISPLAY_Date();
    //	M600_DISPLAY_Time();

}
/***********************************************/
/* 函数名称: M600_DISPLAY_Bowl_Wash                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Bowl_num(uint16 show_x, uint16 show_y)
{
    uint8 str[3] = {0, 0, '\0'}; //
    M600_Set_Palette(0x0000, 0xffff);
    M600_Set_background(show_x, show_y);
    if((Wash_Bowl_Num / 10) == 0)
    {
        str[0] = Wash_Bowl_Num + '0';
    }
    else
    {
        str[1] = (Wash_Bowl_Num % 10) + '0';
        str[0] = (Wash_Bowl_Num / 10) + '0';
    }
    Dwin_Text_Display(cmd_Text_0x6f, show_x, show_y, str);

}
/***********************************************/
/* 函数名称: M600_DISPLAY_Bowl_Wash                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Bowl_Wash(uint16 show_x, uint16 show_y)
{
    uint8 str[12] = {0, 0, 0, 0, '/', 0, 0, 0, 0, 0, 0, '\0'}; //
    M600_Set_Palette(0x0000, 0xffff);
    M600_Set_background(show_x, show_y);
    if((Monitor_Bowl_Wash_Volume) / 1000 == 0)
    {
        //str[0] = (Monitor_Bowl_Wash_Volume)/1000 + '0';
        str[0] = ((uint16)Monitor_Bowl_Wash_Volume % 1000) / 100 + '0';
        str[1] = ((uint16)Monitor_Bowl_Wash_Volume % 100) / 10 + '0';
        str[2] = ((uint16)Monitor_Bowl_Wash_Volume % 10) + '0';
        str[3] = '/';
        str[4] = (Set_Wash_Volume + Set_Wash_ext_Volume) / 1000 + '0';
        str[5] = ((Set_Wash_Volume + Set_Wash_ext_Volume) % 1000) / 100 + '0';
        str[6] = ((Set_Wash_Volume + Set_Wash_ext_Volume) % 100) / 10 + '0';
        str[7] = ((Set_Wash_Volume + Set_Wash_ext_Volume) % 10) + '0';
        str[8] =	'\0';
    }
    else
    {
        str[0] = ((uint16)Monitor_Bowl_Wash_Volume) / 1000 + '0';
        str[1] = ((uint16)Monitor_Bowl_Wash_Volume % 1000) / 100 + '0';
        str[2] = ((uint16)Monitor_Bowl_Wash_Volume % 100) / 10 + '0';
        str[3] = ((uint16)Monitor_Bowl_Wash_Volume % 10) + '0';
        str[5] = (Set_Wash_Volume + Set_Wash_ext_Volume) / 1000 + '0';
        str[6] = ((Set_Wash_Volume + Set_Wash_ext_Volume) % 1000) / 100 + '0';
        str[7] = ((Set_Wash_Volume + Set_Wash_ext_Volume) % 100) / 10 + '0';
        str[8] = ((Set_Wash_Volume + Set_Wash_ext_Volume) % 10) + '0';
    }

    Dwin_Text_Display(cmd_Text_0x6f, show_x, show_y, str);
}
/***********************************************/
/* 函数名称: M600_DISPLAY_Bowl_Wash                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Auto_run(uint16 show_x, uint16 show_y)
{
    uint16 monitor_reservoir_temp;
    uint8 str[12] = {0, 0, 0, 0, '/', 0, 0, 0, 0, 0, 0, '\0'}; //
    M600_Set_Palette(0x0000, 0xffff);
    M600_Set_background(show_x, show_y);
    if(monitor_reservoir_ml > Reservoir_leftover)
        monitor_reservoir_temp = monitor_reservoir_ml - Reservoir_leftover;
    else
        monitor_reservoir_temp = 0;
    if((monitor_reservoir_ml) / 1000 == 0)
    {
        //str[0] = (Monitor_Bowl_Wash_Volume)/1000 + '0';
        str[0] = (monitor_reservoir_temp % 1000) / 100 + '0';
        str[1] = (monitor_reservoir_temp % 100) / 10 + '0';
        str[2] = (monitor_reservoir_temp % 10) + '0';
        str[3] = '/';
        if((Set_Run_Volume) / 1000 == 0)
        {
            //str[4] = (Set_Run_Volume)/1000 + ' ';
            str[4] = (Set_Run_Volume % 1000) / 100 + '0';
            str[5] = (Set_Run_Volume % 100) / 10 + '0';
            str[6] = (Set_Run_Volume % 10) + '0';
            str[7] =	'\0';
        }
        else
        {
            str[4] = (Set_Run_Volume) / 1000 + '0';
            str[5] = (Set_Run_Volume % 1000) / 100 + '0';
            str[6] = (Set_Run_Volume % 100) / 10 + '0';
            str[7] = (Set_Run_Volume % 10) + '0';
            str[8] =	'\0';
        }
    }
    else
    {
        str[0] = (monitor_reservoir_temp) / 1000 + '0';
        str[1] = (monitor_reservoir_temp % 1000) / 100 + '0';
        str[2] = (monitor_reservoir_temp % 100) / 10 + '0';
        str[3] = (monitor_reservoir_temp % 10) + '0';
        if((Set_Run_Volume) / 1000 == 0)
        {
            //str[4] = (Set_Run_Volume)/1000 + ' ';
            str[5] = (Set_Run_Volume % 1000) / 100 + '0';
            str[6] = (Set_Run_Volume % 100) / 10 + '0';
            str[7] = (Set_Run_Volume % 10) + '0';
            str[8] =	'\0';
        }
        else
        {
            str[5] = (Set_Run_Volume) / 1000 + '0';
            str[6] = (Set_Run_Volume % 1000) / 100 + '0';
            str[7] = (Set_Run_Volume % 100) / 10 + '0';
            str[8] = (Set_Run_Volume % 10) + '0';
            str[9] =	'\0';
        }
    }

    Dwin_Text_Display(cmd_Text_0x6f, show_x, show_y, str);
}
/***********************************************/
/* 函数名称: M600_DISPLAY_Bowl_Wash                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Bowl_Prefill(uint16 show_x, uint16 show_y)
{
    uint8 str[8];//
    M600_Set_Palette(0x0000, 0xffff);
    M600_Set_background(show_x, show_y);
    if((Monitor_Bowl_Wash_Volume) / 1000 == 0)
    {
        //str[0] = (Monitor_Bowl_Wash_Volume)/1000 + '0';
        str[0] = ((uint16)Monitor_Pre_fill_Volume % 1000) / 100 + '0';
        str[1] = ((uint16)Monitor_Pre_fill_Volume % 100) / 10 + '0';
        str[2] = ((uint16)Monitor_Pre_fill_Volume % 10) + '0';
        str[3] = '/';
        // str[4] = (Pre_fill_Volume)/1000 + '0';
        str[4] = (Pre_fill_Volume % 1000) / 100 + '0';
        str[5] = (Pre_fill_Volume % 100) / 10 + '0';
        str[6] = (Pre_fill_Volume % 10) + '0';
        str[7] =	'\0';
    }
    //else
    //{
    // str[0] = (Monitor_Pre_fill_Volume)/1000 + '0';
    // str[1] = (Monitor_Pre_fill_Volume%1000)/100 + '0';
    // str[2] = (Monitor_Pre_fill_Volume%100)/10 + '0';
    // str[3] = (Monitor_Pre_fill_Volume%10) + '0';
    // str[5] = (Pre_fill_Volume)/1000 + '0';
    // str[6] = (Pre_fill_Volume%1000)/100 + '0';
    // str[7] = (Pre_fill_Volume%100)/10 + '0';
    // str[8] = (Pre_fill_Volume%10) + '0';
    //}

    Dwin_Text_Display(cmd_Text_0x6f, show_x, show_y, str);
}


/***********************************************/
/* 函数名称: M600_DISPLAY_Tend_page                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年12月19日                      */
/***********************************************/
void M600_DISPLAY_Tend_Monitor(uint8 pic_num, uint16 position_x, uint16 position_y, uint16 display_num )
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    Integer_Number_Change_String(display_num, str);
    M600_Deal_Picture(Command_Cut_Picture, pic_num, position_x, position_y, position_x + 120, position_y + 50, position_x, position_y);
    Dwin_Text_Display(cmd_Text_0x6f, position_x, position_y, str);
    // Dwin_Text_1_Display(position_x,position_y,0x27,0x80,0x05,0x0000,0xef3d,str);
}
/***********************************************/
/* 函数名称: M600_DISPLAY_Date                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月21日                      */
/***********************************************/
void Dwin_Pause_Display(void)
{
    M600_Set_background(128, 148);
    if(Mode == MODE_AUTO)
    {
        if(Work_Stop_Flag == 1)
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 100, 400, 186, 430, 15, 245); //停止中见显示
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 622, 2, 685, 60, 582, 12); //按键改变
        }
        else if(Pump_Pause == 1)
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 100, 455, 186, 480, 15, 247); //暂停中见显示
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 622, 2, 685, 60, 582, 12); //按键改变
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 2, 19, 253, 119, 293, 15, 245);       //清除暂停显示
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 622, 64, 685, 121, 582, 12); //按键改变
        }
    }
    else
    {
        //Work_Stop_Flag=0;
        if(Pump_Pause == 1)
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 100, 455, 186, 485, 15, 245); //暂停中见显示
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 622, 2, 685, 60, 582, 12); //按键改变
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 2, 19, 253, 119, 290, 15, 245);
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 622, 64, 685, 121, 582, 12); //按键改变
            //	Dwin_Text_Display(cmd_Text_0x54,INFO_2_Address(),"按箭头改变泵速");
        }
    }


}
/***********************************************/
/* 函数名称: M600_DISPLAY_Date                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月21日                      */
/***********************************************/
void Dwin_Pump_Delay_Display(void)
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    M600_Set_background(DELYTIME_Address());
    if(Bit_check_1(Bowl_State, 2))                                    //再次启动时需要延时蠕动泵
    {

        Integer_Number_Change_String(pump_dely_time, str);
        Dwin_Text_Display(cmd_Text_0x54, DELYTIME_SEC_Address(), str);
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 420, 280, 550, 300, INFO_2_Address());
        //M600_Deal_Picture(Command_Cut_Picture,PIC_CUT_KEY,424,300,555,320,INFO_1_Address());
        Pump_Pause = 1;


    }
}
/***********************************************/
/* 函数名称: M600_DISPLAY_Date                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Dwin_Alarm_Display(void)      //程序控制方法待改进
{
#define ALARM_DIS_ADD()           100,5
#define EMPTY_AIR_CUT_ADD()             295,0, 516,39
#define AIR_CUT_ADD()             295,40, 516,79
#define WELL_CUT_ADD()            295,200,516,239
#define	CELL_CUT_ADD()            295,80,516,119
#define	FEIYE_CUT_ADD()           295,160,516,199
#define	LOUYE_CUT_ADD()           0,0,222,38
#define	PUMP_CUT_ADD()            295,240,516,279
#define	MOTO_CUT_ADD()            295,120,516,159
    uint8 temp_i;
    uint8 Alarm_num = 0;
    uint8 Alarm_disp_table[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(temp_i = 0; temp_i < 31; temp_i++)
    {
        if(Bit_check_1(Alarm_sta, temp_i))
        {
            Alarm_num++;
            Alarm_disp_table[Alarm_num] = temp_i;
        }
    }
    if(Alarm_num == 1)
        Alarm_switch_num = 32;
    if(Alarm_switch_num > 10)
    {
        Alarm_switch_num = 0;
        for(; (Alarm_dip_now < Alarm_num + 1);)
        {

            switch(Alarm_disp_table[Alarm_dip_now])
            {
            case 0:
                // Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"断流报警!!   ");
                M600_Deal_Picture(Command_Cut_Picture, 47, AIR_CUT_ADD(), ALARM_DIS_ADD());
                break;
            case 1:
                //Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"离心井盖报警!!   ");
                M600_Deal_Picture(Command_Cut_Picture, 47, WELL_CUT_ADD(), ALARM_DIS_ADD());
                break;
            case CELL_ALARM:
                //			       Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"检测到血层!!     ");
                M600_Deal_Picture(Command_Cut_Picture, 47, CELL_CUT_ADD(), ALARM_DIS_ADD());

                break;
            case FEIYE_ALARM:
                //			       Dwin_Text_Display(cmd_Text_0x55,100,410,"废液量已达到最大值!! ");
                M600_Deal_Picture(Command_Cut_Picture, 47, FEIYE_CUT_ADD(), ALARM_DIS_ADD());

                break;
            case louye_1_ALARM:
            case louye_2_ALARM:
                //					   Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"漏液报警!!!     ");
                M600_Deal_Picture(Command_Cut_Picture, 47, LOUYE_CUT_ADD(), ALARM_DIS_ADD());
                break;
            case FILL_AIR_ALARM:
                //Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"断流报警!!      ");
                M600_Deal_Picture(Command_Cut_Picture, 47, AIR_CUT_ADD(), ALARM_DIS_ADD());
                break;
            case WASH_AIR_ALARM:
                //Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"断流报警!!       ");
                M600_Deal_Picture(Command_Cut_Picture, 47, AIR_CUT_ADD(), ALARM_DIS_ADD());
                break;
            case EMPTY_AIR_ALARM:
                //Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"断流报警!!       ");
                M600_Deal_Picture(Command_Cut_Picture, 47, AIR_CUT_ADD(), ALARM_DIS_ADD());
                break;
            case EMPTY_AIR_EARLY_ALARM:
                //Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"过早检测到断流!!  ");
                M600_Deal_Picture(Command_Cut_Picture, 47, EMPTY_AIR_CUT_ADD(), ALARM_DIS_ADD());
                break;
            case PRE_FILL_ALARM:
                //Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"断流报警!!  ");
                M600_Deal_Picture(Command_Cut_Picture, 47, AIR_CUT_ADD(), ALARM_DIS_ADD());
                break;
            case LIXIN_ALARM:
                //				   Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"离心电机故障!!  ");
                M600_Deal_Picture(Command_Cut_Picture, 47, MOTO_CUT_ADD(), ALARM_DIS_ADD());
                break;
            case FILL_PUMP_ALARM:
            case	WASH_PUMP_ALARM:
            case  EMPTY_PUMP_ALARM:
                //
                M600_Deal_Picture(Command_Cut_Picture, 47, PUMP_CUT_ADD(), ALARM_DIS_ADD());
                break;
            default:
                break;
            }
            Alarm_dip_now++;
            break;
        }
        if (Alarm_dip_now > Alarm_num)
            Alarm_dip_now = 1;
    }
    else
        Alarm_switch_num++;

    if(Alarm_num == 0)
    {
        //Dwin_Text_Display(cmd_Text_0x55,ALARM_DIS_ADD(),"                          ");
        M600_Deal_Picture(Command_Cut_Picture, 2, ALARM_DIS_ADD(), 532, 44, ALARM_DIS_ADD()); //按键改变


    }



}
/***********************************************/
/* 函数名称: Dwin_Monitor_Display                 */
/* 功能描述: 显示监测参数                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Dwin_Monitor_Display(uint16 Monitor, uint16 show_x, uint16 show_y)    //程序控制方法待改进
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    Integer_Number_Change_String(Monitor, str);
    M600_Deal_Picture(Command_Cut_Picture, 0x02, show_x, show_y, show_x + 140, show_y + 45, show_x, show_y);
    Dwin_Text_1_Display(show_x, show_y, 0x24, 0x80, 0x07, 0x4cdf, 0xef7d, str);
}
/***********************************************/
/* 函数名称: Dwin_Monitor_Display                 */
/* 功能描述: 显示监测参数                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Dwin_Work_sta_key_Display(uint16 Monitor, uint16 show_x, uint16 show_y)    //程序控制方法待改进
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    Integer_Number_Change_String(Monitor, str);
    M600_Deal_Picture(Command_Cut_Picture, 0x02, show_x, show_y, show_x + 140, show_y + 45, show_x, show_y);
    Dwin_Text_1_Display(show_x, show_y, 0x24, 0x80, 0x07, 0x4cdf, 0xef7d, str);
}
/***********************************************/
/* 函数名称: Dwin_Monitor_Display                 */
/* 功能描述: 显示日期                          */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Dwin_Mode_Display(void)      //程序控制方法待改进
{
    if(Mode == 2)
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 200, 0, 285, 40, 298, 57);
    else if(Mode == 1)
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 200, 40, 285, 81, 298, 57);
    else if(Mode == 3)
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 200, 80, 285, 121, 298, 57);
    else if(Mode == 4)
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 200, 120, 285, 161, 298, 57);


}
/***********************************************/
/* 函数名称: Res_state_Display            */
/* 功能描述: 上称重状态显示                           */
/* 输  　入: Null                               */
/* 输  　出: Null                               */
/* 全局变量: 无                                 */
/* 调用模块: 无                                 */
/* 修改时间：16年01月02日                        */
/***********************************************/
void  Res_state_Display(void)
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    uint16 temp_i, temp_j;
    temp_j = (210 - Set_Run_Volume / 30) - 2;
    temp_i = (210 - (monitor_reservoir_ml - Reservoir_leftover ) / 30);
    if(temp_i < 150)
        temp_i = 150;
    if(temp_j < 150)
        temp_j = 150;
    // M600_Deal_Picture(Command_Cut_Picture,PIC_CUT_KEY,210,20,248,22,210,temp_j);//血液量
    M600_Deal_Picture(Command_Cut_Picture, 12, 210, temp_i, 248, 210, 210, temp_i); //血液量
    M600_Deal_Picture(Command_Cut_Picture, 2, 210, 150, 248, temp_i, 210, 150);
    M600_Deal_Picture(Command_Cut_Picture, 12, 210, 148, 248, 149, 210, temp_j); //启动量
    if(monitor_reservoir_ml > Reservoir_leftover)
        Integer_Number_Change_String((monitor_reservoir_ml - Reservoir_leftover ), str);
    else
        Integer_Number_Change_String(0, str);
    M600_Set_background(210, 205);
    Dwin_Text_Display(cmd_Text_0x6e, 210, 200, str );
}
/***********************************************/
/* 函数名称: Res_state_Display            */
/* 功能描述: 上称重状态显示                           */
/* 输  　入: Null                               */
/* 输  　出: Null                               */
/* 全局变量: 无                                 */
/* 调用模块: 无                                 */
/* 修改时间：16年01月02日                        */
/***********************************************/
void Printer_state_Display(void)
{
    if(print_able_flag == 0)
        M600_Deal_Picture(Command_Cut_Picture, 35, 0, 0, 170, 20, 580, 485);
    else
        M600_Deal_Picture(Command_Cut_Picture, 2, 580, 485, 750, 510, 580, 485);;

}
/***********************************************/
/* 函数名称: Dwin_Pump_speed_Display            */
/* 功能描述: 泵速显示                           */
/* 输  　入: Null                               */
/* 输  　出: Null                               */
/* 全局变量: 无                                 */
/* 调用模块: 无                                 */
/* 修改时间：14年11月1日                        */
/***********************************************/
void Dwin_Pump_speed_Display(void)
{
    switch(work_sta)
    {
    case WORK_PRE_FILL:
    case WORK_WASH:
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 415, 0, 590, 48, 250, 400);
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 415, 160, 482, 187, 250, 375); //250,375);
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 415, 189, 482, 215, 250, 450);
        M600_Text_Display(Show_32X32, Monitor_SPEED_Address(), Set_Wash_speed);
        // M600_Deal_Picture(Command_Cut_Picture,PIC_CUT_KEY,423,260,590,278,INFO_2_Address());

        break;
    case WORK_FILL:
    case WORK_RETURN:
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 415, 48, 590, 96, 250, 400);
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 415, 160, 482, 187, 250, 375); //250,375);
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 415, 189, 482, 215, 250, 450);
        M600_Text_Display(Show_32X32, Monitor_SPEED_Address(), Set_Fill_speed);
        // M600_Deal_Picture(Command_Cut_Picture,PIC_CUT_KEY,423,260,590,278,INFO_2_Address());

        break;
    case WORK_EMPTY:
    case WORK_CONC:
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 415, 98, 590, 144, 250, 400);
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 415, 160, 482, 187, 250, 375); //250,375);
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 415, 189, 482, 215, 250, 450);
        M600_Text_Display(Show_32X32, Monitor_SPEED_Address(), Set_Empty_speed);
        // M600_Deal_Picture(Command_Cut_Picture,PIC_CUT_KEY,423,260,590,278,INFO_2_Address());
        break;
    case WORK_STANDBY:
        M600_Deal_Picture(Command_Cut_Picture, 2, 250, 375, 460, 490, 250, 375);
        // M600_Deal_Picture(Command_Cut_Picture,2,INFO_2_Address(),160,360,INFO_2_Address());
        break;
    default:
        break;
    }


}
/***********************************************/
/* 函数名称: Dwin_Pump_speed_Display            */
/* 功能描述: 泵速显示                           */
/* 输  　入: Null                               */
/* 输  　出: Null                               */
/* 全局变量: 无                                 */
/* 调用模块: 无                                 */
/* 修改时间：14年11月1日                        */
/***********************************************/
void Dwin_info_Display(void)
{
#define PIC_CUT_INFO             34
#define CLR_INFO1_ADD()          10 ,280, 165,302 , 10, 280
#define CLR_INFO2_ADD()          10 ,305, 165,327 , 10, 305
#define CLR_INFO3_ADD()          10 ,330, 165,352, 10, 330
#define CLR_INFO4_ADD()          10 ,355, 165,377 , 10, 355
#define CLR_INFO5_ADD()          10 ,380, 165,402 , 10, 380

    //#define PRE_FILLING_CUT_ADD()     422 ,240, 560, 260
    //#define PRE_FILLING_CUT_ADD()     422 ,240, 560, 260
#define Bowl_num_ADD()            95,428
#define PRE_FILLING_CUT_ADD()     590 ,340, 745, 361
#define PRE_FILL_ALARM_CUT_ADD()  420 ,420, 559, 440
#define PAUSE_CUT_ADD()           420 ,300 ,578 ,320
#define WASHING_CUT_ADD()         420 ,319, 578, 339
#define WASHOK_CUT_ADD()          420 ,340, 578, 360
#define FILL_WAIT_CUT_ADD()       590 ,420 ,745 ,440
#define RES_STATE_CUT_ADD()        10,  0,  160, 20
#define WELL_ON_CUT_ADD()            10, 40 ,160,60
#define WELL_OFF_CUT_ADD()            10, 60 ,160,80
#define BWOL_NUM_CUT_ADD()            590, 280 ,745,300

    M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, BWOL_NUM_CUT_ADD(), INFO_7_Address());
    M600_DISPLAY_Bowl_num(Bowl_num_ADD());
    if(Well_state_flag == 1)
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, WELL_ON_CUT_ADD(), INFO_8_Address());
    else
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, WELL_OFF_CUT_ADD(), INFO_8_Address());
    switch(work_sta)
    {
    case WORK_PRE_FILL:
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO4_ADD());
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, PRE_FILLING_CUT_ADD(), INFO_3_Address());
        M600_DISPLAY_Bowl_Prefill(INFO_4_Address());
        if(Pump_Pause == 1)
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, PAUSE_CUT_ADD(), INFO_1_Address());
        }
        else
            M600_Deal_Picture(Command_Cut_Picture, 2, INFO_1_Address(), 200, 300, INFO_1_Address());

        if(Bit_check_1(Alarm_sta, PRE_FILL_ALARM)) //检查盐水袋提示
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, PRE_FILL_ALARM_CUT_ADD(), INFO_3_Address());
        }
        break;
    case WORK_WASH:
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO1_ADD());

        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO5_ADD());
        if(Bit_check_1(Bowl_State, 2))                                    //再次启动时需要延时蠕动泵
        {
            Dwin_Pump_Delay_Display();
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO2_ADD());
        }
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, WASHING_CUT_ADD(), INFO_3_Address());
        M600_DISPLAY_Bowl_Wash(INFO_4_Address());
        if(Bit_check_1(Pump_state_flag, WASH_OK))
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, WASHOK_CUT_ADD(), INFO_4_Address());
        }
        break;
    case WORK_FILL:
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO3_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO4_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO5_ADD());
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, RES_STATE_CUT_ADD(), INFO_3_Address());
        M600_DISPLAY_Auto_run(INFO_4_Address());
        if(Bit_check_1(Bowl_State, 2))                                    //再次启动时需要延时蠕动泵
        {
            Dwin_Pump_Delay_Display();
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO2_ADD());
        }
        if(Pump_Pause == 1)
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, PAUSE_CUT_ADD(), INFO_1_Address());
        }
        else
            M600_Deal_Picture(Command_Cut_Picture, 2, INFO_1_Address(), 200, 300, INFO_1_Address());
        break;
    case WORK_RETURN:
        if(Pump_Pause == 1)
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, PAUSE_CUT_ADD(), INFO_1_Address());
        }
        else
            M600_Deal_Picture(Command_Cut_Picture, 2, INFO_1_Address(), 200, 300, INFO_1_Address());
        if(Bit_check_1(Bowl_State, 2))                                    //再次启动时需要延时蠕动泵
        {
            Dwin_Pump_Delay_Display();
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO2_ADD());
        }
        // M600_Deal_Picture(Command_Cut_Picture,2,CLR_INFO3_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO4_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO5_ADD());

        // M600_Deal_Picture(Command_Cut_Picture,2,INFO_3_Address(),200,444,INFO_3_Address());
        break;
    case WORK_CONC:
        if(Pump_Pause == 1)
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, PAUSE_CUT_ADD(), INFO_1_Address());
        }
        else
            M600_Deal_Picture(Command_Cut_Picture, 2, INFO_1_Address(), 200, 300, INFO_1_Address());
        //M600_Deal_Picture(Command_Cut_Picture,2,CLR_INFO2_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO3_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO4_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO5_ADD());
        if(Bit_check_1(Bowl_State, 2))                                    //再次启动时需要延时蠕动泵
        {
            Dwin_Pump_Delay_Display();
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO2_ADD());
        }
        break;
    case WORK_EMPTY:

        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO1_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO2_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO3_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO4_ADD());
        M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO5_ADD());
        //M600_Deal_Picture(Command_Cut_Picture,2,INFO_3_Address(),200,444,INFO_3_Address());
        break;
    case WORK_STANDBY:
        if(Mode == MODE_AUTO)
        {
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO1_ADD());
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO2_ADD());
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO3_ADD());
            if(Auto_refill_wait_flag == 1)
            {
                M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, FILL_WAIT_CUT_ADD(), INFO_3_Address());
            }
            else
            {
                M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_INFO, RES_STATE_CUT_ADD(), INFO_3_Address());
            }

            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO4_ADD());
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO5_ADD());
            M600_DISPLAY_Auto_run(INFO_4_Address());
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO1_ADD());
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO2_ADD());
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO3_ADD());
            M600_Deal_Picture(Command_Cut_Picture, 2, CLR_INFO4_ADD());

        }
        break;
    default:
        break;
    }


}

/***********************************************/
/* 函数名称: Dwin_Pump_State_Display                 */
/* 功能描述: 泵速显示                         */
/* 输  　入: Null                                */
/* 输  　出: Null                               */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Dwin_Pump_State_Display(void)
{
    M600_Deal_Picture(Command_Cut_Picture, 51, 400, 340, 640, 380, 400, 340); //显示废液
    switch(work_sta)
    {
    case WORK_STANDBY:
        M600_Deal_Picture(Command_Cut_Picture, 21, 200, 220, 640, 340, 200, 220);
        break;
    case WORK_FILL:
        if(Pump_Pause == 1 || Work_Stop_Flag == 1) //显示静态图片
            M600_Deal_Picture(Command_Cut_Picture, 21, 200, 220, 640, 340, 200, 220);
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, (21 + Show_donghua_num), 200, 220, 640, 340, 200, 220);
        }
        break;

    case WORK_RETURN:
        //  M600_Deal_Picture(Command_Cut_Picture,12,460,510,575,598,460,510);//显示当前状态
        if(Pump_Pause == 1 || Work_Stop_Flag == 1) //显示静态图片
            M600_Deal_Picture(Command_Cut_Picture, 24, 200, 220, 640, 340, 200, 220);
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, (24 - Show_donghua_num), 200, 220, 640, 340, 200, 220);
        }

        break;
    case WORK_WASH:
    case WORK_PRE_FILL:
        //   M600_Deal_Picture(Command_Cut_Picture,12,111,509,220,600,111,509);
        if(Pump_Pause == 1 || Work_Stop_Flag == 1) //显示静态图片
            M600_Deal_Picture(Command_Cut_Picture, 25, 200, 220, 640, 340, 200, 220);
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, (25 + Show_donghua_num), 200, 220, 640, 340, 200, 220);
        }
        break;
    case WORK_EMPTY:
        if(Pump_Pause == 1 || Work_Stop_Flag == 1) //显示静态图片
        {
            M600_Deal_Picture(Command_Cut_Picture, 54, 200, 220, 640, 340, 200, 220);
            M600_Deal_Picture(Command_Cut_Picture, 51, 400, 340, 640, 380, 400, 340); //显示废液
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, (54 - Show_donghua_num), 200, 220, 640, 340, 200, 220);
            M600_Deal_Picture(Command_Cut_Picture, 51, 400, 340, 640, 380, 400, 340);
        }
        break;
    case WORK_CONC:
        if(Pump_Pause == 1 || Work_Stop_Flag == 1) //显示静态图片
            M600_Deal_Picture(Command_Cut_Picture, 51, 200, 220, 640, 355, 200, 220);
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, (51 + Show_donghua_num), 200, 220, 640, 340, 200, 220);
        }
        break;
    default:
        break;
    }
}
/***********************************************/
/* 函数名称: Dwin_Pump_speed_Display                 */
/* 功能描述: 泵速显示                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Dwin_Test_Display(void)
{

    M600_Text_Display(Show_16X16, 550,  430, Monitor_Green_AD);
    Dwin_Text_Display(cmd_Text_0x55, 530, 430, "Gr");
    M600_Text_Display(Show_16X16, 550,  400, Monitor_Red_AD);
    Dwin_Text_Display( cmd_Text_0x55, 530, 400, "RE");
    M600_Text_Display(Show_16X16, 350,  400, Monitor_Blood_AD);
    Dwin_Text_Display(cmd_Text_0x55, 320, 400, "BL");
    M600_Text_Display(Show_16X16, 350,  430, Monitor_Air_AD);
    Dwin_Text_Display(cmd_Text_0x55, 320, 430, "AR");
    M600_Text_Display(Show_16X16, 400,  460, Centrifugal_motor_speed);

}
/***********************************************/
/* 函数名称: Dwin_Set_pra_Display              */
/* 功能描述: 泵速显示                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Dwin_Set_pra_Display(void)
{
#define  CUT_PIC   0X04
#define  	PIC_CUT_TEST 46
#define  PRA_NAME_ADD()   570  ,133
    switch(Set_pra_now)
    {
    case SET_FILL_FLOW:
        M600_Set_background(570, 133);
        M600_Deal_Picture(Command_Cut_Picture, 04, 34, 158, 171, 252, 34, 158);
        //Dwin_Text_Display(cmd_Text_0x55,570,133,"进血流速  ");
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 252, 0, 400, 40, PRA_NAME_ADD()); //清洗
        M600_Set_Display(CUT_PIC, Color_White, SET_PRA_Address(), Set_Fill_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_WASH_MAX_Address(), Set_Wash_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_BOWL_Address(), Set_Bowl);
        M600_Set_Display(CUT_PIC, Color_RED, SET_FILLSPEED_Address(), Set_Fill_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_AUTO_Address(), Set_Run_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_WASHSPEED_Address(), Set_Wash_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_EMPTYSPEED_Address(), Set_Empty_speed);
        break;
    case SET_WASH_FLOW:
        M600_Set_background(570, 133);
        M600_Deal_Picture(Command_Cut_Picture, 04, 194, 159, 332, 252, 194, 158);
        // Dwin_Text_Display(cmd_Text_0x55,570,133,"清洗流速  ");
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 252, 40, 400, 80, PRA_NAME_ADD()); //清洗
        M600_Set_Display(CUT_PIC, Color_White, SET_PRA_Address(), Set_Wash_speed);

        M600_Set_Display(CUT_PIC, Color_White, SET_WASH_MAX_Address(), Set_Wash_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_BOWL_Address(), Set_Bowl);
        M600_Set_Display(CUT_PIC, Color_White, SET_FILLSPEED_Address(), Set_Fill_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_AUTO_Address(), Set_Run_Volume);
        M600_Set_Display(CUT_PIC, Color_RED, SET_WASHSPEED_Address(), Set_Wash_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_EMPTYSPEED_Address(), Set_Empty_speed);
        break;
    case SET_EMPTY_FLOW:
        M600_Set_background(570, 133);
        //Dwin_Text_Display(cmd_Text_0x55,570,133,"清空流速  ");
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 252, 80, 400, 120, PRA_NAME_ADD()); //清洗
        M600_Set_Display(CUT_PIC, Color_White, SET_PRA_Address(), Set_Empty_speed);

        M600_Set_Display(CUT_PIC, Color_White, SET_WASH_MAX_Address(), Set_Wash_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_BOWL_Address(), Set_Bowl);
        M600_Set_Display(CUT_PIC, Color_White, SET_FILLSPEED_Address(), Set_Fill_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_AUTO_Address(), Set_Run_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_WASHSPEED_Address(), Set_Wash_speed);
        M600_Set_Display(CUT_PIC, Color_RED, SET_EMPTYSPEED_Address(), Set_Empty_speed);
        break;
    case SET_WASH_V:
        M600_Set_background(570, 133);
        //Dwin_Text_Display(cmd_Text_0x55,570,133,"清洗量设定");
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 252, 120, 400, 160, PRA_NAME_ADD()); //清洗
        M600_Set_Display(CUT_PIC, Color_White, SET_PRA_Address(), Set_Wash_Volume);

        M600_Set_Display(CUT_PIC, Color_RED, SET_WASH_MAX_Address(), Set_Wash_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_BOWL_Address(), Set_Bowl);
        M600_Set_Display(CUT_PIC, Color_White, SET_FILLSPEED_Address(), Set_Fill_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_AUTO_Address(), Set_Run_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_WASHSPEED_Address(), Set_Wash_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_EMPTYSPEED_Address(), Set_Empty_speed);
        break;
    case SET_AUTO_ML:
        M600_Set_background(570, 133);
        //Dwin_Text_Display(cmd_Text_0x55,570,133,"自动启动量");
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 252, 160, 400, 200, PRA_NAME_ADD()); //清洗
        M600_Set_Display(CUT_PIC, Color_White, SET_PRA_Address(), Set_Run_Volume);

        M600_Set_Display(CUT_PIC, Color_White, SET_WASH_MAX_Address(), Set_Wash_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_BOWL_Address(), Set_Bowl);
        M600_Set_Display(CUT_PIC, Color_White, SET_FILLSPEED_Address(), Set_Fill_speed);
        M600_Set_Display(CUT_PIC, Color_RED, SET_AUTO_Address(), Set_Run_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_WASHSPEED_Address(), Set_Wash_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_EMPTYSPEED_Address(), Set_Empty_speed);
        break;
    case SET_BOWL:
        M600_Set_background(570, 133);
        //Dwin_Text_Display(cmd_Text_0x55,570,133,"离心杯型号");
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 252, 200, 400, 240, PRA_NAME_ADD()); //清洗

        M600_Set_Display(CUT_PIC, Color_White, SET_PRA_Address(), Set_Bowl);
        M600_Set_Display(CUT_PIC, Color_White, SET_WASH_MAX_Address(), Set_Wash_Volume);
        M600_Set_Display(CUT_PIC, Color_RED, SET_BOWL_Address(), Set_Bowl);
        M600_Set_Display(CUT_PIC, Color_White, SET_FILLSPEED_Address(), Set_Fill_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_AUTO_Address(), Set_Run_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_WASHSPEED_Address(), Set_Wash_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_EMPTYSPEED_Address(), Set_Empty_speed);
        break;
    default:
        M600_Set_Display(CUT_PIC, Color_White, SET_WASH_MAX_Address(), Set_Wash_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_BOWL_Address(), Set_Bowl);
        M600_Set_Display(CUT_PIC, Color_White, SET_FILLSPEED_Address(), Set_Fill_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_AUTO_Address(), Set_Run_Volume);
        M600_Set_Display(CUT_PIC, Color_White, SET_WASHSPEED_Address(), Set_Wash_speed);
        M600_Set_Display(CUT_PIC, Color_White, SET_EMPTYSPEED_Address(), Set_Empty_speed);
        break;
    }

}
/***********************************************/
/* 函数名称: Dwin_Set_pra_Display              */
/* 功能描述: 泵速显示                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Main_Dbug_Display(void)
{
    if(main_dbug == 1)
    {
        Dwin_Text_Display(cmd_Text_0x6f, 178, 350, "A " );
        M600_Text_0X64_Display(9, 190, 350, Monitor_Air_AD);
        Dwin_Text_Display(cmd_Text_0x6f, 178, 375, "B " );
        M600_Text_0X64_Display(9, 190, 375, Monitor_Blood_AD);
        Dwin_Text_Display(cmd_Text_0x6f, 178, 400, "U " );
        M600_Text_0X64_Display(9, 190, 400, monitor_reservoir_ml);
        Dwin_Text_Display(cmd_Text_0x6f, 178, 425, "D " );
        M600_Text_0X64_Display(9, 190, 425, monitor_weast_ml);
        Dwin_Text_Display(cmd_Text_0x6f, 178, 450, "R " );
        M600_Text_0X64_Display(9, 190, 450, Monitor_Red_AD);
        Dwin_Text_Display(cmd_Text_0x6f, 178, 475, "G " );
        M600_Text_0X64_Display(9, 190, 475, Monitor_Green_AD);

    }
}
/***********************************************/
/* 函数名称: Dwin_Set_pra_Display              */
/* 功能描述: 泵速显示                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Dwin_Set_Alarm_Display(void)
{
#define  PIC_SET_ALARM   0X05
    switch(Set_pra_now)
    {
    case SET_FEIYE_ALARM:
        if(Set_feiye < 9500)
        {
            M600_Set_Display(PIC_SET_ALARM, Color_RED, SET_PRA_Address(), Set_feiye);
            M600_Set_Display(PIC_SET_ALARM, Color_RED, SET_FEIYE_Address(), Set_feiye);
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_FEIYE_Address(), 30 + 123, 207 + 50, 30, 207);
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_PRA_Address(), 610 + 123, 245 + 50, SET_PRA_Address());
            M600_Set_Text_Display(PIC_SET_ALARM, Color_RED, SET_PRA_Address(), Bit_check_1(Set_Alarm_flag, FEIYE_ALARM_SW));
            M600_Set_Text_Display(PIC_SET_ALARM, Color_RED, SET_FEIYE_ON_Address(), Bit_check_1(Set_Alarm_flag, FEIYE_ALARM_SW));
        }
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_AIR_Address(), Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_BLOOD_Address(), Bit_check_1(Set_Alarm_flag, BLOOD_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_LOUYE_Address(), Bit_check_1(Set_Alarm_flag, LOUYE_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_WELL_Address(), Bit_check_1(Set_Alarm_flag, WELL_ALARM_SW));
        break;
    case SET_BLOOD_ALARM:
        if(Set_feiye < 9500)
        {
            M600_Set_Display(PIC_SET_ALARM, Color_White, SET_PRA_Address(), Set_feiye);
            M600_Set_Display(PIC_SET_ALARM, Color_White, SET_FEIYE_Address(), Set_feiye);
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_FEIYE_Address(), 30 + 123, 207 + 50, 30, 207);
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_PRA_Address(), 610 + 123, 230 + 50, SET_PRA_Address());
            M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_FEIYE_ON_Address(), Bit_check_1(Set_Alarm_flag, FEIYE_ALARM_SW));
        }
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_AIR_Address(), Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_RED, SET_BLOOD_Address(), Bit_check_1(Set_Alarm_flag, BLOOD_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_LOUYE_Address(), Bit_check_1(Set_Alarm_flag, LOUYE_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_WELL_Address(), Bit_check_1(Set_Alarm_flag, WELL_ALARM_SW));

        break;
    case SET_AIR_ALARM:
        if(Set_feiye < 9500)
        {
            M600_Set_Display(PIC_SET_ALARM, Color_White, SET_PRA_Address(), Set_feiye);
            M600_Set_Display(PIC_SET_ALARM, Color_White, SET_FEIYE_Address(), Set_feiye);
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_FEIYE_Address(), 30 + 123, 207 + 50, 30, 207);
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_PRA_Address(), 610 + 123, 230 + 50, SET_PRA_Address());
            M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_FEIYE_ON_Address(), Bit_check_1(Set_Alarm_flag, FEIYE_ALARM_SW));
        }
        M600_Set_Text_Display(PIC_SET_ALARM, Color_RED, SET_AIR_Address(), Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_BLOOD_Address(), Bit_check_1(Set_Alarm_flag, BLOOD_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_LOUYE_Address(), Bit_check_1(Set_Alarm_flag, LOUYE_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_WELL_Address(), Bit_check_1(Set_Alarm_flag, WELL_ALARM_SW));
        break;
    case SET_WELL_ALARM:
        if(Set_feiye < 9500)
        {
            M600_Set_Display(PIC_SET_ALARM, Color_White, SET_PRA_Address(), Set_feiye);
            M600_Set_Display(PIC_SET_ALARM, Color_White, SET_FEIYE_Address(), Set_feiye);
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_FEIYE_Address(), 30 + 123, 207 + 50, 30, 207);
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_PRA_Address(), 610 + 123, 230 + 50, SET_PRA_Address());
            M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_FEIYE_ON_Address(), Bit_check_1(Set_Alarm_flag, FEIYE_ALARM_SW));
        }
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_AIR_Address(), Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_BLOOD_Address(), Bit_check_1(Set_Alarm_flag, BLOOD_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_LOUYE_Address(), Bit_check_1(Set_Alarm_flag, LOUYE_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_RED, SET_WELL_Address(), Bit_check_1(Set_Alarm_flag, WELL_ALARM_SW));
        break;
    case SET_LOUYE_ALARM:
        if(Set_feiye < 9500)
        {
            M600_Set_Display(PIC_SET_ALARM, Color_White, SET_PRA_Address(), Set_feiye);
            M600_Set_Display(PIC_SET_ALARM, Color_White, SET_FEIYE_Address(), Set_feiye);
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_FEIYE_Address(), 30 + 123, 207 + 50, 30, 207);
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_PRA_Address(), 610 + 123, 230 + 50, SET_PRA_Address());
            M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_FEIYE_ON_Address(), Bit_check_1(Set_Alarm_flag, FEIYE_ALARM_SW));
        }
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_AIR_Address(), Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_BLOOD_Address(), Bit_check_1(Set_Alarm_flag, BLOOD_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_RED, SET_LOUYE_Address(), Bit_check_1(Set_Alarm_flag, LOUYE_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_WELL_Address(), Bit_check_1(Set_Alarm_flag, WELL_ALARM_SW));
        break;
    //		case SET_BOWL:
    //			   M600_Set_background(570,133);
    //			    M600_Set_Text_Display(PIC_SET_ALARM,Color_White,SET_AIR_Address(),Bit_check_1(Set_Alarm_flag,AIR_ALARM_SW));
    //	      	M600_Set_Text_Display(PIC_SET_ALARM,Color_White,SET_BLOOD_Address(),Bit_check_1(Set_Alarm_flag,BLOOD_ALARM_SW));
    //	      	M600_Set_Text_Display(PIC_SET_ALARM,Color_White,SET_LOUYE_Address(),Bit_check_1(Set_Alarm_flag,LOUYE_ALARM_SW));
    //	      	M600_Set_Text_Display(PIC_SET_ALARM,Color_White,SET_WELL_Address(),Bit_check_1(Set_Alarm_flag,WELL_ALARM_SW));

    //		     break;
    //	   break;
    default:
        M600_Set_background(570, 133);
        if(Set_feiye < 9500)
        {
            M600_Set_Display(PIC_SET_ALARM, Color_White, SET_PRA_Address(), Set_feiye);
            M600_Set_Display(PIC_SET_ALARM, Color_White, SET_FEIYE_Address(), Set_feiye);
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_FEIYE_Address(), 30 + 123, 207 + 50, 30, 207);
            M600_Deal_Picture(Command_Cut_Picture, PIC_SET_ALARM, SET_PRA_Address(), 610 + 123, 230 + 50, SET_PRA_Address());
            M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_FEIYE_ON_Address(), Bit_check_1(Set_Alarm_flag, FEIYE_ALARM_SW));
        }
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_AIR_Address(), Bit_check_1(Set_Alarm_flag, AIR_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_BLOOD_Address(), Bit_check_1(Set_Alarm_flag, BLOOD_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_LOUYE_Address(), Bit_check_1(Set_Alarm_flag, LOUYE_ALARM_SW));
        M600_Set_Text_Display(PIC_SET_ALARM, Color_White, SET_WELL_Address(), Bit_check_1(Set_Alarm_flag, WELL_ALARM_SW));

        break;
    }

}
/***********************************************/
/* 函数名称: Dwin_Set_Time_Display              */
/* 功能描述: 泵速显示                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月1日                      */
/***********************************************/
void Dwin_Set_Time_Display(void)
{
#define PIC_SET_TIME   0X08
    switch(Set_pra_now)
    {
    case SET_YEAR:
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_TIME_Address(), Time_Set.year);
        M600_Set_Display(PIC_SET_TIME, Color_RED, SET_YEAR_Address(), Time_Set.year);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MONTH_Address(), Time_Set.month);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_DAY_Address(), Time_Set.day);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_HOUR_Address(), Time_Set.hour);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_SEC_Address(), Time_Set.sec);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MIN_Address(), Time_Set.min);
        break;
    case SET_MONTH:
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_TIME_Address(), Time_Set.month);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_YEAR_Address(), Time_Set.year);
        M600_Set_Display(PIC_SET_TIME, Color_RED, SET_MONTH_Address(), Time_Set.month);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_DAY_Address(), Time_Set.day);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_HOUR_Address(), Time_Set.hour);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_SEC_Address(), Time_Set.sec);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MIN_Address(), Time_Set.min);
        break;
    case SET_DAY:
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_TIME_Address(), Time_Set.day);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_YEAR_Address(), Time_Set.year);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MONTH_Address(), Time_Set.month);
        M600_Set_Display(PIC_SET_TIME, Color_RED, SET_DAY_Address(), Time_Set.day);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_HOUR_Address(), Time_Set.hour);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_SEC_Address(), Time_Set.sec);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MIN_Address(), Time_Set.min);
        break;
    case SET_MIN:
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_TIME_Address(), Time_Set.min);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_YEAR_Address(), Time_Set.year);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MONTH_Address(), Time_Set.month);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_DAY_Address(), Time_Set.day);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_HOUR_Address(), Time_Set.hour);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_SEC_Address(), Time_Set.sec);
        M600_Set_Display(PIC_SET_TIME, Color_RED, SET_MIN_Address(), Time_Set.min);
        break;
    case SET_HOUR:
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_TIME_Address(), Time_Set.hour);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_YEAR_Address(), Time_Set.year);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MONTH_Address(), Time_Set.month);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_DAY_Address(), Time_Set.day);
        M600_Set_Display(PIC_SET_TIME, Color_RED, SET_HOUR_Address(), Time_Set.hour);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_SEC_Address(), Time_Set.sec);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MIN_Address(), Time_Set.min);
        break;
    case SET_SEC:
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_TIME_Address(), Time_Set.sec);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_YEAR_Address(), Time_Set.year);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MONTH_Address(), Time_Set.month);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_DAY_Address(), Time_Set.day);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_HOUR_Address(), Time_Set.hour);
        M600_Set_Display(PIC_SET_TIME, Color_RED, SET_SEC_Address(), Time_Set.sec);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MIN_Address(), Time_Set.min);
        break;
    default:
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_YEAR_Address(), Time_Set.year);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MONTH_Address(), Time_Set.month);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_DAY_Address(), Time_Set.day);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_HOUR_Address(), Time_Set.hour);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_SEC_Address(), Time_Set.sec);
        M600_Set_Display(PIC_SET_TIME, Color_White, SET_MIN_Address(), Time_Set.min);
        break;
    }

}
/*******************************************************************************
* Function Name  : Gui_factory_display
* Description    : 工厂模式界面显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_factory_display(void)//
{
#define VER_DIS_ADD()   200 ,85
    uint8 str[11] = {0, 2, 0, 0, '-', 0, 0, '-', 0, 0, '\0'};
    M600_Set_Palette(0x0000, 0xFFFF);
    switch(Display_Loop)
    {
    case 0:

        M600_Text_0X64_Display(9, Monitor_R_WEIGHT_Address(), Monitor_Reservoir_weigh);
        M600_Text_0X64_Display(9, Monitor_R_WEIGHT_ML_Address(), monitor_reservoir_ml);
        M600_Text_0X64_Display(9, Monitor_D_WEIGHT_Address(), Monitor_Dwon_weigh);
        M600_Text_0X64_Display(9, Monitor_D_WEIGHT_ML_Address(), monitor_weast_ml);
        if(seting_flag == 1)
        {
            M600_Set_Palette(0xffff, 0x0000);
            M600_Text_0X64_Display(9, 315, 119, Reservoir_zero);
            M600_Text_0X64_Display(9, 430, 119, Reservoir_500);
            M600_Text_0X64_Display(9, 315, 159, Dwon_weigh_7000);
            M600_Text_0X64_Display(9, 430, 159, Dwon_weigh_9000);
            M600_Set_Palette(0x0000, 0xFFFF);
        }
        else
        {
            M600_Text_0X64_Display(9, 315, 119, Reservoir_zero);
            M600_Text_0X64_Display(9, 430, 119, Reservoir_500);
            M600_Text_0X64_Display(9, 315, 159, Dwon_weigh_7000);
            M600_Text_0X64_Display(9, 430, 159, Dwon_weigh_9000);
        }
        Display_Loop = 1;
        break;
    case 1:
        M600_Text_0X55_Display(9, Monitor_S_LOUYE_Address(), READ_FUILT_UP_STATE());
        M600_Text_0X55_Display(9, Monitor_X_LOUYE_Address(), READ_FUILT_DWON_STATE());
        Display_Loop = 2;
        break;
    case 2:
        M600_Text_0X55_Display(9, Monitor_RED_Address(), Monitor_Red_AD);
        M600_Text_0X55_Display(9, Monitor_GRE_Address(), Monitor_Green_AD);
        Display_Loop = 3;
        break;
    case 3:
        Dwin_Text_Display(cmd_Text_0x6f, VER_DIS_ADD(), ver_str);
        M600_Text_0X55_Display(9, Monitor_WELL_Address(), READ_WELL_STATE());
        M600_Text_0X55_Display(9, Monitor_AIR_Address(), Monitor_Air_AD);
        Display_Loop = 4;
        break;
    case 4:
        M600_Text_0X55_Display(9, Monitor_CELL_Address(), Monitor_Blood_AD);
        //M600_Deal_Picture(Command_Cut_Picture,9,Monitor_RPM_Address(),580,  420,Monitor_RPM_Address());
        M600_Text_0X55_Display(Show_32X32, Monitor_RPM_Address(), Centrifugal_motor_speed_Set);
        Display_Loop = 0;
        break;
    default:
        break;
    }

}
/*******************************************************************************
* Function Name  : Gui_factory_display
* Description    : 工厂模式界面显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Factory_Line_display(uint8 line, uint8 *title, uint8 *content)
{
#define VER_DIS_ADD()   200 ,85
#define SYS_BEGEIN_ADD_Y  50
#define SYSINFO_WIDE      30

    uint8 title_len;
    uint8 temp_i;
    for(temp_i = 0;; temp_i++)
    {
        if(temp_i > 250)
            break;
        if(title[temp_i] == 0)
            break;
    }
    //title_len=strlrn(title);
    if(line == line_select)
        M600_Set_Palette(0x0000, 0xffff);
    else
        M600_Set_Palette(0xffff, 0x0000);
    Dwin_Text_Display(cmd_Text_0x6f, 10, SYS_BEGEIN_ADD_Y + line * SYSINFO_WIDE, title);
    Dwin_Text_Display(cmd_Text_0x6f, 10 + 12 * temp_i, SYS_BEGEIN_ADD_Y + line * SYSINFO_WIDE, content);
    M600_Set_Palette(0xffff, 0x0000);
}

/*******************************************************************************
* Function Name  : Factory_Line_num_display
* Description    : 工厂模式界面显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Factory_Line_num_display(uint8 line, uint8 *title, uint16 num)
{
#define VER_DIS_ADD()   200 ,85
#define SYS_BEGEIN_ADD_Y  50
#define SYSINFO_WIDE      30
    //uint8 line_select=1;
    uint8 title_len;
    uint8 temp_i;
    uint8 str[10];
    Integer_Number_Change_String(num, str);
    for(temp_i = 0;; temp_i++)
    {
        if(temp_i > 250)
        {
            temp_i = 250;
            break;
        }
        if(title[temp_i] == 0)
            break;
    }
    //title_len=strlrn(title);
    if(line == line_select)
        M600_Set_Palette(0x0000, 0xffff);
    else
        M600_Set_Palette(0xffff, 0x0000);
    Dwin_Text_Display(cmd_Text_0x6f, 10, SYS_BEGEIN_ADD_Y + line * SYSINFO_WIDE, title);
    Dwin_Text_Display(cmd_Text_0x6f, 10 + 12 * temp_i, SYS_BEGEIN_ADD_Y + line * SYSINFO_WIDE, str);
    M600_Set_Palette(0xffff, 0x0000);
}
/*******************************************************************************
* Function Name  : Gui_factory_display
* Description    : 工厂模式界面显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_factory_time_display(uint8 line)
{
    uint32 power_on_Time_sec_remp;
    uint16 day;
    uint8 str[9] = {0, 0, ':', 0, 0, ':', 0, 0, '\0'};
    day = power_on_Time_sec / 86400;
    power_on_Time_sec_remp = power_on_Time_sec % 86400;
    power_on_Time.hour = power_on_Time_sec_remp ;
    power_on_Time.hour = power_on_Time_sec_remp / 3600; //小时
    power_on_Time.min = (power_on_Time_sec_remp % 3600) / 60; //分钟
    power_on_Time.sec = (power_on_Time_sec_remp % 3600) % 60; //秒钟
    Factory_Line_num_display(line, "累计开机时间:", day);
    str[0] = power_on_Time.hour / 10 + '0';
    str[1] = power_on_Time.hour % 10 + '0';
    str[3] = power_on_Time.min / 10 + '0';
    str[4] = power_on_Time.min % 10 + '0';
    str[6] = power_on_Time.sec / 10 + '0';
    str[7] = power_on_Time.sec % 10 + '0';
    Dwin_Text_Display(cmd_Text_0x6f, 10 + 12 * 18, SYS_BEGEIN_ADD_Y + line * SYSINFO_WIDE, str);


}

/*******************************************************************************
* Function Name  : Gui_factory_display
* Description    : 工厂模式界面显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_factory_1_display(void)//
{
    uint8 str[11] = {0, 2, 0, 0, '-', 0, 0, '-', 0, 0, '\0'};

    M600_Set_Palette(0xffff, 0x0000);
    switch(Display_Loop)
    {
    case 0:
        Factory_Line_display(1, "SW:", ver_str);
        Factory_Line_display(2, "HW:", "HW1.00");
        Factory_Line_display(3, "SN:", Series_No);
        Factory_Line_display(5, "生产编号", Fac_Series_No);
        Gui_factory_time_display(6);
        Factory_Line_display(4, "sw_inside:", ver_str_insine);
        Display_Loop = 1;
        break;
    case 1:
        Factory_Line_display(7, "电机类型:", ((Default_Pump_set == PEMP_CHECK_DISABLE) ? "闭环电机" : "开环电机"));
        Factory_Line_num_display(9, "清洗泵系数:", wash_pump_adjust);
        Factory_Line_num_display(8, "进血泵系数:", fill_pump_adjust);
        Factory_Line_num_display(10, "清空泵系数125:" , empty_pump_adjust_125);
        Factory_Line_num_display(11, "清空泵系数225:" , empty_pump_adjust_225);
        Display_Loop = 2;
        break;
    case 2:
        Factory_Line_num_display(12, "默认预充量：" , Set_Prefill_Volume);
        Factory_Line_display(13, "默认模式：" , ((Default_Mode_set == MODE_AUTO) ? "自动" : "手动")); //				Default_Mode_set
        Factory_Line_num_display(14, "离心电机系数" , Centrifugal_motor_speed_Adj); //				Default_Mode_set
        Factory_Line_display(15, "清洗溢出保护：" , ((Wash_cell_Protect == PROTECT_ABLE) ? "有效" : "无效")); //				Default_Mode_set
        Factory_Line_num_display(16, "血层阈值：" , Cell_check_gate); //				Default_Mode_set
        Display_Loop = 3;
        break;
    case 3:
        //Dwin_Text_Display(cmd_Text_0x64,VER_DIS_ADD(),ver_str);
        //M600_Text_0X55_Display(9,Monitor_WELL_Address(),READ_WELL_STATE());
        // M600_Text_0X55_Display(9,Monitor_AIR_Address(),Monitor_Air_AD);
        Display_Loop = 4;
        break;
    case 4:
        // M600_Text_0X55_Display(9,Monitor_CELL_Address(),Monitor_Blood_AD);
        //M600_Deal_Picture(Command_Cut_Picture,9,Monitor_RPM_Address(),580,  420,Monitor_RPM_Address());
        //  M600_Text_0X55_Display(Show_32X32,Monitor_RPM_Address(),Centrifugal_motor_speed_Set);
        Display_Loop = 0;
        break;
    default:
        break;
    }

}
/*******************************************************************************
* Function Name  : Gui_factory_display
* Description    : 工厂模式界面显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_work_display(void)//
{

    switch(Display_Loop)
    {
    case 0:
        M600_Set_Palette(0x389f, 0xf6ef);
        M600_Set_background(454, 201);
        Dwin_Pump_State_Display();
        Dwin_Monitor_Display(Monitor_Fill_Volume, Monitor_FILL_Address()); //	Monitor_Fill_Volume
        Dwin_Monitor_Display(Monitor_Empty_Volume, Monitor_EMPTY_Address());
        //Dwin_Monitor_Display(Monitor_Waste_Volume,Monitor_WASTE_Address());
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 14, (120 + work_sta * 43), 105, (162 + work_sta * 43), 55, 165);

        Display_Loop = 1;
        break;
    case 1:
        Main_Dbug_Display();
        if(work_sta == WORK_PRE_FILL)
            Dwin_Monitor_Display(Monitor_Pre_fill_Volume, Monitor_WASH_Address());
        else
            Dwin_Monitor_Display(Monitor_Wash_Volume, Monitor_WASH_Address());
        M600_DISPLAY_Date_time();
        Dwin_info_Display();
        Display_Loop = 2;
        break;
    case 2:
        Printer_state_Display();
        Dwin_Pause_Display();
        Res_state_Display();
        Display_Loop = 3;
        break;
    case 3:
        Dwin_Pump_speed_Display();
        Dwin_Mode_Display();
        if(Mode == MODE_NOWASH)
        {
            M600_Deal_Picture(Command_Cut_Picture, 32, 0, 508, 574, 600, 0, 508); //刷新按键底图
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 02, 0, 508, 574, 600, 0, 508); //刷新按键底图
            if(work_sta > 1)
            {
                switch(work_sta)
                {
                case 2:
                    M600_Deal_Picture(Command_Cut_Picture, 12, (0 + (work_sta - 2) * 112), 508, (112 + (work_sta - 2) * 112), 600, (0 + (work_sta - 2) * 112), 508); //状态按键反色
                    break;
                case 3:
                    M600_Deal_Picture(Command_Cut_Picture, 12, (0 + (work_sta - 2) * 112), 508, (112 + (work_sta - 2) * 112), 600, (0 + (work_sta - 2) * 112), 508); //状态按键反色
                    break;
                case 4:
                    M600_Deal_Picture(Command_Cut_Picture, 12, (0 + (work_sta - 2) * 114), 508, (112 + (work_sta - 2) * 114), 600, (0 + (work_sta - 2) * 114), 508); //状态按键反色
                    break;
                case 5:
                    M600_Deal_Picture(Command_Cut_Picture, 12, (0 + (work_sta - 2) * 115), 508, (112 + (work_sta - 2) * 115), 600, (0 + (work_sta - 2) * 115), 508); //状态按键反色
                    break;
                case 6:
                    M600_Deal_Picture(Command_Cut_Picture, 12, (0 + (work_sta - 2) * 116), 508, (112 + (work_sta - 2) * 116), 600, (0 + (work_sta - 2) * 116), 508); //状态按键反色
                    break;
                default:
                    break;
                }
            }
        }
        if(Set_Bowl == 225)
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 2, 4, 98, 39, 21, 60);
        else
            M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 2, 40, 98, 81, 21, 59);

        Display_Loop = 4;
        break;
    case 4:

        if(Mode == MODE_YINGJI)
        {
            M600_Deal_Picture(Command_Cut_Picture, 47, 2, 48, 200, 83, 100, 10);
        }
        else
            Dwin_Alarm_Display();

        Display_Loop = 0;
        break;
    default:
        break;
    }

}
/*******************************************************************************
* Function Name  : Gui_Set_display
* Description    : 设置显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_Set_pra_display(void)//
{
    switch(Display_Loop)
    {
    case 0:
        Dwin_Set_pra_Display();
        Display_Loop = 1;
        break;
    case 1:
        Display_Loop = 2;
        break;
    case 2:
        Display_Loop = 3;
        break;
    case 3:
        Display_Loop = 4;
        break;
    case 4:
        Display_Loop = 0;
        break;
    default:
        Display_Loop = 0;
        break;
    }
}
/*******************************************************************************
* Function Name  : Gui_Set_mode_display
* Description    : 设置显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_Set_mode_display(void)//
{
    uint8 temp_i;
    switch(Display_Loop)
    {
    case 0:
        M600_Set_background(MODE_Address());
        Display_Loop = 1;
        break;
    case 1:

        if(set_mode > 0)
        {
            for(temp_i = 1; temp_i < 5; temp_i++)
            {
                if(temp_i == set_mode)
                {
                    M600_Deal_Picture(Command_Cut_Picture, 13, (118 + (set_mode - 1) * 140), 220, (118 + (set_mode) * 140), 340, (118 + (set_mode - 1) * 140), 220);
                    M600_Deal_Picture(Command_Cut_Picture, 33, 0, (0 + (set_mode - 1) * 65), 295, (0 + (set_mode) * 65), 200, 370);
                }
                else
                    M600_Deal_Picture(Command_Cut_Picture, 3, (118 + (temp_i - 1) * 140), 220, (118 + (temp_i) * 140), 340, (118 + (temp_i - 1) * 140), 220);
            }

        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 3, 115, 220, 698, 341, 115, 220);
            M600_Deal_Picture(Command_Cut_Picture, 3, 200, 370, ( 200 + 295 ), ( 370 + 65 ), 200, 370);
        }
        Display_Loop = 2;
        break;
    case 2:
        // M600_Deal_Picture(Command_Cut_Picture,PIC_CUT_KEY,200,200,280,220,120,160);
        M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_KEY, 200, (220 + (set_mode - 1) * 20), 280, (240 + (set_mode - 1) * 20), 200, 160);

        Display_Loop = 3;
        break;
    case 3:
        Display_Loop = 4;
        break;
    case 4:
        Display_Loop = 0;
        break;
    default:
        break;
    }
}
/*******************************************************************************
* Function Name  : Gui_Set_huanzhe_display
* Description    : 设置显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_Set_huanzhe_display(void)//
{
    uint8 str[6] = {'a', 'b', '1', '2', '3', '\0'};
    M600_Set_Palette(0x0000, 0xffff);
    M600_Set_background(175, 106);
    switch(Display_Loop)
    {
    case 0:
        if(Set_pra_now == SET_patients)
        {
            M600_Deal_Picture(Command_Cut_Picture, 16, 155, 94, 490, 138, 155, 94);
            Dwin_Text_Display(cmd_Text_0x6f, 180, 107, Set_person);
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 0x06, 155, 94, 490, 138, 155, 94);
            Dwin_Text_Display(cmd_Text_0x6f, 180, 107, Set_person);
        }

        Display_Loop = 1;
        break;
    case 1:

        if(Set_pra_now == SET_KESHI)
        {

            M600_Deal_Picture(Command_Cut_Picture, 16, 549, 90, 672, 140, 549, 90);
            M600_Deal_Picture(Command_Cut_Picture, 16, 680, 90, 780, 230, 680, 90);
            M600_Set_background(570, 107);
        }
        else
        {

            M600_Deal_Picture(Command_Cut_Picture, 6, 680, 90, 780, 230, 680, 90);
            M600_Deal_Picture(Command_Cut_Picture, 6, 549, 90, 672, 140, 549, 90);
            M600_Set_background(570, 107);
        }
        switch (Set_keshi)
        {
        case 0x31:
            Dwin_Text_Display(cmd_Text_0x6f, 570, 107, " 骨科   ");
            break;
        case 0x32:
            Dwin_Text_Display(cmd_Text_0x6f, 570, 107, " 心外科 ");
            break;
        case 0x33:
            Dwin_Text_Display(cmd_Text_0x6f, 570, 107, " 妇产科 ");
            break;
        case 0x34:
            Dwin_Text_Display(cmd_Text_0x6f, 570, 107, " 其他   ");
            break;
        case 0x35:
            break;
        default:
            break;

        }
        Display_Loop = 2;
        break;
    case 2:
        if(Set_pra_now == SET_AGE)
        {

            M600_Deal_Picture(Command_Cut_Picture, 16, 550, 152, 647, 199, 550, 152);

        }
        else
        {

            M600_Deal_Picture(Command_Cut_Picture, 6, 550, 152, 647, 199, 550, 152);

        }
        M600_Set_background(577, 160);
        Dwin_Text_Display(cmd_Text_0x6f, 577, 160, Set_person_age);
        Display_Loop = 3;
        break;
    case 3:
        if(Set_pra_now == SET_SEX)
        {
            M600_Deal_Picture(Command_Cut_Picture, 16, 400, 150, 485, 200, 400, 150);
            M600_Set_background(416, 160);
            if(person_sex == 1)
                Dwin_Text_Display(cmd_Text_0x6f, 416, 160, "女");
            else if(person_sex == 2)
                Dwin_Text_Display(cmd_Text_0x6f, 416, 160, "男");
            M600_Deal_Picture(Command_Cut_Picture, 16, 405, 200, 480, 225, 405, 200);

        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 6, 400, 150, 485, 200, 400, 150);
            M600_Set_background(416, 160);
            if(person_sex == 1)
                Dwin_Text_Display(cmd_Text_0x6f, 416, 160, "女");
            else if(person_sex == 2)
                Dwin_Text_Display(cmd_Text_0x6f, 416, 160, "男");
            M600_Deal_Picture(Command_Cut_Picture, 6, 405, 200, 480, 225, 405, 200);

        }
        Display_Loop = 4;
        break;
    case 4:
        if(Set_pra_now == SET_NAME)
        {
            M600_Deal_Picture(Command_Cut_Picture, 16, 150, 155, 329, 200, 150, 155);
            M600_Set_background(170, 160);
            Dwin_Text_Display(cmd_Text_0x6f, 170, 160, Set_person_name);
        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 6, 150, 155, 329, 200, 150, 155);
            M600_Set_background(170, 160);
            Dwin_Text_Display(cmd_Text_0x6f, 170, 160, Set_person_name);
        }
        Display_Loop = 0;
        break;
    default:
        break;
    }
}
/*******************************************************************************
* Function Name  : Gui_Set_alarm_display
* Description    : 设置显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_Set_alarm_display(void)//
{
#define PIC_SET_ALARM   0X05
    // M600_Set_Palette(0xFFFF,0xf0f0);
    //M600_Set_background(SET_FEIYE_Address());
    switch(Display_Loop)
    {
    case 0:

        Dwin_Set_Alarm_Display();
        Display_Loop = 1;
        break;
    case 1:
        Display_Loop = 2;
        break;
    case 2:
        Display_Loop = 3;
        break;
    case 3:
        Display_Loop = 4;
        break;
    case 4:
        Display_Loop = 0;
        break;
    default:
        break;
    }
}
/*******************************************************************************
* Function Name  : Gui_Tend_display
* Description    : 趋势查看显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_Tend_display(void)//
{
#define P_ON_TIME_ADD()    20,170
#define FILL_TIME_ADD()    20,208
#define WASH_TIME_ADD()    20,250
#define EMPTY_TIME_ADD()   20,290

    switch(Display_Loop)
    {
    case 0:
        //M600_Time_set_Display(Show_32X32,SET_YEAR_Address(),Tend_num);
        Tend_Time = Read_Poweron_time(GUI_Tend_num - 1);
        prt_Time = Tend_Time;
        M600_DISPLAY_Tend_time(Tend_Time, P_ON_TIME_ADD());
        Display_Loop = 1;
        break;
    case 1:
        Tend_fill_Time = Read_fill_time(GUI_Tend_num - 1);
        Tend_fill_Time.year = Tend_Time.year;
        Tend_fill_Time.day = Tend_Time.day;
        Tend_fill_Time.month = Tend_Time.month;
        M600_DISPLAY_Tend_time(Tend_fill_Time, FILL_TIME_ADD());

        Display_Loop = 2;
        break;
    case 2:
        M600_DISPLAY_Tend_page();
        Tend_wash_Time = Read_wash_time(GUI_Tend_num - 1);
        Tend_wash_Time.year = Tend_Time.year;
        Tend_wash_Time.day = Tend_Time.day;
        Tend_wash_Time.month = Tend_Time.month;
        M600_DISPLAY_Tend_time(Tend_wash_Time, WASH_TIME_ADD());

        Display_Loop = 3;
        break;
    case 3:
        M600_DISPLAY_Tend_huanzhe();
        Display_Loop = 4;
        break;
    case 4:
        Tend_empty_Time = Read_wash_time(GUI_Tend_num - 1);
        Tend_empty_Time.year = Tend_Time.year;
        Tend_empty_Time.day = Tend_Time.day;
        Tend_empty_Time.month = Tend_Time.month;
        M600_DISPLAY_Tend_time(Tend_empty_Time, EMPTY_TIME_ADD());
        Read_sta(GUI_Tend_num - 1);
        M600_DISPLAY_Tend_Monitor(7, 561, 215, Tend_Fill_Volume);
        M600_DISPLAY_Tend_Monitor(7, 561, 255, Tend_Wash_Volume);
        M600_DISPLAY_Tend_Monitor(7, 561, 295, Tend_Empty_Volume);
        Display_Loop = 0;
        break;
    default:
        break;
    }
}
/*******************************************************************************
* Function Name  : Gui_Set_display
* Description    : 设置显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_Set_time_display(void)   //
{
    switch(Display_Loop)
    {
    case 0:

        Display_Loop = 1;
        break;
    case 1:

        Display_Loop = 2;
        break;
    case 2:

        Display_Loop = 3;
        break;
    case 3:

        Display_Loop = 4;
        break;
    case 4:
        Dwin_Set_Time_Display();
        Display_Loop = 0;
        break;
    default:
        break;
    }

}
/*******************************************************************************
* Function Name  : Gui_select_display
* Description    : 设置显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_select_display(void)//
{
    switch(Display_Loop)
    {
    case 0:

        if(poweroff_time > 59)
        {
            ;//M600_Deal_Picture(Command_Cut_Picture,10,220,500,800,600,220,500);

        }
        else
        {
            M600_Deal_Picture(Command_Cut_Picture, 10, 220, 500, 800, 600, 220, 500);
            M600_Text_2_Display(10, POWEROFF_TIME_Address(), poweroff_time);
        }

        Display_Loop = 1;
        break;
    case 1:
        M600_Set_Palette(0x35ff, 0x0000);
        M600_Set_background(3, 590);
        Dwin_Text_Display(cmd_Text_0x6e, 3, 590, ver_str);
        Display_Loop = 2;
        break;
    case 2:
        Display_Loop = 3;
        break;
    case 3:
        Display_Loop = 4;
        break;
    case 4:
        //M600_DISPLAY_Soft_ver();
        Display_Loop = 0;
        break;
    default:
        break;
    }

}
/*******************************************************************************
* Function Name  : Gui_Set_display
* Description    : 设置显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_Set_display(void)//
{
    switch(GUI_Set_State)
    {
    case GUI_SET_PRA:
        Gui_Set_pra_display();
        break;
    case GUI_SET_MODE:
        Gui_Set_mode_display();
        break;
    case GUI_SET_ARLAM:
        Gui_Set_alarm_display();
        break;
    case GUI_SET_HUANZHE:
        Gui_Set_huanzhe_display();
        break;
    case GUI_SET_TEND:
        Gui_Tend_display();
        break;
    case GUI_SET_TIME:
        Gui_Set_time_display();
        break;
    default:
        break;
    }
}

/*******************************************************************************
* Function Name  : Gui_factory_display
* Description    : 工厂模式界面显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_Test_display(void)//
{
#define PIC_CUT_TEST      46
#define FILL_CHECK_ADD()    300,340
#define WASH_CHECK_ADD()    300,340
#define EMPTY_CHECK_ADD()   300,340
#define SENSOR_CHECK_ADD()  300,340
#define LOUYE_CHECK_ADD()   300,340
#define MOTO_CHECK_ADD()    300,340
#define TEST_SHOW_E()       M600_Deal_Picture(Command_Cut_Picture,PIC_CUT_TEST,170,288,199,312,420,340)
#define TEST_SHOW_R()	     M600_Deal_Picture(Command_Cut_Picture,PIC_CUT_TEST,170,330,199,360,420,340)
    switch(Display_Loop)
    {
    case 0:
        M600_Set_Palette(0x389f, 0xf6ef);
        M600_Set_background(200, 190);
        if(Test_Step == 2)
        {
            if(test_time > 200)
            {
                if(Bit_check_1(Test_Flag, WASH_TEST_OK))
                    TEST_SHOW_R();//
                else
                    TEST_SHOW_E();
            }
            else
            {
                M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 0, 42, 160, 72, WASH_CHECK_ADD());
                //Dwin_Text_Display(cmd_Text_0x55,WASH_CHECK_ADD(), "清洗泵自检中   ");

            }

        }
        else
        {
        }
        Display_Loop = 1;
        break;
    case 1:
        M600_Deal_Picture(Command_Cut_Picture, 11, 93, 282, (90 + 88 * Test_Step), 329, 93, 282);
        if(Test_Step == 6)
        {
            if(test_time > 200)
            {
                if(Bit_check_1(Test_Flag, MOTO_TEST_OK))
                    TEST_SHOW_R();
                else
                    TEST_SHOW_E();
            }
            else
            {
                M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 0, 162, 160, 192, MOTO_CHECK_ADD()); //清空
            }

        }
        else
        {
        }
        Display_Loop = 2;
        break;
    case 2:
        if(Test_Step == 3)
        {
            if(test_time > 250)
            {
                if(Bit_check_1(Test_Flag, EMPTY_TEST_OK))
                    TEST_SHOW_R();
                else
                    TEST_SHOW_E();
            }
            else
            {
                M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 0, 82, 160, 112, EMPTY_CHECK_ADD()); //清空
            }

        }
        else
        {
        }
        Display_Loop = 3;
        break;
    case 3:

        if(Test_Step == 1)
        {
            if(test_time > 350)
            {
                if(Bit_check_1(Test_Flag, FILL_TEST_OK))
                    TEST_SHOW_R();//进血泵
                else
                    TEST_SHOW_E();
            }
            else
            {
                M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 0, 0, 160, 30, FILL_CHECK_ADD()); //进血泵
            }
        }
        Display_Loop = 4;
        break;
    case 4:
        if(Test_Step == 5)
        {
            if(test_time > 55)
            {
                if(Bit_check_1(Test_Flag, WELL_TEST_OK))
                    M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 170, 330, 199, 360, 420, 340);
                else
                    M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 170, 288, 199, 312, 420, 340);
            }
            else
            {
                M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 0, 242, 160, 272, SENSOR_CHECK_ADD()); //自检中
            }

        }
        if(Test_Step == 4)
        {
            if(test_time > 200)
            {
                if(Bit_check_1(Test_Flag, LOUYE_TEST_OK))
                    TEST_SHOW_R();
                else
                    TEST_SHOW_E();
            }
            else
            {
                M600_Deal_Picture(Command_Cut_Picture, PIC_CUT_TEST, 0, 122, 160, 152, LOUYE_CHECK_ADD()); //louye
            }

        }
        Display_Loop = 0;
        break;
    default:
        break;
    }

}
/*******************************************************************************
* Function Name  : Gui_factory_display
* Description    : 工厂模式界面显示函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示工厂界面函数
*******************************************************************************/
void Gui_Show_Pic_full(uint8 pic_num)//
{
    switch(Display_Loop)
    {
    case 0:
        M600_Deal_Picture(Command_Show_Picture, pic_num, 0, 0, 0, 0, 0, 0); //K600中需要给它发一个指令(任意)才能正确显示
        Display_Loop = 1;
        break;
    case 1:
        Display_Loop = 2;
        break;
    case 2:
        Display_Loop = 3;
        break;
    case 3:
        Display_Loop = 4;
        break;
    case 4:
        Display_Loop = 0;
        show_pic_flag = 1;
        break;
    default:
        break;
    }

}
/*******************************************************************************
* Function Name  : GUI_contorl
* Description    : 显示控制函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : 显示控制函数
*******************************************************************************/
void GUI_contorl(void)
{
    if(show_pic_flag == 1)
    {
        switch(GUI_State)
        {
        case GUI_FACTORY:
            Gui_factory_display();
            break;
        case GUI_TEST:
            Gui_Test_display();
            break;
        case GUI_STANDBY:
            Gui_work_display();
            break;
        case GUI_0:
            Gui_select_display();
            break;
        case GUI_SET:
            Gui_Set_display();
        case GUI_MODE_SET:
            Gui_Set_display();
        case GUI_ARLAM_SET:
            Gui_Set_display();
            break;
        case GUI_TIME_SET:
            Gui_Set_time_display();
            break;
        case GUI_NEW_OLD:
            Gui_select_display();
            break;
        case GUI_HELP:

            break;
        case GUI_FACTORY_1:
            Gui_factory_1_display();
            break;
        default:
            break;
        }
    }
    else    //显示整张图片
    {
        if(PWOER_ON_FLAG == 1)
        {
            PWOER_ON_cont++;
            Gui_Show_Pic_full(Show_pic_num);
            if(PWOER_ON_cont > 30)
            {
                PWOER_ON_cont = 0;
                PWOER_ON_FLAG = 0;

            }
            else
            {
                Display_Loop = 0;
                show_pic_flag = 0;
            }

        }
        else
            Gui_Show_Pic_full(Show_pic_num);
    }

}

