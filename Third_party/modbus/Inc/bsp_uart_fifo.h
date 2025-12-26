
#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_


/*
	串口分配：
	【串口1】 
		PA9/USART1_TX	  --- 打印调试口
		P10/USART1_RX

	【串口2】  RS485 通信
		PD5 USART2_TX
		PD6 USART2_RX

	【串口3】 


*/

#define USART1_RXBUFFERSIZE 12	//每次接收1个数据进入一次中断
#define USART1_REC_LEN 200		//定义最大接收字节数 200

#define RS485DIR_TX HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);		//定义我们的控制引脚为发送状态
#define RS485DIR_RX HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);	//定义我们的控制引脚为接收状态

// 初始化
void Rs485_Init(void);
// 发送void
void Rs485_Send(uint8_t *puData, uint16_t usNum);

//接受
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

#endif

