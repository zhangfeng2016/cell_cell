#include "stm32f10x.h"
#include "../User/user_conf.h"
#include "Uart.h"
#include "../GUI/User_dispaly.h"
//#include "..\User\user_conf.h"


// TODO: MARK ADD GLOBAL DEFINE
uint16 g_nFifoRecvStart = 1;			//处理指针，等于要开始处理的位置
uint16 g_nFifoRecvEnd;			   //接收指针，等于下一个字节存放的位置
uint8  Uart3RecvBuf[256];		      //底层数据接收缓冲区
uint8  g_nFifoSendStart;			    //处理指针，等于要开始处理的位置
uint8  g_nFifoSendEnd;			      //发送指针，等于下一个字节发送的位置
uint8  UartSendBuf[20];		        //底层数据发送缓冲区
uint8  bUart0SendBusy;		  			//底层数据发送忙标志
uint8  CommRecvBufLen = 0;				  //通讯层数据接收缓冲区内容长度

//extern
uint8  CommRecvBuf[256];			//通讯层数据接收缓冲区
const uint8   COMM_TAIL[COMM_TAIL_LEN] = {0xCC, 0x33, 0xC3, 0x3C};
uint8 TI0;
uint8 WaitDataType;
uint8 iTmp;
uint8 SBUF0;
//////////////////////////////////////////////
uint8 UART1_RX[20];
uint8 uart1_rx_num = 0;

uint8 Uart2_state;

uint8  Uart2RecvBuf_Str[50];
uint8  Uart2RecvBuf_Str_cont;
uint16 Uart2_g_nFifoRecvStart = 0;			//处理指针，等于要开始处理的位置
uint16 Uart2_g_nFifoRecvEnd;			   //接收指针，等于下一个字节存放的位置
uint8  Uart2RecvBuf[256];		      //底层数据接收缓冲区
uint8  Uart2_g_nFifoSendStart;			    //处理指针，等于要开始处理的位置
uint8  Uart2_g_nFifoSendEnd;			      //发送指针，等于下一个字节发送的位置
uint8  Uart2_SendBuf[20];		        //底层数据发送缓冲区
uint8  Uart2_bUart0SendBusy;		  			//底层数据发送忙标志
uint8  Uart2_CommRecvBufLen = 0;				  //通讯层数据接收缓冲区内容长度
//uint8  Rec_head_flag;

//extern
uint8  Comm2_RecvBuf[256];			//通讯层数据接收缓冲区
const uint8   COMM2_TAIL[COMM_TAIL_LEN] = {0xCC, 0x33};
uint8 Uart2_WaitDataType;


uint16 CRC_CHK(uint8 *tx_data, uint8 length);
void USART0_Int(void)
{


}


