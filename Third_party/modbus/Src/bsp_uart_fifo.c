
#include "bsp.h"
#include "modbus_slave.h"
#include "stm32h7xx_hal_gpio.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
uint8_t USART2_RX_DATA[USART1_RXBUFFERSIZE];


/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化串口硬件，并对全局变量赋初值.
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	
	/* 必须先初始化全局变量,再配置硬件 */

	/* 配置串口的硬件参数(波特率等) */

	/* 配置RS485芯片的发送使能硬件，配置为推挽输出 */
}

/**
 * @description: 初始化RS485为接受模式
 * @return {*}
 */
void Rs485_Init(void) {

	RS485DIR_RX
    //使能串口接收中断
	HAL_UARTEx_ReceiveToIdle_IT(&huart2,USART2_RX_DATA,sizeof(USART2_RX_DATA));
}

/**
 * @description: RS485发送数据
 * @return {*}
 */
void Rs485_Send(uint8_t *puData, uint16_t usNum) {
	RS485DIR_TX	//将485芯片设置为发送模式

	//把收到的一包数据通过串口回传
	HAL_UART_Transmit(&huart2,puData,usNum,0xff);
	
	RS485DIR_RX	//将485芯片设置为接收模式
}

// extern MODS_ReciveNew(uint8_t *Data, uint16_t Size);
void Rs485_Recv(uint8_t *puData, uint16_t usNum) {
	MODS_ReciveNew(puData,  usNum);
}
 
//空闲中断回调函数，参数Size为串口实际接收到数据字节数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance==USART2)
	{  
		
		Rs485_Recv(USART2_RX_DATA, Size);
		//再次开启空闲中断接收，不然只会接收一次数据
		HAL_UARTEx_ReceiveToIdle_IT(&huart2,USART2_RX_DATA,sizeof(USART2_RX_DATA));
	}
}
 