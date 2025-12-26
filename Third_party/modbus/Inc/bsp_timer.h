#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

//使用stm32cubemx初始化TIM4 CH1 
#define TIM_HARD					TIM4
#define	RCC_TIM_HARD_CLK_ENABLE()	__HAL_RCC_TIM4_CLK_ENABLE()
#define TIM_HARD_IRQn				TIM4_IRQn
#define TIM_HARD_IRQHandler			TIM4_IRQHandler     //使用stm32cubemx屏蔽中断函数的自动生成

// 全局回调函数指针（4个通道对应4个回调，初始化为NULL）
extern void (*s_TIM_CallBack1)(void);

// 函数声明
void Timer_Init(void);
void bsp_StartHardTimer(uint32_t _uiTimeOut, void * _pCallBack);

#endif
