
/* Includes ------------------------------------------------------------------*/
#include "RTC.h"
#include "stm32f10x.h"
#include "..\User\user_conf.h"


//time_type timer;
u8 Is_Leap_Year(u16 year)
{
    if(year % 4 == 0) //必须能被4整除
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)return 1; //如果以00结尾,还要能被400整除
            else return 0;
        }
        else return 1;
    }
    else return 0;
}
const u8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
    u16 t;
    u32 seccount = 0;
    if((syear < 1970) || (syear > 2099))return 1;
    for(t = 1970; t < syear; t++)	//把所有年份的秒钟相加
    {
        if(Is_Leap_Year(t))seccount += 31622400; //闰年的秒钟数
        else seccount += 31536000;			 //平年的秒钟数
    }
    smon -= 1;
    for(t = 0; t < smon; t++)	 //把前面月份的秒钟数相加
    {
        seccount += (u32)mon_table[t] * 86400; //月份秒钟数相加
        if(Is_Leap_Year(syear) && t == 1)seccount += 86400; //闰年2月份增加一天的秒钟数
    }
    seccount += (u32)(sday - 1) * 86400; //把前面日期的秒钟数相加
    seccount += (u32)hour * 3600; //小时秒钟数
    seccount += (u32)min * 60;	 //分钟秒钟数
    seccount += sec; //最后的秒钟加上去

    //设置时钟
    RCC->APB1ENR |= 1 << 28; //使能电源时钟
    RCC->APB1ENR |= 1 << 27; //使能备份时钟
    PWR->CR |= 1 << 8; //取消备份区写保护
    //上面三步是必须的!
    RTC->CRL |= 1 << 4; //允许配置
    RTC->CNTL = seccount; //&0xffff;
    RTC->CNTH = seccount >> 16;
    RTC->CRL &= ~(1 << 4); //配置更新
    while(!(RTC->CRL & (1 << 5))); //等待RTC寄存器操作完成
    return 0;
}
u8 RTC_Get(void)
{
    static u16 daycnt = 0;
    u32 timecount = 0;
    u32 temp = 0;
    u16 temp1 = 0;

    timecount = RTC->CNTH; //得到计数器中的值(秒钟数)
    timecount <<= 16;
    timecount += RTC->CNTL;
    //	TOTAL_Time=timecount;
    temp = timecount / 86400; //得到天数(秒钟数对应的)
    if(daycnt != temp) //超过一天了
    {
        daycnt = temp;
        temp1 = 1970;	//从1970年开始
        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))//是闰年
            {
                if(temp >= 366)temp -= 366; //闰年的秒钟数
                else break;
            }
            else temp -= 365;	 //平年
            temp1++;
        }
        timer.year = temp1; //得到年份
        temp1 = 0;
        while(temp >= 28) //超过了一个月
        {
            if(Is_Leap_Year(timer.year) && temp1 == 1) //当年是不是闰年/2月份
            {
                if(temp >= 29)temp -= 29; //闰年的秒钟数
                else break;
            }
            else
            {
                if(temp >= mon_table[temp1])temp -= mon_table[temp1]; //平年
                else break;
            }
            temp1++;
        }
        timer.month = temp1 + 1; //得到月份
        timer.day = temp + 1; //得到日期
    }
    temp = timecount % 86400; //得到秒钟数
    timer.hour = temp / 3600; //小时
    timer.min = (temp % 3600) / 60; //分钟
    timer.sec = (temp % 3600) % 60; //秒钟
    //	timer.week=RTC_Get_Week(timer.w_year,timer.w_month,timer.w_date);//获取星期

    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
    power_on_Time_sec++;
    PWR_BackupAccessCmd(ENABLE);
    BKP_WriteBackupRegister(BKP_DR2, RTC->CNTH);
    BKP_WriteBackupRegister(BKP_DR3, RTC->CNTL);
    PWR_BackupAccessCmd(DISABLE);

    return 0;
}
/*******************************************************************************
* Function Name  : Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  Power_Time (void)
{
    u32 timecount = 0;
    uint32 timecount_buf;
    //u32 poweroff_time=0;
    timecount = RTC->CNTH; //得到计数器中的值(秒钟数)
    timecount <<= 16;
    timecount += RTC->CNTL;
    //BKP_WriteBackupRegister(BKP_DR2, 0x5850);
    timecount_buf = ((BKP_ReadBackupRegister(BKP_DR2) << 16) + BKP_ReadBackupRegister(BKP_DR3));
    poweroff_time = timecount - timecount_buf;
    poweroff_time = poweroff_time / 60 + 1;
}
/***********************************************/
/* 函数名称: time_type Time_change(uint32 timer_cont)                   */
/* 功能描述: 获取时间函数                        */
/* 输  　入: No                                */
/* 输  　出: time_type                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：15年7月1 日                      */
/***********************************************/
time_type Time_change(uint32 timer_cont)
{
    time_type Time_temp;
    static u16 daycnt = 0;
    u32 timecount = 0;
    u32 temp = 0;
    u16 temp1 = 0;

    //	timecount=RTC->CNTH;//得到计数器中的值(秒钟数)
    //	timecount<<=16;
    //	timecount+=RTC->CNTL;
    //	TOTAL_Time=timecount;
    temp = timer_cont / 86400; //得到天数(秒钟数对应的)
    if(daycnt != temp) //超过一天了
    {
        daycnt = temp;
        temp1 = 1970;	//从1970年开始
        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))//是闰年
            {
                if(temp >= 366)temp -= 366; //闰年的秒钟数
                else break;
            }
            else temp -= 365;	 //平年
            temp1++;
        }
        Time_temp.year = temp1; //得到年份
        temp1 = 0;
        while(temp >= 28) //超过了一个月
        {
            if(Is_Leap_Year(Time_temp.year) && temp1 == 1) //当年是不是闰年/2月份
            {
                if(temp >= 29)temp -= 29; //闰年的秒钟数
                else break;
            }
            else
            {
                if(temp >= mon_table[temp1])temp -= mon_table[temp1]; //平年
                else break;
            }
            temp1++;
        }
        Time_temp.month = temp1 + 1; //得到月份
        Time_temp.day = temp + 1; //得到日期
    }
    temp = timecount % 86400; //得到秒钟数
    Time_temp.hour = temp / 3600; //小时
    Time_temp.min = (temp % 3600) / 60; //分钟
    Time_temp.sec = (temp % 3600) % 60; //秒钟
    return Time_temp;

}

