#include "Bsp.h"

void Bsp_Init(void)
{
    Delay_Init(168);
    Motor_Init();
    HAL_TIM_Base_Start_IT(&htim6);

    Usart_Init();

    Delay_ms(1000);
}

uint8_t HuaGui_Init_State = 0;
void HuaGui_Init_Proc(void)
{
    if(HuaGui_Init_State == 1){
        printf("HuaGui Task");
        Motor_SetPosition(5,8000,-50,0);
        Motor_Run();
        HuaGui_Init_State = 2;
    } else if (HuaGui_Init_State == 2){
        Motor_Read_Current(5);
#if Serial_Debug == 1
        printf("Current_amount : %d\r\n", Motor_HuaGui_Current_amount); // 200
#endif
        if (Motor_HuaGui_Current_amount >= 520) {
            HuaGui_Init_State = 3;
            Motor_Reset(5);
            HuaGui_JiaoZhun();
            Servo_Init();
        }
    }else if (HuaGui_Init_State == 3) {
        if (HuaGui_Turn(HuaGui_IN) == 1) {
            HuaGui_Init_State = 0;
        }
    }
}