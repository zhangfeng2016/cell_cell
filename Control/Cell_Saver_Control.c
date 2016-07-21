#include "stm32f10x.h"
#include "../User/user_conf.h"
#include "Cell_Saver_Control.h"
#include "..\Drive\Uart.h"
#include "..\GUI\User_dispaly.h"
#include "..\Drive\ADC.h"



uint16 speed_test;
uint16 start_time;
uint16 start_step;
uint16 stop_time;
uint8  pump_start_delay;
uint8 wash_pump_adj_time = 0;
//uint16 test_time;
//uint8 Test_Step=1;
#define STOP_TIME_MAX     550
#define START_TIME_MAX    150


uint16 crc;
uint8 Tx_485_Buf[8] = {0x01};
uint16 RS_485_error_num = 0;
uint8  RS_485_error_time = 0;
uint8  RS_485_error_time_max = 0;


#define ADR              0x01
#define CMD_READ         0x03
#define CMD_WRITE_ONE    0x06
#define D_ADR_ORDE_H     0X20



#define START 0
#define STOP  1
///*******************************************************************************
//* Function Name  : Standby
//* Description    : ??
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		   : ????,???????????
//*******************************************************************************/
uint16 CRC_CHK(uint8 *tx_data, uint8 length)
{
    uint8 temp_j;
    uint16 reg_crc = 0xffff;
    while(length--)
    {
        reg_crc ^= *tx_data++;
        for(temp_j = 0; temp_j < 8; temp_j++)
        {
            if(reg_crc & 0x01)
            {
                reg_crc = (reg_crc >> 1) ^ 0xa001;
            }
            else
            {
                reg_crc = reg_crc >> 1;
            }
        }
    }
    return reg_crc;
}

///*******************************************************************************
//* Function Name  : Motor_Start
//* Description    : ??????????
//* Input          : uint8
//* Output         : None
//* Return         : None
//* Attention		 : ??????
//*******************************************************************************/
void Motor_Start(void)
{
    uint8 temp_i;
    if(Bit_check_0(RS_485_flag, 1)) // ??????
    {
        Tx_485_Buf[1] = CMD_WRITE_ONE;
        Tx_485_Buf[2] = 0x20;
        Tx_485_Buf[3] = 0x00;
        Tx_485_Buf[4] = 0x00;
        Tx_485_Buf[5] = 0x12;
        crc = CRC_CHK(Tx_485_Buf, 6);
        Tx_485_Buf[6] = crc & 0xff;
        Tx_485_Buf[7] = (crc >> 8) & 0xff;
        RS485_TX_EN();
        for(temp_i = 0; temp_i < 8; temp_i++)
        {
            USART1_TX_Byte(Tx_485_Buf[temp_i]);
        }
        RS485_RX_EN(); //?????????
        uart1_rx_num = 0;
        Bit_set(RS_485_flag, 1);
    }
    else                        //???????????,
    {
        if(Bit_check_1(RS_485_flag, 5))      // ??????
        {
            if(Tx_485_Buf[7] == UART1_RX[7])
            {
                Bit_set(RS_485_flag, 0);          //?????,???????
                Bit_clr(RS_485_flag, 1);
                RS_485_error_num = 0;
                Bit_clr(RS_485_flag, 5);         //????????
                Centrifugal_motor_orde = 3;
                Bit_clr(Alarm_sta, LIXIN_ALARM);
            }
            else
            {
                RS_485_error_num++;
                if(RS_485_error_num == 50)
                {
                    Bit_clr(RS_485_flag, 1);       //??????
                    // Bit_clr(RS_485_flag,6);        //????????
                }
            }
        }
        else                                //????????????
        {
            RS_485_error_num++;
            if(RS_485_error_num % 50 == 0)
            {
                Bit_clr(RS_485_flag, 1);                //????????
                Bit_set(RS_485_flag, RS_485_ERR);       //????????
                Bit_set(Alarm_sta, LIXIN_ALARM);
            }

        }
    }


}
/***********************************************/
/* ????:  Motor_Check                      */
/* ????: ????????                  */
/* ?   ?: ?                                */
/* ?   ?:                                   */
/* ????: ?                                */
/* ????: ADResult                          */
/* ????:14?11?27?                      */
/***********************************************/
void Motor_Check(void)
{
    if(Bit_check_1(Time_flag, TIME_50MS_FLAG))
    {
        if(Centrifugal_motor_orde == 5)
        {
            Bit_clr(RS_485_flag, 1);
            if(Centrifugal_motor_stop_flag == 1) //未停止时，持续发停止命令，停止后发查询命令
            {
                if(Bit_check_1(Alarm_sta, WELL_ALARM) || Bit_check_1(Alarm_sta, louye_1_ALARM) || Bit_check_1(Alarm_sta, louye_2_ALARM) || (Work_Stop_Flag == 1) || (Centrifugal_motor_stop_orde == 1))
                {
                    Centrifugal_motor_orde = 2; //漏液报警后停止
                    Pump_Pause = 1;
                }
                else
                {
                    Bit_clr(Alarm_sta, WELL_ALARM);
                    Centrifugal_motor_orde = 4;
                }
            }
            else
            {
                //	Bit_clr(Alarm_sta,WELL_ALARM);
                RS_485_error_time = 0;
                Centrifugal_motor_orde = 4;
            }
        }
        Bit_clr(Time_flag, TIME_50MS_FLAG);
    }

    if(Centrifugal_motor_speed == 0)
    {
        Centrifugal_motor_stop_flag = 0; //已停止
    }
    else
    {
        Centrifugal_motor_stop_flag = 1;
    }
		
}

///*******************************************************************************
//* Function Name  : Motor_Start
//* Description    : ????????
//* Input          : uint8 orde
//* Output         : None
//* Return         : None
//* Attention		 : ??????
//*******************************************************************************/
void Motor_Stop(void)
{
    uint8 temp_i;
    if(Bit_check_1(RS_485_flag, 0)) //can send
    {
        if(Bit_check_0(RS_485_flag, 1)) // ??????
        {
            Tx_485_Buf[1] = CMD_WRITE_ONE;
            Tx_485_Buf[2] = 0x20;
            Tx_485_Buf[3] = 0x00;
            Tx_485_Buf[4] = 0x00;
            Tx_485_Buf[5] = 0x01;
            crc = CRC_CHK(Tx_485_Buf, 6);
            Tx_485_Buf[6] = crc & 0xff;
            Tx_485_Buf[7] = (crc >> 8) & 0xff;
            RS485_TX_EN();
            for(temp_i = 0; temp_i < 8; temp_i++)
            {
                USART1_TX_Byte(Tx_485_Buf[temp_i]);
            }
            uart1_rx_num = 0;
            RS485_RX_EN(); //?????????
            Bit_set(RS_485_flag, 1);
        }
        else
        {
            if(Bit_check_1(RS_485_flag, 5))      // 发送完成，检测接收
            {
                if(Tx_485_Buf[7] == UART1_RX[7])
                {
                    Bit_set(RS_485_flag, 0);          //?????,???????
                    Bit_clr(RS_485_flag, 1);
                    RS_485_error_num = 0;
                    Bit_clr(RS_485_flag, 5);         //????????
                    //Centrifugal_motor_stop_flag=1;
                    Centrifugal_motor_orde = 4;
                }
                else
                {
                    RS_485_error_num++;
                    if(RS_485_error_num == 50)
                    {
                        Bit_clr(RS_485_flag, 1);                              //??????
                    }
                }
            }
            else                                //????????????
            {
                RS_485_error_num++;
                if(RS_485_error_num == 50)
                {
                    Bit_clr(RS_485_flag, 1);
                }

            }
        }
    }
}
///*******************************************************************************
//* Function Name  : Motor_Start
//* Description    : ????????
//* Input          : uint16 motor_speed  ????
//* Output         : None
//* Return         : None
//* Attention		 : ??????
//*******************************************************************************/
void Motor_Set_Fre(uint16 motor_speed)
{
    uint16 fre;
    uint8 temp_i;
    if(Bit_check_1(RS_485_flag, 0)) //??????????,??????
    {
        if(Bit_check_0(RS_485_flag, 1)) // ??????
        {
            fre = motor_speed * 100.0 / 60.0;
            Tx_485_Buf[1] = CMD_WRITE_ONE;
            Tx_485_Buf[2] = 0x20;
            Tx_485_Buf[3] = 0x01;
            Tx_485_Buf[4] = (fre >> 8) & 0XFF;
            Tx_485_Buf[5] = fre & 0XFF;
            crc = CRC_CHK(Tx_485_Buf, 6);
            Tx_485_Buf[6] = crc & 0xff;
            Tx_485_Buf[7] = (crc >> 8) & 0xff;
            RS485_TX_EN();
            for(temp_i = 0; temp_i < 8; temp_i++)
            {
                USART1_TX_Byte(Tx_485_Buf[temp_i]);
            }
            uart1_rx_num = 0;
            RS485_RX_EN(); //?????????
            Bit_set(RS_485_flag, 1);
        }
        else                                   //?????????
        {
            if(Bit_check_1(RS_485_flag, 5))      // ??????
            {
                if(Tx_485_Buf[7] == UART1_RX[7])
                {
                    Bit_set(RS_485_flag, 0);          //?????,???????
                    Bit_clr(RS_485_flag, 1);
                    RS_485_error_num = 0;
                    Bit_clr(RS_485_flag, 5);         //????????
                    Centrifugal_motor_orde = 5;
                }
                else
                {
                    RS_485_error_num++;
                    if(RS_485_error_num == 50)
                    {
                        Bit_clr(RS_485_flag, 1);                              //??????
                        Centrifugal_motor_orde = 5;
                    }
                }
            }
            else                                //????????????
            {
                RS_485_error_num++;
                if(RS_485_error_num == 50)
                {
                    Bit_clr(RS_485_flag, 1);
                    Centrifugal_motor_orde = 3;
                }

            }

        }
    }
}

