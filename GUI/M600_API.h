/*************** Radio File (h) ***************/
/**                                          **/
/**      基于STM32芯片开发的血液回收机       **/
/**                                          **/
/**                                          **/
/**--------------文件信息--------------------**/
/**文    件   名: M600_API.h                 **/
/**创    建   人: ZHANGFENG                  **/
/**创  建  日 期: AUG.1 2014                 **/
/**描         述: 函数声明                   **/
/**               函数声明                   **/
/**                                          **/
/**********************************************/

//#include "../User/config.h"
#include "../User/user_conf.h"
/**************   M600中一些常用的操作指令，它们都以cmd_为前缀    ******************************************/

#define cmd_Getcolor_Fogrd              0x22  //取某一点的前景色到调色板 
#define cmd_Getcolor_Bkgrd              0x23  //取某一点的背景色到调色板

#define cmd_Setdot_Off                  0x00  //光标关闭
#define cmd_Setdot_On                   0x01  //光标显示

#define cmd_Rect_Display                0x59 //显示矩形框
#define cmd_Rect_Delete                 0x69 //删除矩形区域
#define cmd_Rect_Clear                  0x5a //清除矩形区域
#define cmd_Rect_Fill                   0x5b //填充矩形区域
#define cmd_Rect_Reverse                0x5c //反色矩形区域

#define cmd_Text_0x53                  0x53 //显示8*8点阵
#define cmd_Text_0x54                   0x54 //显示16*16点阵
#define cmd_Text_0x55                   0x55 //显示32*32点阵
#define cmd_Text_0x6e                   0x6e //显示12*12点阵
#define cmd_Text_0x6f                  0x6f //显示24*24点阵

#define Command_Show_Picture            0x70 //显示保存在终端中的一幅全屏图片，PICNUM 为显示图片索引号，范围随显示驱动模式不同而不同。
#define Command_Cut_Picture             0x71 //将存储在 Flash 中的索引号为 PICNUM 的图片中的（Xs:Ys）为左上角坐标，（Xe:Ye）为右下角坐标的显示内容在当前屏幕的（X,Y）位置显示出来。
#define Command_Save_Picture            0x72 //将屏幕当前显示画面保存到 PICNUM 索引的指定空间， PICNUM 随显示驱动模式不同而不同。
#define Command_Cut_Clarity_Picture		  0x9D //剪贴透明背景

#define cmd_PointList_Delete            0x50 //删除指定点序列
#define cmd_PointList_Put               0x51 //置点指定点序列

#define cmd_TFT_ID_LQ104V               0x00
#define cmd_TFT_ID_PD104VT              0x01
#define cmd_TFT_ID_ET057007             0x06
#define cmd_TFT_ID_CLAA057              0x0A
/**********************************       End        *******************************************/
/**********************************************/
/* 函数名称: M600_Refresh_Screen              */
/* 功能描述: 发送强制刷新1次全屏显示函数      */
/*           试验用非正式                     */
/* 输  　入: data                             */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
extern void M600_Refresh_Screen(void);

/**********************************************/
/* 函数名称: M600_Refresh_Screen              */
/* 功能描述: 发送强制刷新1次全屏显示函数      */
/*           试验用非正式                     */
/* 输  　入: data                             */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
extern void M600_Refresh_Screen(void);

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
extern void M600_Set_Palette(uint16 display_color, uint16 background_color);

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
extern void M600_Set_Distance(uint16 D_X, uint16 D_Y);
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
void M600_Set_background(uint16 x, uint16 y);

/**********************************************/
/* 函数名称: M600_Text_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*           试验用非正式                     */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			  */
/*           C_Mode--字符显示模式和编码方法   */
/*			 C_dots--显示点阵大小 			  */
/*			 Color--字符的前景色			  */
/*			 Bcolor--字符的背景色			  */
/*			 srt--要发送的字符串的指针		  */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
extern void M600_Text_Display(uint8 big, uint16 position_x, uint16 position_y, uint16 display_num);
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
extern void  M600_Set_Display(uint8 Pic_num, uint16 Color, uint16 position_x, uint16 position_y, uint16 display_num);
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
extern void  M600_Set_Text_Display(uint8 Pic_num, uint16 Color, uint16 position_x, uint16 position_y, uint8 state);
/**********************************************/
/* 函数名称: Dwin_Text_Display                */
/* 功能描述: 任意文本显示采用0x98指令函数     */
/*                                            */
/* 输  　入: position_x--字体显示的起始X坐标  */
/*           position_y--字体显示的起始Y坐标  */
/*           Lib_ID--字库选择    			  */
/*           C_Mode--字符显示模式和编码方法   */
/*			 C_dots--显示点阵大小 			  */
/*			 Color--字符的前景色			  */
/*			 Bcolor--字符的背景色			  */
/*			 srt--要发送的字符串的指针		  */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void  Dwin_Text_Display(uint8 command , uint16 position_x, uint16 position_y, uint8 *str);
extern void  Dwin_Text_1_Display(uint16 position_x, uint16 position_y, uint8 Lib_ID, uint8 C_Mode, uint8 C_dots, uint16 Color, uint16 Bcolor, uint8 *str);

/**********************************************/
/* 函数名称: M600_One_Point                   */
/* 功能描述: 画单独一个点函数                 */
/*           试验用非正式                     */
/* 输  　入: x、y--坐标                       */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
extern void M600_One_Point(uint16 x, uint16 y);

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
extern void M600_Line(uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end);

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
extern void M600_Dynamic_Line(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint16 x2, uint16 y2);

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
extern void M600_Deal_Rectangle_Group(uint8 command, uint16 *x_start, uint16 *y_start, uint16 *x_end, uint16 *y_end, uint8 num);

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
extern void M600_Deal_Rectangle(uint8 command, uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end);

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
extern void M600_Deal_Picture(uint8 command, int8 Picnum, int16 xs, int16 ys, int16 xe, int16 ye, int16 x, int16 y);

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
extern void Integer_Number_Change_String(int32 Value, uint8 *String_Data);

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
extern void Decimal_Number_Change_String(int32 Value, uint8 *String_Data);

/*************************************************/
/**                   End Of File               **/
/**                 显示驱动函数头文件结束      **/
/*************************************************/
