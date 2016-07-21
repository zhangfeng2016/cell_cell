

//extern void I2C_Configuration(void);
//extern unsigned char I2C_Read(I2C_TypeDef *I2Cx,unsigned char I2C_Addr,unsigned char addr,unsigned char *buf,unsigned char num);

//extern unsigned char I2C_Write(I2C_TypeDef *I2Cx,unsigned char I2C_Addr,unsigned char addr,unsigned char *buf,unsigned char num);



#include "user_conf.h"

#define  FALSE   0
#define  TURE    1

#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6

#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}



#define SCL_read      GPIOB->IDR  & GPIO_Pin_6
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7





//IIC所有操作函数
void IIC_Configuration(void);        //初始化IIC的IO口
void IIC_delay(void);            //IIC延时
void IIC_Start(void);				       //发送IIC初始信号
void IIC_Stop(void);	  			   //发送IIC停止信号
void IIC_Send_Byte(u8 txd);			 //IIC发送一个字节
u8 IIC_Read_Byte(void);          //IIC读取一个字节
u8 IIC_Wait_Ack(void); 				   //IIC等待ACK信号
void IIC_Ack(void);					     //IIC发送ACK信号
void IIC_NAck(void);				     //IIC不发送ACK信号




