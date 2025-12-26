
#include "bsp.h"

extern TIM_HandleTypeDef htim4;

// 初始化回调函数指针为NULL，避免野指针
void (*s_TIM_CallBack1)(void) = NULL;


void Timer_Init(void) {

    __HAL_TIM_ENABLE(&htim4);
}
/*
*********************************************************************************************************
*	函 数 名: bsp_StartHardTimer
*	功能说明: 使用TIM2-5做单次定时器使用, 定时时间到后执行回调函数。可以同时启动4个定时器通道，互不干扰。
*             定时精度正负1us （主要耗费在调用本函数的执行时间）
*			  TIM2和TIM5 是32位定时器。定时范围很大
*			  TIM3和TIM4 是16位定时器。
*	形    参: 
*             _uiTimeOut : 超时时间, 单位 1us. 对于16位定时器，最大 65.5ms; 对于32位定时器，最大 4294秒
*             _pCallBack : 定时时间到后，被执行的函数
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartHardTimer(uint32_t _uiTimeOut, void * _pCallBack)
{
    uint32_t cnt_now;
    uint32_t cnt_tar;
	TIM_TypeDef* TIMx = TIM_HARD;
	
    /* 延时没有验证精度:xx? */
    cnt_now = TIMx->CNT; 
    cnt_tar = cnt_now + _uiTimeOut;			/* 计算捕获的计数器值 */

    // 关闭全局中断，避免配置过程中被中断干扰（提升稳定性，可选）
    /*
    __disable_irq();
    .....
    __enable_irq();
    */
    
    s_TIM_CallBack1 = (void (*)(void))_pCallBack;

    TIMx->CCR1 = cnt_tar; 			    /* 设置捕获比较计数器CC1 */
    TIMx->SR = (uint16_t)~TIM_IT_CC1;   /* 清除CC1中断标志 */
    TIMx->DIER |= TIM_IT_CC1;			/* 使能CC1中断 */
}

void TIM_HARD_IRQHandler(void)
{
	uint16_t itstatus = 0x0, itenable = 0x0;
	TIM_TypeDef* TIMx = TIM_HARD;
	
    
  	itstatus = TIMx->SR & TIM_IT_CC1;
	itenable = TIMx->DIER & TIM_IT_CC1;
    
	if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
	{
		TIMx->SR = (uint16_t)~TIM_IT_CC1;
		TIMx->DIER &= (uint16_t)~TIM_IT_CC1;		/* 禁能CC1中断 */	

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack1();
    }
}