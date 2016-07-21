
#include "stm32f10x.h"
#include "../User/user_conf.h"






u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite);		//指定地址写入一个字节

void  I2C_WriteByte(u8 SendByte, u16 WriteAddress);
u8 I2C_ReadByte( u16 ReadAddress);

void AT24CXX_Writeword(u16 DataToWrite, u16 WriteAddr);
u16 AT24CXX_Readword(u16 ReadAddr);


void I2C_24CXX_Write(u16 Addr, u8 *pDat, u16 Len);
void I2C_24CXX_Read(u16 Addr, u8 *pDat, u16 Len);

void iic_test(void);



void AT24CXX_Init(void); //初始化IIC

/***********************************************/
/* 函数名称: Save_Poweron_time                */
/* 功能描述: 存储数据                         */
/* 输  　入: No                                */
/* 输  　出: No                                */
/* 全局变量: 无                                */
/* 调用模块: 无                                */
/* 修改时间：15年06月24日  //                  */
/***********************************************/
extern time_type Read_Poweron_time(uint8 tend_num);

extern      u8 decba[];
