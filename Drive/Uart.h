#include "../User/user_conf.h"

#define COMM_HEAD				0xAA
#define COMM_HEAD_LEN			1
#define COMM_CMD_LEN			1
#define COMM_TAIL_LEN			4

#define COMM2_TAIL_LEN			2

#define RECV_CMD_NODATA			0
#define RECV_CMD_ERRDATA		255
#define RECV_CMD_MOREDATA		1
#define RECV_CMD_ENDDATA		2

#define WAIT_DATA_TYPE_CMDHEAD	1
#define WAIT_DATA_TYPE_CONTENT	2

#define false  				        0
#define true  				        1
#define UART_SEND_BUSY				1
#define UART_SEND_FREE				0

//extern
extern uint8  CommRecvBuf[256];			//通讯层数据接收缓冲区
extern uint8 TI0;
extern uint8 WaitDataType;


extern uint8 UART1_RX[20];
extern uint8 uart1_rx_num;


extern void Usart_int(void);
extern void USART1_TX_Byte(uint8 tx_byte);//离心电机控制
extern void USART3_TX_Byte(uint8 tx_byte);//显示屏控制
/**********************************************/
/* 函数名称: UART3_Normal_Test_Check         */
/* 功能描述: 串口3 读接收到的指令            */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
extern void UART3_Normal_Test_Check(void);
/**********************************************/
/* 函数名称: Touch_Key_analysis_8200B         */
/* 功能描述: 串口3 读接收到的指令            */
/*           有需要可改变程序                 */
/* 输  　入: 无                               */
/* 输  　出: 无                               */
/* 全局变量: 无                               */
/* 调用模块: 无                               */
/**********************************************/
extern  void Touch_Key_analysis_8200B(void);



