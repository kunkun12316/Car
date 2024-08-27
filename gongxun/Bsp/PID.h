#ifndef __PID_H__
#define __PID_H__
#include "main.h"
typedef struct
{
    uint8_t Enable;
    float Target_value;  // 目标值
    float Current_value; // 当前值
    float Err;           // 目前误差
    float Last_Err;      // 上一次误差
    float Kp, Ki, Kd;    // 比例常数，积分常数，微分常数
    float Output;        // PID输出，通常作为控制执行器的变量
    float integral;      // 积分值
    float OutPut_Limit;
} PID_TypeDef;
void PID_Init(PID_TypeDef *PID);
float PID_Speed_Cal(float Current, PID_TypeDef *PID);
float PID_Position_Cal(float Current, PID_TypeDef *PID);
float PID_Xunji_Cal(float Current, PID_TypeDef *PID); // PID操作

void Set_PID(PID_TypeDef *PID, float Kp, float Ki, float Kd);
void Set_PID_Target(PID_TypeDef *PID, float Target);
float Get_PID_Target(PID_TypeDef *PID);
void PID_Turn(PID_TypeDef *PID, uint8_t STATE);

#endif
