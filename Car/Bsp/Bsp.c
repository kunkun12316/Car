#include "Bsp.h"

void Bsp_Init(void)
{
    Delay_Init(168);
    Motor_Init();
    HAL_TIM_Base_Start_IT(&htim6);

    JY901S_Init();

    Delay_ms(1000);
}