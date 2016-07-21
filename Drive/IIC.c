

#include "stm32f10x.h"
#include "IIC.h"
//********************************************************************************
//V1.2修改说明
//修正了中断中调用出现死循环的错误
//防止延时不准确,采用do while结构!
//////////////////////////////////////////////////////////////////////////////////
static u8  fac_us = 0; //us延时倍乘数
static u16 fac_ms = 0; //ms延时倍乘数
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(u8 SYSCLK)
{
    //	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
    fac_us = SYSCLK / 8;
    fac_ms = (u16)fac_us * 1000;
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms; //时间加载(SysTick->LOAD为24bit)
    SysTick->VAL = 0x00;          //清空计数器
    SysTick->CTRL = 0x01 ;        //开始倒数
    do
    {
        temp = SysTick->CTRL;
    }
    while(temp & 0x01 && !(temp & (1 << 16))); //等待时间到达
    SysTick->CTRL = 0x00;     //关闭计数器
    SysTick->VAL = 0X00;      //清空计数器
}
//延时nus
//nus为要延时的us数.
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us; //时间加载
    SysTick->VAL = 0x00;      //清空计数器
    SysTick->CTRL = 0x01 ;    //开始倒数
    do
    {
        temp = SysTick->CTRL;
    }
    while(temp & 0x01 && !(temp & (1 << 16))); //等待时间到达
    SysTick->CTRL = 0x00;     //关闭计数器
    SysTick->VAL = 0X00;      //清空计数器
}
/***********************************************/
/* 函数名称: IIC_Configuration                   */
/* 功能描述: IIC初始化                     */
/* 输  　入: 无                                */
/* 输  　出: 无                                */
/* 全局变量: 无                                */
/* 调用模块:                                   */
/* 修改时间：15年02月9日                       */
/***********************************************/
void IIC_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7); 	     //PB6,PB7 输出高

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;        //24c256使能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}



//IIC延时函数
void IIC_delay(void)
{
    u8 i = 50;   //延时10um
    while(i)
    {
        i--;
    }
}


//产生IIC起始信号
void  IIC_Start(void)
{
    SDA_H;
    delay_us(10);
    SCL_H;
    delay_us(10);
    //	if(!SDA_read)  return FALSE; //SDA线为低电平则总线忙,退出
    SDA_L;
    delay_us(10);
    // if(SDA_read)   return FALSE; //SDA线为高电平则总线出错,退出
    SCL_L;
    delay_us(10);
}


//产生IIC停止信号
void IIC_Stop(void)
{
    SCL_L;
    delay_us(10);
    SDA_L;//STOP:when CLK is high DATA change form low to high
    IIC_delay();
    SCL_H;
    delay_us(10);
    SDA_H;//发送I2C总线结束信号
    delay_us(10);
}



//产生ACK应答
void IIC_Ack(void)
{
    SCL_L;
    SDA_L;
    delay_us(10);
    SCL_H;
    delay_us(10);
    SCL_L;

}


//不产生ACK应答
void IIC_NAck(void)
{
    SCL_L;
    SDA_H;
    delay_us(10);
    SCL_H;
    delay_us(10);
    SCL_L;
    delay_us(10);
}



//等待应答信号的到来
//返回值:1,接收应答失败
//       0,接收应答成功
u8 IIC_Wait_Ack(void)
{
    u16 timer = 0;
    SCL_L;
    delay_us(10);

    SDA_H;
    delay_us(10);
    SCL_H;
    delay_us(10);
    while(SDA_read)
    {
        timer++;
        if(timer > 50)
        {
            IIC_Stop();
            return 1;
        }
    }
    SCL_L;
    return 0;
}





//IIC发送一个字节
//返回从机有无应答
//1,有应答
//0,无应答
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SCL_L;    //拉低时钟开始数据传输
    delay_us(10);
    for(t = 0; t < 8; t++)
    {
        if(txd & 0x80)
            SDA_H;
        else
            SDA_L;
        txd <<= 1;
        delay_us(10);
        SCL_H;
        delay_us(10);
        SCL_L;
        delay_us(10);
    }


}



//读1个字节,ack=1时,发送ACK,ack=0,发送nACK
u8 IIC_Read_Byte(void)
{
    u8 i, receive = 0;
    SDA_H;
    for(i = 0; i < 8; i++ )
    {
        receive <<= 1;
        SCL_L;
        delay_us(2);
        SCL_H;
        delay_us(2);
        if(SDA_read)
        {
            receive++;
            delay_us(2);
        }
    }
    return receive;
}