void  RTC_Init(void)
{
    u16 temp = 0;
    if(BKP_ReadBackupRegister(BKP_DR1) != 0x5850)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
        PWR_BackupAccessCmd(ENABLE);
        BKP_DeInit();
        //RCC_LSEConfig(RCC_LSE_ON);
        RCC_LSEConfig(RCC_LSE_ON);
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            temp++;
            if(temp >= 250)
                break;
        }
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        //RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
        RCC_RTCCLKCmd(ENABLE);
        RTC_WaitForLastTask();
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_EnterConfigMode();
        RTC_SetPrescaler(32767);
        RTC_WaitForLastTask();

        RTC_ExitConfigMode();
        BKP_WriteBackupRegister(BKP_DR1, 0x5850);
    }
    else
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_EnterConfigMode();
        RTC_WaitForLastTask();
        RTC_ExitConfigMode();
    }
    Power_Time();
    RTC_Get();
    if((timer.year == 0) || (timer.month == 0) || (timer.day == 0))
        RTC_Set(2015, 12, 12, 12, 12, 12);
}



/***********************************************/
/* 函数名称: LED_flash                   */
/* 功能描述: 指示灯闪烁                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月18日                      */
/***********************************************/
void LED_flash(void)
{
    if(GPIO_ReadOutputDataBit(GPIOD , GPIO_Pin_8))
    {
        GPIO_ResetBits(GPIOD , GPIO_Pin_8);
        GPIO_SetBits(GPIOD , GPIO_Pin_9);
        GPIO_SetBits(GPIOD , GPIO_Pin_10);
    }
    else
    {
        GPIO_SetBits(GPIOD , GPIO_Pin_8);
        GPIO_ResetBits(GPIOD , GPIO_Pin_9);
        GPIO_ResetBits(GPIOD , GPIO_Pin_10);
    }
}
/***********************************************/
/* 函数名称: Pump_cunt_time                   */
/* 功能描述: 蠕动泵开始计时                    */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：14年11月18日                      */
/***********************************************/
void Pump_cunt_time(void)
{
    switch(work_sta)
    {
    case WORK_FILL:
        if(Pump_Pause == 0)
            Monitor_Fill_time++;
        break;
    case WORK_WASH:
        if(Pump_Pause == 0)
            Monitor_Wash_time++;
        break;
    case WORK_EMPTY:
        if(Pump_Pause == 0)
            Monitor_Empty_time++;
        break;
    case WORK_PRE_FILL:
        if(Pump_Pause == 0)
            Monitor_Pre_Fill_time++;
        break;
    case WORK_STANDBY:
        break;
    default:
        break;
    }
}