/*************** Radio File (c) ***************/
/**                                          **/
/**      基于STM32芯片开发的血液回收机    **/
/**                                          **/
/**--------------文件信息--------------------**/
/**文    件   名: M600_API.C                 **/
/**创    建   人: 张峰                      **/
/**创  建  日 期: Aug.1 2014                **/
/**描         述: 封装用于619显示API函数     **/
/**               16位色图形显示             **/
/**                                          **/
/**********************************************/

#include "../User/user_conf.h"
#include "../Drive/Uart.h"
#include "M600_API.h"
//#include "Macro_define.h"
void  UART2_TX(uint8 data)//FOR TEST
{
}
/**********************************************/
/* 函数名称: Tx_Byte                          */
/* 功能描述: 发送字节函数                     */
/*           试验用非正式                     */
/* 输  　入: data                             */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  Tx_Byte(uint8 data)
{
    //while(Dwin_Busy_IO_Detect_0())  ;               //判断迪文串口是否在发数据	 //0:busy
    USART3_TX_Byte(data);
}

/**********************************************/
/* 函数名称: Tx_Word                          */
/* 功能描述: 发送字函数                       */
/*           试验用非正式                     */
/* 输  　入: data                             */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  Tx_Word(uint16 data)
{
    Tx_Byte((data >> 8) & 0xFF); //先发送高字节
    Tx_Byte(data & 0xFF);	 //再发低字节
}

/****************************************************************************
* 名称：TxString()
* 功能：向串口发送一字符串
* 入口参数：srt--要发送的字符串的指针
* 出口参数：无
****************************************************************************/
void  TxString(uint8 const *str)
{
    while(*str != '\0')
    {
        Tx_Byte(*str++);	    // 发送数据
    }
}

/**********************************************/
/* 函数名称: Tx_EOF                           */
/* 功能描述: 发送帧的结束符函数               */
/*           试验用非正式                     */
/* 输  　入: data                             */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  Tx_EOF(void)
{
    Tx_Byte(0xCC);
    Tx_Byte(0x33);
    Tx_Byte(0xC3);
    Tx_Byte(0x3C);
}

/**********************************************/
/* 函数名称: M600_Handshake                   */
/* 功能描述: 发送握手信号函数                 */
/*           试验用非正式                     */
/* 输  　入: data                             */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Handshake(void)
{
    Tx_Byte(0xAA);
    Tx_Byte(0x00);
    Tx_EOF();
}

/**********************************************/
/* 函数名称: M600_Refresh_Screen              */
/* 功能描述: 发送强制刷新1次全屏显示函数      */
/*           试验用非正式                     */
/* 输  　入: data                             */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Refresh_Screen(void)
{
    Tx_Byte(0xAA);
    Tx_Byte(0xD0);
    Tx_EOF();
}
/**********************************************/
/* 函数名称: M600_Set_Palette                 */
/* 功能描述: 调色板函数                       */
/*           试验用非正式                     */
/* 输  　入: display_color					  */
/*           background_color                 */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Set_Palette(uint16 display_color, uint16 background_color)
{
    Tx_Byte(0xAA);
    Tx_Byte(0x40);
    Tx_Word(display_color);
    Tx_Word(background_color);
    Tx_EOF();
}

/**********************************************/
/* 函数名称: M600_Set_Palette                 */
/* 功能描述: 调色板函数                       */
/*           试验用非正式                     */
/* 输  　入: display_color					  */
/*           background_color                 */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Set_Distance(uint16 D_X, uint16 D_Y)
{
    Tx_Byte(0xAA);
    Tx_Byte(0x41);
    Tx_Word(D_X);
    Tx_Word(D_Y);
    Tx_EOF();
}
/**********************************************/
/* 函数名称: M600_Set_background                 */
/* 功能描述: 调色板函数                       */
/*           试验用非正式                     */
/* 输  　入: display_color					  */
/*           background_color                 */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Set_background(uint16 x, uint16 y)
{
    Tx_Byte(0xAA);
    Tx_Byte(0x42);
    Tx_Word(x);
    Tx_Word(y);
    Tx_EOF();
}
/**********************************************/
/* 函数名称: M600_Set_background                 */
/* 功能描述: 调色板函数                       */
/*           试验用非正式                     */
/* 输  　入: display_color					  */
/*           background_color                 */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Set_foreground(uint16 x, uint16 y)
{
    Tx_Byte(0xAA);
    Tx_Byte(0x43);
    Tx_Word(x);
    Tx_Word(y);
    Tx_EOF();
}