///*******************************************************************************
//* Function Name  : Motor_Start
//* Description    : ????????
//* Input          : uint16 motor_speed  ????
//* Output         : None
//* Return         : None
//* Attention		 : ??????
//*******************************************************************************/
void Motor_Read_State(void)
{
    //	uint16 fre;
    uint8 temp_i;

    if(Bit_check_0(RS_485_flag, 1)) // ??????
    {
        Tx_485_Buf[1] = CMD_READ;
        Tx_485_Buf[2] = 0x21;
        Tx_485_Buf[3] = 0x03;
        Tx_485_Buf[4] = 0x00;
        Tx_485_Buf[5] = 0x01;
        crc = CRC_CHK(Tx_485_Buf, 6);
        Tx_485_Buf[6] = crc & 0xff;
        Tx_485_Buf[7] = (crc >> 8) & 0xff;
        RS485_TX_EN();
        for(temp_i = 0; temp_i < 8; temp_i++)
        {
            USART1_TX_Byte(Tx_485_Buf[temp_i]);
        }
        RS485_RX_EN(); //?????????
        uart1_rx_num = 0;
        Bit_set(RS_485_flag, 1);
    }
    else                                   //?????????
    {
        if(Bit_check_1(RS_485_flag, 5))      // ??????
        {
            if(UART1_RX [ 1 ] == 0x03)
            {
                Bit_clr(RS_485_flag, 1);          //?????,???????
                RS_485_error_num = 0;
                Centrifugal_motor_speed = (UART1_RX[3] * 256) + UART1_RX[4];

                Bit_clr(RS_485_flag, 5);         //????????
                Centrifugal_motor_orde = 5;
                Bit_clr(Alarm_sta, LIXIN_ALARM);
            }
            else
            {
                RS_485_error_num++;
                if(RS_485_error_num == 50)
                {
                    Bit_clr(RS_485_flag, 1);                              //??????
                    //Bit_set(Alarm_sta,LIXIN_ALARM);
                }
            }
        }
        else                                //????????????
        {
            RS_485_error_num++;
            if(( RS_485_error_num % 50 ) == 0)
            {
                Bit_clr(RS_485_flag, 1);
                //Bit_set(Alarm_sta,LIXIN_ALARM);
                RS_485_error_time ++ ;
                if(RS_485_error_time > RS_485_error_time_max)
                    RS_485_error_time_max = RS_485_error_time;
                Centrifugal_motor_orde = 4;     //重新发送
                uart1_rx_num = 0;

            }
            if(RS_485_error_num > 1000)
            {
                ;//Bit_set(Alarm_sta,LIXIN_ALARM);
            }

        }

    }
}
///*******************************************************************************
//* Function Name  : Centrifugal_motor
//* Description    : ????????
//* Input          : uint8 orde
//* Output         : None
//* Return         : None
//* Attention		   :
//*******************************************************************************/
void Centrifugal_motor(void)
{
    Bit_set(RS_485_flag, 0);

    switch(Centrifugal_motor_orde)
    {
    case 1:
        Motor_Start();
        // Motor_Set_Fre(5650);
        break;
    case 2:
        Motor_Stop();
        break;
    case 3:
        Motor_Set_Fre(Centrifugal_motor_speed_Set + Centrifugal_motor_speed_Adj - 100);
        break;
    case 4:
        Motor_Read_State();
        break;
    case 5://????
        //Motor_Read_State();
        break;
    default:
        break;

    }


}

///*******************************************************************************
//* Function Name  : vuint16 Pump_puls_change(uint16 set_speed)
//* Description    : Step moto contorl
//* Input          : set_fre       ??khz
//* Output         : None
//* Return         : None
//* Attention		   : evry 10ms do it
//*******************************************************************************/
uint16 Pump_puls_change(uint16 set_fre)
{
    uint16 Timer_ARR;
    uint16 Temp;
    Temp = 10000 / (set_fre * 2); //?????*10??
    Timer_ARR = Temp * 18; //??
    Timer_ARR = Data_Round(Timer_ARR);
    return Timer_ARR;
}

///*******************************************************************************
//* Function Name  : vuint16 Pump_puls_change(uint16 set_speed)
//* Description    : Step moto contorl
//* Input          : set_fre       ??khz
//* Output         : None
//* Return         : None
//* Attention		   : evry 10ms do it
//*******************************************************************************/
uint16 Pump_speed_rpm(uint16 set_fre)
{
    uint16 speed_rpm;
    //uint16 Temp;
    speed_rpm = set_fre * 6 / 1280; //???????
    return speed_rpm;
}
///*******************************************************************************
//* Function Name  : vuint16 Pump_puls_change(uint16 set_speed)
//* Description    : Step moto contorl
//* Input          : set_fre       ??khz
//* Output         : Pump_fre
//* Return         : None
//* Attention		   : evry 10ms do it
//*******************************************************************************/
uint16 Pump_Fre_SET(uint16 Mlpr)
{
    uint32 Pump_fre;
    uint32 Pump_rpm;
    Pump_rpm = Mlpr * 1000 / 3125; //?????1????3.125ml
    Pump_fre = Pump_rpm * 12800 / 60; //?????=??*60/12800
    Pump_fre = Pump_fre / 1000;    //?????KHZ
    return (uint16)Pump_fre;
}
///*******************************************************************************
//* Function Name  : vuint16 Pump_speed_change(uint16 set_speed)
//* Description    : Step moto contorl
//* Input          : set_speed
//* Output         : None
//* Return         : None
//* Attention		   : evry 10ms do it
//*******************************************************************************/

uint16 Pump_speed_change_TO_ARR(uint16 set_speed_mlpm)
{
    uint16 Pump_fre;
    uint16 Timer_ARR ;
    Pump_fre = Pump_Fre_SET(set_speed_mlpm);
    //Pump_puls_change(Pump_fre);

    Timer_ARR = Pump_puls_change(Pump_fre);
}
///*******************************************************************************
//* Function Name  : void Pump_smooth_start(uint8 pump,uint16 speed)
//* Description    : Step moto contorl
//* Input          :
//* Output         : None
//* Return         : None
//* Attention		   : evry 10ms do it
//*******************************************************************************/
void Pump_set_ARR(uint16 set_speed_mlpm)
{
    uint16 Pump_fre;
    Pump_fre = Pump_Fre_SET(set_speed_mlpm);
    TIM2->ARR = Pump_puls_change(Pump_fre);
}

