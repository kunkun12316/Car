#ifndef __BSP_H__
#define __BSP_H__

#include "main.h"
#include "../Task/Motor.h"
#include "../Task/Control.h"
#include "../Bsp/My_Delay.h"
#include "../Task/Control_Logic.h"
#include "../Task/Servo.h"
#include "../Task/Move.h"
#include "../Bsp/Uart_Bsp.h"
#include "../Bsp/JY901.h"

#include "stdio.h"
#include "gpio.h"
#include "led.h"
#include "usart.h"
#include <math.h>
#include "tim.h"
#include <stdint.h>
#include "string.h"

#include "retarget.h"

extern uint8_t HuaGui_Init_State;

void HuaGui_Init_Proc(void);
void Bsp_Init(void);

#endif