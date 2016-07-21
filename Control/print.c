#include "stm32f10x.h"
#include "../User/user_conf.h"
#include "..\Drive\Uart.h"
#include "..\Drive\RTC.h"
//time_type Tend_Time;
#define PRT_china      0x26
#define PRT_english    0x2E
#define TAG_SN          0X01
#define TAG_PRODUCT_NUM   0X02
#define TAG_SW            0X03
#define TAG_HW            0X04


uint8 PRT_MACHINE[] = {"  BW-8200B型自体血液回收机"};
uint8 PRT_Med_No[] = {"病历号:"};
uint8 PRT_patient_massage[] = {"性别:    年龄:     体重:    "};
uint8 PRT_yar[] = {"年"};
uint8 PRT_mounth[] = {"月"};
uint8 PRT_day[] = {"日"};
uint8 PRT_empty_tolal[] = {"总回收血量:"};
uint8 PRT_wash_tolal[] = {"总盐水用量:"};
uint8 PRT_fill_tolal[] = {"总进血量:"};
uint8 PRT_ml[] = {"mL"};
uint8 PRT_company[] = {"北京万东康源科技开发有限公司"};

uint8 print_able_flag = 1;
uint16 print_able_cont;
uint8 send_cont = 0;
void USART2_TX_Byte(uint8 tx_byte);//print函数
void Integer_Number_Change_String(int32 Value, uint8 *String_Data);
/*******************************************************************************
* Function Name  : print_STRING
* Description    : 打印字符串
* Input          : sendstring
* Output         : None
* Return         : None
* Attention		   :
*******************************************************************************/
void USART2_TX_STRING(uint8 *p)
{
    while((*p) != '\0')
    {
        USART2_TX_Byte(*p);
        p++;
    }
    //USART2_TX_Byte(0x0A);
}




/*******************************************************************************
* Function Name  : word_size
* Description    : 设置点阵打印规格
* Input          : size
* Output         : None
* Return         : None
* Attention		   :
*******************************************************************************/
void word_size(uint8 i)
{
    USART2_TX_Byte(0x1c);
    USART2_TX_Byte(0x69);
    USART2_TX_Byte(i);
}



/*******************************************************************************
* Function Name  : print_init
* Description    : 初始化打印机
* Input          : none
* Output         : None
* Return         : None
* Attention		   :
*******************************************************************************/
void print_init()
{
    uint8 h = 4;
    while(h--)
    {
        USART2_TX_Byte(0x1B);   //初始化
        USART2_TX_Byte(0x40);
    }
    USART2_TX_Byte(PRT_china);
    USART2_TX_Byte(0x1B);       //正向打印
    USART2_TX_Byte(0x63);
    USART2_TX_Byte(0);
    //	  USART2_TX_Byte(0x1C); //汉字打印
    //	  USART2_TX_Byte(0x26);
    word_size(0);  //16*16点阵
    USART2_TX_Byte(0x1B);   //初始化
    USART2_TX_Byte(0x40);
}





void ChinaEnglish(uint8 i)
{
    USART2_TX_Byte(0x1c);

    if(i == PRT_china ) //??
    {
        USART2_TX_Byte(PRT_china);
        USART2_TX_Byte(0x0E);
    }
    else
        USART2_TX_Byte(PRT_english);   //????

}


/*******************************************************************************
* Function Name  : PRT_data
* Description    : 打印日期
* Input          : none
* Output         : None
* Return         : None
* Attention		   :
*******************************************************************************/
void PRT_Date(void)
{
    uint8 str[11] = {0, 0, 0, 0, '-', 0, 0, '-', 0, 0, '\0'};
    str[0] = ((timer.year)) / 1000 + '0';
    str[1] = (((timer.year)) % 1000) / 100 + '0';
    str[2] = ((timer.year)) % 100 / 10 + '0';
    str[3] = (((timer.year) & 0x0FFF) % 100) % 10 + '0';
    str[5] = ((timer.month)) / 10 + '0';
    str[6] = ((timer.month)) % 10 + '0';
    str[8] = (timer.day) / 10 + '0';
    str[9] = (timer.day) % 10 + '0';
    USART2_TX_STRING(str);
    USART2_TX_Byte(' ');
    USART2_TX_Byte(' ');
    // USART2_TX_Byte(0x0A);
}