///*******************************************************************************
//* Function Name  : void Pump_smooth_start(uint8 pump,uint16 speed)
//* Description    : Step moto contorl
//* Input          :
//* Output         : None
//* Return         : None
//* Attention		   : evry 10ms do it
//*******************************************************************************/
uint16  Pump_smooth_start(uint8 pump, uint16 speed)
{
    uint16 speed_AD;
    uint16 Start_time_Set;
    uint8 Start_temp_num;
    uint16 speed_now;

    Start_temp_num = speed / 10 + 1;
    if(start_time < 15)
    {
        start_time++;
    }
    else
    {
        start_time = 0;
        if(start_step < Start_temp_num)
        {
            if(start_step < 50)
            {
                TIM2->ARR = Pump_speed_change_TO_ARR(start_step * 10);
                start_step += 5;
            }
            else
            {
                TIM2->ARR = Pump_speed_change_TO_ARR(start_step * 10);
                start_step += 1;
            }
        }
        else
        {
            // Pump_Sta=PUMP_WORK;
            Bit_set(pump_flag, PUMP_START_OK);
        }
    }
    speed_now = start_step * 10;
    return speed_now;
}

//*******************************************************************************
//* Function Name  : void Pump_smooth_start(uint8 pump,uint16 speed)
//* Description    : Step moto contorl
//* Input          :
//* Output         : None
//* Return         : None
//* Attention		   : evry 10ms do it
//*******************************************************************************/
void Pump_smooth_Stop(uint8 pump, uint16 speed)
{
    uint16 speed_AD;

#define STOP_SPEED_RATE   900

    speed_AD	= Pump_speed_change_TO_ARR(speed);
    if(stop_time < STOP_TIME_MAX)
    {
        TIM2->ARR = speed_AD + (STOP_SPEED_RATE - speed_AD) * stop_time / STOP_TIME_MAX;
        stop_time++;
    }
    else
        ;


}
//*******************************************************************************
//* Function Name  : void Wash_Pump_Adjust( void )
//* Description    : Step moto contorl
//* Input          :
//* Output         : None
//* Return         : None
//* Attention		   : evry 10ms do it
//*******************************************************************************/
void Wash_Pump_Adjust_contorl(void )
{
    if(Wash_cell_Protect == PROTECT_ABLE) //???????
    {
        if(Monitor_Red_AD < 200)
        {
            Set_Wash_speed_adj = 200;
            Pump_set_ARR(Set_Wash_speed_adj);
        }
        if(Monitor_Red_AD > 600)
        {
            if(wash_pump_adj_time > 80)
            {

                wash_pump_adj_time = 0;
                Set_Wash_speed_adj += 50;
                Set_Wash_speed_adj = ((Set_Wash_speed_adj > Set_Wash_speed) ? Set_Wash_speed : Set_Wash_speed_adj);
                Pump_set_ARR(Set_Wash_speed_adj);
            }
            else
            {
                wash_pump_adj_time++;
            }
        }
    }
    else
    {
        if(wash_pump_adj_time > 15)
        {

            wash_pump_adj_time = 0;
            Set_Wash_speed_adj += 50;
            Set_Wash_speed_adj = ((Set_Wash_speed_adj > Set_Wash_speed) ? Set_Wash_speed : Set_Wash_speed_adj);
            Pump_set_ARR(Set_Wash_speed_adj);
        }
        else
        {
            wash_pump_adj_time++;
        }
    }
}
//*******************************************************************************
//* Function Name  : void Wash_Volume_Adjust( void )
//* Description    : Step moto contorl
//* Input          :
//* Output         : None
//* Return         : None
//* Attention		   : evry 10ms do it
//*******************************************************************************/
void Wash_Volume_Adjust(void )
{

    if((Monitor_Bowl_Wash_Volume > (Set_Wash_Volume + Set_Wash_ext_Volume - 25)	)) //开始判断清洗质量
    {

        if(Monitor_Red_AD < 400)
        {
            if(Set_Wash_ext_Volume < 300)
                Set_Wash_ext_Volume += 50;
        }
    }
}
//*******************************************************************************
//* Function Name  : void Wash_Pump_Adjust( void )
//* Description    : Step moto contorl
//* Input          :
//* Output         : None
//* Return         : None
//* Attention		   : evry 10ms do it
//*******************************************************************************/
void Yellow_pump_puls_set(void )
{
    FILL_PUMP_ABLE();
    FILL_PUMP_DIR_FF();
    Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
    GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_3;

    TIM2->ARR = Pump_speed_change_TO_ARR(100);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

}

///*******************************************************************************
//* Function Name  : Standby
//* Description    : ??
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		   : ????,???????????
//*******************************************************************************/
void Standby(void)
{
    FILL_DISABLE();
    WASH_DISABLE();
    EMPTY_DISABLE();
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
    Pump_Sta = PUMP_START;
    Bit_set(work_percheck, FILL_ALLOW_CHECK);
    work_sta = WORK_STANDBY;
    stop_time = 0;


}