/*******************************************************************************
* Function Name  : USART3_Configuration
* Description    : 串口2初始化函数
* Input          : none
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void USART2_Configuration(void)					//lcd screen
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // USART_DeInit(USART3);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
    // RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);

    /*
    *  USART1_TX -> PA2 , USART1_RX ->	PA3
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    USART_Cmd(USART2, ENABLE);

}









/*******************************************************************************
* Function Name  : USART3_Configuration
* Description    : 串口3初始化函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void USART3_Configuration(void)					//
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // USART_DeInit(USART3);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
    // RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);

    /*
    *  USART1_TX -> PA9 , USART1_RX ->	PA10
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    USART_Cmd(USART3, ENABLE);

}
/*******************************************************************************
* Function Name  : USART1_Configuration
* Description    : 串口1初始化函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void USART1_Configuration(void)					//lcd screen
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // USART_DeInit(USART3);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);
    // RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);

    /*
    *  USART1_TX -> PA9 , USART1_RX ->	PA10
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD , ENABLE);
    //	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10  ;
    //  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    //  GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_2;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    USART_Cmd(USART1, ENABLE);

}
/*******************************************************************************
* Function Name  : USART3Configuration
* Description    : 串口3初始化函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void USART3_Configuration_PD8_9(void)						 //rs485
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    //NVIC_InitTypeDef NVIC_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);


}
/*******************************************************************************
* Function Name  : Usart_int
* Description    : 初始化函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Usart_int(void)
{
    USART3_Configuration();
    USART1_Configuration();
    USART2_Configuration();
}
/*******************************************************************************
* Function Name  : USART1_TX_Byte
* Description    : 串口1数据发送函数
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void USART1_TX_Byte(uint8 tx_byte)//离心电机控制
{
    USART_SendData(USART1, tx_byte)  ;
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void USART3_TX_Byte(uint8 tx_byte)//迪文显示屏串口接收函数
{
    USART_SendData(USART3, tx_byte)  ;
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

void USART2_TX_Byte(uint8 tx_byte)//print函数
{
    USART_SendData(USART2, tx_byte)  ;
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}




void USART1_IRQHandler(void)
{
    uint16 crc_temp;
    uint8 crc_l;
    uint8 crc_h;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        //检测接收数据可用
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        UART1_RX[uart1_rx_num] = USART_ReceiveData(USART1);
        uart1_rx_num++;
        if(uart1_rx_num > 1 && UART1_RX[1] == 0x03)
        {
            if(uart1_rx_num == UART1_RX[2] + 5) //传输完毕开始校验
            {
                crc_temp = CRC_CHK (UART1_RX, UART1_RX[2] + 3);
                crc_l = crc_temp & 0xff;    //Tx_485_Buf[6]
                crc_h = (crc_temp >> 8) & 0xff; //Tx_485_Buf[7]
                if(crc_l == UART1_RX[UART1_RX[2] + 3] && crc_h == UART1_RX[UART1_RX[2] + 4])
                {
                    Bit_set(RS_485_flag, 5);
                }
            }
        }
        else
        {
            if(uart1_rx_num == 8) //传输完毕开始校验
            {
                crc_temp = CRC_CHK(UART1_RX, 6);
                crc_l = crc_temp & 0xff;    //Tx_485_Buf[6]
                crc_h = (crc_temp >> 8) & 0xff; //Tx_485_Buf[7]
                if(crc_l == UART1_RX[6] && crc_h == UART1_RX[7])
                {
                    Bit_set(RS_485_flag, 5);
                    uart1_rx_num = 0;
                }
                else
                {
                    Bit_clr(RS_485_flag, 5);
                    uart1_rx_num = 0;
                }
            }
        }
    }

}


void USART3_IRQHandler(void)
{
    uint8 t_bytmp;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        //检测接收数据可用
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        t_bytmp = USART_ReceiveData(USART3);
        //	UART3_TX(t_bytmp);
        if (g_nFifoRecvEnd + 1 != g_nFifoRecvStart)
        {
            Uart3RecvBuf[g_nFifoRecvEnd++] = t_bytmp; 	//保存接收到的数据
            if(g_nFifoRecvEnd == 255)
                g_nFifoRecvEnd = 0;
        }
    }
}
void USART2_IRQHandler(void)
{
    uint8 t_bytmp;
    //uint8 Rec_head_flag;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        //检测接收数据可用
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        t_bytmp = USART_ReceiveData(USART2);
        switch(Uart2_state)
        {
        case 0:

            if (Uart2_g_nFifoRecvEnd + 1 != Uart2_g_nFifoRecvStart)
            {
                Uart2RecvBuf [ Uart2_g_nFifoRecvEnd++] = t_bytmp; 	//保存接收到的数据
                if(Uart2_g_nFifoRecvEnd == 255)
                    Uart2_g_nFifoRecvEnd = 0;
            }
            break;
        case 1:
        case 2:
            Uart2RecvBuf_Str [Uart2RecvBuf_Str_cont] = t_bytmp;
            if(Uart2RecvBuf_Str_cont < 49)
                Uart2RecvBuf_Str_cont++;
            else
            {
                Uart2_state = 0;
                Uart2RecvBuf_Str_cont = 0;
            }
            break;
        default:
            break;
        }
    }
}
/**********************************************/
/* 函数名称: ComTestHead                 */
/* 功能描述: 串口3 验证头函数       */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
uint8 ComTestHead(void)
{
    if(COMM_HEAD != Uart3RecvBuf[g_nFifoRecvStart])
        return false;
    return true;
}
/**********************************************/
/* 函数名称: ComTestTail                 */
/* 功能描述: 串口3 验证尾巴函数           */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
uint8 ComTestTail(void)
{
    for(iTmp = 0; iTmp < COMM_TAIL_LEN ; iTmp++)
    {
        if(g_nFifoRecvStart + 1 + iTmp >= COMM_TAIL_LEN)
        {
            if(COMM_TAIL[iTmp] != Uart3RecvBuf[g_nFifoRecvStart + 1  - COMM_TAIL_LEN + iTmp])
                return false;
        }
        else
        {
            if(COMM_TAIL[iTmp] != Uart3RecvBuf[g_nFifoRecvStart + 1 + 255  - COMM_TAIL_LEN + iTmp])
                return false;
        }
    }
    return true;
}
/**********************************************/
/* 函数名称: ComReadCmd                       */
/* 功能描述: 串口3 读接收到的指令            */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
uint8 ComReadCmd(void)
{
    //没有命令
    if(g_nFifoRecvStart == g_nFifoRecvEnd)
    {
        return RECV_CMD_NODATA;
    }

    if ( WaitDataType == WAIT_DATA_TYPE_CMDHEAD)
    {
        if(ComTestHead())
        {
            g_nFifoRecvStart ++;
            if (g_nFifoRecvStart == 255)
                g_nFifoRecvStart = 0;
            WaitDataType = WAIT_DATA_TYPE_CONTENT;
            CommRecvBufLen = 0;
            return RECV_CMD_MOREDATA;
        }
        else
        {
            g_nFifoRecvStart ++;
            if (g_nFifoRecvStart == 255 )
                g_nFifoRecvStart = 0;
            WaitDataType = WAIT_DATA_TYPE_CMDHEAD;
            return RECV_CMD_ERRDATA;
        }
    }
    else
    {
        //	if ( WaitDataType == WAIT_DATA_TYPE_CONTENT){
        if(ComTestTail())
        {
            CommRecvBuf[CommRecvBufLen++] = Uart3RecvBuf[g_nFifoRecvStart++];
            if (g_nFifoRecvStart == 255)
                g_nFifoRecvStart = 0;

            WaitDataType = WAIT_DATA_TYPE_CMDHEAD;
            CommRecvBufLen = CommRecvBufLen - COMM_TAIL_LEN;
            return RECV_CMD_ENDDATA;
        }
        else
        {
            CommRecvBuf[CommRecvBufLen++] = Uart3RecvBuf[g_nFifoRecvStart++];
            if (g_nFifoRecvStart == 255)
            {
                g_nFifoRecvStart = 0;
                CommRecvBufLen = 0xff;
            }

            WaitDataType = WAIT_DATA_TYPE_CONTENT;
            return RECV_CMD_MOREDATA;
        }
    }
    return 	RECV_CMD_ERRDATA;
}