/**********************************************/
/* 函数名称: Dwin_Text_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*                                            */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			  */
/*           C_Mode--字符显示模式和编码方法   */
/*			     C_dots--显示点阵大小 			  */
/*			     Color--字符的前景色			  */
/*			     Bcolor--字符的背景色			  */
/*			     srt--要发送的字符串的指针		  */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  Dwin_Text_Display(uint8 command , uint16 position_x, uint16 position_y, uint8 *str)
{
    //uint8 temp_i;
    Tx_Byte(0xAA);
    Tx_Byte(command);
    Tx_Word(position_x);
    Tx_Word(position_y);
    TxString(str);
    Tx_EOF();

}
/**********************************************/
/* 函数名称: M600_Text_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*           试验用非正式                     */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			        */
/*           C_Mode--字符显示模式和编码方法   */
/*			 C_dots--显示点阵大小 			         */
/*			 Color--字符的前景色			           */
/*			 Bcolor--字符的背景色			           */
/*			 srt--要发送的字符串的指针		       */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  Dwin_Text_1_Display(uint16 position_x, uint16 position_y, uint8 Lib_ID, uint8 C_Mode, uint8 C_dots, uint16 Color, uint16 Bcolor, uint8 *str)
{
    Tx_Byte(0xAA);
    Tx_Byte(0x98);
    Tx_Word(position_x);
    Tx_Word(position_y);
    Tx_Byte(Lib_ID);
    Tx_Byte(C_Mode);
    Tx_Byte(C_dots);
    Tx_Word(Color);
    Tx_Word(Bcolor);
    TxString(str);
    Tx_EOF();
}
/**********************************************/
/* 函数名称: M600_Text_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*           试验用非正式                     */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			        */
/*           C_Mode--字符显示模式和编码方法   */
/*			 C_dots--显示点阵大小 			          */
/*			 Color--字符的前景色			            */
/*			 Bcolor--字符的背景色			            */
/*			 srt--要发送的字符串的指针		        */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  M600_Text_Display(uint8 big, uint16 position_x, uint16 position_y, uint16 display_num)
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    Integer_Number_Change_String(display_num, str);
    //M600_Deal_Picture(Command_Cut_Picture,0x02,position_x,position_y,position_x+80,position_y+54,position_x,position_y);
    Dwin_Text_1_Display(position_x, position_y, 0x27, 0x80, 0x05, 0x4adf, 0xef3d, str);

}
/**********************************************/
/* 函数名称: M600_Text_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*           试验用非正式                     */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			        */
/*           C_Mode--字符显示模式和编码方法   */
/*			 C_dots--显示点阵大小 			          */
/*			 Color--字符的前景色			            */
/*			 Bcolor--字符的背景色			            */
/*			 srt--要发送的字符串的指针		        */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  M600_Text_2_Display(uint8 pic_num, uint16 position_x, uint16 position_y, uint16 display_num)
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    Integer_Number_Change_String(display_num, str);
    M600_Deal_Picture(Command_Cut_Picture, pic_num, position_x, position_y, position_x + 100, position_y + 30, position_x, position_y);
    Dwin_Text_1_Display(position_x, position_y, 0x27, 0x80, 0x05, 0x0000, 0xef3d, str);

}
/**********************************************/
/* 函数名称: M600_Text_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*           试验用非正式                     */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			        */
/*           C_Mode--字符显示模式和编码方法   */
/*			 C_dots--显示点阵大小 			          */
/*			 Color--字符的前景色			            */
/*			 Bcolor--字符的背景色			            */
/*			 srt--要发送的字符串的指针		        */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  M600_Text_0X55_Display(uint8 pic_num, uint16 position_x, uint16 position_y, uint16 display_num)
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    Integer_Number_Change_String(display_num, str);
    // M600_Deal_Picture(Command_Cut_Picture,pic_num,position_x,position_y,position_x+100,position_y+30,position_x,position_y);
    Dwin_Text_Display(0X55, position_x, position_y, str);

}
/**********************************************/
/* 函数名称: M600_Text_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*           试验用非正式                     */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			        */
/*           C_Mode--字符显示模式和编码方法   */
/*			 C_dots--显示点阵大小 			          */
/*			 Color--字符的前景色			            */
/*			 Bcolor--字符的背景色			            */
/*			 srt--要发送的字符串的指针		        */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  M600_Text_0X64_Display(uint8 pic_num, uint16 position_x, uint16 position_y, uint16 display_num)
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    Integer_Number_Change_String(display_num, str);
    // M600_Deal_Picture(Command_Cut_Picture,pic_num,position_x,position_y,position_x+100,position_y+30,position_x,position_y);
    Dwin_Text_Display(0X6f, position_x, position_y, str);

}
/**********************************************/
/* 函数名称: M600_Text_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*           试验用非正式                     */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			        */
/*           C_Mode--字符显示模式和编码方法   */
/*			 C_dots--显示点阵大小 			          */
/*			 Color--字符的前景色			            */
/*			 Bcolor--字符的背景色			            */
/*			 srt--要发送的字符串的指针		        */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  M600_Time_set_Display(uint8 big, uint16 position_x, uint16 position_y, uint16 display_num)
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    Integer_Number_Change_String(display_num, str);
    M600_Deal_Picture(Command_Cut_Picture, 0x08, position_x, position_y, position_x + 80, position_y + 54, position_x, position_y);
    Dwin_Text_1_Display(position_x, position_y, 0x27, 0x80, 0x05, 0xffff, 0xef3d, str);

}
/**********************************************/
/* 函数名称: M600_Set_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*           试验用非正式                     */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			        */
/*           C_Mode--字符显示模式和编码方法   */
/*			 C_dots--显示点阵大小 			          */
/*			 Color--字符的前景色			            */
/*			 Bcolor--字符的背景色			            */
/*			 srt--要发送的字符串的指针		        */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  M600_Set_Display(uint8 Pic_num, uint16 Color, uint16 position_x, uint16 position_y, uint16 display_num)
{
    uint8 str[6] = {0, 0, 0, 0, 0, '\0'};
    Integer_Number_Change_String(display_num, str);
    M600_Deal_Picture(Command_Cut_Picture, Pic_num, position_x, position_y, position_x + 123, position_y + 50, position_x, position_y);
    Dwin_Text_1_Display(position_x, position_y, 0x27, 0x80, 0x05, Color, 0xef3d, str);

}
/**********************************************/
/* 函数名称: M600_Set_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*           试验用非正式                     */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			        */
/*           C_Mode--字符显示模式和编码方法   */
/*			 C_dots--显示点阵大小 			          */
/*			 Color--字符的前景色			            */
/*			 Bcolor--字符的背景色			            */
/*			 srt--要发送的字符串的指针		        */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  M600_Set_Text_Display(uint8 Pic_num, uint16 Color, uint16 position_x, uint16 position_y, uint8 state)
{
    M600_Set_Palette(Color, 0x000);
    M600_Set_background(381, 355);
    if(state == 1)
        Dwin_Text_Display(cmd_Text_0x55, position_x, position_y, "开");
    else
        Dwin_Text_Display(cmd_Text_0x55, position_x, position_y, "关");


    //  M600_Deal_Picture(Command_Cut_Picture,Pic_num,position_x,position_y,position_x+120,position_y+50,position_x,position_y);
    //  Dwin_Text_1_Display(position_x,position_y,0x27,0x80,0x05,Color,0xef3d,str);

}
/**********************************************/
/* 函数名称: M600_One_Point                   */
/* 功能描述: 画单独一个点函数                 */
/*           试验用非正式                     */
/* 输  　入: x、y--坐标                       */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_One_Point(uint16 x, uint16 y)
{
    Tx_Byte(0xAA);
    Tx_Byte(0x51);
    Tx_Word(x);
    Tx_Word(y);
    Tx_EOF();
}

/**********************************************/
/* 函数名称: M600_Line                        */
/* 功能描述: 画连接两点间的线段函数           */
/*           试验用非正式                     */
/* 输  　入: x_start、y_start--左上角坐标     */
/*           x_end、y_end--右下角坐标         */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Line(uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end)
{
    Tx_Byte(0xAA);
    Tx_Byte(0x56);
    Tx_Word(x_start);
    Tx_Word(y_start);
    Tx_Word(x_end);
    Tx_Word(y_end);
    Tx_EOF();
}

/**********************************************/
/* 函数名称: M600_Line                        */
/* 功能描述: 画连接两点间的线段函数           */
/*           试验用非正式                     */
/* 输  　入: x_start、y_start--左上角坐标     */
/*           x_end、y_end--右下角坐标         */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Dynamic_Line(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
    Tx_Byte(0xAA);
    Tx_Byte(0x56);
    Tx_Word(x0);
    Tx_Word(y0);
    Tx_Word(x1);
    Tx_Word(y1);
    Tx_Word(x2);
    Tx_Word(y2);
    Tx_EOF();
}

/**********************************************/
/* 函数名称: M600_Deal_Rectangle_Group        */
/* 功能描述: 处理矩形组函数                   */
/*           试验用非正式                     */
/* 输  　入: command--指令                    */
/*           *x_start、*y_start--左上角坐标   */
/*           *x_end、*y_end--右下角坐标       */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Deal_Rectangle_Group(uint8 command, uint16 *x_start, uint16 *y_start, uint16 *x_end, uint16 *y_end, uint8 num)
{
    uint32 i;
    Tx_Byte(0xAA);
    Tx_Byte(command);

    for(i = 0; i < num; i++)
    {
        Tx_Word(*(x_start + i));
        Tx_Word(*(y_start + i));
        Tx_Word(*(x_end + i));
        Tx_Word(*(y_end + i));
    }

    Tx_EOF();
}

