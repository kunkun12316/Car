#include "JY901.h"
#include "string.h"
#include "usart.h"
#include "Bsp.h"

User_USART JY901_data;//角度值左+右-，180为分水岭
struct SAcc stcAcc;
struct SGyro stcGyro;
struct SAngle stcAngle;

KalmanFilter kf_x, kf_y, kf_z;

// 低通滤波器系数
float alpha = 0.05;

// 定义前一次滤波后的加速度值
float prev_filtered_acc_x = 0.0f;
float prev_filtered_acc_y = 0.0f;
float prev_filtered_acc_z = 0.0f;

float prev_filtered_ax = 0.0f;
float prev_filtered_ay = 0.0f;
float prev_filtered_az = 0.0f;

//接收结构体初始化
void User_USART_Init(User_USART *Data) {
    for (uint16_t i = 0; i < RXBUFFER_LEN; i++) Data->RxBuffer[i] = 0;
    Data->frame_head = 0x55;
    Data->Rx_flag = 0;
    Data->Rx_len = 0;

//    Kalman_Init(&kf_x, 0.0001, 0.5, 0.1, 0); // 初始化 X 轴滤波器
//    Kalman_Init(&kf_y, 0.0001, 0.5, 0.1, 0); // 初始化 Y 轴滤波器
//    Kalman_Init(&kf_z, 0.0001, 0.5, 0.1, 9.8); // 初始化 Z 轴滤波器
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
}

void Kalman_Init(KalmanFilter *kf, float process_noise, float measurement_noise, float estimated_error, float initial_value) {
    kf->Q = process_noise;     // 过程噪声
    kf->R = measurement_noise; // 测量噪声
    kf->P = estimated_error;   // 初始协方差
    kf->x = initial_value;     // 初始估计
}

float Kalman_Update(KalmanFilter *kf, float measurement) {
    // 预测阶段
    kf->P = kf->P + kf->Q;

    // 计算卡尔曼增益
    kf->K = kf->P / (kf->P + kf->R);

    // 更新估计值
    kf->x = kf->x + kf->K * (measurement - kf->x);

    // 更新协方差
    kf->P = (1 - kf->K) * kf->P;

    return kf->x;
}

float my_abs_float(float value) {
    if (value < 0) {
        return -value;
    } else {
        return value;
    }
}

