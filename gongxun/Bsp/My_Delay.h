#ifndef _BSP_H__
#define _BSP_H__

#include "main.h"

void PY_usDelayTest(void);
void PY_Delay_us_t(uint32_t Delay);
void PY_usDelayOptimize(void);
void PY_Delay_us(uint32_t Delay);
void PY_Delay_ms(uint32_t Delay);

void Delay_Init(uint8_t SysCLK);
void Delay_us(uint32_t nus);
void Delay_ms(uint32_t nms);

#endif