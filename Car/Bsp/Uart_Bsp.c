#include "Bsp.h"


extern JY_USART JY901_data;

float Yaw;
float Last_Yaw;

uint8_t JY_Start_Flag = 1;

void Usart_Init(void)
{
    JY901S_Init();
    //OpenMV_Init();
}


JY_USART JY901_data;

struct SAcc stcAcc;
struct SGyro stcGyro;
struct SAngle stcAngle;

void JY901S_Init(void)
{
    JY901S_USART_Init(&JY901_data);

    __HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);

    if (HAL_UART_Receive_DMA(&huart3, JY901_data.RxBuffer, RXBUFFER_LEN_JY) != HAL_OK)
    {
        //Error_Handler();
    }

}

//接收结构体初始化
void JY901S_USART_Init(JY_USART *Data) {
    for (uint16_t i = 0; i < RXBUFFER_LEN_JY; i++) Data->RxBuffer[i] = 0;
    Data->frame_head = 0x55;
    Data->Rx_flag = 0;
    Data->Rx_len = 0;
}

void JY901_Process() {
    if (JY901_data.Rx_len < RXBUFFER_LEN_JY) return;    //如果位数不对

    JY901_data.Rx_flag = 1;

    for (uint8_t i = 0; i < 4; i++) {
        if (JY901_data.RxBuffer[i * 11] != JY901_data.frame_head) return;    //如果帧头不对
        switch (JY901_data.RxBuffer[i * 11 + 1]) {
            case 0x51:
                memcpy(&stcAcc, &JY901_data.RxBuffer[2 + i * 11], 8);
                for (uint8_t j = 0; j < 3; j++)
                    JY901_data.acc.a[j] = (float) stcAcc.a[j] / 32768 * 16;             //官方加速度解算
                break;
            case 0x52:
                memcpy(&stcGyro, &JY901_data.RxBuffer[2 + i * 11], 8);
                for (uint8_t j = 0; j < 3; j++)
                    JY901_data.w.w[j] = (float) stcGyro.w[j] / 32768 * 2000;                //官方角速度解算
                break;
            case 0x53:
                memcpy(&stcAngle, &JY901_data.RxBuffer[2 + i * 11], 8);
                for (uint8_t j = 0; j < 3; j++)
                    JY901_data.angle.angle[j] = (float) stcAngle.Angle[j] / 32768 * 180;        //官方角度解算
                break;
        }
    }
}

void IMU_Proc(void)
{
    if (JY901_data.Rx_flag == 1)
    {
        static float JY_Start_Buf[5] = {0};
        static uint8_t JY_Start_num;
        if(JY_Start_Flag == 1){
            JY_Start_Buf[JY_Start_num] = JY901_data.angle.angle[2];
            JY_Start_num++;
            if (JY_Start_num == 5){
                float num = 0;
                for (int i = 0; i < 5; ++i) {
                    num += JY_Start_Buf[i];
                }
                Last_Yaw = num / 5.0;
                printf("Car_Last_Turn_angle finish! %.2f\r\n",Last_Yaw);
                JY_Start_Flag = 0;
                JY_Start_num = 0;
            }
        } else{
            Yaw = JY901_data.angle.angle[2];
            if (Yaw - Car_Turn_angle > 90)
            {
                Car_Turn_angle += 90;
                printf("Angle + 90!\r\n");
            }
            else if (Yaw - Car_Turn_angle < -90)
            {
                Car_Turn_angle -= 90;
                printf("Angle - 90!\r\n");
            }
            Last_Yaw = Yaw;
            Yaw += Yaw_Compensate;
            JY901_data.Rx_flag = 0;
        }

    }
}