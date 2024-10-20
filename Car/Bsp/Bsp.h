#ifndef __BSP_H__
#define __BSP_H__

#include "main.h"
#include "../Task/Motor.h"
#include "../Task/Control.h"
#include "../Bsp/My_Delay.h"
#include "../Task/Servo.h"
#include "../Task/Move.h"
#include "../Bsp/Uart_Bsp.h"
#include "../Bsp/JY901.h"
#include "PI.h"
#include "gm65.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"


#include "stdio.h"
#include "gpio.h"
#include "led.h"
#include "usart.h"
#include <math.h>
#include "tim.h"
#include <stdint.h>
#include "string.h"
#include "i2c.h"

#include "retarget.h"

extern uint8_t HuaGui_Init_State;
extern uint8_t Bsp_Flag;

void Key_Proc(void);
void HuaGui_Init_Proc(void);
void Bsp_Init(void);
void OLED_proc(void);

#endif