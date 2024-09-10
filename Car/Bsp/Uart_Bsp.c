#include "Bsp.h"

float Yaw; //陀螺仪角度值
float Last_Yaw;//陀螺仪上一次角度值

float Car_Turn_angle = 0;//当前小车的绝对角度值,只有当转向或回正使被调用

// DMA接收缓冲区
uint8_t JY_rx_buffer[22]; // 数据长度为11字节
float Angle_temp_x, Angle_temp_y, Angle_temp_z;
float Angle_speed_temp_x, Angle_speed_temp_y, Angle_speed_temp_z;

uint8_t JY_Rx_Flag = 0; // 接收标志位
uint8_t JY_Init_Flag = 1; //初始化JY901S，只有当初始化完成后才能使用

//暂存接收数据


void Usart_Init(void)
{
    JY901S_Init();
    //OpenMV_Init();
}


void JY901S_Init(void)
{
    User_USART_Init(&JY901_data);

    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart3, JY901_data.RxBuffer, RXBUFFER_LEN);

    //DMA
//    __HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);
//    if (HAL_UART_Receive_DMA(&huart3, JY_rx_buffer, 22) != HAL_OK)
//        //Error_Handler();

}