/**********************************************/
/* 函数名称: M600_Deal_Rectangle              */
/* 功能描述: 处理矩形函数                     */
/*           试验用非正式                     */
/* 输  　入: command--指令                    */
/*           x_start、y_start--左上角坐标     */
/*           x_end、y_end--右下角坐标         */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Deal_Rectangle(uint8 command, uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end)
{
    Tx_Byte(0xAA);
    Tx_Byte(command);

    Tx_Word(x_start);
    Tx_Word(y_start);
    Tx_Word(x_end);
    Tx_Word(y_end);

    Tx_EOF();
}

/**********************************************/
/* 函数名称: M600_Deal_Picture                */
/* 功能描述: 处理图片函数                     */
/*           试验用非正式                     */
/* 输  　入: command--指令                    */
/*           Picnum--图片索引号				  */
/*           xs、ys--图片的左上角坐标         */
/*           xe、ye--图片的右下角坐标         */
/*			 x、y-- 屏幕的(x,y)位置			  */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Deal_Picture(uint8 command, int8 Picnum, int16 xs, int16 ys, int16 xe, int16 ye, int16 x, int16 y)
{

    Tx_Byte(0xAA);
    Tx_Byte(command);
    Tx_Byte(Picnum);
    if(command == Command_Cut_Picture)
    {
        Tx_Word(xs);
        Tx_Word(ys);
        Tx_Word(xe);
        Tx_Word(ye);
        Tx_Word(x);
        Tx_Word(y);
    }
    Tx_EOF();
}

/**********************************************/
/* 函数名称: M600_Config_TFT                  */
/* 功能描述: 显示器配置函数                   */
/*           试验用非正式                     */
/* 输  　入: TFT_ID--显示器ID                 */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void M600_Config_TFT(uint8 TFT_ID)
{
    Tx_Byte(0xAA);
    Tx_Byte(0xE0);
    Tx_Byte(0x55);
    Tx_Byte(0xAA);
    Tx_Byte(0x5A);
    Tx_Byte(0xA5);
    Tx_Byte(TFT_ID);
    Tx_Byte(0x07);
    Tx_Byte(0x00);
    Tx_EOF();
}