/**********************************************/
/* 函数名称: Com2TestHead                 */
/* 功能描述: 串口2 验证头函数       */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
uint8 Com2_TestHead(void)
{
    if(COMM_HEAD != Uart2RecvBuf[Uart2_g_nFifoRecvStart])
        return false;
    return true;
}
/**********************************************/
/* 函数名称: ComTestTail                 */
/* 功能描述: 串口3 验证尾巴函数           */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
uint8 Com2_TestTail(void)
{
    for(iTmp = 0; iTmp < COMM2_TAIL_LEN ; iTmp++)
    {
        if(Uart2_g_nFifoRecvStart + 1 + iTmp >= COMM2_TAIL_LEN)
        {
            if(COMM2_TAIL[iTmp] != Uart2RecvBuf [ Uart2_g_nFifoRecvStart + 1  - COMM2_TAIL_LEN + iTmp])
                return false;
        }
        else
        {
            if(COMM2_TAIL[iTmp] != Uart2RecvBuf [ Uart2_g_nFifoRecvStart + 1 + 255  - COMM2_TAIL_LEN + iTmp])
                return false;
        }
    }
    return true;
}
/**********************************************/
/* 函数名称: ComReadCmd                       */
/* 功能描述: 串口3 读接收到的指令            */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
uint8 Com2_ReadCmd(void)
{
    //没有命令
    if(Uart2_g_nFifoRecvStart == Uart2_g_nFifoRecvEnd)
    {
        return RECV_CMD_NODATA;
    }

    if ( Uart2_WaitDataType == WAIT_DATA_TYPE_CMDHEAD)
    {
        if(Com2_TestHead())
        {
            Uart2_g_nFifoRecvStart ++;
            if (Uart2_g_nFifoRecvStart == 255)
                Uart2_g_nFifoRecvStart = 0;
            Uart2_WaitDataType = WAIT_DATA_TYPE_CONTENT;
            Uart2_CommRecvBufLen = 0;
            return RECV_CMD_MOREDATA;
        }
        else
        {
            Uart2_g_nFifoRecvStart ++;
            if (Uart2_g_nFifoRecvStart == 255 )
                Uart2_g_nFifoRecvStart = 0;
            Uart2_WaitDataType = WAIT_DATA_TYPE_CMDHEAD;
            return RECV_CMD_ERRDATA;
        }
    }
    else
    {
        //	if ( WaitDataType == WAIT_DATA_TYPE_CONTENT){
        if(Com2_TestTail())
        {
            Comm2_RecvBuf[ Uart2_CommRecvBufLen ++] = Uart2RecvBuf[ Uart2_g_nFifoRecvStart ++];
            if (Uart2_g_nFifoRecvStart == 255)
                Uart2_g_nFifoRecvStart = 0;

            Uart2_WaitDataType = WAIT_DATA_TYPE_CMDHEAD;
            Uart2_CommRecvBufLen = Uart2_CommRecvBufLen - COMM2_TAIL_LEN;
            return RECV_CMD_ENDDATA;
        }
        else
        {
            Comm2_RecvBuf[ Uart2_CommRecvBufLen ++ ] = Uart2RecvBuf[Uart2_g_nFifoRecvStart++];
            if (Uart2_g_nFifoRecvStart == 255)
            {
                Uart2_g_nFifoRecvStart = 0;
                Uart2_CommRecvBufLen = 0xff;
            }

            Uart2_WaitDataType = WAIT_DATA_TYPE_CONTENT;
            return RECV_CMD_MOREDATA;
        }
    }
    return 	RECV_CMD_ERRDATA;
}

