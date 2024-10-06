#ifndef PI_H__
#define PI_H__

#include "Bsp.h"

uint8_t Char_To_Num(char Char_Temp);
extern uint8_t PI_Data0;
extern int16_t PI_Data1;
extern int16_t PI_Data2;
void PI_Data_Receive_Proc(void);
extern uint8_t uart4_rxbuff;
extern uint8_t Task_NUM;
void PI_Receive_Data(char com_data[]);
void PI_Init(void);
void PI_Set_Mode(uint8_t Mode);


#endif