/**********************************************/
/* 函数名称: Integer_Number_Change_String     */
/* 功能描述: 整形数字转字符函数               */
/* 输  　入: Value--需转换数字                */
/*           *String_Data字符存储空间         */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/* 修改时间：08年12月10日                     */
/**********************************************/
void Integer_Number_Change_String(int32 Value, uint8 *String_Data)
{
    if(Value < -9999 || Value > 99999)
    {
        if(Value > 0)
            Value = 99999;
        else
            Value = -9999;
    }
    if(Value >= 0)
    {
        if(Value / 10000 != 0)
        {
            *String_Data = (Value / 10000) + '0';
            *(String_Data + 1) = ((Value % 10000) / 1000) + '0';
            *(String_Data + 2) = (((Value % 10000) % 1000) / 100) + '0';
            *(String_Data + 3) = (((Value % 10000) % 1000) % 100) / 10 + '0';
            *(String_Data + 4) = (((Value % 10000) % 1000) % 100) % 10 + '0';
        }
        else
        {
            *String_Data = ' ';
            if(Value / 1000 != 0)
            {
                *(String_Data + 1) = (Value / 1000) + '0';
                *(String_Data + 2) = ((Value % 1000) / 100) + '0';
                *(String_Data + 3) = ((Value % 1000) % 100) / 10 + '0';
                *(String_Data + 4) = ((Value % 1000) % 100) % 10 + '0';
            }
            else
            {
                *(String_Data + 4) = ' ';
                if(Value / 100 != 0)
                {
                    *(String_Data + 1) = (Value / 100) + '0';
                    *(String_Data + 2) = (Value % 100) / 10 + '0';
                    *(String_Data + 3) = (Value % 100) % 10 + '0';
                }
                else
                {
                    *(String_Data + 1) = ' ';
                    if(Value / 10 != 0)
                    {
                        *(String_Data + 2) = Value / 10 + '0';
                        *(String_Data + 3) = Value % 10 + '0';
                    }
                    else
                    {
                        *(String_Data + 2) = ' ';
                        *(String_Data + 3) = Value + '0';
                    }
                }
            }
        }
    }
    else
    {
        if((0 - Value) / 1000 != 0)
        {
            *String_Data = '-';
            *(String_Data + 1) = ((0 - Value) / 1000) + '0';
            *(String_Data + 2) = (((0 - Value) % 1000) / 100) + '0';
            *(String_Data + 3) = (((0 - Value) % 1000) % 100) / 10 + '0';
            *(String_Data + 4) = (((0 - Value) % 1000) % 100) % 10 + '0';
        }
        else
        {
            *String_Data = ' ';
            if((0 - Value) / 100 != 0)
            {
                *(String_Data + 1) = '-';
                *(String_Data + 2) = ((0 - Value) / 100) + '0';
                *(String_Data + 3) = ((0 - Value) % 100) / 10 + '0';
                *(String_Data + 4) = ((0 - Value) % 100) % 10 + '0';
            }
            else
            {
                *(String_Data + 4) = ' ';
                if((0 - Value) / 10 != 0)
                {
                    *(String_Data + 1) = '-';
                    *(String_Data + 2) = (0 - Value) / 10 + '0';
                    *(String_Data + 3) = (0 - Value) % 10 + '0';
                }
                else
                {
                    *(String_Data + 1) = ' ';
                    *(String_Data + 2) = '-';
                    *(String_Data + 3) = (0 - Value) + '0';
                }
            }
        }
    }
}

/**********************************************/
/* 函数名称: Decimal_Number_Change_String     */
/* 功能描述: 一位小数数字转字符函数           */
/* 输  　入: Value--需转换数字                */
/*           *String_Data字符存储空间         */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/* 修改时间：08年12月10日                     */
/**********************************************/
void Decimal_Number_Change_String(int32 Value, uint8 *String_Data)
{
    if(Value < -999 || Value > 9999)
    {
        if(Value > 0)
            Value = 9999;
        else
            Value = -999;
    }
    if(Value >= 0)
    {
        if(Value / 1000 != 0)
        {
            *String_Data = (Value / 1000) + '0';
            *(String_Data + 1) = ((Value % 1000) / 100) + '0';
            *(String_Data + 2) = ((Value % 1000) % 100) / 10 + '0';
            *(String_Data + 3) = '.';
            *(String_Data + 4) = ((Value % 1000) % 100) % 10 + '0';
        }
        else
        {
            *String_Data = ' ';
            if(Value / 100 != 0)
            {
                *(String_Data + 1) = (Value / 100) + '0';
                *(String_Data + 2) = (Value % 100) / 10 + '0';
                *(String_Data + 3) = '.';
                *(String_Data + 4) = (Value % 100) % 10 + '0';
            }
            else
            {
                *(String_Data + 4) = ' ';
                *(String_Data + 1) = Value / 10 + '0';
                *(String_Data + 2) = '.';
                *(String_Data + 3) = Value % 10 + '0';
            }
        }
    }
    else
    {
        if((0 - Value) / 100 != 0)
        {
            *String_Data = '-';
            *(String_Data + 1) = ((0 - Value) / 100) + '0';
            *(String_Data + 2) = ((0 - Value) % 100) / 10 + '0';
            *(String_Data + 3) = '.';
            *(String_Data + 4) = ((0 - Value) % 100) % 10 + '0';
        }
        else
        {
            *String_Data = ' ';
            *(String_Data + 1) = '-';
            *(String_Data + 2) = (0 - Value) / 10 + '0';
            *(String_Data + 3) = '.';
            *(String_Data + 4) = (0 - Value) % 10 + '0';
        }
    }
}

/*************************************************/
/**                   End Of File               **/
/**                 显示驱动函数结束            **/
/*************************************************/