/**********************************************/
/* 函数名称: ComReadCmd                       */
/* 功能描述: 串口3 读接收到的指令            */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
void blue_teeth_rx(void)
{
    switch(Uart2_state) //Uart2RecvBuf_Str
    {
    case 0:
        if(Com2_ReadCmd() == RECV_CMD_ENDDATA)
        {
            switch(Comm2_RecvBuf[7])
            {
            case 0x50:
                Key_flag = 1;
                key_buf[1] = Comm2_RecvBuf[9];
                key_buf[2] = Comm2_RecvBuf[10];

                break;
            case 0x35:
                Set_Run_Volume = Comm2_RecvBuf[9] + Comm2_RecvBuf[10] * 256;
                break;
            case 0x34:
                Set_Wash_Volume = Comm2_RecvBuf[9] + Comm2_RecvBuf[10] * 256;
                break;
            case 0x33:
                Set_Empty_speed = Comm2_RecvBuf[9] + Comm2_RecvBuf[10] * 256;
                break;
            case 0x32:
                Set_Wash_speed = Comm2_RecvBuf[9] + Comm2_RecvBuf[10] * 256;
                break;
            case 0x31:
                Set_Fill_speed = Comm2_RecvBuf[9] + Comm2_RecvBuf[10] * 256;
                break;
            default:
                break;
            }
            //         if(Comm2_RecvBuf[6]==0x50)
            //		     {
            //			     Key_flag=1;
            //			     key_buf[1]=Comm2_RecvBuf[8];
            //	         key_buf[2]=Comm2_RecvBuf[9];
            //
            //		      }
            //         if(Comm2_RecvBuf[6]==0x35)
            //		     {
            //					 Set_Run_Volume=Comm2_RecvBuf[8]+Comm2_RecvBuf[9]*256;
            //
            //		      }
            if(Comm2_RecvBuf[1] == 0xFF)
            {
                USART2_TX_Byte(0x0A);
                USART2_TX_STRING("请输入SN!");
                USART2_TX_Byte(0x0A);
                Uart2RecvBuf_Str[0] = Uart2RecvBuf_Str[1] = Uart2RecvBuf_Str[2] = Uart2RecvBuf_Str[3] = Uart2RecvBuf_Str[4] = Uart2RecvBuf_Str[5] = 0;;
                Uart2RecvBuf_Str[6] = Uart2RecvBuf_Str[7] = Uart2RecvBuf_Str[8] = Uart2RecvBuf_Str[9] = Uart2RecvBuf_Str[10] = Uart2RecvBuf_Str[5] = 0;;
                Uart2RecvBuf_Str_cont = 0;
                Uart2_state = 1;

            }
            if(Comm2_RecvBuf[1] == 0xFE)
            {
                USART2_TX_Byte(0x0A);
                USART2_TX_STRING("请输入生产序号!");
                USART2_TX_Byte(0x0A);
                Uart2RecvBuf_Str[0] = Uart2RecvBuf_Str[1] = Uart2RecvBuf_Str[2] = Uart2RecvBuf_Str[3] = Uart2RecvBuf_Str[4] = Uart2RecvBuf_Str[5] = 0;;
                Uart2RecvBuf_Str[6] = Uart2RecvBuf_Str[7] = Uart2RecvBuf_Str[8] = Uart2RecvBuf_Str[9] = Uart2RecvBuf_Str[10] = Uart2RecvBuf_Str[5] = 0;;
                Uart2RecvBuf_Str_cont = 0;
                Uart2_state = 2;

            }
            break;
        case 1:
            if(Uart2RecvBuf_Str[0] == ';' && (Uart2RecvBuf_Str[10] == ';' || Uart2RecvBuf_Str[11] == ';'))
            {
                Series_No[0] = Uart2RecvBuf_Str[1];
                Series_No[1] = Uart2RecvBuf_Str[2];
                Series_No[2] = Uart2RecvBuf_Str[3];
                Series_No[3] = Uart2RecvBuf_Str[4];
                Series_No[4] = Uart2RecvBuf_Str[5];
                Series_No[5] = Uart2RecvBuf_Str[6];
                Series_No[6] = Uart2RecvBuf_Str[7];
                Series_No[7] = Uart2RecvBuf_Str[8];
                Series_No[8] = Uart2RecvBuf_Str[9];
                if(Uart2RecvBuf_Str[11] == ';')
                    Series_No[9] = Uart2RecvBuf_Str[10];
                else
                    Series_No[9] = 0;
                Uart2RecvBuf_Str[0] = Uart2RecvBuf_Str[1] = Uart2RecvBuf_Str[2] = Uart2RecvBuf_Str[3] = Uart2RecvBuf_Str[4] = Uart2RecvBuf_Str[5] = 0;;
                Uart2RecvBuf_Str[6] = Uart2RecvBuf_Str[7] = Uart2RecvBuf_Str[8] = Uart2RecvBuf_Str[9] = Uart2RecvBuf_Str[10] = Uart2RecvBuf_Str[11] = 0;;
                Uart2_state = 0;
                Bit_set(Save_to_EEP_flag, 8);
                USART2_TX_Byte(0x0A);
                USART2_TX_STRING("保存成功");
                USART2_TX_Byte(0x0A);

            }
            break;
        case 2:
            if(Uart2RecvBuf_Str[0] == ';' && Uart2RecvBuf_Str[5] == ';')
            {
                Fac_Series_No[0] = Uart2RecvBuf_Str[1];
                Fac_Series_No[1] = Uart2RecvBuf_Str[2];
                Fac_Series_No[2] = Uart2RecvBuf_Str[3];
                Fac_Series_No[3] = Uart2RecvBuf_Str[4];
                //					 Series_No[4]=Uart2RecvBuf_Str[5];
                //					 Series_No[5]=Uart2RecvBuf_Str[6];
                //					 Series_No[6]=Uart2RecvBuf_Str[7];
                //					 Series_No[7]=Uart2RecvBuf_Str[8];
                //					 Series_No[8]=Uart2RecvBuf_Str[9];
                Uart2RecvBuf_Str[0] = Uart2RecvBuf_Str[1] = Uart2RecvBuf_Str[2] = Uart2RecvBuf_Str[3] = Uart2RecvBuf_Str[4] = Uart2RecvBuf_Str[5] = 0;;
                Uart2RecvBuf_Str[6] = Uart2RecvBuf_Str[7] = Uart2RecvBuf_Str[8] = Uart2RecvBuf_Str[9] = Uart2RecvBuf_Str[10] = Uart2RecvBuf_Str[5] = 0;;
                Uart2_state = 0;
                Bit_set(Save_to_EEP_flag, 9);
                USART2_TX_Byte(0x0A);
                USART2_TX_STRING("保存成功");
                USART2_TX_Byte(0x0A);

            }
            break;
        }
    }
}
