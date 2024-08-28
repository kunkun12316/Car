#ifndef _BSP_H__
#define _BSP_H__

#include "main.h"
#include "core_cm4.h"
#include "stm32f4xx_hal.h"
void Delay_Init(uint8_t SysCLK);
void Delay_us(uint32_t nus);
void Delay_ms(uint32_t nms);

#endif