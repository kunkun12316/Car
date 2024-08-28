#include "Control.h"

uint8_t Task_Num_0 = 0;
int16_t Task_Num_1 = 0;
int16_t Task_Num_2 = 0;
uint8_t Running_Flag = 0; // 任务运行标志位

uint8_t Running_Mode = 0; // 速度模式
uint16_t Car_Speed = 250;
uint16_t Car_Acc = 150;
uint16_t Car_Speed_Slow = 150;
uint16_t Car_Acc_Slow = 100;
uint16_t Car_Turn_Speed_Limit = 50;
uint16_t Motor_HuaGui_Speed = 250;
uint16_t Motor_HuaGui_ACC = 240;


/*
uint8_t Task_Move_Flag[10] = {0};
uint8_t Task_Turn_Flag[10] = {0};
uint8_t Task_HuaGui_Flag[10] = {0};

uint8_t Task_Move_Data_1[10] = {0};
uint8_t Task_Turn_Data_1[10] = {0};
uint8_t Task_HuaGui_Data_1[10] = {0};

uint8_t Task_Move_Data_2[10] = {0};
uint8_t Task_Turn_Data_2[10] = {0};
uint8_t Task_HuaGui_Data_2[10] = {0};
*/

uint8_t Task_Flag[100] = {0};
int16_t Task_Data1[100] = {0};
int16_t Task_Data2[100] = {0};

extern uint8_t Car_Counter_Enable;
extern uint16_t Car_Counter_Times;


