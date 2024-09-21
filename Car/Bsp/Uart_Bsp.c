#include "Bsp.h"

float JY_Yaw; //陀螺仪角度值
float Last_JY_Yaw;//陀螺仪上一次角度值

float Car_Turn_angle = 0;//当前小车的绝对角度值,只有当转向或回正使被调用

// DMA接收缓冲区
uint8_t JY_rx_buffer[22]; // 数据长度为11字节
float Angle_temp_x, Angle_temp_y, Angle_temp_z;
float Angle_speed_temp_x, Angle_speed_temp_y, Angle_speed_temp_z;

uint8_t JY_Rx_Flag = 0; // 接收标志位
uint8_t JY_Init_Flag = 1; //初始化JY901S，只有当初始化完成后才能使用

//二维码数据
uint8_t QR_Data_1[3] = {0};
uint8_t QR_Data_2[3] = {0};

void Usart_Init(void) {
    JY901S_Init();
    //OpenMV_Init();
}


void JY901S_Init(void) {
    User_USART_Init(&JY901_data);

    Reset_JY901S_Command();

    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart3, JY901_data.RxBuffer, RXBUFFER_LEN);

    //DMA
//    __HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);
//    if (HAL_UART_Receive_DMA(&huart3, JY_rx_buffer, 22) != HAL_OK)
//        //Error_Handler();

}

//将二维码信息发送给树莓派
void Send_QR_Data_to_RB(void) {
    uint8_t Start_Byte[2] = {0x3B, 0xB3};  // 起始字节
    uint8_t End_Byte = 0x6B;               // 结束字节

    uint8_t Send_Data[9] = {0};  // 9字节数据帧
    Send_Data[0] = Start_Byte[0];  // 起始字节
    Send_Data[1] = Start_Byte[1];

    // 填入 QR_Data_1
    Send_Data[2] = QR_Data_1[0];
    Send_Data[3] = QR_Data_1[1];
    Send_Data[4] = QR_Data_1[2];

    // 填入 QR_Data_2
    Send_Data[5] = QR_Data_2[0];
    Send_Data[6] = QR_Data_2[1];
    Send_Data[7] = QR_Data_2[2];

    // 结束字节
    Send_Data[8] = End_Byte;

    // 通过 UART 发送数据
    HAL_UART_Transmit(&huart4, Send_Data, 9, HAL_MAX_DELAY);
}