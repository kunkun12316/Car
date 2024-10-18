#ifndef GM65_H__
#define GM65_H__

#include "Bsp.h"

extern uint8_t Gm65_RxFlag;
extern uint8_t data[19];
extern uint8_t gm65_buf[8];
extern uint8_t fssj[];
extern uint8_t GM65_Send_Data[9];
extern uint8_t hydata[];

void GM65_Init(void);
void UART_LCD(void);
void GM65_QR_Send(void);
uint8_t ASC_TO_INT(uint8_t ASC_Data);

#endif