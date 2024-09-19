#include "JY901.h"
#include "string.h"
#include "Bsp.h"

User_USART JY901_data;//角度值左+右-，180为分水岭
struct SAcc stcAcc;
struct SGyro stcGyro;
struct SAngle stcAngle;

uint8_t Kalman_Set_Data_Flag = 0;
float last_Kalman[3] = {0.0};
float kalman[20][3] = {0.0};
uint8_t Kalman_count = 0;

//接收结构体初始化
void User_USART_Init(User_USART *Data) {
    for (uint16_t i = 0; i < RXBUFFER_LEN; i++) Data->RxBuffer[i] = 0;
    Data->frame_head = 0x55;
    Data->Rx_flag = 0;
    Data->Rx_len = 0;

}

void JY901_Process() {
    if (JY901_data.Rx_len < RXBUFFER_LEN) return;    //如果位数不对

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

    JY_Yaw = JY901_data.angle.angle[2];
}

float my_abs_float(float value) {
    if (value < 0) {
        return -value;
    } else {
        return value;
    }
}

// 执行 Kalman 滤波
void process_kalman_filter(void) {
    if (Kalman_Set_Data_Flag == 1) {

        float Kalman_Acc[3] = {0.0};

        // 计算均值
        if (Kalman_count > 0) {  // 避免除以零
            for (int j = 0; j < 3; ++j) {
                for (int i = 0; i < Kalman_count; ++i) {
                    Kalman_Acc[j] += kalman[i][j];
                }
                Kalman_Acc[j] /= Kalman_count;  // 计算均值
            }

            // 判断加速度变化是否小于等于0.02
            if (my_abs_float(Kalman_Acc[0] - last_Kalman[0]) <= 0.02 &&
                my_abs_float(Kalman_Acc[1] - last_Kalman[1]) <= 0.02 &&
                my_abs_float(Kalman_Acc[2] - last_Kalman[2]) <= 0.02) {
                Motor_Stop_Flag_Car_Kalman = 1;  // 小车静止
//                printf("car kalman stop!\n");
            } else {
                Motor_Stop_Flag_Car_Kalman = 0;  // 小车移动
//                printf("car kalman running!\n");
            }

//                printf("Kalman: ax: %f, ay: %f, az: %f\n", my_abs_float(Acc[0] - last_Kalman[0]), my_abs_float(Acc[1] - last_Kalman[1]), my_abs_float(Acc[2] - last_Kalman[2]));

            // 更新最后的加速度均值
            for (int i = 0; i < 3; ++i) {
                last_Kalman[i] = Kalman_Acc[i];
            }

            // 重置计数器
            Kalman_count = 0;
            Kalman_time = 0;
        }
        Kalman_Set_Data_Flag = 0;
//        printf("Kalman flag = 0!\n");
    }
}


// 用于通过UART发送命令的函数
void JY901S_send_command(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5) {
    uint8_t command[5] = {byte1, byte2, byte3, byte4, byte5};
    HAL_UART_Transmit(&huart3, command, 5, HAL_MAX_DELAY);
}

void Reset_JY901S_Command(void)
{
    // 发送解锁指令
    JY901S_send_command(0xFF, 0xAA, 0x69, 0x88, 0xB5);
    Delay_ms(200);  // 等待解锁完成的延时

    // 发送校准指令
    JY901S_send_command(0xFF, 0xAA, 0x01, 0x08, 0x00);
    Delay_ms(200); // 等待校准完成的延时

    // 发送Z轴归0指令
    JY901S_send_command(0xFF, 0xAA, 0x01, 0x04, 0x00);
    Delay_ms(200); // 等待Z轴归0完成的延时

    // 发送保存配置指令
    JY901S_send_command(0xFF, 0xAA, 0x00, 0x00, 0x00);
    Delay_ms(200);  // 等待保存完成的延时
}