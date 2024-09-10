#ifndef __UART_BSP_H__
#define __UART_BSP_H__

#include "Bsp.h"

extern float Yaw;
extern float Last_Yaw;

extern uint8_t JY_rx_buffer[22]; // 数据长度为22位
extern float Angle_temp_x, Angle_temp_y, Angle_temp_z;
extern float Angle_speed_temp_x, Angle_speed_temp_y, Angle_speed_temp_z;

// 接收标志位
extern uint8_t JY_Init_Flag;
extern uint8_t JY_Rx_Flag;

//暂存接收数据
extern uint8_t JY_Rxdata;

extern void Usart_Init(void);

void JY901S_Init(void);
void IMU_Proc(void);

#endif