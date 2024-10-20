#include "Bsp.h"

uint8_t Bsp_Flag = 0;

void Bsp_Init(void) {
    ssd1306_Init();

    Delay_Init(168);
    Motor_Init();
    HAL_TIM_Base_Start_IT(&htim6);

    Usart_Init();

    Delay_ms(500);
}

uint8_t HuaGui_Init_State = 0;

void HuaGui_Init_Proc(void) {
    if (HuaGui_Init_State == 0 && Bsp_Flag == 0){
        ssd1306_Fill(White);
        ssd1306_SetCursor(3, 20);
        ssd1306_WriteString("Init..... ", Font_16x24, Black);
        ssd1306_UpdateScreen(); // 确保调用此函数来更新显示
    }

    if (HuaGui_Init_State == 1) {
        Motor_SetPosition(5, 4500, -50, 0);
        Motor_Run();
        HuaGui_Init_State = 2;
    } else if (HuaGui_Init_State == 2) {
        Motor_Read_Current(5);
#if Serial_Debug == 1
//        printf("Current_amount : %d\r\n", Motor_HuaGui_Current_amount); // 200
#endif
        HuaGui_Counter_Enable = 1;
        if (Motor_HuaGui_Current_amount >= 50 && HuaGui_Counter > 200) { //默认520
            HuaGui_Counter_Enable = 0;
            HuaGui_Init_State = 3;
            Motor_Reset(5);
            Servo_Init();
        }
    } else if (HuaGui_Init_State == 3) {
        if (HuaGui_Turn(HuaGui_IN) == 1 && JiaZhua_Turn(JiaZhua_Open) == 1) {
            LED_2(1);
            LED_1(1);

            HuaGui_Init_State = 0;
            Running_Flag = 0;
            printf("HuaGui Init Over!\n");
            Bsp_Flag = 1;

            HAL_UART_Transmit(&huart4,"99",2,0xffff);
        }
    }
}

//按键相关配置
uint8_t Motor_State = 1;
extern uint8_t Running_Mode; // 运动模式
uint8_t temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0;

void Key_Proc(void) {
    if (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET) {
        while (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET);
        Motor_State = !Motor_State;
        if (Motor_State == 1) {
            Motor_Enable_All();
        } else {
            Motor_Disable_All();
        }
        temp1 = 1;
        temp2 = 0;
        temp3 = 0;
        temp4 = 0;
    }
    if (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == GPIO_PIN_RESET) {
        while (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == GPIO_PIN_RESET);
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
        HuaGui_Init_State = 1;
#if Serial_Debug == 1
        printf("GO!!!!!\r\n");
#endif

        temp1 = 0;
        temp2 = 0;
        temp3 = 0;
        temp4 = 1;
    }
}

char OLED_buffer[3]; // 用于存放转换后的字符串

void OLED_proc(void)
{
    ssd1306_Fill(White);

    ssd1306_SetCursor(3, 1);
    ssd1306_WriteString("Car : ", Font_11x18, Black);
    ssd1306_SetCursor(70, 1);
    snprintf(OLED_buffer, sizeof(OLED_buffer), "%d", Motor_Stop_Flag_Car);
    ssd1306_WriteString(OLED_buffer, Font_11x18, Black);


    ssd1306_SetCursor(3, 20);
    ssd1306_WriteString("HG  : ", Font_11x18, Black);
    ssd1306_SetCursor(70, 20);
    snprintf(OLED_buffer, sizeof(OLED_buffer), "%d", Stop_Flag_HuaGui);
    ssd1306_WriteString(OLED_buffer, Font_11x18, Black);

    ssd1306_SetCursor(3, 40);
    ssd1306_WriteString("IMU : ", Font_11x18, Black);
    ssd1306_SetCursor(70, 40);
    snprintf(OLED_buffer, sizeof(OLED_buffer), "%d", Motor_Stop_Flag_Car_Kalman);
    ssd1306_WriteString(OLED_buffer, Font_11x18, Black);

    ssd1306_SetCursor(100, 1);
    snprintf(OLED_buffer, sizeof(OLED_buffer), "%d", task_num_show_0);
    ssd1306_WriteString(OLED_buffer, Font_11x18, Black);

    ssd1306_SetCursor(100, 20);
    snprintf(OLED_buffer, sizeof(OLED_buffer), "%d", task_num_show_1);
    ssd1306_WriteString(OLED_buffer, Font_11x18, Black);

    ssd1306_SetCursor(100, 40);
    snprintf(OLED_buffer, sizeof(OLED_buffer), "%d", task_num_show_2);
    ssd1306_WriteString(OLED_buffer, Font_11x18, Black);


    ssd1306_UpdateScreen(); // 确保调用此函数来更新显示
}