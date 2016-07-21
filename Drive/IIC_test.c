
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "IIC.h"
#include "..\User\user_conf.h"

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	  8191
#define AT24C128	16383
#define AT24C256	32767
//Mini STM32������ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
#define EE_TYPE  AT24C256
//********************************************************************************
//V1.2�޸�˵��
//�������ж��е��ó�����ѭ���Ĵ���
//��ֹ��ʱ��׼ȷ,����do while�ṹ!
//////////////////////////////////////////////////////////////////////////////////
static u8  fac_us = 0; //us��ʱ������
static u16 fac_ms = 0; //ms��ʱ������
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(u8 SYSCLK)
{
    //	SysTick->CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
    fac_us = SYSCLK / 8;
    fac_ms = (u16)fac_us * 1000;
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms; //ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL = 0x00;          //��ռ�����
    SysTick->CTRL = 0x01 ;        //��ʼ����
    do
    {
        temp = SysTick->CTRL;
    }
    while(temp & 0x01 && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
    SysTick->CTRL = 0x00;     //�رռ�����
    SysTick->VAL = 0X00;      //��ռ�����
}
//��ʱnus
//nusΪҪ��ʱ��us��.
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us; //ʱ�����
    SysTick->VAL = 0x00;      //��ռ�����
    SysTick->CTRL = 0x01 ;    //��ʼ����
    do
    {
        temp = SysTick->CTRL;
    }
    while(temp & 0x01 && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
    SysTick->CTRL = 0x00;     //�رռ�����
    SysTick->VAL = 0X00;      //��ռ�����
}

//��ʼ��IIC
void I2C_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ��
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    IIC_SCL = 1;
    IIC_SDA = 1;

}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
    SDA_OUT();     //sda�����
    delay_us(10);
    IIC_SDA = 1;
    delay_us(10);
    IIC_SCL = 1;
    delay_us(10);

    IIC_SDA = 0; //START:when CLK is high,DATA change form high to low
    delay_us(10);
    IIC_SCL = 0; //ǯסI2C���ߣ�׼�����ͻ��������
    delay_us(10);
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
    delay_us(10);
    SDA_OUT();//sda�����
    IIC_SCL = 0;
    delay_us(10);
    IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
    delay_us(10);
    IIC_SCL = 1;
    delay_us(10);
    IIC_SDA = 1; //����I2C���߽����ź�
    delay_us(10);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN();      //SDA����Ϊ����
    IIC_SDA = 1;
    delay_us(10);
    IIC_SCL = 1;
    delay_us(10);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL = 0; //ʱ�����0
    return 0;
}
//����ACKӦ��
void IIC_Ack(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 0;
    delay_us(2);
    IIC_SCL = 1;
    delay_us(2);
    IIC_SCL = 0;
}
//������ACKӦ��
void IIC_NAck(void)
{
    SDA_OUT();
    //	IIC_SCL=0;
    //	SDA_OUT();
    //	IIC_SDA=1;
    //	delay_us(2);
    //	IIC_SCL=1;
    //	delay_us(2);
    //	IIC_SCL=0;
    delay_us(10);
    IIC_SDA = 1;
    delay_us(10);
    IIC_SCL = 1;
    delay_us(10);
    IIC_SCL = 0;
    delay_us(10);
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL = 0; //����ʱ�ӿ�ʼ���ݴ���
    for(t = 0; t < 8; t++)
    {
        IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1;
        delay_us(10);   //��TEA5767��������ʱ���Ǳ����
        IIC_SCL = 1;
        delay_us(10);
        IIC_SCL = 0;
        delay_us(10);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();//SDA����Ϊ����
    for(i = 0; i < 8; i++ )
    {
        IIC_SCL = 0;
        delay_us(10);
        IIC_SCL = 1;
        receive <<= 1;
        if(READ_SDA)receive++;
        delay_us(10);
    }
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}



///////////////////////////
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
    u8 temp = 0;
    IIC_Start();
    if(EE_TYPE > AT24C16)
    {
        IIC_Send_Byte(0XA8);	   //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(ReadAddr >> 8); //���͸ߵ�ַ
        IIC_Wait_Ack();
    }
    else IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1)); //����������ַ0XA0,д����

    IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr % 256); //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(0XA9);           //�������ģʽ
    IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop();//����һ��ֹͣ����
    return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
    IIC_Start();
    if(EE_TYPE > AT24C16)
    {
        IIC_Send_Byte(0XA8);	    //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(WriteAddr >> 8); //���͸ߵ�ַ
    }
    else
    {
        IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1)); //����������ַ0XA0,д����
    }
    IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr % 256); //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite);     //�����ֽ�
    IIC_Wait_Ack();
    IIC_Stop();//����һ��ֹͣ����
    delay_ms(10);
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{
    u8 t;
    for(t = 0; t < Len; t++)
    {
        AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
    }
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len)
{
    u8 t;
    u32 temp = 0;
    for(t = 0; t < Len; t++)
    {
        temp <<= 8;
        temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1);
    }
    return temp;
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
    u8 temp;
    temp = AT24CXX_ReadOneByte(255); //����ÿ�ο�����дAT24CXX
    if(temp == 0X55)return 0;
    else//�ų���һ�γ�ʼ�������
    {
        AT24CXX_WriteOneByte(255, 0X55);
        temp = AT24CXX_ReadOneByte(255);
        if(temp == 0X55)return 0;
    }
    return 1;
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
    while(NumToRead)
    {
        *pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
    while(NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}

