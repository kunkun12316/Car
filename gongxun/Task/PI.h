#ifndef __PI_H__
#define __PI_H__
#include "OLED.h"
#include "main.h"
#include "usart.h"
uint8_t Char_To_Num(uint8_t Char_Temp);
extern uint8_t PI_Data0;
extern int16_t PI_Data1;
extern int16_t PI_Data2;
void PI_Data_Receive_Proc(void);
extern uint8_t uart4_rxbuff;
extern uint8_t Task_NUM;
void PI_Receive_Data(uint8_t com_data);
void PI_Init(void);
void PI_Set_Mode(uint8_t Mode);

#endif
