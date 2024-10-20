#ifndef __UART_BSP_H__
#define __UART_BSP_H__

#include "Bsp.h"

extern float JY_Yaw;
extern float Last_JY_Yaw;

extern uint8_t JY_rx_buffer[22]; // 数据长度为22位
extern float Angle_temp_x, Angle_temp_y, Angle_temp_z;
extern float Angle_speed_temp_x, Angle_speed_temp_y, Angle_speed_temp_z;

// 接收标志位
extern uint8_t JY_Init_Flag;
extern uint8_t JY_Rx_Flag;

//暂存接收数据
extern uint8_t JY_Rxdata;

//二维码数据
extern uint8_t QR_Data_1[3];
extern uint8_t QR_Data_2[3];

//OLED显示数据
extern uint8_t task_num_show_0;
extern uint8_t task_num_show_1;
extern uint8_t task_num_show_2;

extern void Usart_Init(void);
uint8_t Send_QR_Data_to_RB(void);
uint8_t Send_Task_ID_To_RB(uint8_t task_id);

void JY901S_Init(void);
void IMU_Proc(void);

#endif