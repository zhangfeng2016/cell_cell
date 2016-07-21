

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





//IIC���в�������
void IIC_Configuration(void);        //��ʼ��IIC��IO��
void IIC_delay(void);            //IIC��ʱ
void IIC_Start(void);				       //����IIC��ʼ�ź�
void IIC_Stop(void);	  			   //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			 //IIC����һ���ֽ�
u8 IIC_Read_Byte(void);          //IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				   //IIC�ȴ�ACK�ź�
void IIC_Ack(void);					     //IIC����ACK�ź�
void IIC_NAck(void);				     //IIC������ACK�ź�




