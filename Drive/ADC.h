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
extern void AD_Loop(void);       //�ú���ÿ5msִ��һ�Σ����м�����������һ����Ҫ50ms
extern void Sensor_Update(void) ;
/***********************************************/
/* ��������: Alarm_Update                     */
/* ��������: ���������ݸ���                    */
/* ��  ����: ��                                */
/* ��  ����: ��ͨ�����                        */
/* ȫ�ֱ���: ��                                */
/* ����ģ��: ADResult                          */
/* �޸�ʱ�䣺14��09��15��                      */
/***********************************************/
extern void 	Alarm_Sound(void)   ;    //�ú���ÿ10msִ��һ�Σ����м�����������һ����Ҫ50ms
/***********************************************/
/* ��������: Cleaner_check                     */
/* ��������: ��ϴ���ȼ�⺯��                */
/* ��  ����: ��                                */
/* ��  ����:                                   */
/* ȫ�ֱ���: ��                                */
/* ����ģ��:                                   */
/* �޸�ʱ�䣺14��12��26��                      */
/**********************************************/
extern void Cleaner_check(void);
