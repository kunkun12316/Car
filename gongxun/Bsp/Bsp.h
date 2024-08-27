#ifndef __BSP_H_
#define __BSP_H_

#include "stdio.h"

#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "stm32f4xx_it.h"
#include "string.h"
#include <math.h>

#include "retarget.h"
#include "key.h"
#include "led.h"
#include "My_Delay.h"
#include "Uart_Bsp.h"
#include "OLED.h"
#include "LCD.h"
#include "PID.h"

#include "Motor.h"
#include "Servo.h"
#include "Move.h"
#include "Control.h"
#include "Control_Logic.h"

extern uint8_t HuaGui_Init_State;
extern uint8_t Motor_State;
extern uint8_t Running_Mode;

void Bsp_Init(void);
void HuaGui_Init_Proc(void);
void Key_Proc(void);
void OLED_Proc(void);

#endif