/*******************************************************************************
* Function Name  : PRT_Time
* Description    : 打印时间
* Input          : none
* Output         : None
* Return         : None
* Attention		   :
*******************************************************************************/
void PRT_Time(void)
{
    uint8 str[9] = {0, 0, ':', 0, 0, ':', 0, 0, '\0'};
    str[0] = timer.hour / 10 + '0';
    str[1] = timer.hour % 10 + '0';
    str[3] = timer.min / 10 + '0';
    str[4] = timer.min % 10 + '0';
    str[6] = timer.sec / 10 + '0';
    str[7] = timer.sec % 10 + '0';
    USART2_TX_STRING(str);
    USART2_TX_Byte(0x0A);
    USART2_TX_Byte(0x0A);

}
/*******************************************************************************
* Function Name  : PRT_data
* Description    : 打印日期
* Input          : none
* Output         : None
* Return         : None
* Attention		   :
*******************************************************************************/
void Print_Date(time_type prt_time)
{
    uint8 str[11] = {0, 0, 0, 0, '-', 0, 0, '-', 0, 0, '\0'};
    str[0] = ((prt_time.year)) / 1000 + '0';
    str[1] = (((prt_time.year)) % 1000) / 100 + '0';
    str[2] = ((prt_time.year)) % 100 / 10 + '0';
    str[3] = (((prt_time.year) & 0x0FFF) % 100) % 10 + '0';
    str[5] = ((prt_time.month)) / 10 + '0';
    str[6] = ((prt_time.month)) % 10 + '0';
    str[8] = (prt_time.day) / 10 + '0';
    str[9] = (prt_time.day) % 10 + '0';
    USART2_TX_STRING(str);
    USART2_TX_Byte(' ');
    USART2_TX_Byte(' ');
    // USART2_TX_Byte(0x0A);
}




/*******************************************************************************
* Function Name  : PRT_Time
* Description    : 打印时间
* Input          : none
* Output         : None
* Return         : None
* Attention		   :
*******************************************************************************/
void Print_Time(time_type prt_time)
{
    uint8 str[9] = {0, 0, ':', 0, 0, ':', 0, 0, '\0'};
    str[0] = prt_time.hour / 10 + '0';
    str[1] = prt_time.hour % 10 + '0';
    str[3] = prt_time.min / 10 + '0';
    str[4] = prt_time.min % 10 + '0';
    str[6] = prt_time.sec / 10 + '0';
    str[7] = prt_time.sec % 10 + '0';
    USART2_TX_STRING(str);
    USART2_TX_Byte(0x0A);
    USART2_TX_Byte(0x0A);

}
//time_type timer

/*******************************************************************************
* Function Name  : PRT_EMPTY_TOTAL
* Description    : 打印总回收血量
* Input          : EMPTY_TOTAL
* Output         : None
* Return         : None
* Attention		   :
*******************************************************************************/
void PRT_EMPTY_TOTAL(uint16 i)
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    Integer_Number_Change_String(i, str);
    USART2_TX_STRING(str);

}




