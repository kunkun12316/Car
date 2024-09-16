#ifndef __MOVE_H__
#define __MOVE_H__

#include "../Bsp/Bsp.h"

//定义了载物操作的各个状态
typedef enum {
    Stop_State = 0,
    Get_JiaoZhun_State,
    Ready_State,
    Get_State1,
    Get_State1_5,
    Get_State2,
    Get_State3,
    Get_State4,
    Get_State5,
    Get_State6,
    Get_State7,
    Get_State8,
    End_State,

} Get_Loads_States;

//定义了卸物操作的各个状态
typedef enum {
    Put_State,
    Put_JiaoZhun_State,
    Put_Ready_State,
    Put_State1,
    Put_State2,
    Put_State3,
    Put_State4,
    Put_State5,
    Put_State5_5,
    Put_State6,
    Put_State7,
    Put_End_State,

} Put_Loads_States;

extern uint8_t Motor_Stop_Flag_Car_Kalman;

void Move_Init(void);

uint8_t Get_Loads(uint8_t Dir, uint8_t From, uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc);
uint8_t Put_Loads(uint8_t Dir, uint8_t Tar, uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc);
extern double Yaw_Compensate;

extern uint16_t Car_Task_Time;
extern uint8_t Car_Counter_Enable;
extern uint16_t Car_Counter_Times;

#endif
