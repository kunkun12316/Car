#include "Bsp.h"

float Yaw; //�����ǽǶ�ֵ
float Last_Yaw;//��������һ�νǶ�ֵ

float Car_Turn_angle = 0;//��ǰС���ľ��ԽǶ�ֵ,ֻ�е�ת������ʹ������

// DMA���ջ�����
uint8_t JY_rx_buffer[22]; // ���ݳ���Ϊ11�ֽ�
float Angle_temp_x, Angle_temp_y, Angle_temp_z;
float Angle_speed_temp_x, Angle_speed_temp_y, Angle_speed_temp_z;

uint8_t JY_Rx_Flag = 0; // ���ձ�־λ
uint8_t JY_Init_Flag = 1; //��ʼ��JY901S��ֻ�е���ʼ����ɺ����ʹ��

void Usart_Init(void)
{
    JY901S_Init();
    //OpenMV_Init();
}


void JY901S_Init(void)
{

    __HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);

    if (HAL_UART_Receive_DMA(&huart3, JY_rx_buffer, 22) != HAL_OK)
    {
        //Error_Handler();
    }

}
