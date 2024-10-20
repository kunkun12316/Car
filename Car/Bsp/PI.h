#ifndef PI_H__
#define PI_H__

#include "Bsp.h"

extern uint8_t RxFlag;

//OP 环形缓冲区
#define OP_BUFFER_SIZE 128  // 缓冲区大小

extern volatile uint8_t uart_buffer[OP_BUFFER_SIZE];  // 环形缓冲区
extern volatile uint8_t write_pos;  // 缓冲区写入位置
extern volatile uint8_t read_pos;   // 缓冲区读取位置

uint8_t Char_To_Num(char Char_Temp);
extern uint8_t PI_Data0;
extern int16_t PI_Data1;
extern int16_t PI_Data2;
void PI_Data_Receive_Proc(void);
extern uint8_t uart4_rxbuff;
extern uint8_t Task_NUM;
//void PI_Receive_Data(char com_data[]);
void PI_Receive_Data(uint8_t com_data);
void PI_Init(void);
void PI_Set_Mode(uint8_t Mode);
void Process_OP_Data(void);


#endif