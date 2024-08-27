#include "Bsp.h"

uint8_t HuaGui_Init_State = 0;//滑轨状态

void Bsp_Init(void) {
    Delay_Init(168); //Delay函数初始化
    Usart_Init(); //串口外设初始化
//    Move_Init(); //电机和舵机初始化
//    OLED_Init(); //OLED屏上电初始化
//    LCD_Init();

#if Serial_Debug == 1
    Uart_Debug_Init();
#endif

    Delay_ms(1000);

    HuaGui_Init_State = 1;
}

void HuaGui_Init_Proc(void) {
    // Motor_Read_Current(5);
    if (HuaGui_Init_State == 1) {
        Motor_SetPosition(5, 15000, -50, 0);
        Motor_Run();
        HuaGui_Init_State = 2;
    } else if (HuaGui_Init_State == 2) {
        Motor_Read_Current(5);
#if Serial_Debug == 1
        printf("%d\r\n", Motor_HuaGui_Current); // 200
#endif
        if (Motor_HuaGui_Current_amount >= 520) {
            HuaGui_Init_State = 3;
            Motor_Reset(5);
            HuaGui_JiaoZhun();
            Servo_Init();
        }
    } else if (HuaGui_Init_State == 3) {
        if (HuaGui_Turn(HuaGui_IN) == 1) {
            HuaGui_Init_State = 0;
        }
    }
}

// uint16_t Temp_Val = 90;
uint8_t Motor_State = 1;//底盘电机状态
extern uint8_t Running_Mode; // �ٶ�ģʽ
uint8_t temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0;

void Key_Proc(void) {
    if (temp1 == 1) {
        // uint8_t temp = ZaiWu_Turn(ZaiWu_Mid);
        // if (temp == 1)
        // {
        //       temp1 = 0;
        //       temp2 = 0;
        //       temp3 = 0;
        //       temp4 = 0;
        // }
    }
    if (temp2 == 1) {
        // uint8_t temp = ZaiWu_Turn(ZaiWu_Left);
        // if (temp == 1)
        // {
        //       temp1 = 0;
        //       temp2 = 0;
        //       temp3 = 0;
        //       temp4 = 0;
        // }
    }
    if (temp3 == 1) {
        // uint8_t temp = Put_Loads(0, 0);
        // if (temp == 1)
        // {
        //       temp1 = 0;
        //       temp2 = 0;
        //       temp3 = 0;
        //       temp4 = 0;
        // }
    }
    if (temp4 == 1) {
        // uint8_t temp = Car_Calibration(150, 100);
        // if (temp == 1)
        // {
        //       temp1 = 0;
        //       temp2 = 0;
        //       temp3 = 0;
        //       temp4 = 0;
        // }
    }
    if (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET) // �������ʹ��ʧ��?
    {
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
    if (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == GPIO_PIN_RESET) // ����������ת��
    {
        while (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == GPIO_PIN_RESET);
        // HuaGui_Init_State = 1;
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
    if (HAL_GPIO_ReadPin(KEY_4_GPIO_Port, KEY_4_Pin) == GPIO_PIN_RESET) // ����λ�����Ϳ�ʼ����
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

void OLED_Proc(void) {
    OLED_ShowString(1, 1, "H ");
    OLED_ShowNum(1, 3, Stop_Flag_HuaGui, 1);
    OLED_ShowString(1, 5, "C ");
    OLED_ShowNum(1, 7, Motor_Stop_Flag_Car, 1);
    OLED_ShowString(1, 9, "Y ");
    OLED_ShowSignedNum(1, 11, Yaw, 3);

    OLED_ShowString(2, 1, "Task:");
    OLED_ShowNum(2, 6, Task_Num_0, 2);
    OLED_ShowNum(2, 9, Running_Flag, 1);

    OLED_ShowString(3, 1, "Data:");
    OLED_ShowSignedNum(3, 6, Task_Num_1, 3);
    OLED_ShowSignedNum(3, 11, Task_Num_2, 3);

    OLED_ShowNum(4, 15, Running_Mode, 1);
    // OLED_ShowNum(4, 10, Temp_Val, 3);
    // Servo_SetAngle(SERVO_ZaiWu, Temp_Val);
    if (Motor_State == 1) {
        OLED_ShowString(4, 1, "Motor ON ");
    } else {
        OLED_ShowString(4, 1, "Motor OFF");
    }
}

extern void Uart_Debug_Init(void);