void Control_Proc(void) {
    if (Running_Mode == 0) {
        Car_Speed = 250;
        Car_Acc = 150;
        Car_Speed_Slow = 150;
        Car_Acc_Slow = 100;
        Car_Turn_Speed_Limit = 50;
        Motor_HuaGui_Speed = 250;
        Motor_HuaGui_ACC = 230;
    } else if (Running_Mode == 1) {
        Car_Speed = 280;
        Car_Acc = 180;
        Car_Speed_Slow = 150;
        Car_Acc_Slow = 100;
        Car_Turn_Speed_Limit = 55;
        Motor_HuaGui_Speed = 250;
        Motor_HuaGui_ACC = 230;
    } else if (Running_Mode == 2) {
        Car_Speed = 300;
        Car_Acc = 210;
        Car_Speed_Slow = 150;
        Car_Acc_Slow = 100;
        Car_Turn_Speed_Limit = 60;
        Motor_HuaGui_Speed = 250;
        Motor_HuaGui_ACC = 230;
    } else if (Running_Mode == 3) {
        Car_Speed = 320;
        Car_Acc = 210;
        Car_Speed_Slow = 180;
        Car_Acc_Slow = 110;
        Car_Turn_Speed_Limit = 65;
        Motor_HuaGui_Speed = 260;
        Motor_HuaGui_ACC = 235;
    }

    if (Car_Task_Data_0 != 0) {
        Task_Flag[Car_Task_Data_0] = 1;
        Task_Data1[Car_Task_Data_0] = Car_Task_Data_1;
        Task_Data2[Car_Task_Data_0] = Car_Task_Data_2;
        Task_Num_0 = Car_Task_Data_0;
        Task_Num_1 = Car_Task_Data_1;
        Task_Num_2 = Car_Task_Data_2;

        Car_Task_Data_0 = 0;
        Car_Task_Data_1 = 0;
        Car_Task_Data_2 = 0;
        Running_Flag = 1;
    }
    if (Task_Flag[1]) // 小车移动（x，y）
    {
        static uint8_t temp = 0;
        Car_Counter_Times = 1000;

        if (temp == 0) {
            if (Task_Data1[1] != 0 && Task_Data2[1] != 0) {
                Car_Go_Target(Task_Data1[1] * 128, 0, Car_Speed, Car_Acc);
                Car_Counter_Enable = 1;
                temp = 1;
            } else if (Task_Data1[1] == 0 || Task_Data2[1] == 0) {
                Car_Go_Target(Task_Data1[1] * 128, Task_Data2[1] * 128, Car_Speed, Car_Acc);
                Car_Counter_Enable = 1;
                temp = 2;
            }

        } else if (temp == 1) {
            if (Motor_Stop_Flag_Car == 1 || Car_Counter_Enable == 0) {
                Car_Go_Target(0, Task_Data2[1] * 128, Car_Speed, Car_Acc);
                Car_Counter_Enable = 2;
                temp = 2;
            }
        } else if (temp == 2) {
            if (Motor_Stop_Flag_Car == 1 || Car_Counter_Enable == 0) {
                Car_Counter_Enable = 0;
                temp = 0;
                Task_Flag[1] = 0;
                //HAL_UART_Transmit(&huart4, "01", 2, 0xffff);
#if Serial_Debug == 1
                printf("Task 1 End\r\n");
#endif
                // Delay_ms(20);
                // HAL_UART_Transmit(&huart4, "01", 2, 0xffff);

                Running_Flag = 0;

                LED2_TOGGLE();
            }
        }
    }
    if (Task_Flag[2]) // 小车转向
    {
        static uint8_t temp = 0;
        if (temp == 0)
        {
            if (Car_Turn(Task_Data1[2], Car_Turn_Speed_Limit, Car_Acc_Slow) == 1)
                temp = 1;
        }
        else if (temp == 1)
        {
            temp = 0;
            Task_Flag[2] = 0;
            //HAL_UART_Transmit(&huart4, "02", 2, 0xffff);
#if Serial_Debug == 1
            printf("Task 2 End\r\n");
#endif
            // Delay_ms(20);
            // HAL_UART_Transmit(&huart4, "02", 2, 0xffff);

            Running_Flag = 0;
        }
    }
    if (Task_Flag[3]) // 滑轨上下移动
    {
        static uint8_t temp = 0;
        static uint8_t Last_Data1 = 0;
        if (temp == 0)
        {
            switch (Task_Data1[3])
            {
                case 0:
                    HuaGui_UP(Motor_HuaGui_Speed, Motor_HuaGui_ACC);
                    break;
                case 1:
                    HuaGui_DOWN(Motor_HuaGui_Speed, Motor_HuaGui_ACC);
                    break;
                default:
                    break;
            }
            Last_Data1 = Task_Data1[3];
            temp = 1;
        }
        else if (temp == 1)
        {
            if (Task_Data1[3] != Last_Data1)
            {
                switch (Task_Data1[3])
                {
                    case 0:
                        HuaGui_UP(Motor_HuaGui_Speed, Motor_HuaGui_ACC);
                        break;
                    case 1:
                        HuaGui_DOWN(Motor_HuaGui_Speed, Motor_HuaGui_ACC);
                        break;
                    default:
                        break;
                }
                Last_Data1 = Task_Data1[3];
            }
            if (Stop_Flag_HuaGui == 1)
            {
                temp = 0;
                Task_Flag[3] = 0;
//                HAL_UART_Transmit(&huart4, "03", 2, 0xffff);
#if Serial_Debug == 1
                printf("Task 3 End\r\n");
#endif
                // Delay_ms(20);
                // HAL_UART_Transmit(&huart4, "03", 2, 0xffff);

                Running_Flag = 0;
                Last_Data1 = 0;
            }
        }
    }
    if (Task_Flag[4]) // 滑轨内外旋转
    {
        static uint8_t temp = 0;
        if (temp == 0)
        {
            if (Task_Data1[4] == 0)
            {
                if (HuaGui_Turn(HuaGui_IN) == 1)
                {
                    temp = 1;
                }
            }
            else if (Task_Data1[4] == 1)
            {
                if (HuaGui_Turn(HuaGui_OUT) == 1)
                {
                    temp = 1;
                }
            }
        }
        else if (temp == 1)
        {
            temp = 0;
            Task_Flag[4] = 0;
//            HAL_UART_Transmit(&huart4, "04", 2, 0xffff);
#if Serial_Debug == 1
            printf("Task 4 End\r\n");
#endif
            // Delay_ms(20);
            // HAL_UART_Transmit(&huart4, "04", 2, 0xffff);

            Running_Flag = 0;
        }
    }
    if (Task_Flag[5]) // 取物料
    {
        static uint8_t temp = 0;
        static uint8_t temp1 = 0;
        static uint8_t temp2 = 0;
        temp1 = Task_Data1[5];
        temp2 = Task_Data2[5];
        if (temp == 0)
        {
            uint8_t temp_state = Get_Loads(temp1, temp2, Motor_HuaGui_Speed, Motor_HuaGui_ACC);
            if (temp_state == 1)
            {
                temp = 1;
            }
            else if (temp_state == 2)
            {
//                HAL_UART_Transmit(&huart4, "95", 2, 0xffff);
            }
            else if (temp_state == 3)
            {
//                HAL_UART_Transmit(&huart4, "85", 2, 0xffff);
            }
        }
        else if (temp == 1)
        {
            temp = 0;
            temp1 = 0;
            temp2 = 0;
            Task_Flag[5] = 0;
//            HAL_UART_Transmit(&huart4, "05", 2, 0xffff);
#if Serial_Debug == 1
            printf("Task 5 End\r\n");
#endif
            // Delay_ms(20);
            // HAL_UART_Transmit(&huart4, "05", 2, 0xffff);

            Running_Flag = 0;
        }
    }
    if (Task_Flag[6]) // 放置物料
    {
        static uint8_t temp = 0;
        static uint8_t temp1 = 0;
        static uint8_t temp2 = 0;
        temp1 = Task_Data1[6];
        temp2 = Task_Data2[6];
        if (temp == 0)
        {
            uint8_t temp_state = Put_Loads(temp1, temp2, Motor_HuaGui_Speed, Motor_HuaGui_ACC);
            if (temp_state == 1)
            {
                temp = 1;
            }
            else if (temp_state == 2)
            {
//                HAL_UART_Transmit(&huart4, "96", 2, 0xffff);
            }
        }
        else if (temp == 1)
        {
            temp = 0;
            temp1 = 0;
            temp2 = 0;
            Task_Flag[6] = 0;
//            HAL_UART_Transmit(&huart4, "06", 2, 0xffff);
#if Serial_Debug == 1
            printf("Task 6 End\r\n");
#endif
            // Delay_ms(20);
            // HAL_UART_Transmit(&huart4, "06", 2, 0xffff);

            Running_Flag = 0;
        }
    }
    if (Task_Flag[7]) // 车身角度回正
    {
        uint8_t temp = Car_Calibration(Car_Speed_Slow, Car_Acc_Slow);
        if (temp == 1)
        {
            Task_Flag[7] = 0;
            //HAL_UART_Transmit(&huart4, "07", 2, 0xffff);
#if Serial_Debug == 1
            printf("Task 7 End\r\n");
#endif
            // Delay_ms(20);
            // HAL_UART_Transmit(&huart4, "07", 2, 0xffff);

            Running_Flag = 0;
        }
    }
    if (Task_Flag[9]) // 小车移动毫米（x，y）
    {
        static uint8_t temp = 0;
        if (Task_Data1[9] <= 5 && Task_Data2[9] <= 5)
        {
            Car_Counter_Times = 150;
        }
        else
        {
            Car_Counter_Times = 400;
        }
        if (temp == 0)
        {
            Car_Go_Target(Task_Data1[9] * 13, Task_Data2[9] * 13, Car_Speed_Slow, Car_Acc_Slow);
            temp = 1;
            Car_Counter_Enable = 1;
        }
        else if (temp == 1)
        {
            if (Motor_Stop_Flag_Car == 1 || Car_Counter_Enable == 0)
            {
                Car_Counter_Enable = 0;
                temp = 0;
                Task_Flag[9] = 0;
//                HAL_UART_Transmit(&huart4, "09", 2, 0xffff);
#if Serial_Debug == 1
                printf("Task 9 End\r\n");
#endif
                // Delay_ms(20);
                // HAL_UART_Transmit(&huart4, "09", 2, 0xffff);

                Running_Flag = 0;
            }
        }
    }

    if (Running_Flag){
        Motor_State_Scan(1);
    }
}