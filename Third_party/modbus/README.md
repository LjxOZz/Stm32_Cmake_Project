# 目录结构

- Inc是头文件
- Src是源文件

# 文件介绍

- **bsp_timer**: 实现一个1us定时器,用于处理modbus每帧数据间隔时间
- **bsp_uart_fifo**: 实现了485的收发(空闲中断), 会调用bsp_modbus.h里的相关函数
- **bsp_timer**: 实现modbus从机的一些协议逻辑