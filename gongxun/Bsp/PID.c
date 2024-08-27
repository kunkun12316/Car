#include "PID.h"
#include "OLED.h"
/**************************PID******************************/
// 位置式PID
//  初始化PID各项参数
void PID_Init(PID_TypeDef *PID)
{
    PID->Enable = 0;
    PID->Target_value = 0.0;
    PID->Current_value = 0.0;
    PID->Err = 0.0;
    PID->Last_Err = 0.0;
    PID->Output = 0.0;
    PID->integral = 0.0;
    PID->Kp = 0.0;
    PID->Ki = 0.0;
    PID->Kd = 0.0;
    PID->OutPut_Limit = 7200;
}
void Set_PID(PID_TypeDef *PID, float Kp, float Ki, float Kd)
{
    PID->Kp = Kp;
    PID->Ki = Ki;
    PID->Kd = Kd;
}
void Set_PID_Target(PID_TypeDef *PID, float Target)
{
    PID->Target_value = Target;
}
float Get_PID_Target(PID_TypeDef *PID)
{
    return PID->Target_value;
}
void PID_Turn(PID_TypeDef *PID, uint8_t STATE)
{
    if (STATE)
        PID->Enable = 1;
    else
        PID->Enable = 0;
}
/*
float PID_Operation(float Current, PID_TypeDef *PID)// PID操作
{
      if (PID->Enable == 1)
      {
            PID->Current_value = Current;
            PID->Err = PID->Target_value - PID->Current_value; // 计算偏差量

            if ((PID->Err >= -1) && (PID->Err <= 1)) // 设置闭环死区
                  PID->Err = 0;

            if ((PID->Err <= -1) || (PID->Err >= 1)) // 设置积分分离
                  PID->integral += PID->Err;

            if (PID->integral >= 1000) // 设置积分限幅
            {
                  PID->integral = 1000;
            }
            else if (PID->integral < -1000)
            {
                  PID->integral = -1000;
            }
            // PID运算
            PID->Output = PID->Kp * PID->Err + PID->Ki * PID->integral + PID->Kd * (PID->Err - PID->Last_Err);

            PID->Last_Err = PID->Err;     // 更新误差

            if (PID->Output > PID->OutPut_Limit) // 设置输出限幅
                  PID->Output = PID->OutPut_Limit;
            if (PID->Output < -PID->OutPut_Limit)
                  PID->Output = -PID->OutPut_Limit;

            return PID->Output; // 返回当前值
      }
      else
      {
            PID->Err = 0;
            PID->Last_Err = 0;
            PID->integral = 0;
            PID->Output = 0;
            return 0;
      }
}
*/
float PID_Speed_Cal(float Current, PID_TypeDef *PID)
{
    if (PID->Enable == 1)
    {
        PID->Current_value = Current;
        PID->Err = PID->Target_value - PID->Current_value; // 计算偏差量

        if ((PID->Err >= -1) && (PID->Err <= 1)) // 设置闭环死区
            PID->Err = 0;
        if ((PID->Err <= -1) || (PID->Err >= 1)) // 设置积分分离
            PID->integral += PID->Err;

        if (PID->integral >= 3000) // 设置积分限幅
        {
            PID->integral = 3000;
        }
        else if (PID->integral < -3000)
        {
            PID->integral = -3000;
        }
        // PID运算
        PID->Output = PID->Kp * PID->Err + PID->Ki * PID->integral;

        PID->Last_Err = PID->Err; // 更新误差

        if (PID->Output > PID->OutPut_Limit) // 设置输出限幅
            PID->Output = PID->OutPut_Limit;
        if (PID->Output < -PID->OutPut_Limit)
            PID->Output = -PID->OutPut_Limit;

        return PID->Output; // 返回当前值
    }
    else
    {
        PID->Err = 0;
        PID->Last_Err = 0;
        PID->Output = 0;
        return 0;
    }
}

float PID_Xunji_Cal(float Current, PID_TypeDef *PID) // PID操作
{
    if (PID->Enable == 1)
    {
        PID->Current_value = Current;
        PID->Err = PID->Current_value - PID->Target_value; // 计算偏差量

        if ((PID->Err >= -3) && (PID->Err <= 3)) // 设置闭环死区
            PID->Err = 0;

        // if ((PID->Err <= -1) || (PID->Err >= 1)) // 设置积分分离
        //       PID->integral += PID->Err;

        // if (PID->integral >= 1000) // 设置积分限幅
        // {
        //       PID->integral = 1000;
        // }
        // else if (PID->integral < -1000)
        // {
        //       PID->integral = -1000;
        // }
        // PID运算
        PID->Output = PID->Kp * PID->Err + PID->Ki * PID->integral + PID->Kd * (PID->Err - PID->Last_Err);

        PID->Last_Err = PID->Err; // 更新误差

        if (PID->Output > PID->OutPut_Limit) // 设置输出限幅
            PID->Output = PID->OutPut_Limit;
        if (PID->Output < -PID->OutPut_Limit)
            PID->Output = -PID->OutPut_Limit;

        return PID->Output; // 返回当前值
    }
    else
    {
        PID->Err = 0;
        PID->Last_Err = 0;
        PID->integral = 0;
        PID->Output = 0;
        return 0;
    }
}

// 位置环PID实现
float PID_Position_Cal(float Current, PID_TypeDef *PID)
{
    if (PID->Enable == 1)
    {
        PID->Current_value = Current;
        PID->Err = PID->Target_value - PID->Current_value; // 计算偏差量

        if ((PID->Err >= -10) && (PID->Err <= 10)) // 设置闭环死区
            PID->Err = 0;

        PID->Output = PID->Kp * PID->Err + PID->Kd * (PID->Err - PID->Last_Err);

        PID->Last_Err = PID->Err; // 更新误差

        if (PID->Output > PID->OutPut_Limit) // 设置输出限幅
            PID->Output = PID->OutPut_Limit;
        if (PID->Output < -PID->OutPut_Limit)
            PID->Output = -PID->OutPut_Limit;

        return PID->Output; // 返回当前值
    }
    else
    {
        PID->Err = 0;
        PID->Output = 0;
        return 0;
    }
}
