
#ifndef _BSP_H_
#define _BSP_H_

#define STM32_H7

#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "bsp_msg.h"
#include "bsp_timer.h"
#include "bsp_uart_fifo.h"
#include "bsp_user_lib.h"


void bsp_Init(void);
void bsp_Idle(void);


#endif

