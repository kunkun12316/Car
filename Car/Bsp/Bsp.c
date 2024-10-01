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
        printf("HuaGui Task 1 \n");
        Motor_SetPosition(5,4000,-50,0);
        Motor_Run();
        HuaGui_Init_State = 2;
    } else if (HuaGui_Init_State == 2){
        Motor_Read_Current(5);
#if Serial_Debug == 1
        printf("Current_amount : %d\r\n", Motor_HuaGui_Current_amount); // 200
#endif
        if (Motor_HuaGui_Current_amount >= 150) { //默认520
            printf("HuaGui Task 2\n");
            HuaGui_Init_State = 3;
            Motor_Reset(5);
            HuaGui_JiaoZhun();
            Servo_Init();
        }
    }else if (HuaGui_Init_State == 3) {
        printf("HuaGui Task 3!\n");
        if (HuaGui_Turn(HuaGui_IN) == 1 && JiaZhua_Turn(JiaZhua_Open) == 1) {
            HuaGui_Init_State = 0;
        }
    }
}

//按键相关配置
uint8_t Motor_State = 1;
extern uint8_t Running_Mode; // 运动模式
uint8_t temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0;
void Key_Proc(void)
{
    if(HAL_GPIO_ReadPin(KEY_1_GPIO_Port,KEY_1_Pin) == GPIO_PIN_RESET)
    {
        while(HAL_GPIO_ReadPin(KEY_1_GPIO_Port,KEY_1_Pin) == GPIO_PIN_RESET);
        Motor_State = !Motor_State;
        if(Motor_State == 1){
            Motor_Enable_All();
        } else {
            Motor_Disable_All();
        }
        temp1 = 1;
        temp2 = 0;
        temp3 = 0;
        temp4 = 0;
    }
    if (HAL_GPIO_ReadPin(KEY_2_GPIO_Port,KEY_2_Pin) == GPIO_PIN_RESET)
    {
        while (HAL_GPIO_ReadPin(KEY_2_GPIO_Port,KEY_2_Pin) == GPIO_PIN_RESET);
        temp1 = 0;
        temp2 = 1;
        temp3 = 0;
        temp4 = 0;
    }
    if (HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin) == GPIO_PIN_RESET) {
        while (HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin) == GPIO_PIN_RESET);
        // Temp_Val += 5;
        Running_Mode++;
        if (Running_Mode > 3)
            Running_Mode = 0;
        temp1 = 0;
        temp2 = 0;
        temp3 = 1;
        temp4 = 0;
    }
    if (HAL_GPIO_ReadPin(KEY_4_GPIO_Port, KEY_4_Pin) == GPIO_PIN_RESET) // 小车启动
    {
        while (HAL_GPIO_ReadPin(KEY_4_GPIO_Port, KEY_4_Pin) == GPIO_PIN_RESET);
        HAL_UART_Transmit(&huart4, "99", 2, 0xffff);
#if Serial_Debug == 1
        printf("GO!!!!!\r\n");
#endif

        temp1 = 0;
        temp2 = 0;
        temp3 = 0;
        temp4 = 1;
    }
}