

#include "stm32f10x.h"
#include "IIC.h"
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
/***********************************************/
/* ��������: IIC_Configuration                   */
/* ��������: IIC��ʼ��                     */
/* ��  ����: ��                                */
/* ��  ����: ��                                */
/* ȫ�ֱ���: ��                                */
/* ����ģ��:                                   */
/* �޸�ʱ�䣺15��02��9��                       */
/***********************************************/
void IIC_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7); 	     //PB6,PB7 �����

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;        //24c256ʹ��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}



//IIC��ʱ����
void IIC_delay(void)
{
    u8 i = 50;   //��ʱ10um
    while(i)
    {
        i--;
    }
}


//����IIC��ʼ�ź�
void  IIC_Start(void)
{
    SDA_H;
    delay_us(10);
    SCL_H;
    delay_us(10);
    //	if(!SDA_read)  return FALSE; //SDA��Ϊ�͵�ƽ������æ,�˳�
    SDA_L;
    delay_us(10);
    // if(SDA_read)   return FALSE; //SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
    SCL_L;
    delay_us(10);
}


//����IICֹͣ�ź�
void IIC_Stop(void)
{
    SCL_L;
    delay_us(10);
    SDA_L;//STOP:when CLK is high DATA change form low to high
    IIC_delay();
    SCL_H;
    delay_us(10);
    SDA_H;//����I2C���߽����ź�
    delay_us(10);
}



//����ACKӦ��
void IIC_Ack(void)
{
    SCL_L;
    SDA_L;
    delay_us(10);
    SCL_H;
    delay_us(10);
    SCL_L;

}


//������ACKӦ��
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



//�ȴ�Ӧ���źŵĵ���
//����ֵ:1,����Ӧ��ʧ��
//       0,����Ӧ��ɹ�
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





//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1,��Ӧ��
//0,��Ӧ��
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SCL_L;    //����ʱ�ӿ�ʼ���ݴ���
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



//��1���ֽ�,ack=1ʱ,����ACK,ack=0,����nACK
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


