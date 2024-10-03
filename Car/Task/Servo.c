#include "../Bsp/Bsp.h"
#include "Servo.h"

void Servo_Init(void) {
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    Servo_SetAngle(SERVO_ZaiWu, ZaiWu_Angle_Mid);
    Servo_SetAngle(SERVO_HuaGui, HuaGui_Angle_IN);
    Servo_SetAngle(SERVO_JiaZhua, JiaZhua_Angle_Open);
}

void Servo_SetAngle(uint8_t servo, float angle) {
    float pulse = 0;
    switch (servo) {
        case SERVO_ZaiWu:
            if (angle >= ZaiWu_Angle_Right)
                angle = ZaiWu_Angle_Right;
            else if (angle <= ZaiWu_Angle_Left)
                angle = ZaiWu_Angle_Left;
            pulse = (float) (SERVO_MIN_PULSE + (SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle / 180);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse);
            break;
        case SERVO_HuaGui:
            if (angle >= HuaGui_Angle_IN)
                angle = HuaGui_Angle_IN;
            else if (angle <= HuaGui_Angle_OUT)
                angle = HuaGui_Angle_OUT;

            pulse = (float) (SERVO_MIN_PULSE + (SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle / 180);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pulse);
            break;
        case SERVO_JiaZhua:
            if (angle >= JiaZhua_Angle_Close)
                angle = JiaZhua_Angle_Close;
            else if (angle <= JiaZhua_Angle_Open)
                angle = JiaZhua_Angle_Open;
            pulse = (float) (SERVO_MIN_PULSE + (SERVO_MAX_PULSE - SERVO_MIN_PULSE) * angle / 180);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulse);
            break;
        default:
            break;
    }
}

float JiaZhua_Angle = JiaZhua_Angle_Close;
int8_t JiaZhua_Servo_Flag = 0;
#define JiaZhua_Servo_Speed 1.3f;

float HuaGui_Angle = HuaGui_Angle_IN;
int8_t HuaGui_Servo_Flag = 0;
#define HuaGui_Servo_Speed 1.3f;

float ZaiWu_Angle = ZaiWu_Angle_Mid;
int8_t ZaiWu_Servo_Flag = 0;
#define ZaiWu_Servo_Speed 1.3f;

//根据控制标志变量调整三个舵机的角度，然后调用 Servo_SetAngle 设置舵机角度。
void Servo_CallBack(void) {
    if (JiaZhua_Servo_Flag == 1)
        JiaZhua_Angle += JiaZhua_Servo_Speed;
    if (JiaZhua_Servo_Flag == -1)
        JiaZhua_Angle -= JiaZhua_Servo_Speed;
    if (HuaGui_Servo_Flag == 1)
        HuaGui_Angle += HuaGui_Servo_Speed;
    if (HuaGui_Servo_Flag == -1)
        HuaGui_Angle -= HuaGui_Servo_Speed;
    if (ZaiWu_Servo_Flag == 1)
        ZaiWu_Angle += ZaiWu_Servo_Speed;
    if (ZaiWu_Servo_Flag == -1)
        ZaiWu_Angle -= ZaiWu_Servo_Speed;
    Servo_SetAngle(SERVO_ZaiWu, (uint16_t) ZaiWu_Angle);
    Servo_SetAngle(SERVO_HuaGui, (uint16_t) HuaGui_Angle);
    Servo_SetAngle(SERVO_JiaZhua, (uint16_t) JiaZhua_Angle);
}

uint8_t JiaZhua_Turn(JiaZhua_State state) {
    uint8_t ret = 0;
    if (state == JiaZhua_Open) {
        if (JiaZhua_Angle > JiaZhua_Angle_Open) {
            JiaZhua_Servo_Flag = -1;
        } else {
            JiaZhua_Servo_Flag = 0;
            JiaZhua_Angle = JiaZhua_Angle_Open;
            ret = 1;
        }
    } else if (state == JiaZhua_Close) {
        if (JiaZhua_Angle < JiaZhua_Angle_Close) {
            JiaZhua_Servo_Flag = 1;
        } else {
            JiaZhua_Servo_Flag = 0;
            JiaZhua_Angle = JiaZhua_Angle_Close;
            ret = 1;
        }
    }
    return ret;
}

uint8_t HuaGui_Turn(HuaGui_Servo_States state) {
    uint8_t ret = 0;
    if (state == HuaGui_OUT) {
        if (HuaGui_Angle > HuaGui_Angle_OUT) {
            HuaGui_Servo_Flag = -1;
        } else {
            HuaGui_Servo_Flag = 0;
            HuaGui_Angle = HuaGui_Angle_OUT;
            ret = 1;
        }
    } else if (state == HuaGui_IN && HuaGui_Motor_State == HuaGui_Motor_State_UP) {
        if (HuaGui_Angle < HuaGui_Angle_IN) {
            HuaGui_Servo_Flag = 1;
        } else {
            HuaGui_Servo_Flag = 0;
            HuaGui_Angle = HuaGui_Angle_IN;
            ret = 1;
        }
    }
    return ret;
}

uint8_t ZaiWu_Turn(ZaiWu_State state) {
    uint8_t ret = 0;
    if (state == ZaiWu_Left && HuaGui_Motor_State != HuaGui_Motor_State_Qu) {
        if (ZaiWu_Angle > ZaiWu_Angle_Left) {
            ZaiWu_Servo_Flag = -1;
        } else {
            ZaiWu_Servo_Flag = 0;
            ZaiWu_Angle = ZaiWu_Angle_Left;
            ret = 1;
        }
    } else if (state == ZaiWu_Right && HuaGui_Motor_State != HuaGui_Motor_State_Qu) {
        if (ZaiWu_Angle < ZaiWu_Angle_Right) {
            ZaiWu_Servo_Flag = 1;
        } else {
            ZaiWu_Servo_Flag = 0;
            ZaiWu_Angle = ZaiWu_Angle_Right;
            ret = 1;
        }
    } else if (state == ZaiWu_Mid) {
        if (ZaiWu_Angle < ZaiWu_Angle_Mid - 6) {
            ZaiWu_Servo_Flag = 1;
        } else if (ZaiWu_Angle > ZaiWu_Angle_Mid + 6) {
            ZaiWu_Servo_Flag = -1;
        } else {
            ZaiWu_Servo_Flag = 0;
            ZaiWu_Angle = ZaiWu_Angle_Mid;
            ret = 1;
        }
    }
    return ret;
}
