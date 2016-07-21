//#include "../User/user_conf.h"
#include "stm32f10x.h"
typedef struct
{
    uint16_t Fill_Pump_count;
    uint16_t Wash_Pump_count;
    uint16_t Empty_Pump_count;
} Monitor_Pump_type;


extern Monitor_Pump_type Monitor_Pump;

extern void Adc_Init(void);
extern uint16_t ADResult(uint8_t ch);
extern void AD_Loop(void);       //该函数每5ms执行一次，所有监测参数都更新一次需要50ms
extern void Sensor_Update(void) ;
/***********************************************/
/* 函数名称: Alarm_Update                     */
/* 功能描述: 传感器数据更新                    */
/* 输  　入: 无                                */
/* 输  　出: 该通道结果                        */
/* 全局变量: 无                                */
/* 调用模块: ADResult                          */
/* 修改时间：14年09月15日                      */
/***********************************************/
extern void 	Alarm_Sound(void)   ;    //该函数每10ms执行一次，所有监测参数都更新一次需要50ms
/***********************************************/
/* 函数名称: Cleaner_check                     */
/* 功能描述: 清洗清洁度检测函数                */
/* 输  　入: 无                                */
/* 输  　出:                                   */
/* 全局变量: 无                                */
/* 调用模块:                                   */
/* 修改时间：14年12月26日                      */
/**********************************************/
extern void Cleaner_check(void);