///*******************************************************************************
//* Function Name  : Standby
//* Description    : ??
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		   : ????,???????????
//*******************************************************************************/
void Stop(void)
{
    FILL_DISABLE();
    WASH_DISABLE();
    EMPTY_DISABLE();
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
    //Pump_Sta=PUMP_START;
    //Bit_set(work_percheck,FILL_ALLOW_CHECK);



}
///*******************************************************************************
//* Function Name  : Fill
//* Description    : ??????
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		   : ??????
//*******************************************************************************/
void Empty(void)
{
    if(Bit_check_1(work_percheck, FILL_ALLOW_CHECK))    //??????
    {
        WASH_DISABLE();
        FILL_DISABLE();
        EMPTY_DISABLE();
        start_step = 3;
        Pump_Sta = PUMP_START;
        Bit_set(Save_to_EEP_flag, SAVE_EMPTY_TIME);
        ////////////清除气泡报警标记///////////////////////////
        Bit_clr(Alarm_sta, WASH_AIR_ALARM);
        Bit_clr(Alarm_sta, FILL_AIR_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
        Bit_clr(Alarm_sta, PRE_FILL_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
        Bit_clr(Alarm_sta, AIR_ALARM);

        //////////////////////////////////////////
        TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
        if(Centrifugal_motor_speed < 500)
        {
            Bit_clr(work_percheck, FILL_ALLOW_CHECK);        //????
            start_time = 0;
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOE;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_0;
            TIM2->ARR = Pump_speed_change_TO_ARR(50);
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            Pump_Sta = PUMP_START;
        }
        if(Mode == MODE_AUTO)
        {
            Auto_run_step = AUTO_EMPTY; //
        }
    }
    else      //EMPTY
    {
        switch(Pump_Sta)
        {
        case PUMP_START:       //????
            EMPTY_PUMP_ABLE();
            EMPTY_PUMP_DIR_RR();

            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOE;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_0;
            if(Pump_smooth_start(1, Set_Empty_speed) == Set_Empty_speed)
            {
                Pump_Sta = PUMP_WORK;  //
                start_step = 1;
            }

            if(Monitor_Bowl_Empty_Volume > Set_Bowl + 50)
            {
                Pump_Sta = PUMP_STOP;
            }
            if(Bit_check_1(Pump_state_flag, EMPTY_OK)) //二次清空
            {
                if((Monitor_Pump.Empty_Pump_count * 375 / 1000) > 50)
                {
                    Pump_Sta = PUMP_STOP;
                }
            }
            else
            {
                if((Monitor_Pump.Empty_Pump_count * 375 / 1000) > (Set_Bowl + 60))
                {
                    Pump_Sta = PUMP_STOP;
                }
            }
            if(Pump_Pause == 1)
            {
                EMPTY_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            break;
        case PUMP_WORK:
            if(speed_change_flag == 1)                                   //???????????
            {
                Pump_set_ARR(Set_Empty_speed);
                speed_change_flag = 0;
            }
            if(Pump_Pause == 1)
            {
                EMPTY_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            //????
            if(Monitor_Bowl_Empty_Volume > Set_Bowl + 50)
            {
                Pump_Sta = PUMP_STOP;
            }

            if(Bit_check_1(Pump_state_flag, EMPTY_OK)) //二次清空
            {
                if((Monitor_Pump.Empty_Pump_count * 375 / 1000) > 50)
                {
                    Pump_Sta = PUMP_STOP;
                }
            }
            else
            {
                if((Monitor_Pump.Empty_Pump_count * 375 / 1000) > (Set_Bowl + 60))
                {
                    Pump_Sta = PUMP_STOP;
                }
            }
            break;
        case PUMP_STOP://??????                                    //stop EMPTY
        {
            EMPTY_DISABLE();
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            Standby();
            if(Bit_check_0(Pump_state_flag, EMPTY_OK))
                Wash_Bowl_Num += 1;
            Bit_clr(Bowl_State, FILL_BLOOD);          //离心杯内有血		biaoahzi
            Bit_clr(Pump_state_flag, FILL_OK);
            Bit_clr(Pump_state_flag, WASH_OK);
            Bit_clr(Pump_state_flag, CONC_OK);
            Bit_set(Pump_state_flag, EMPTY_OK);
            Auto_run_step = 0;
            Auto_run_step_cont = 0;
            if(Set_Bowl == 225)
                Set_Run_Volume = 800;
            else if (Set_Bowl == 125)
                Set_Run_Volume = 600;
            Monitor_Bowl_Fill_Volume = 0;
            Monitor_Bowl_Wash_Volume = 0;
            Monitor_Bowl_Empty_Volume = 0;
            Monitor_Pump.Wash_Pump_count = 0;
            Monitor_Pump.Empty_Pump_count = 0;
            //if(Bit_check_1(Bowl_State,FILL_BLOOD))


        }
        break;
        case PUMP_AFTER://????

            break;
        case PUMP_PAUSE: //
            if(Pump_Pause == 0)
            {
                Pump_Sta = PUMP_START;
                TIM2->ARR = Pump_speed_change_TO_ARR(50);
                TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

                start_step = 1;
            }
            break;
        default:
            break;
        }
    }
}

///*******************************************************************************
//* Function Name  : Wash
//* Description    : ??????
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		   : ??????
//*******************************************************************************/
void Wash(void)
{
    if(Bit_check_1(work_percheck, FILL_ALLOW_CHECK))    //??????????
    {
        EMPTY_DISABLE();
        FILL_DISABLE();
        TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
        start_step = 3;
        //Pump_Sta=PUMP_START;
        //Pump_set_ARR(150);
        //Set_Wash_speed_adj=150;
        Bit_set(Save_to_EEP_flag, SAVE_WASH_TIME);
        ////////////清除气泡报警标记///////////////////////////
        Bit_clr(Alarm_sta, WASH_AIR_ALARM);
        Bit_clr(Alarm_sta, FILL_AIR_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
        Bit_clr(Alarm_sta, PRE_FILL_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
        Bit_clr(Alarm_sta, AIR_ALARM);

        //////////////////////////////////////////
        if((Centrifugal_motor_speed > 8000) || ((Mode == MODE_YINGJI)))
        {
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOB;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_8;
            Pump_set_ARR(50);
            Pump_Sta = PUMP_START;
            Bit_clr(work_percheck, FILL_ALLOW_CHECK);        //???? Set_Empty_speed
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        }
        if(Mode == MODE_AUTO)
        {
            Auto_run_step = AUTO_WASH; //
        }
    }
    else                  //wash
    {
        switch(Pump_Sta)
        {
        case PUMP_START:       //
            WASH_PUMP_ABLE();
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_7;

            Pump_smooth_start(1, 400);   //
            if(Monitor_Pump.Wash_Pump_count * 375 / 1000 > 100)
            {

                Pump_Sta = PUMP_WORK;
                Set_Wash_speed_adj = 200;
                start_step = 20;
            }
            if(Pump_Pause == 1)
            {
                EMPTY_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            break;
        case PUMP_WORK:
            if(Monitor_Bowl_Wash_Volume < 170)
            {
                FILL_PUMP_ABLE();
                FILL_PUMP_DIR_RR();
                Pump_smooth_start(1, 300);
                Pump_GPIOx_1 = GPIOD;
                GPIOx_1_Pin = GPIO_Pin_3;
                Set_Wash_speed_adj = 300;
                //Pump_set_ARR(200);
            }
            else
            {
                FILL_DISABLE();
                WASH_PUMP_ABLE();
                Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD; //=Pump_GPIOx_1
                GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_7; //=GPIOx_1_Pin
                Wash_Pump_Adjust_contorl();

            }
            Wash_Volume_Adjust();
            if(Pump_Pause == 1)
            {
                WASH_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            if(Monitor_Bowl_Wash_Volume > (Set_Wash_Volume + Set_Wash_ext_Volume)	)
            {
                Pump_Sta = PUMP_AFTER;
                stop_time = 0;
                Monitor_Pump.Fill_Pump_count = 0;
            }
            break;
        case PUMP_STOP://??????
        {
            FILL_DISABLE();
            WASH_DISABLE();
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);

            if(Mode == MODE_MU || Mode == MODE_YINGJI)
            {
                Centrifugal_motor_orde = 2; //清洗后离心电机停止
                Work_Stop_Flag = 1;
                if(Bit_check_1(Bowl_State, FILL_BLOOD))  //设置蠕动泵延时标志
                {
                    Bit_set(Bowl_State, 1);               //再次启动时需要延时蠕动泵
                    Bit_clr(Bowl_State, 2);               //清除正在延时标志位
                    pump_dely_time = 50;
                }
            }
            Bit_set(Pump_state_flag, WASH_OK);
        }
        break;
        case PUMP_AFTER://????
            WASH_DISABLE();
            //WASH_PUMP_ABLE();
            FILL_PUMP_ABLE();
            FILL_PUMP_DIR_RR();
            Pump_GPIOx_1 = GPIOD;
            GPIOx_1_Pin = GPIO_Pin_3;
            TIM2->ARR = Pump_speed_change_TO_ARR(200);
            if((Monitor_Pump.Fill_Pump_count * 375 / 1000) > (23))
            {
                Pump_Sta = PUMP_STOP;
                Monitor_Pump.Fill_Pump_count = 0;
                EMPTY_DISABLE();
            }
            if(Pump_Pause == 1)
            {
                WASH_DISABLE();
                FILL_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            break;
        case PUMP_PAUSE: //
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            if(Pump_Pause == 0)
            {
                Pump_Sta = PUMP_START;
                start_step = 5;
                TIM2->ARR = Pump_speed_change_TO_ARR(50);
                TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            }
            break;
        default:
            break;
        }

    }
}
///*******************************************************************************
//* Function Name  : Pre_Fill
//* Description    : ???????
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		   : ??????
//*******************************************************************************/
void Pre_Fill(void)
{
    if(Bit_check_1(work_percheck, FILL_ALLOW_CHECK))    //??????????
    {
        EMPTY_DISABLE();
        FILL_DISABLE();
        //TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
        start_step = 3;
        Pump_Sta = PUMP_START;
        ////////////清除气泡报警标记///////////////////////////
        Bit_clr(Alarm_sta, WASH_AIR_ALARM);
        Bit_clr(Alarm_sta, FILL_AIR_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
        Bit_clr(Alarm_sta, PRE_FILL_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
        Bit_clr(Alarm_sta, AIR_ALARM);

        //////////////////////////////////////////
        if((Centrifugal_motor_speed > 8000)) //||((Mode==MODE_YINGJI)))
        {
            Bit_clr(work_percheck, FILL_ALLOW_CHECK);        //????
            Monitor_Pre_Fill_time = 0;		      //??????
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOB;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_8;
            TIM2->ARR = Pump_speed_change_TO_ARR(50);
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        }

    }
    else                  //wash
    {
        switch(Pump_Sta)
        {
        case PUMP_START:       //????
            WASH_PUMP_ABLE();

            Pump_choose = WORK_WASH;
            speed_test = Pump_smooth_start(1, Set_Wash_speed);
            if(Pump_smooth_start(1, Set_Wash_speed) >= Set_Wash_speed)
            {
                Pump_Sta = PUMP_WORK;    //??????
                start_step = 1;
            }
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_7;
            //
            if(Pump_Pause == 1)
            {
                EMPTY_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            if(Monitor_Pre_fill_Volume > (Pre_fill_Volume)	)
            {
                Pump_Sta = PUMP_STOP;
            }
            break;
        case PUMP_WORK:
            if(speed_change_flag == 1)                                   //???????????
            {
                Pump_set_ARR(Set_Wash_speed);
                speed_change_flag = 0;
            }
            if(Pump_Pause == 1)
            {
                WASH_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            if(Monitor_Pre_fill_Volume > (Pre_fill_Volume)	)
            {
                Pump_Sta = PUMP_STOP;
            }
            break;
        case PUMP_STOP://??????
            //stop FILL
        {
            WASH_DISABLE();
            EMPTY_DISABLE();
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            Bit_set(Pump_state_flag, PRE_FILL_OK);
            prefill_flag = 0;
            sta = WORK;
            work_sta = WORK_FILL;
            Centrifugal_motor_orde = 1;
            Monitor_Pump.Fill_Pump_count = 0;
            Centrifugal_motor_speed_Set = 5650;
            Bit_set(work_percheck, FILL_ALLOW_CHECK);

        }
        break;
        case PUMP_AFTER://??????
            EMPTY_PUMP_ABLE();
            WASH_PUMP_ABLE();
            EMPTY_PUMP_DIR_RR();
            Pump_GPIOx_2 = GPIOE;
            GPIOx_2_Pin = GPIO_Pin_2;
            Pump_set_ARR(200);
            if(Monitor_Pre_fill_Volume > Set_Bowl + 50)
                Pump_Sta = PUMP_STOP;
            break;
        case PUMP_PAUSE: //
            if(Pump_Pause == 0)
            {
                Pump_Sta = PUMP_START;
                TIM2->ARR = Pump_speed_change_TO_ARR(50);
                TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            }
            break;
        default:
            break;
        }

    }
}
///*******************************************************************************
//* Function Name  : Fill
//* Description    : ??????
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		   : ??????
//*******************************************************************************/
void Fill(void)
{
    if(Bit_check_1(work_percheck, FILL_ALLOW_CHECK))    //??????????
    {
        WASH_DISABLE();
        EMPTY_DISABLE();
        FILL_DISABLE();
        TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
        Bit_set(Save_to_EEP_flag, 1);
        ////////////清除气泡报警标记///////////////////////////
        Bit_clr(Alarm_sta, WASH_AIR_ALARM);
        Bit_clr(Alarm_sta, FILL_AIR_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
        Bit_clr(Alarm_sta, PRE_FILL_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
        Bit_clr(Alarm_sta, AIR_ALARM);
        Monitor_Bowl_Wash_Volume = 0;
        Monitor_Pump.Wash_Pump_count = 0;

        //////////////////////////////////////////
        if((Centrifugal_motor_speed > 5000)) //||(Mode==MODE_YINGJI))
        {
            FILL_PUMP_ABLE();
            FILL_PUMP_DIR_FF();
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_3;
            TIM2->ARR = Pump_speed_change_TO_ARR(50);
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            Bit_clr(work_percheck, FILL_ALLOW_CHECK);
            start_step = 2;
            Pump_Sta = PUMP_START;
        }
        if(Mode == MODE_AUTO)
        {
            Auto_run_step = AUTO_FILL; //
        }
    }
    else      //FILLING
    {
        switch(Pump_Sta)
        {
        case PUMP_START:       //????
            FILL_PUMP_ABLE();
            FILL_PUMP_DIR_FF();
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_3;
            Bit_set(Bowl_State, FILL_BLOOD);
            if(Pump_smooth_start(1, Set_Fill_speed) >= Set_Fill_speed)
            {
                Pump_Sta = PUMP_WORK;  //??????
                start_step = 1;
            }
            if(Pump_Pause == 1)
            {
                FILL_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            break;
        case PUMP_WORK:
            //FILL_PUMP_ABLE();
            if(speed_change_flag == 1)                                   //???????????
            {
                Pump_set_ARR(Set_Fill_speed);
                speed_change_flag = 0;
            }
            if(Pump_Pause == 1)
            {
                FILL_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            if(Cell_line == 1)
            {
                Pump_Sta = PUMP_STOP;
                stop_time = 0;
            }
            else
            {

            }
            break;
        case PUMP_STOP://??????
        {
            FILL_DISABLE();
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); //puls out disable
            Bit_set(Pump_state_flag, FILL_OK);       //set flag fill_ok
        }
        break;
        case PUMP_PAUSE: //
            if(Pump_Pause == 0)
            {
                Pump_Sta = PUMP_START;
                start_step = 2;
                // if(pump_start_delay)
                FILL_PUMP_ABLE();
                FILL_PUMP_DIR_FF();
                TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            }
            break;
        default:
            break;
        }

    }
}

///*******************************************************************************
//* Function Name  : Conc
//* Description    : ??????
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		   : ??????
//*******************************************************************************/
void Conc(void)
{
    if(Bit_check_1(work_percheck, FILL_ALLOW_CHECK))    //??????
    {
        WASH_DISABLE();
        FILL_DISABLE();
        EMPTY_DISABLE();
        start_step = 2;
        Pump_Sta = PUMP_START;
        ////////////清除气泡报警标记///////////////////////////
        Bit_clr(Alarm_sta, WASH_AIR_ALARM);
        Bit_clr(Alarm_sta, FILL_AIR_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
        Bit_clr(Alarm_sta, PRE_FILL_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
        Bit_clr(Alarm_sta, AIR_ALARM);

        //////////////////////////////////////////
        if((Centrifugal_motor_speed > 8000)) //||((Mode==MODE_YINGJI)))
        {
            Bit_clr(work_percheck, FILL_ALLOW_CHECK);        //????
            Monitor_Conc_time = 0;		                      //????
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOE;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_0;
            TIM2->ARR = Pump_speed_change_TO_ARR(50);      //??????
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        }

    }
    else      //FILLING
    {
        switch(Pump_Sta)
        {
        case PUMP_START:       //????
            EMPTY_PUMP_ABLE();
            EMPTY_PUMP_DIR_FF();
            if(Pump_smooth_start(1, Set_Empty_speed) >= Set_Empty_speed)
            {
                Pump_Sta = PUMP_WORK;  //??????
                start_step = 1;
            }
            if(Pump_Pause == 1)
            {
                EMPTY_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            if(Cell_line == 1)
            {
                Pump_Sta = PUMP_STOP;
                stop_time = 0;
            }
            break;
        case PUMP_WORK:
            if(speed_change_flag == 1)                                   //???????????
            {
                Pump_set_ARR(Set_Empty_speed);
                speed_change_flag = 0;
            }
            if(Pump_Pause == 1)
            {
                EMPTY_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            if(Cell_line == 1)
            {
                Pump_Sta = PUMP_STOP;
                stop_time = 0;
            }
            //to do ??????
            break;
        case PUMP_STOP://??????
        {
            EMPTY_DISABLE();
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            Bit_set(Pump_state_flag, CONC_OK);
        }
        break;
        case PUMP_PAUSE: //
            if(Pump_Pause == 0)
            {
                Pump_Sta = PUMP_START;
                start_step = 2;
                TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            }
            break;
        default:
            break;
        }
    }
}

///*******************************************************************************
//* Function Name  : Fill
//* Description    : ??????
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		   : ??????
//*******************************************************************************/
void Return(void)
{
    if(Bit_check_1(work_percheck, FILL_ALLOW_CHECK))    //??????
    {
        WASH_DISABLE();
        EMPTY_DISABLE();
        FILL_DISABLE();
        start_step = 3;
        Pump_Sta = PUMP_START;
        ////////////清除气泡报警标记///////////////////////////
        Bit_clr(Alarm_sta, WASH_AIR_ALARM);
        Bit_clr(Alarm_sta, FILL_AIR_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_EARLY_ALARM);
        Bit_clr(Alarm_sta, PRE_FILL_ALARM);
        Bit_clr(Alarm_sta, EMPTY_AIR_ALARM);
        Bit_clr(Alarm_sta, AIR_ALARM);

        //////////////////////////////////////////
        if(Centrifugal_motor_speed < 500)
        {
            Bit_clr(work_percheck, FILL_ALLOW_CHECK);        //????
            Monitor_Return_time = 0;		      //????
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_3;
            TIM2->ARR = Pump_speed_change_TO_ARR(50);
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        }

    }
    else      //FILLING
    {
        switch(Pump_Sta)
        {
        case PUMP_START:       //????
            FILL_PUMP_ABLE();
            FILL_PUMP_DIR_RR();
            if(Pump_smooth_start(1, Set_Fill_speed) >= Set_Fill_speed)
            {
                Pump_Sta = PUMP_WORK;  //??????
                start_step = 1;
            }
            if(Pump_Pause == 1)
            {
                FILL_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            break;
        case PUMP_WORK:
            if(speed_change_flag == 1)                                   //???????????
            {
                Pump_set_ARR(Set_Fill_speed);//
                speed_change_flag = 0;
            }
            if(Pump_Pause == 1)
            {
                FILL_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            break;
        case PUMP_STOP://??????
            FILL_PUMP_ABLE();
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            Bit_set(Pump_state_flag, RETURN_OK);
            break;
        case PUMP_PAUSE: //
            FILL_DISABLE();
            if(Pump_Pause == 0)
            {
                Pump_Sta = PUMP_START;
                start_step = 3;
                TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            }
            break;
        default:
            break;
        }
    }
}
///*******************************************************************************
//* Function Name  : Fill
//* Description    : ??????
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		   : ??????
//*******************************************************************************/
void NO_wash(void)
{
    if(Bit_check_1(work_percheck, FILL_ALLOW_CHECK))    //??????
    {
        FILL_DISABLE();
        WASH_DISABLE();
        EMPTY_DISABLE();
        start_step = 3;
        Pump_Sta = PUMP_START;
        if(Centrifugal_motor_speed < 500)
        {
            Bit_clr(work_percheck, FILL_ALLOW_CHECK);        //????
            Monitor_Return_time = 0;		      //????
            TIM2->ARR = Pump_speed_change_TO_ARR(100);
        }

    }
    else      //FILLING
    {
        switch(Pump_Sta)
        {
        case PUMP_START:       //????
            FILL_PUMP_ABLE();
            FILL_PUMP_DIR_FF();
            EMPTY_PUMP_ABLE();
            EMPTY_PUMP_DIR_RR();
            Pump_GPIOx_2 = GPIOE;
            Pump_GPIOx_1 = GPIOD;
            GPIOx_2_Pin = GPIO_Pin_0;
            GPIOx_1_Pin = GPIO_Pin_3;
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

            if(Pump_smooth_start(1, Set_Fill_speed) >= Set_Fill_speed)
            {
                Pump_Sta = PUMP_WORK;  //??????
                start_step = 1;
            }
            if(Pump_Pause == 1)
            {
                EMPTY_DISABLE();
                FILL_DISABLE();
                Pump_Sta = PUMP_PAUSE;
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            }
            break;
        case PUMP_WORK:
            if(speed_change_flag == 1)                                   //???????????
            {
                Pump_set_ARR(Set_Fill_speed);//
                speed_change_flag = 0;
            }
            if(Pump_Pause == 1)
            {
                EMPTY_DISABLE();
                Pump_Sta = PUMP_PAUSE;
            }
            break;
        case PUMP_STOP://??????
        {
            EMPTY_DISABLE();
            EMPTY_DISABLE();
            FILL_DISABLE();
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            Bit_set(Pump_state_flag, RETURN_OK);
        }
        break;
        case PUMP_PAUSE: //
            FILL_DISABLE();
            WASH_DISABLE();
            EMPTY_DISABLE();
            if(Pump_Pause == 0)
            {
                Pump_Sta = PUMP_START;
                start_step = 3;
            }
            break;
        default:
            break;
        }
    }
}

///*******************************************************************************
//* Function Name  : Test_self
//* Description    : ??????
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		 : ??????
//*******************************************************************************/
void Auto_Run(void)
{
    uint16 MO_temp;
    uint16 MO_temp_1;
    uint16 monitor_reservoir_temp;
    switch (Auto_run_step)
    {
    case AUTO_RUN_START://wait for resevior

        if((Work_Stop_Flag == 0) && (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0)) //非停止状态下进行检测
        {
            if(monitor_reservoir_ml > Reservoir_leftover)
                monitor_reservoir_temp = monitor_reservoir_ml - Reservoir_leftover;
            else
                monitor_reservoir_temp = 0;
            if(monitor_reservoir_temp > Set_Run_Volume)
            {
                Centrifugal_motor_stop_orde = 0;
                Auto_run_step_cont++;
                if(Auto_run_step_cont > 150)
                {
                    if((Auto_run_step_cont % 30) < 10)
                        GPIO_SetBits(GPIOE , GPIO_Pin_11);
                    else
                        GPIO_ResetBits(GPIOE , GPIO_Pin_11);

                }
                if(Auto_run_step_cont > 200)
                {
                    GPIO_ResetBits(GPIOE , GPIO_Pin_11);
                    Auto_run_step = 1;
                    if(prefill_flag == 1)
                    {
                        sta = WORK;
                        Bit_clr(Pump_state_flag, EMPTY_OK);
                        Bit_set(Auto_check_bowl_falg, 1);
                        work_sta = WORK_PRE_FILL;
                        Auto_refill_wait_flag = 0;
                        Centrifugal_motor_orde = 1;
                        Centrifugal_motor_speed_Set = 5650;
                        Bit_set(work_percheck, FILL_ALLOW_CHECK);
                    }
                    else
                    {
                        sta = WORK;
                        work_sta = WORK_FILL;
                        Bit_clr(Pump_state_flag, EMPTY_OK);
                        Monitor_Pump.Fill_Pump_count = 0;
                        Auto_refill_wait_flag = 0;
                        Centrifugal_motor_orde = 1;
                        Centrifugal_motor_speed_Set = 5650;
                        pump_dely_time = 50;
                        Bit_set(work_percheck, FILL_ALLOW_CHECK);
                    }
                }
            }
            else
            {
                if(Auto_refill_wait_flag == 1)
                {
                    if(Auto_refill_wait_time > 120)
                    {
                        Centrifugal_motor_stop_orde = 1; //执行离心电机停止命令
                        Auto_refill_wait_flag = 0;
                        Bit_clr(Alarm_sta, WELL_ALARM);
                        if(Bit_check_1(Bowl_State, FILL_BLOOD))  //设置蠕动泵延时标志
                        {
                            Bit_set(Bowl_State, 1);               //再次启动时需要延时蠕动泵
                            Bit_clr(Bowl_State, 2);               //清除正在延时标志位
                            pump_dely_time = 50;
                        }
                    }
                    else
                    {
                        Centrifugal_motor_stop_orde = 0;
                        if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 1) //检测井盖
                        {
                            Bit_set(Alarm_sta, WELL_ALARM);
                            if(Bit_check_1(Bowl_State, FILL_BLOOD))  //设置蠕动泵延时标志
                            {
                                Bit_set(Bowl_State, 1);               //再次启动时需要延时蠕动泵
                                Bit_clr(Bowl_State, 2);               //清除正在延时标志位
                                pump_dely_time = 50;
                            }
                        }

                    }
                }
            }
        }
        else    //提示继续等待
        {
            Auto_run_step_cont = 0;
            GPIO_ResetBits(GPIOE , GPIO_Pin_11);
        }
        break;
    case AUTO_FILL://fill
        GPIO_ResetBits(GPIOE , GPIO_Pin_11);
        Centrifugal_motor_stop_orde = 0;
        if(Bit_check_1(Pump_state_flag, FILL_OK) || Bit_check_1(Pump_state_flag, CONC_OK))
        {
            Auto_run_step = 2;
            sta = WORK;
            Auto_refill_wait_flag = 0;
            work_sta = WORK_WASH;
            Monitor_Pump.Wash_Pump_count = 0;
            Monitor_Bowl_Wash_Volume = 0;
            Bit_clr(Pump_state_flag, WASH_OK);
            if(Bit_check_0(Alarm_sta, WELL_ALARM)) //无井盖报警
            {
                Centrifugal_motor_orde = MOTOR_START;
                Centrifugal_motor_speed_Set = 5650;
            }
            Bit_set(work_percheck, FILL_ALLOW_CHECK);
        }
        else    //检测状态
        {
            if(Bit_check_1(Alarm_sta, FILL_AIR_ALARM))
            {
                if(monitor_reservoir_ml < 600)
                {
                    //提示继续等待
                    Auto_run_step = 0;
                    // Centrifugal_motor_orde=2;
                    Set_Run_Volume = 400;
                    Auto_refill_wait_flag = 1;
                    Auto_refill_wait_time = 0;
                    if(monitor_reservoir_ml < 300)
                        Reservoir_leftover = monitor_reservoir_ml;
                    else
                        Reservoir_leftover = 300;
                    Standby();
                    //							 if(Bit_check_1(Bowl_State,FILL_BLOOD))   //设置蠕动泵延时标志
                    //								 {
                    //									 Bit_set(Bowl_State,1);                //再次启动时需要延时蠕动泵
                    //									 Bit_clr(Bowl_State,2);                //清除正在延时标志位
                    //									 pump_dely_time=50;
                    //								 }
                }
                else
                {
                    Pump_Pause = 1;
                }

            }
            if(reservoir_state == 1)
            {
                if((monitor_reservoir_ml < 300) && (work_sta == WORK_FILL) && (Pump_Sta == PUMP_WORK) && (reservoir_delay_check_cont > 10))
                {
                    if((monitor_reservoir_ml_buf_1s[15] > monitor_reservoir_ml_buf_1s[19]) || (monitor_reservoir_ml_buf_1s[15] == monitor_reservoir_ml_buf_1s[19]))
                    {
                        MO_temp = monitor_reservoir_ml_buf_1s[15] - monitor_reservoir_ml_buf_1s [19];
                        MO_temp_1 = monitor_reservoir_ml_buf_1s[14] - monitor_reservoir_ml_buf_1s[18];
                        if((MO_temp < 15) && (MO_temp_1 < 15))
                        {
                            //提示继续等待
                            Auto_run_step = 0;
                            Set_Run_Volume = 400;
                            Auto_refill_wait_flag = 1;
                            Auto_refill_wait_time = 0;
                            Reservoir_leftover = monitor_reservoir_ml;
                            Standby();
                        }
                    }
                }
            }
        }

        break;
    case AUTO_WASH ://wash
        GPIO_ResetBits(GPIOE , GPIO_Pin_11);
        if(Bit_check_1(Pump_state_flag, WASH_OK))
        {
            Auto_run_step = AUTO_EMPTY;
            sta = WORK;
            Pump_Pause = 0;
            Work_Stop_Flag = 0;
            WASH_DISABLE();
            FILL_DISABLE();
            EMPTY_DISABLE();
            Auto_refill_wait_flag = 0;
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
            work_sta = WORK_EMPTY;
            Monitor_Bowl_Empty_Volume = 0;
            Monitor_Pump.Empty_Pump_count = 0; //清零蠕动泵强制计数
            Monitor_Empty_Force_Count = 0;
            Centrifugal_motor_orde = 2;
            Bit_set(work_percheck, FILL_ALLOW_CHECK);
        }
        break;
    case AUTO_EMPTY ://empty
        GPIO_ResetBits(GPIOE , GPIO_Pin_11);
        //				 if(Bit_check_1(Pump_state_flag,EMPTY_OK))
        //				 {
        //					 Auto_run_step=0;
        //					 Auto_run_step_cont=0;
        //					 Standby();
        //				 }
        break;
    case AUTO_RETURN ://empty
        GPIO_ResetBits(GPIOE , GPIO_Pin_11);
        if(Bit_check_1(Pump_state_flag, RETURN_OK))
        {
            Auto_run_step = 0;
            Auto_run_step_cont = 0;
            Standby();
        }
        break;
    default:
        break;
    }

}
///*******************************************************************************
//* Function Name  : Test_self
//* Description    : ??????
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		 : ??????
//*******************************************************************************/
void Test_self(void)
{
    static uint8 pump_run_time;
    switch(Test_Step)
    {
    case 1:
        //if(Monitor_Pump.Fill_Pump_count);
        if(Default_Pump_set == PEMP_CHECK_DISABLE) //没有测速版
        {
            if(pump_run_time > 7)
            {
                Monitor_Pump.Fill_Pump_count++;
                pump_run_time = 0;
            }
            else
            {
                pump_run_time++;
            }
        }
        FILL_PUMP_ABLE();
        if(Monitor_Pump.Fill_Pump_count < 8) //
            FILL_PUMP_DIR_FF();
        else
            FILL_PUMP_DIR_RR();
        Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
        GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_3;
        TIM2->ARR = Pump_speed_change_TO_ARR(100);
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        if(Monitor_Pump.Fill_Pump_count > 16)
        {
            Bit_set(Test_Flag, FILL_TEST_OK);
            Monitor_Pump.Fill_Pump_count = 0;
            test_time = 0;
            Test_Step = 2;
            start_step = 3;
            stop_time = 0;
            Standby();
        }
        else
        {
            if(test_time > 250) //??????
            {
                Bit_clr(Test_Flag, FILL_TEST_OK);
                Standby();
                if(Test_step_next == 1)
                {
                    Test_Step++;
                    test_time = 0;
                    Test_step_next = 0;

                }
                if(Test_step_retest == 1)
                {
                    test_time = 0;
                    Test_step_retest = 0;
                }

            }
            else
                test_time++;
        }
        break;
    case 2:
        if(Default_Pump_set == PEMP_CHECK_DISABLE) //没有测速版
        {
            if(pump_run_time > 7)
            {
                Monitor_Pump.Wash_Pump_count++;
                pump_run_time = 0;
            }
            else
            {
                pump_run_time++;
            }
        }
        WASH_PUMP_ABLE();
        Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
        GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_7;
        TIM2->ARR = Pump_speed_change_TO_ARR(100);
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        if(Monitor_Pump.Wash_Pump_count > 8)
        {
            Bit_set(Test_Flag, WASH_TEST_OK);
            Monitor_Pump.Wash_Pump_count = 0;
            test_time = 0;
            Test_Step = 3;
            stop_time = 0;
            Standby();
        }
        else
        {
            if(test_time > 200) //??????
            {
                Bit_clr(Test_Flag, WASH_TEST_OK);
                Standby();
                if(Test_step_next == 1)
                {
                    Test_Step++;
                    test_time = 0;
                    Test_step_next = 0;

                }
                if(Test_step_retest == 1)
                {
                    test_time = 0;
                    Test_step_retest = 0;
                }

            }
            else
                test_time++;
        }
        break;
    case 3:
        if(Default_Pump_set == PEMP_CHECK_DISABLE) //没有测速版
        {
            if(pump_run_time > 7)
            {
                Monitor_Pump.Empty_Pump_count++;
                pump_run_time = 0;
            }
            else
            {
                pump_run_time++;
            }
        }
        EMPTY_PUMP_ABLE();
        if(Monitor_Pump.Empty_Pump_count < 8) //
            EMPTY_PUMP_DIR_FF();
        else
            EMPTY_PUMP_DIR_RR();
        Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOE;
        GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_0;
        TIM2->ARR = Pump_speed_change_TO_ARR(100);
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        if(Monitor_Pump.Empty_Pump_count > 16)
        {
            Bit_set(Test_Flag, EMPTY_TEST_OK);
            Monitor_Pump.Empty_Pump_count = 0;
            test_time = 0;
            Test_Step = 4;
            start_step = 3;
            stop_time = 0;
            Standby();
        }
        else
        {
            if(test_time > 250) //??????
            {
                Bit_clr(Test_Flag, EMPTY_TEST_OK);
                Standby();
                if(Test_step_next == 1)
                {
                    Test_Step++;
                    test_time = 0;
                    Test_step_next = 0;

                }
                if(Test_step_retest == 1)
                {
                    test_time = 0;
                    Test_step_retest = 0;
                }

            }
            else
                test_time++;

        }
        break;
    case 4:
        if(test_time > 200) //??????
        {
            Bit_clr(Test_Flag, LOUYE_TEST_OK);
            if(Test_step_next == 1)
            {
                Test_Step++;
                test_time = 0;
                Test_step_next = 0;

            }
            if(Test_step_retest == 1)
            {
                test_time = 0;
                Test_step_retest = 0;
            }

        }
        else
        {
            test_time++;
            if(test_time > 50)
            {
                if(READ_FUILT_UP_STATE() == 0) //||(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1)==1))
                {
                    Test_Step = 5;
                    Bit_set(Test_Flag, LOUYE_TEST_OK);
                }
                else
                {
                    Bit_clr(Test_Flag, LOUYE_TEST_OK);
                }
            }
        }
        break;
    case 5:
        if(test_time > 200) //??????
        {
            Bit_clr(Test_Flag, WELL_TEST_OK);
            if(Test_step_next == 1)
            {
                Test_Step = 6;
                Centrifugal_motor_orde = 1;
                Centrifugal_motor_speed_Set = 500;
                test_time = 0;
                Test_step_next = 0;
            }
            if(Test_step_retest == 1)
            {
                test_time = 0;
                Test_step_retest = 0;
            }
            if(READ_WELL_STATE() == 0)   //well ok
            {
                Test_Step = 6;
                Bit_set(Test_Flag, WELL_TEST_OK);
                Centrifugal_motor_orde = 1;
                Centrifugal_motor_speed_Set = 500;
            }

        }
        else
        {
            test_time++;
            if(test_time > 50)
            {
                if(READ_WELL_STATE() == 0)   //well ok
                {
                    Test_Step = 6;
                    Bit_set(Test_Flag, WELL_TEST_OK);
                    Centrifugal_motor_orde = 1;
                    Centrifugal_motor_speed_Set = 500;
                }
                else
                {
                    Bit_clr(Test_Flag, WELL_TEST_OK);
                }
            }
        }
        break;
    case 6:
        if(Centrifugal_motor_speed > 100)
        {
            Centrifugal_motor_orde = 2;

            Bit_set(Test_Flag, MOTO_TEST_OK);
            test_time = 0;
            Test_Step = 7;
        }
        else
        {
            if(test_time > 200) //??????
            {
                Bit_clr(Test_Flag, MOTO_TEST_OK);
                if(Test_step_next == 1)
                {
                    Test_Step++;
                    test_time = 0;
                    Test_step_next = 0;
                }
                if(Test_step_retest == 1)
                {
                    test_time = 0;
                    Test_step_retest = 0;
                }
            }
            else
                test_time++;
        }
        break;
    case 7:
        if(test_time > 100)
        {
            //			     Monitor_Empty_count=0;
            //		       Monitor_Wash_count=0;
            //		       Monitor_Fill_count=0;
            Monitor_Pre_fill_count = 0;
            Monitor_Fill_Volume = 0 ;
            Monitor_Wash_Volume = 0;
            Monitor_Empty_Volume = 0;
            Monitor_Pre_fill_Volume = 0 ;
            prefill_flag = 1; //加入预充冗余
            Set_Fill_speed = 400;
            Centrifugal_motor_orde = 2;
            sta = WORK;
            GUI_State = GUI_STANDBY;
            Show_pic_num = 2;
            show_pic_flag = 0;
            Display_Loop = 0;
            Pump_puls_count = 0;
            //					 if(Set_Bowl==225)
            //					   Set_Wash_Volume=1000;
            //					 else if (Set_Bowl==125)
            //					 {
            //						 Set_Wash_Volume=800;
            //					 }
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
        }
        else
        {
            test_time++;
        }
        break;
    default:
        Bit_set(work_percheck, FILL_ALLOW_CHECK);
        break;
    }


}
///*******************************************************************************
//* Function Name  : Factory_Auto_run
//* Description    : 工厂模式自动老化程序
//* Input          : none
//* Output         : None
//* Return         : None
//* Attention		 : ??????
//*******************************************************************************/
void Factory_Auto_run(void)
{
    //	uint8 Factory_Auto_step;
    //	uint16 Factory_test_time;
    switch(Factory_Auto_step)
    {
    case 0:
        if(Factory_test_time < 1000) //测试时间到
        {
            Factory_test_time++;

        }
        else
        {
            Factory_Auto_step = 1;
            Factory_test_time = 0;
            FILL_DISABLE();
            EMPTY_DISABLE();
            WASH_PUMP_ABLE();
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_7;
            Pump_set_ARR(Set_Pump_Test_speed);
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        }

        break;
    case 1:
        if(Factory_test_time < 1000) //测试时间到
        {
            Factory_test_time++;

        }
        else
        {
            Factory_Auto_step = 2;
            Factory_test_time = 0;
            //Bit_clr(pump_flag,PUMP_START_OK);
            FILL_DISABLE();
            WASH_DISABLE();
            EMPTY_PUMP_ABLE();
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOE;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_0;
            Pump_set_ARR(Set_Pump_Test_speed);
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        }
        //			   if(Bit_check_0(pump_flag,PUMP_START_OK))//start
        //			   {
        //					 Pump_smooth_start(1,300);
        //
        //			   }
        //				 else
        {
            // Green_Pump_Test(0,300);
        }

        break;
    case 2:
        if(Factory_test_time < 1000) //测试时间到
        {
            Factory_test_time++;

        }
        else
        {
            Factory_Auto_step = 3;
            Factory_test_time = 0;
            Centrifugal_motor_orde = 1;
            Centrifugal_motor_speed_Set = 500;
            Bit_clr(pump_flag, PUMP_START_OK);

        }
        //			   if(Bit_check_0(pump_flag,PUMP_START_OK))//start
        //			   {
        //					 Pump_smooth_start(1,300);
        //
        //			   }
        //				 else
        {
            //Red_Pump_Test(0,300);
        }

        break;
    case 3:
        if(Factory_test_time < 1000) //测试时间到
        {
            Factory_test_time++;
            FILL_DISABLE();
            WASH_DISABLE();
            EMPTY_DISABLE();

        }
        else
        {
            Factory_Auto_step = 0;
            Factory_test_time = 0;
            Centrifugal_motor_orde = 2;
            WASH_DISABLE();
            EMPTY_DISABLE();
            FILL_PUMP_ABLE();
            Pump_GPIOx_2 = Pump_GPIOx_1 = GPIOD;
            GPIOx_2_Pin = GPIOx_1_Pin = GPIO_Pin_3;
            TIM2->ARR = Pump_speed_change_TO_ARR(Set_Pump_Test_speed);
            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            //if(dir==1)
            FILL_PUMP_DIR_FF();
            // else
            //  FILL_PUMP_DIR_RR();
            // Bit_clr(pump_flag,PUMP_START_OK);
        }
        break;
    default:
        break;

    }


}

/*******************************************************************************
* Function Name  : ??????
* Description    : ??????
* Input          : none
* Output         : None
* Return         : None
* Attention		 :
*******************************************************************************/
void Cell_Saver_Control(void)
{
    if(Mode == MODE_AUTO && (sta == WORK))
    {
        Auto_Run();
    }

    switch(sta)
    {
    case TEST_STA:
        Test_self();
        break;
    case WORK:
        switch(work_sta)
        {
        case WORK_PRE_FILL:
            if(Work_Stop_Flag != 1)
            {
                Pre_Fill();
            }
            break;
        case WORK_FILL:
            if(Work_Stop_Flag != 1)
            {
                if(Bit_check_1(Bowl_State, 1)) //??????
                {
                    Bit_set(Bowl_State, 2); //??????
                    if(pump_dely_time == 0)
                    {
                        Bit_clr(Bowl_State, 1);
                        Bit_clr(Bowl_State, 2);
                        Pump_Pause = 0; //????
                    }
                }
                else
                    Fill();
            }
            break;
        case WORK_WASH:
            if(Work_Stop_Flag != 1)
            {
                if(Bit_check_1(Bowl_State, 1)) //??????
                {
                    Bit_set(Bowl_State, 2);   //??????
                    if(pump_dely_time == 0)
                    {
                        Bit_clr(Bowl_State, 1);
                        Bit_clr(Bowl_State, 2);
                        Pump_Pause = 0; //????
                    }
                }
                else
                    Wash();
            }
            break;
        case WORK_EMPTY:

            Bit_clr(Bowl_State, 2); //
            pump_dely_time = 0;
            if(Work_Stop_Flag != 1)
            {
                Empty();
            }
            break;
        case WORK_CONC:
            if(Work_Stop_Flag != 1)
            {
                if(Bit_check_1(Bowl_State, 1)) //??????
                {
                    Bit_set(Bowl_State, 2);   //??????
                    if(pump_dely_time == 0)
                    {
                        Bit_clr(Bowl_State, 1);
                        Bit_clr(Bowl_State, 2);
                        Pump_Pause = 0; //????

                    }
                }
                else
                    Conc();
            }
            break;
        case WORK_RETURN:
            if(Work_Stop_Flag != 1)
            {
                Return();
            }
            break;
        case WORK_JINJI:
            NO_wash();
            break;
        case WORK_STANDBY:
            Standby();
            break;
        }
        break;
    case FACTORY:
        if(Factory_autorun_flag == 1)
        {
            Factory_Auto_run();
        }
        break;
    }

    Motor_Check();
    Centrifugal_motor(); //??????
}







