#ifndef __SERVO_H_
#define __SERVO_H_

#include "../Bsp/Bsp.h"

//舵机的最小和最大脉冲宽度
#define SERVO_MIN_PULSE 500
#define SERVO_MAX_PULSE 2500
//夹爪的打开和关闭角度
#define JiaZhua_Angle_Open 99   //45
#define JiaZhua_Angle_Close 160    //153
//滑轨舵机的内外角度
#define HuaGui_Angle_OUT 35 //初始36
#define HuaGui_Angle_IN 150
//载物舵机的左、中、右角度
#define ZaiWu_Angle_Left 20 //默认20
#define ZaiWu_Angle_Mid 80.5
#define ZaiWu_Angle_Right 138

typedef enum {
    SERVO_ZaiWu = 0,
    SERVO_HuaGui,
    SERVO_JiaZhua
} Servo_t;
typedef enum {
    ZaiWu_Mid = 0,
    ZaiWu_Left,
    ZaiWu_Right
} ZaiWu_State;
typedef enum {
    HuaGui_IN = 0,
    HuaGui_OUT
} HuaGui_Servo_States;
typedef enum {
    JiaZhua_Close = 0,
    JiaZhua_Open
} JiaZhua_State;


void Servo_Init(void);

void Servo_SetAngle(uint8_t servo, float angle);

uint8_t JiaZhua_Turn(JiaZhua_State state);

uint8_t HuaGui_Turn(HuaGui_Servo_States state);

uint8_t ZaiWu_Turn(ZaiWu_State state);

void Servo_CallBack(void);

extern float JiaZhua_Angle;
extern float HuaGui_Angle;
extern float ZaiWu_Angle;

#endif