
#include "stm32f10x.h"
extern void delay_init(uint8_t SYSCLK);
extern void I2C_Configuration(void);
extern void delay_ms(u16 nms);

u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len); //ָ����ַ��ʼд��ָ�����ȵ�����
u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC