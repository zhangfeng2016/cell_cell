
/* Includes ------------------------------------------------------------------*/
#include "RTC.h"
#include "stm32f10x.h"
#include "..\User\user_conf.h"


//time_type timer;
u8 Is_Leap_Year(u16 year)
{
    if(year % 4 == 0) //�����ܱ�4����
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)return 1; //�����00��β,��Ҫ�ܱ�400����
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
    for(t = 1970; t < syear; t++)	//��������ݵ��������
    {
        if(Is_Leap_Year(t))seccount += 31622400; //�����������
        else seccount += 31536000;			 //ƽ���������
    }
    smon -= 1;
    for(t = 0; t < smon; t++)	 //��ǰ���·ݵ����������
    {
        seccount += (u32)mon_table[t] * 86400; //�·����������
        if(Is_Leap_Year(syear) && t == 1)seccount += 86400; //����2�·�����һ���������
    }
    seccount += (u32)(sday - 1) * 86400; //��ǰ�����ڵ����������
    seccount += (u32)hour * 3600; //Сʱ������
    seccount += (u32)min * 60;	 //����������
    seccount += sec; //�������Ӽ���ȥ

    //����ʱ��
    RCC->APB1ENR |= 1 << 28; //ʹ�ܵ�Դʱ��
    RCC->APB1ENR |= 1 << 27; //ʹ�ܱ���ʱ��
    PWR->CR |= 1 << 8; //ȡ��������д����
    //���������Ǳ����!
    RTC->CRL |= 1 << 4; //��������
    RTC->CNTL = seccount; //&0xffff;
    RTC->CNTH = seccount >> 16;
    RTC->CRL &= ~(1 << 4); //���ø���
    while(!(RTC->CRL & (1 << 5))); //�ȴ�RTC�Ĵ����������
    return 0;
}
u8 RTC_Get(void)
{
    static u16 daycnt = 0;
    u32 timecount = 0;
    u32 temp = 0;
    u16 temp1 = 0;

    timecount = RTC->CNTH; //�õ��������е�ֵ(������)
    timecount <<= 16;
    timecount += RTC->CNTL;
    //	TOTAL_Time=timecount;
    temp = timecount / 86400; //�õ�����(��������Ӧ��)
    if(daycnt != temp) //����һ����
    {
        daycnt = temp;
        temp1 = 1970;	//��1970�꿪ʼ
        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))//������
            {
                if(temp >= 366)temp -= 366; //�����������
                else break;
            }
            else temp -= 365;	 //ƽ��
            temp1++;
        }
        timer.year = temp1; //�õ����
        temp1 = 0;
        while(temp >= 28) //������һ����
        {
            if(Is_Leap_Year(timer.year) && temp1 == 1) //�����ǲ�������/2�·�
            {
                if(temp >= 29)temp -= 29; //�����������
                else break;
            }
            else
            {
                if(temp >= mon_table[temp1])temp -= mon_table[temp1]; //ƽ��
                else break;
            }
            temp1++;
        }
        timer.month = temp1 + 1; //�õ��·�
        timer.day = temp + 1; //�õ�����
    }
    temp = timecount % 86400; //�õ�������
    timer.hour = temp / 3600; //Сʱ
    timer.min = (temp % 3600) / 60; //����
    timer.sec = (temp % 3600) % 60; //����
    //	timer.week=RTC_Get_Week(timer.w_year,timer.w_month,timer.w_date);//��ȡ����

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
    timecount = RTC->CNTH; //�õ��������е�ֵ(������)
    timecount <<= 16;
    timecount += RTC->CNTL;
    //BKP_WriteBackupRegister(BKP_DR2, 0x5850);
    timecount_buf = ((BKP_ReadBackupRegister(BKP_DR2) << 16) + BKP_ReadBackupRegister(BKP_DR3));
    poweroff_time = timecount - timecount_buf;
    poweroff_time = poweroff_time / 60 + 1;
}
/***********************************************/
/* ��������: time_type Time_change(uint32 timer_cont)                   */
/* ��������: ��ȡʱ�亯��                        */
/* ��  ����: No                                */
/* ��  ����: time_type                                */
/* ȫ�ֱ���: ��                                */
/* ����ģ��: ��                                */
/* �޸�ʱ�䣺15��7��1 ��                      */
/***********************************************/
time_type Time_change(uint32 timer_cont)
{
    time_type Time_temp;
    static u16 daycnt = 0;
    u32 timecount = 0;
    u32 temp = 0;
    u16 temp1 = 0;

    //	timecount=RTC->CNTH;//�õ��������е�ֵ(������)
    //	timecount<<=16;
    //	timecount+=RTC->CNTL;
    //	TOTAL_Time=timecount;
    temp = timer_cont / 86400; //�õ�����(��������Ӧ��)
    if(daycnt != temp) //����һ����
    {
        daycnt = temp;
        temp1 = 1970;	//��1970�꿪ʼ
        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))//������
            {
                if(temp >= 366)temp -= 366; //�����������
                else break;
            }
            else temp -= 365;	 //ƽ��
            temp1++;
        }
        Time_temp.year = temp1; //�õ����
        temp1 = 0;
        while(temp >= 28) //������һ����
        {
            if(Is_Leap_Year(Time_temp.year) && temp1 == 1) //�����ǲ�������/2�·�
            {
                if(temp >= 29)temp -= 29; //�����������
                else break;
            }
            else
            {
                if(temp >= mon_table[temp1])temp -= mon_table[temp1]; //ƽ��
                else break;
            }
            temp1++;
        }
        Time_temp.month = temp1 + 1; //�õ��·�
        Time_temp.day = temp + 1; //�õ�����
    }
    temp = timecount % 86400; //�õ�������
    Time_temp.hour = temp / 3600; //Сʱ
    Time_temp.min = (temp % 3600) / 60; //����
    Time_temp.sec = (temp % 3600) % 60; //����
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
/* ��������: LED_flash                   */
/* ��������: ָʾ����˸                         */
/* ��  ����: No                                */
/* ��  ����: No                                */
/* ȫ�ֱ���: ��                                */
/* ����ģ��: ��                                */
/* �޸�ʱ�䣺14��11��18��                      */
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
/* ��������: Pump_cunt_time                   */
/* ��������: �䶯�ÿ�ʼ��ʱ                    */
/* ��  ����: No                                */
/* ��  ����: No                                */
/* ȫ�ֱ���: ��                                */
/* ����ģ��: ��                                */
/* �޸�ʱ�䣺14��11��18��                      */
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