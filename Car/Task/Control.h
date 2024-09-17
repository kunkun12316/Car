#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "../Bsp/Bsp.h"

#define Car_Turn_Use_IMU 0
void Control_Proc(void);
extern uint8_t Task_Num_0;
extern int16_t Task_Num_1;
extern int16_t Task_Num_2;
extern uint8_t Running_Flag;

/*
extern uint8_t Task_Move_Flag[10];
extern uint8_t Task_Turn_Flag[10];
extern uint8_t Task_HuaGui_Flag[10];

extern uint8_t Task_Move_Data_1[10];
extern uint8_t Task_Turn_Data_1[10];
extern uint8_t Task_HuaGui_Data_1[10];

extern uint8_t Task_Move_Data_2[10];
extern uint8_t Task_Turn_Data_2[10];
extern uint8_t Task_HuaGui_Data_2[10];
 */


#endif
