#ifndef __UART_BSP_H__
#define __UART_BSP_H__

#include "Bsp.h"

extern float Yaw;
extern float Last_Yaw;

extern uint8_t JY_rx_buffer[22]; // ���ݳ���Ϊ22λ
extern float Angle_temp_x, Angle_temp_y, Angle_temp_z;
extern float Angle_speed_temp_x, Angle_speed_temp_y, Angle_speed_temp_z;

// ���ձ�־λ
extern uint8_t JY_Init_Flag;
extern uint8_t JY_Rx_Flag;

//�ݴ��������
extern uint8_t JY_Rxdata;

extern void Usart_Init(void);

void JY901S_Init(void);
void IMU_Proc(void);

#endif