/*******************************************************************************
* Function Name  : print
* Description    : print
* Input          : none
* Output         : None
* Return         : None
* Attention		   : 打印机函数
*******************************************************************************/
void print()
{
    uint8 temp_i;
    print_init();
    USART2_TX_Byte(0x1C); //汉字打印
    USART2_TX_Byte(0x26);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);

    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);

    USART2_TX_STRING(PRT_MACHINE);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);                               //换行
    USART2_TX_STRING(PRT_Med_No);
    USART2_TX_STRING(Set_person);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);
    USART2_TX_STRING(PRT_patient_massage);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);
    PRT_Date();
    PRT_Time();

    USART2_TX_STRING(PRT_fill_tolal);
    PRT_EMPTY_TOTAL(Monitor_Fill_Volume);
    USART2_TX_STRING(PRT_ml);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);

    USART2_TX_STRING(PRT_wash_tolal);
    PRT_EMPTY_TOTAL(Monitor_Wash_Volume);
    USART2_TX_STRING(PRT_ml);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);

    USART2_TX_STRING(PRT_empty_tolal);
    PRT_EMPTY_TOTAL(Monitor_Empty_Volume);
    USART2_TX_STRING(PRT_ml);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);

    //		  USART2_TX_Byte(0x1b);
    //      USART2_TX_Byte(0x56);
    //      USART2_TX_Byte(1);
    USART2_TX_STRING(PRT_company);
    for(temp_i = 0; temp_i < 7; temp_i++)
        USART2_TX_Byte(0x0a);

}
/*******************************************************************************
* Function Name  : print
* Description    : print
* Input          : none
* Output         : None
* Return         : None
* Attention		   : 打印机函数
*******************************************************************************/
void print_Tend()
{
    uint8 temp_i;
    print_init();
    USART2_TX_Byte(0x1C); //汉字打印
    USART2_TX_Byte(0x26);
    USART2_TX_STRING(PRT_MACHINE);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);
    USART2_TX_STRING("历史信息打印");
    USART2_TX_Byte(0x0A);
    //	   USART2_TX_STRING("手术时间");
    //	   Print_Date( prt_Time);
    //	   Print_Time(prt_Time);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);                               //换行
    USART2_TX_STRING(PRT_Med_No);
    USART2_TX_STRING(Set_person);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);
    USART2_TX_STRING(PRT_patient_massage);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);
    USART2_TX_STRING("手术时间");
    Print_Date( prt_Time);
    Print_Time(prt_Time);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);

    USART2_TX_STRING(PRT_fill_tolal);
    PRT_EMPTY_TOTAL(Tend_Fill_Volume);
    USART2_TX_STRING(PRT_ml);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);

    USART2_TX_STRING(PRT_wash_tolal);
    PRT_EMPTY_TOTAL(Tend_Wash_Volume);
    USART2_TX_STRING(PRT_ml);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);

    USART2_TX_STRING(PRT_empty_tolal);
    PRT_EMPTY_TOTAL(Tend_Empty_Volume);
    USART2_TX_STRING(PRT_ml);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);

    USART2_TX_Byte(0x1b);
    USART2_TX_Byte(0x56);
    USART2_TX_Byte(1);

    USART2_TX_STRING(PRT_company);
    USART2_TX_Byte(0x0D);
    USART2_TX_Byte(0x0A);
    //		 	USART2_TX_Byte(0x1C); //取消汉字打印
    //	   USART2_TX_Byte(0x2E);
    USART2_TX_STRING("打印时间");
    PRT_Date();
    PRT_Time();
    for(temp_i = 0; temp_i < 7; temp_i++)
        USART2_TX_Byte(0x0A);

}
/*******************************************************************************
* Function Name  : print
* Description    : print
* Input          : none
* Output         : None
* Return         : None
* Attention		   : 打印机函数
*******************************************************************************/
void print_factory(void)
{
    print_init();
    USART2_TX_Byte(0x1C); //汉字打印
    USART2_TX_Byte(0x26);
    USART2_TX_Byte(0x0A);
    USART2_TX_STRING("上称重零点");
    PRT_EMPTY_TOTAL(Reservoir_zero);
    //USART2_TX_Byte(0x0A);

    USART2_TX_STRING("上称重500");
    PRT_EMPTY_TOTAL(Reservoir_500);
    USART2_TX_Byte(0x0A);

    USART2_TX_STRING("下称重7000：");
    PRT_EMPTY_TOTAL(Dwon_weigh_7000);
    //USART2_TX_Byte(0x0A);
    USART2_TX_STRING("下称重9000：");
    PRT_EMPTY_TOTAL(Dwon_weigh_9000);

    USART2_TX_Byte(0x0A);
    USART2_TX_STRING("进血泵系数");
    PRT_EMPTY_TOTAL(fill_pump_adjust);
    USART2_TX_Byte(0x0A);
    USART2_TX_STRING("清洗泵系数");
    PRT_EMPTY_TOTAL(wash_pump_adjust);
    USART2_TX_Byte(0x0A);
    USART2_TX_STRING("清空泵系数");
    PRT_EMPTY_TOTAL(empty_pump_adjust_125);
    USART2_TX_Byte(0x0A);



    //	   USART2_TX_STRING("上称重零点");
    //     PRT_EMPTY_TOTAL(Tend_Fill_Volume);

    //	   USART2_TX_STRING(PRT_fill_tolal);
    //		 PRT_EMPTY_TOTAL(Tend_Fill_Volume);
    //		 USART2_TX_STRING(PRT_ml);
    //     USART2_TX_Byte(0x0A);
    //
    //	   USART2_TX_STRING(PRT_wash_tolal);
    //		 PRT_EMPTY_TOTAL(Tend_Wash_Volume);
    //		 USART2_TX_STRING(PRT_ml);
    //     USART2_TX_Byte(0x0A);
    //
    //     USART2_TX_STRING(PRT_empty_tolal);
    //		 PRT_EMPTY_TOTAL(Tend_Empty_Volume);
    //		 USART2_TX_STRING(PRT_ml);
    //     USART2_TX_Byte(0x0A);

}
/*******************************************************************************
* Function Name  : void Blue_teeth_TX_Head(void)
* Description    : print
* Input          : none
* Output         : None
* Return         : None
* Attention		   : 蓝牙传输函数传输数据头
*******************************************************************************/
void Blue_teeth_TX_Head(void)
{
    USART2_TX_Byte(0xaa);
    USART2_TX_Byte(0x55);

    USART2_TX_Byte(0x01); //发送方地址
    USART2_TX_Byte(0x02); //接收方地址

    USART2_TX_Byte(0x01);	//发送或者接受
    USART2_TX_Byte(0x01); //通信协议版本号
}
/*******************************************************************************
* Function Name  : void Blue_teeth_TX_Tail(void)
* Description    : print
* Input          : none
* Output         : None
* Return         : None
* Attention		   : 蓝牙传输函数传输数据头
*******************************************************************************/
void Blue_teeth_TX_Tail(void)
{
    USART2_TX_Byte(0xCC);
    USART2_TX_Byte(0x33);
}
/*******************************************************************************
* Function Name  : void Blue_teeth_TX_Tail(void)
* Description    : print
* Input          : none
* Output         : None
* Return         : None
* Attention		   : 蓝牙传输函数传输数据头
*******************************************************************************/
void Blue_teeth_TX_Data(uint8 Tag, uint8 Data_len, uint8 *data)
{
    uint8 temp_i;
    uint8 Content_ver = 0x01;
    uint8 Content_len;
    uint16 CRC_Sum;
    uint8 CRC_Sum_h;
    uint8 CRC_Sum_l;
    Content_len = Data_len + 3; //不包括本字节

    Blue_teeth_TX_Head();
    USART2_TX_Byte(Content_len);
    USART2_TX_Byte(Content_ver);
    USART2_TX_Byte(Tag);
    USART2_TX_Byte(Data_len);
    for(temp_i = 0; temp_i < Data_len; temp_i++)
    {
        USART2_TX_Byte (*(data + temp_i));
        CRC_Sum += *(data + temp_i);
    }
    CRC_Sum = CRC_Sum + Content_ver + Tag + Data_len;
    CRC_Sum_h =  (CRC_Sum >> 8) & 0xff;
    CRC_Sum_l = CRC_Sum & 0x00ff;
    USART2_TX_Byte(CRC_Sum_l);
    USART2_TX_Byte(CRC_Sum_h);
    Blue_teeth_TX_Tail();
}
/*******************************************************************************
* Function Name  : void Blue_teeth(void)
* Description    : print
* Input          : none
* Output         : None
* Return         : None
* Attention		   : 蓝牙传输函数
*******************************************************************************/
void Blue_teeth_1(void)
{
    uint8 send_message[10];
    uint16 temp;
    switch(send_cont)
    {
    case 0:
        send_cont++;
        temp = (uint16)Monitor_Fill_Volume;
        send_message[0] = (uint16)temp & 0x00FF;
        send_message[1] = ((uint16)temp >> 8) & 0x00FF;;
        Blue_teeth_TX_Data(0x14, 2, send_message);
        break;
    case 1:
        send_cont++;

        temp = (uint16)Monitor_Wash_Volume;
        send_message[0] = (uint16)temp & 0x00FF;
        send_message[1] = ((uint16)temp >> 8) & 0x00FF;;
        Blue_teeth_TX_Data(0x15, 2, send_message);
        break;
    case 2:
        send_cont++;;
        temp = (uint16)Monitor_Empty_Volume;
        send_message[0] = (uint16)temp & 0x00FF;
        send_message[1] = ((uint16)temp >> 8) & 0x00FF;;
        Blue_teeth_TX_Data(0x16, 2, send_message);
        break;
    case 3:
        send_cont++;
        temp = work_sta;
        send_message[0] = temp & 0x00FF;
        //send_message[1]=((uint16)temp>>8)&0x00FF;;
        Blue_teeth_TX_Data(0x12, 1, send_message);
        break;
    case 4:
        send_cont++;
        temp = Mode;
        send_message[0] = temp & 0x00FF;
        //send_message[1]=((uint16)temp>>8)&0x00FF;;
        Blue_teeth_TX_Data(0x11, 1, send_message);
        break;
    case 5:
        send_cont++;
        temp = Set_Bowl;
        send_message[0] = temp & 0x00FF;
        //send_message[1]=((uint16)temp>>8)&0x00FF;;
        Blue_teeth_TX_Data(0x17, 1, send_message);
        break;
    case 6:
        send_cont++;
        temp = Set_Bowl;
        send_message[0] = temp & 0x00FF;
        //send_message[1]=((uint16)temp>>8)&0x00FF;;
        Blue_teeth_TX_Data(0x13, 1, send_message);
        break;
    case 7:
        send_cont++;
        if(Pump_Pause == 1)
            temp = 2;
        else
            temp = 0;
        send_message[0] = temp & 0x00FF;
        //send_message[1]=((uint16)temp>>8)&0x00FF;;
        Blue_teeth_TX_Data(0x17, 1, send_message);
        break;
    case 8:
        send_cont++;
        switch(work_sta)
        {
        case WORK_FILL:
        case WORK_RETURN:
            temp = Set_Fill_speed;
            break;
        case WORK_WASH:
            temp = Set_Wash_speed;
            break;
        case WORK_EMPTY:
            temp = Set_Empty_speed;
            break;
        default:
            break;

        }
        send_message[0] = temp & 0x00FF;
        send_message[1] = ((uint16)temp >> 8) & 0x00FF;;
        Blue_teeth_TX_Data(0x18, 2, send_message);
        break;
    case 9:
        send_cont++;
        //send_cont=0;
        temp = Set_Fill_speed;
        send_message[0] = temp & 0x00FF;
        send_message[1] = ((uint16)temp >> 8) & 0x00FF;;
        Blue_teeth_TX_Data(0x31, 2, send_message);
        break;
    case 10:
        send_cont++;
        //send_cont=0;
        temp = Set_Wash_speed;
        send_message[0] = temp & 0x00FF;
        send_message[1] = ((uint16)temp >> 8) & 0x00FF;;
        Blue_teeth_TX_Data(0x32, 2, send_message);
        break;
    case 11:
        send_cont++;
        //send_cont=0;
        temp = Set_Empty_speed;
        send_message[0] = temp & 0x00FF;
        send_message[1] = ((uint16)temp >> 8) & 0x00FF;;
        Blue_teeth_TX_Data(0x33, 2, send_message);
        break;
    case 12:
        send_cont++;
        //send_cont=0;
        temp = Set_Wash_Volume;
        send_message[0] = temp & 0x00FF;
        send_message[1] = ((uint16)temp >> 8) & 0x00FF;;
        Blue_teeth_TX_Data(0x34, 2, send_message);
        break;
    case 13:
        send_cont++;
        ;
        temp = Set_Run_Volume;
        send_message[0] = temp & 0x00FF;
        send_message[1] = ((uint16)temp >> 8) & 0x00FF;;
        Blue_teeth_TX_Data(0x35, 2, send_message);
        break;
		case 14:
        send_cont++;
        ;
        temp = Set_Run_Volume++;
        send_message [0] = Set_Run_Volume;
        send_message[1] = 2;
		    send_message[2] = 'c';
		    send_message[3] = 'D';
		    send_message[4] = 'E';
		     send_message[5] = 'F';
        send_message[6] = 'G';
		    send_message[7] = 'H';
		    send_message[8] = 'I';
		    send_message[9] = 'J';
        Blue_teeth_TX_Data(0x01, 7, PRT_Med_No);
        break;
		case 15:
        send_cont++;
        temp = Set_Run_Volume++;
        send_message [0] = Set_Run_Volume;
        send_message[1] = 2;
		    send_message[2] = 'c';
		    send_message[3] = 'D';
		    send_message[4] = 'E';
		     send_message[5] = 'F';
        send_message[6] = 'G';
		    send_message[7] = 'H';
		    send_message[8] = 'I';
		    send_message[9] = 'J';
        Blue_teeth_TX_Data( TAG_PRODUCT_NUM  , 7, send_message);
        break;
			case 16:
        send_cont++;
        Blue_teeth_TX_Data( TAG_SW  , 4, ver_str_insine);
        break;
			case 17:
        send_cont++;
        Blue_teeth_TX_Data( TAG_HW  , 4, hw_str);
        break;
    default:
        send_cont = 0;
        break;
    }
}
/*******************************************************************************
* Function Name  : print
* Description    : print
* Input          : none
* Output         : None
* Return         : None
* Attention		   : 打印机函数
*******************************************************************************/
void Print_Report(void)
{
    if(print_able_flag == 1)
    {
        if(print_flag == 1)
        {
					  GPIO_ResetBits(GPIOE , GPIO_Pin_8);
            print();

            print_flag = 0;
            print_able_cont = 500;
            print_able_flag = 0;
					  
        }
        if(print_flag == 2)
        {
            print_Tend();
            print_flag = 0;
            print_able_cont = 10;
            print_able_flag = 0;
        }
        if(print_flag == 4)
        {
            print_factory();
            print_flag = 0;
            print_able_cont = 10;
            print_able_flag = 0;
        }
    }
    else
    {
        print_able_cont--;
        print_flag = 0;
			  if(print_able_cont<490)
						GPIO_SetBits(GPIOE , GPIO_Pin_8);
        if(print_able_cont == 0)
            print_able_flag = 1;
				GPIO_SetBits(GPIOE , GPIO_Pin_8);

    }

}