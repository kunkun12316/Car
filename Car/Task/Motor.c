#include "Motor.h"
#include "../Bsp/Bsp.h"

uint8_t Motor_RxBuff;
uint8_t Send_Data[20];
uint8_t Motor_Stop_Flag_Car = 0;// 小车停止标志位 1停止 0运行
uint8_t Last_Motor_Stop_Flag_Car = 1; // 1停止 0运行
uint16_t RxBuffer1[10] = {0};
uint16_t Motor_HuaGui_Current_amount = 0; //电机电流值

uint8_t HuaGui_Motor_State = HuaGui_Motor_State_UP;
uint8_t Stop_Flag_HuaGui = 1; // 滑轨电机停止标志位

/*
---------------------------------电机发送指令函数---------------------------------
*/
// 绝对值函数
uint32_t My_ABS(int32_t temp) {
    if (temp >= 0)
        return temp;
    else
        return -temp;
}

//电机数据接受处理函数
void Motor_Receive_Data(uint8_t com_data) {
    uint8_t i;
    static uint8_t RxCounter1 = 0; // 计数器
    static uint8_t RxState = 0; // 状态机的状态

    // 初始状态：等待接收起始标志
    if (RxState == 0 && (com_data == 0x01 || com_data == 0x05)) {
        RxState = 1; // 转换到状态1
        RxBuffer1[RxCounter1++] = com_data; // 存储接收到的数据并增加计数器
    }
        // 状态1：接收数据
    else if (RxState == 1) {
        RxBuffer1[RxCounter1++] = com_data; // 存储接收到的数据并增加计数器
        if (RxCounter1 >= 10 || com_data == 0x6B) // 判断是否接收完毕或接收到结束标志
        {
#if Serial_Debug == 1
            for (int j = 0; j < RxCounter1; ++j) {
                printf("%X\n", RxBuffer1[j]);
            }
            printf("\r\n");
#endif
            RxState = 2; // 转换到状态2
        }
    }

    // 状态2：处理接收到的数据
    if (RxState == 2) {
        if (RxBuffer1[RxCounter1 - 1] == 0x6B) // 检查结束标志是否正确
        {
            RxCounter1 = 0; // 重置计数器
            RxState = 0; // 返回初始状态

            // 根据接收的数据做不同的处理
//            if (RxBuffer1[0] == 0x01 && RxBuffer1[1] == 0xFD && RxBuffer1[2] == 0x9F) {
//                Motor_Stop_Flag_Car = 1; // 设置小车停止标志
//#if Serial_Debug == 1
//                printf("Car_Stop\r\n"); // 如果开启调试，打印调试信息
//#endif
//                for (i = 0; i < 4; i++) {
//                    RxBuffer1[i] = 0x00; // 清空数据数组前4个元素
//                }
            if (RxBuffer1[0] == 0x01 && RxBuffer1[1] == 0x3A) {
                Last_Motor_Stop_Flag_Car = Motor_Stop_Flag_Car;
                if (RxBuffer1[2] == 0x01) {
                    //此时地盘电机还未停止
                    Motor_Stop_Flag_Car = 0;
#if Serial_Debug == 1
                    printf("Car not Stop!!\r\n"); // 如果开启调试，打印调试信息
#endif
                } else if (RxBuffer1[2] == 0x03) {
                    //这时底盘电机停止
                    Motor_Stop_Flag_Car = 1; // 设置小车停止标志
#if Serial_Debug == 1
                    printf("Car Stop!\r\n"); // 如果开启调试，打印调试信息
#endif
                }
                for (i = 0; i < 4; i++) {
                    RxBuffer1[i] = 0x00; // 清空数据数组前4个元素
                }

            } else if (RxBuffer1[0] == 0x05 && RxBuffer1[1] == 0x3A){
                if (RxBuffer1[2] == 0x01) {
                    //此时滑轨电机还未停止
                    Stop_Flag_HuaGui = 0;
#if Serial_Debug == 1
                    printf("HuaGui not Stop!!\r\n"); // 如果开启调试，打印调试信息
#endif
                } else if (RxBuffer1[2] == 0x03) {
                    //这时滑轨电机停止
                    Stop_Flag_HuaGui = 1; // 设置滑轨停止标志
#if Serial_Debug == 1
                    printf("HuaGui Stop!\r\n"); // 如果开启调试，打印调试信息
#endif
                }
                for (i = 0; i < 4; i++) {
                    RxBuffer1[i] = 0x00; // 清空数据数组前4个元素
                }
            } else if (RxBuffer1[0] == 0x05 && RxBuffer1[1] == 0x27) {
                Motor_HuaGui_Current_amount = RxBuffer1[2] << 8 | RxBuffer1[3]; // 合成当前电流量
            } else {
                for (i = 0; i < 10; i++) {
                    RxBuffer1[i] = 0x00; // 清空数据数组
                }
            }
        } else // 结束标志错误，数据接收错误
        {
            RxState = 0; // 返回初始状态
            RxCounter1 = 0; // 重置计数器
            Motor_Stop_Flag_Car = 2; // 设置小车停止错误标志
            for (i = 0; i < 10; i++) {
                RxBuffer1[i] = 0x00; // 清空数据数组
            }
        }
    }
}

// 实现使能指定电机。
void Motor_Enable(uint8_t Motor_Num) {
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xF3;
    Send_Data[2] = 0xAB;
    Send_Data[3] = 0x01;
    Send_Data[4] = 0x00;
    Send_Data[5] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 6, 1000);
    Delay_ms(10);
}

// 实现禁用指定电机。
void Motor_Disable(uint8_t Motor_Num) {
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xF3;
    Send_Data[2] = 0xAB;
    Send_Data[3] = 0x00;
    Send_Data[4] = 0x00;
    Send_Data[5] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 6, 1000);
    Delay_ms(10);
}

// 实现重置指定电机。
void Motor_Reset(uint8_t Motor_Num) {
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xFE;
    Send_Data[2] = 0x98;
    Send_Data[3] = 0x00;
    Send_Data[4] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 5, 1000);
    Delay_ms(10);
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0x0A;
    Send_Data[2] = 0x6D;
    Send_Data[3] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 4, 1000);
    Delay_ms(10);
}

// 实现清除指定电机。
void Motor_Clear(uint8_t Motor_Num) {
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0x0A;
    Send_Data[2] = 0x6D;
    Send_Data[3] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 4, 1000);
    Delay_ms(10);
}

void Motor_Stop(uint8_t Motor_Num)
{
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xFE;
    Send_Data[2] = 0x98;
    Send_Data[3] = 0x01;
    Send_Data[4] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 5, 1000);
    Delay_ms(10);
}

// 实现初始化电机设置并使能UART。
void Motor_Init(void) {
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart1, &Motor_RxBuff, 1);
    Motor_Reset(0);
    Delay_ms(100);
    // HuaGui_JiaoZhun();
    // Delay_ms(10);
    Motor_Enable_All();
}

// 实现使能所有电机。
void Motor_Enable_All(void) {
    Motor_Enable(1);
    Motor_Enable(2);
    Motor_Enable(3);
    Motor_Enable(4);
    Motor_Enable(5);
}

// 实现禁用所有电机。
void Motor_Disable_All(void) {
    Motor_Disable(1);
    Motor_Disable(2);
    Motor_Disable(3);
    Motor_Disable(4);
    Motor_Disable(5);
}

//底盘电机停止
void Motor_Low_Stop(void)
{
    Motor_Stop(1);
    Motor_Stop(2);
    Motor_Stop(3);
    Motor_Stop(4);
    Motor_Run();

}
// 电机状态检测
void Motor_State_Scan(uint8_t Motor_Num) {
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0x3A;
    Send_Data[2] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 3, 1000);
    Delay_ms(20);
}

// 实现为指定电机设置PID值。
void Motor_Set_PID(uint8_t Motor_Num, uint32_t KP, uint32_t KI, uint32_t KD) {
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0x4A;
    Send_Data[2] = 0xC3;
    Send_Data[3] = 0x01;
    Send_Data[4] = KP >> 24;
    Send_Data[5] = (KP << 8) >> 24;
    Send_Data[6] = (KP << 16) >> 24;
    Send_Data[7] = (KP << 24) >> 24;
    Send_Data[8] = KI >> 24;
    Send_Data[9] = (KI << 8) >> 24;
    Send_Data[10] = (KI << 16) >> 24;
    Send_Data[11] = (KI << 24) >> 24;
    Send_Data[12] = KD >> 24;
    Send_Data[13] = (KD << 8) >> 24;
    Send_Data[14] = (KD << 16) >> 24;
    Send_Data[15] = (KD << 24) >> 24;
    Send_Data[16] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 17, 1000);
    Delay_ms(10);
}

// 实现从指定电机读取电流值。
void Motor_Read_Current(uint8_t Motor_Num) {
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0x27;
    Send_Data[2] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 3, 1000);
    Delay_ms(20);
}

// 电机同步运动标志
void Motor_Run(void) {
    Send_Data[0] = 0x00;
    Send_Data[1] = 0xFF;
    Send_Data[2] = 0x66;
    Send_Data[3] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 4, 1000);
    Delay_ms(10);
}

// Speed 单位RPM
// Acc   0---255
void Motor_SetSpeed(uint8_t Motor_Num, int16_t Speed, uint8_t Acc) {
    uint8_t Direction;
    uint16_t Speed_Temp = My_ABS(Speed);
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xF6;
    if (Motor_Num == 1 || Motor_Num == 4) {
        if (Speed >= 0)
            Direction = 0;
        else
            Direction = 1;
    } else {
        if (Speed >= 0)
            Direction = 0;
        else
            Direction = 1;
    }
    Send_Data[2] = Direction;
    Send_Data[3] = Speed_Temp >> 8;
    Send_Data[4] = (Speed_Temp << 8) >> 8;
    Send_Data[5] = Acc;
    Send_Data[6] = 0x01;
    Send_Data[7] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 8, 1000);
    Delay_ms(10);
}

// Speed 单位RPM
// Acc   0---255 (0代表关闭梯形加减速)
// Pulse 3200脉冲一圈
// 相对脉冲
void Motor_SetPosition(uint8_t Motor_Num, uint32_t Pulse, int16_t Speed, uint8_t Acc) {
    uint8_t Direction;
    uint16_t Speed_Temp = My_ABS(Speed);
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xFD;

    if (Motor_Num == 1 || Motor_Num == 3) { // 左前或左后
        if (Speed >= 0)
            Direction = 0;
        else
            Direction = 1;
    } else { // 右前或右后
        if (Speed >= 0)
            Direction = 1;
        else
            Direction = 0;
    }

    Send_Data[2] = Direction;
    Send_Data[3] = Speed_Temp >> 8;
    Send_Data[4] = (Speed_Temp << 8) >> 8;
    Send_Data[5] = Acc;
    Send_Data[6] = Pulse >> 24;
    Send_Data[7] = (Pulse << 8) >> 24;
    Send_Data[8] = (Pulse << 16) >> 24;
    Send_Data[9] = (Pulse << 24) >> 24;
    Send_Data[10] = 0x00;
    Send_Data[11] = 0x01;
    Send_Data[12] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 13, 1000);
    if (Motor_Num == 1) {
        Motor_Stop_Flag_Car = 0;
        Delay_ms(10);
        Motor_Stop_Flag_Car = 0;
    }
    Delay_ms(10);
}

// Speed 单位RPM
// Acc   0---255 (0代表关闭梯形加减速)
// Pulse 3200脉冲一圈
// 相对脉冲
void Motor_SetPosition_Dir(uint8_t Dir, uint8_t Motor_Num, uint32_t Pulse, int16_t Speed, uint8_t Acc) {
    uint8_t Direction;
    uint16_t Speed_Temp = My_ABS(Speed);
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xFD;

    //Direction 0 顺时针 1 逆时针
    if (Dir == 0) {
        if (Motor_Num == 1 || Motor_Num == 3) { // 左前或左后
            if (Speed >= 0)
                Direction = 0;
            else
                Direction = 1;
        } else { // 右前或右后
            if (Speed >= 0)
                Direction = 1;
            else
                Direction = 0;
        }
    } else if (Dir == 1) {
        if (Motor_Num == 1 || Motor_Num == 4) {
            if (Speed >= 0)
                Direction = 0;
            else
                Direction = 1;
        } else {
            if (Speed >= 0)
                Direction = 1;
            else
                Direction = 0;
        }
    }

    Send_Data[2] = Direction;
    Send_Data[3] = Speed_Temp >> 8;
    Send_Data[4] = (Speed_Temp << 8) >> 8;
    Send_Data[5] = Acc;
    Send_Data[6] = Pulse >> 24;
    Send_Data[7] = (Pulse << 8) >> 24;
    Send_Data[8] = (Pulse << 16) >> 24;
    Send_Data[9] = (Pulse << 24) >> 24;
    Send_Data[10] = 0x00;
    Send_Data[11] = 0x01;
    Send_Data[12] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 13, 1000);
    if (Motor_Num == 1) {
        Motor_Stop_Flag_Car = 0;
        Delay_ms(10);
        Motor_Stop_Flag_Car = 0;
    }
    Delay_ms(10);
}

// Speed 单位RPM
// Acc   0---255 (0代表关闭梯形加减速)
// Pulse 3200脉冲一圈
// 绝对脉冲
void Motor_SetPosition_A(uint8_t Motor_Num, uint32_t Pulse, int16_t Speed, uint8_t Acc) {
    uint16_t Speed_Temp = My_ABS(Speed);
    uint8_t Direction;
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xFD;
    if (Motor_Num == 1 || Motor_Num == 3) {
        if (Speed >= 0)
            Direction = 0;
        else
            Direction = 1;
    } else {
        if (Speed >= 0)
            Direction = 1;
        else
            Direction = 0;
    }

    Send_Data[2] = Direction;
    Send_Data[3] = Speed_Temp >> 8;
    Send_Data[4] = (Speed_Temp << 8) >> 8;
    Send_Data[5] = Acc;
    Send_Data[6] = Pulse >> 24;
    Send_Data[7] = (Pulse << 8) >> 24;
    Send_Data[8] = (Pulse << 16) >> 24;
    Send_Data[9] = (Pulse << 24) >> 24;
    Send_Data[10] = 0x01;
    Send_Data[11] = 0x01;
    Send_Data[12] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 13, 1000);
    Delay_ms(10);
    if (Motor_Num == 1) {
        uint8_t i = 0;
        Motor_Stop_Flag_Car = 0;
        for (i = 0; i < 4; i++) {
            RxBuffer1[i] = 0x00; // 将存放数据数组清零
        }
    }
    if (Motor_Num == 5) {
        uint8_t i = 0;
        Stop_Flag_HuaGui = 0;
        for (i = 0; i < 4; i++) {
            RxBuffer1[i] = 0x00; // 将存放数据数组清零
        }
        Stop_Flag_HuaGui = 0;
    }
}
/*
---------------------------------小车运动函数---------------------------------
*/
// 相对位置闭环 12800脉冲---4圈---1米
//Angle 角度值
//Speed 速度
//Distance 脉冲数
//ACC 加减速 0-255
void Car_Go(int16_t Angle, int16_t Speed, int32_t Distance, uint16_t Car_ACC) {
    if (Angle >= 0 && Angle <= 90) {
        Motor_SetPosition(1, Distance, Speed, Car_ACC);
        Motor_SetPosition(2, My_ABS((45 * Distance / 45) - (Distance * Angle / 45)),
                          (45 * Speed / 45) - (Speed * Angle / 45), Car_ACC);
        Motor_SetPosition(3, My_ABS((45 * Distance / 45) - (Distance * Angle / 45)),
                          (45 * Speed / 45) - (Speed * Angle / 45), Car_ACC);
        Motor_SetPosition(4, Distance, Speed, Car_ACC);
    } else if (Angle > 90 && Angle <= 180) {
        Motor_SetPosition(1, My_ABS((135 * Distance / 45) - (Distance * Angle / 45)),
                          (135 * Speed / 45) - (Speed * Angle / 45), Car_ACC);
        Motor_SetPosition(2, Distance, -Speed, Car_ACC);
        Motor_SetPosition(3, Distance, -Speed, Car_ACC);
        Motor_SetPosition(4, My_ABS((135 * Distance / 45) - (Distance * Angle / 45)),
                          (135 * Speed / 45) - (Speed * Angle / 45), Car_ACC);
    } else if (Angle > -90 && Angle < 0) {
        Motor_SetPosition(1, My_ABS((45 * Distance + Distance * Angle) / 45), (45 * Speed / 45) + (Speed * Angle / 45),
                          Car_ACC);
        Motor_SetPosition(2, Distance, Speed, Car_ACC);
        Motor_SetPosition(3, Distance, Speed, Car_ACC);
        Motor_SetPosition(4, My_ABS((45 * Distance + Distance * Angle) / 45), (45 * Speed / 45) + (Speed * Angle / 45),
                          Car_ACC);
    } else if (Angle >= -180 && Angle <= -90) {
        Motor_SetPosition(1, Distance, -Speed, Car_ACC);
        Motor_SetPosition(2, My_ABS((135 * Distance + Distance * Angle) / 45), (135 * Speed + Speed * Angle) / 45,
                          Car_ACC);
        Motor_SetPosition(3, My_ABS((135 * Distance + Distance * Angle) / 45), (135 * Speed + Speed * Angle) / 45,
                          Car_ACC);
        Motor_SetPosition(4, Distance, -Speed, Car_ACC);
    }
    Motor_Run();
}

// 相对位置闭环 12800脉冲---4圈---1米
//Tar_X 目标位置的X坐标
//Tar_Y 目标位置的Y坐标
void Car_Go_Target(int32_t Tar_X, int32_t Tar_Y, int16_t Speed, uint16_t Car_ACC) {
    if (Tar_X != 0 && Tar_Y == 0) {
        int32_t V1, V2;
        V2 = Speed * My_ABS(Tar_Y - Tar_X) / (1.4 * sqrt(Tar_X * Tar_X + Tar_Y * Tar_Y));
        V1 = Speed * My_ABS(Tar_Y + Tar_X) / (1.4 * sqrt(Tar_X * Tar_X + Tar_Y * Tar_Y));

        if (Tar_X + Tar_Y >= 0) {
            //DIr 0 X轴运动 1 Y轴运动
            Motor_SetPosition_Dir(0, 1, Tar_X + Tar_Y, V1, Car_ACC);
            Motor_SetPosition_Dir(0, 4, Tar_X + Tar_Y, V1, Car_ACC);
            printf("V1 + \r\n");
        } else {
            Motor_SetPosition_Dir(0, 1, -(Tar_X + Tar_Y), -V1, Car_ACC);
            Motor_SetPosition_Dir(0, 4, -(Tar_X + Tar_Y), -V1, Car_ACC);
            printf("V1 - \r\n");
        }
        if (Tar_Y - Tar_X >= 0) {
            Motor_SetPosition_Dir(0, 2, Tar_Y - Tar_X, V2, Car_ACC);
            Motor_SetPosition_Dir(0, 3, Tar_Y - Tar_X, V2, Car_ACC);
            printf("V2 + \r\n");
        } else {
            Motor_SetPosition_Dir(0, 2, Tar_X - Tar_Y, -V2, Car_ACC);
            Motor_SetPosition_Dir(0, 3, Tar_X - Tar_Y, -V2, Car_ACC);
            printf("V2 - \r\n");
        }
        Motor_Run();
    } else if (Tar_X == 0 && Tar_Y != 0) {
        int32_t V1, V2;
        V2 = Speed * My_ABS(Tar_Y - Tar_X) / (1.4 * sqrt(Tar_X * Tar_X + Tar_Y * Tar_Y));
        V1 = Speed * My_ABS(Tar_Y + Tar_X) / (1.4 * sqrt(Tar_X * Tar_X + Tar_Y * Tar_Y));
        if (Tar_Y >= 0) {
            Motor_SetPosition_Dir(1, 1, Tar_X + Tar_Y, V1, Car_ACC);
            Motor_SetPosition_Dir(1, 2, Tar_X + Tar_Y, V2, Car_ACC);
            Motor_SetPosition_Dir(1, 3, Tar_X + Tar_Y, V2, Car_ACC);
            Motor_SetPosition_Dir(1, 4, Tar_X + Tar_Y, V1, Car_ACC);
        } else {
            Motor_SetPosition_Dir(1, 1, My_ABS(Tar_X + Tar_Y), -V1, Car_ACC);
            Motor_SetPosition_Dir(1, 2, My_ABS(Tar_X + Tar_Y), -V2, Car_ACC);
            Motor_SetPosition_Dir(1, 3, My_ABS(Tar_X + Tar_Y), -V2, Car_ACC);
            Motor_SetPosition_Dir(1, 4, My_ABS(Tar_X + Tar_Y), -V1, Car_ACC);
        }
        Motor_Run();
    }


//    int32_t V1, V2;
//    int32_t Magnitude = sqrt(Tar_X * Tar_X + Tar_Y * Tar_Y);
//
//    if (Magnitude != 0) {
//        V2 = Speed * My_ABS(Tar_Y - Tar_X) / (1.4 * Magnitude);
//        V1 = Speed * My_ABS(Tar_Y + Tar_X) / (1.4 * Magnitude);
//    } else {
//        V1 = V2 = 0;
//    }
//
//    // 前后和左右运动逻辑
//    // 左前 (Motor 1), 右前 (Motor 2), 左后 (Motor 4), 右后 (Motor 3)
//
//    // X 轴方向运动
//    if (Tar_X != 0) {
//        Motor_SetPosition(1, My_ABS(Tar_X), (Tar_X > 0) ? -V1 : V1, Car_ACC);
//        Motor_SetPosition(2, My_ABS(Tar_X), (Tar_X > 0) ? V2 : -V2, Car_ACC);
//        Motor_SetPosition(3, My_ABS(Tar_X), (Tar_X > 0) ? -V2 : V2, Car_ACC);
//        Motor_SetPosition(4, My_ABS(Tar_X), (Tar_X > 0) ? V1 : -V1, Car_ACC);
//
//    }
//    // Y 轴方向运动
//    if (Tar_Y != 0) {
//        Motor_SetPosition(1, My_ABS(Tar_Y), (Tar_Y > 0) ? V1 : -V1, Car_ACC);
//        Motor_SetPosition(2, My_ABS(Tar_Y), (Tar_Y > 0) ? V2 : -V2, Car_ACC);
//        Motor_SetPosition(3, My_ABS(Tar_Y), (Tar_Y > 0) ? V2 : -V2, Car_ACC);
//        Motor_SetPosition(4, My_ABS(Tar_Y), (Tar_Y > 0) ? V1 : -V1, Car_ACC);
//    }
//
//    Motor_Run();

}

// 绝对位置闭环
//Tar_X 目标位置的X坐标
//Tar_Y 目标位置的Y坐标
void Car_Go_Target_A(int32_t Tar_X, int32_t Tar_Y, int16_t Speed, uint16_t Car_ACC) {
    static int32_t Car_Now_X = 0, Car_Now_Y = 0;
    int32_t Go_X, Go_Y;
    Go_X = Tar_X - Car_Now_X;
    Go_Y = Tar_Y - Car_Now_Y;
    Car_Now_X = Tar_X;
    Car_Now_Y = Tar_Y;
    int32_t V1, V2;
    V2 = Speed * My_ABS(Go_Y - Go_X) / (1.4 * sqrt(Go_X * Go_X + Go_Y * Go_Y));
    V1 = Speed * My_ABS(Go_Y + Go_X) / (1.4 * sqrt(Go_X * Go_X + Go_Y * Go_Y));

    if (Go_X + Go_Y >= 0) {
        Motor_SetPosition_A(1, Go_X + Go_Y, V1, Car_ACC);
        Motor_SetPosition_A(4, Go_X + Go_Y, V1, Car_ACC);
    } else {
        Motor_SetPosition_A(1, -(Go_X + Go_Y), -V1, Car_ACC);
        Motor_SetPosition_A(4, -(Go_X + Go_Y), -V1, Car_ACC);
    }
    if (Go_Y - Go_X >= 0) {
        Motor_SetPosition_A(2, Go_Y - Go_X, V2, Car_ACC);
        Motor_SetPosition_A(3, Go_Y - Go_X, V2, Car_ACC);
    } else {
        Motor_SetPosition_A(2, Go_X - Go_Y, -V2, Car_ACC);
        Motor_SetPosition_A(3, Go_X - Go_Y, -V2, Car_ACC);
    }
    Motor_Run();
}

// 清除车轮电机当前编码器累计量
void Car_Clear(void) {
    Motor_Clear(1);
    Motor_Clear(2);
    Motor_Clear(3);
    Motor_Clear(4);
}


// 小车转弯
//Tar_Yaw: 目标偏航角度（单位：度）
//Speed_Limit: 转向速度限制
//Car_ACC: 小车加速度
uint8_t Car_Turn(int16_t Tar_Yaw, uint16_t Speed_Limit, uint16_t Car_ACC) {
#if Car_Turn_Use_IMU               // 结合IMU转向
    static uint8_t Motor_Kp = 2; // 转向环KP
    uint8_t ret = 0;// 返回值,用于指示转向是否完成
    static uint8_t Temp_State = 0; //转向临时状态
    static uint8_t Stop_Counter = 0;//停止计数器
    static float Temp_Yaw = 90; //临时存储目标偏航角
    static float Last_Yaw = 0; //上一次的偏航角
    if (Temp_State == 0) {
        Temp_State = 1;

//        Temp_Yaw = Last_Yaw + Tar_Yaw;
//        if (Temp_Yaw > 180){
//            Temp_Yaw -= 360;
//            printf("Tar_Yaw = %.2f\n",Temp_Yaw);
//        } else if (Temp_Yaw < -180){
//            Temp_Yaw = 180 - my_abs_float(0-Tar_Yaw);
//            printf("Tar_Yaw = %.2f\n",Temp_Yaw);
//        }
//


        if (Tar_Yaw >= 90 || Tar_Yaw <= -90) {
            Temp_Yaw = Last_Yaw + Tar_Yaw;
            if (Temp_Yaw > 180) {
                Temp_Yaw -= 360;
                printf("Tar_Yaw = %0.2f\n", Temp_Yaw);
            } else if (Temp_Yaw < -180) {
                Temp_Yaw = 360 - my_abs_float(0 - Tar_Yaw);
                printf("Tar_Yaw = %0.2f\n", Temp_Yaw);
            }
            printf("Tar_Yaw = %0.2f\n",Temp_Yaw);
            Last_Yaw += Tar_Yaw;
        } else {
            Temp_Yaw = JY_Yaw + Tar_Yaw;
        }
    } else if (Temp_State == 1) {
        float Yaw_Error = JY_Yaw - Temp_Yaw;
        Yaw_Error *= Motor_Kp;
        if (Yaw_Error > Speed_Limit)
            Yaw_Error = Speed_Limit;
        else if (Yaw_Error < -Speed_Limit)
            Yaw_Error = -Speed_Limit;

        if(Temp_Yaw > 0)
        {
            float angle_1 =  JY_Yaw + 180;
            float angle_2 = 180 - Temp_Yaw;
            if(angle_1 + angle_2 >= 180){
                //逆时针
                printf("CCW!\n");
                Motor_SetSpeed(1, -Yaw_Error, Car_ACC);
                Motor_SetSpeed(2, -Yaw_Error, Car_ACC);
                Motor_SetSpeed(3, -Yaw_Error, Car_ACC);
                Motor_SetSpeed(4, -Yaw_Error, Car_ACC);
            } else{
                //顺时针
                printf("CW!\n");
                Motor_SetSpeed(1, Yaw_Error, Car_ACC);
                Motor_SetSpeed(2, Yaw_Error, Car_ACC);
                Motor_SetSpeed(3, Yaw_Error, Car_ACC);
                Motor_SetSpeed(4, Yaw_Error, Car_ACC);
            }
        } else if (Temp_Yaw <= 0){
            float angle_1 = 180 - JY_Yaw;
            float angle_2 = Temp_Yaw + 180;
            if(angle_1 + angle_2 <= 180){
                //逆时针
                printf("CCW!\n");
                Motor_SetSpeed(1, -Yaw_Error, Car_ACC);
                Motor_SetSpeed(2, -Yaw_Error, Car_ACC);
                Motor_SetSpeed(3, -Yaw_Error, Car_ACC);
                Motor_SetSpeed(4, -Yaw_Error, Car_ACC);
            } else{
                //顺时针
                printf("CW!\n");
                Motor_SetSpeed(1, Yaw_Error, Car_ACC);
                Motor_SetSpeed(2, Yaw_Error, Car_ACC);
                Motor_SetSpeed(3, Yaw_Error, Car_ACC);
                Motor_SetSpeed(4, Yaw_Error, Car_ACC);
            }
        }

        Motor_Run();

        if (JY_Yaw >= Temp_Yaw - 3 && JY_Yaw <= Temp_Yaw + 3)
            Stop_Counter++;
        else
            Stop_Counter = 0;
        if (Stop_Counter >= 10) {
            Motor_Low_Stop();
            Stop_Counter = 0;
            ret = 1;
            Temp_State = 0;
        }
    }
    return ret;
#endif
#if Car_Turn_Use_IMU == 0        // 不结合IMU转向
    static float Alpha = 45.8; // Alpha 46.8 转向比例因子
    uint8_t ret = 0; //返回值
    static uint8_t Temp_State = 0; //临时状态
    if (Temp_State == 0) {
        Temp_State = 1;
        if (Tar_Yaw >= 0) {
            Motor_SetPosition(1, Tar_Yaw * Alpha, -Speed_Limit, Car_ACC);
            Motor_SetPosition(2, Tar_Yaw * Alpha, Speed_Limit, Car_ACC);
            Motor_SetPosition(3, Tar_Yaw * Alpha, -Speed_Limit, Car_ACC);
            Motor_SetPosition(4, Tar_Yaw * Alpha, Speed_Limit, Car_ACC);
            Motor_Run();
        } else {
            Motor_SetPosition(1, -Tar_Yaw * Alpha, Speed_Limit, Car_ACC);
            Motor_SetPosition(2, -Tar_Yaw * Alpha, -Speed_Limit, Car_ACC);
            Motor_SetPosition(3, -Tar_Yaw * Alpha, Speed_Limit, Car_ACC);
            Motor_SetPosition(4, -Tar_Yaw * Alpha, -Speed_Limit, Car_ACC);
            Motor_Run();
        }
    } else if (Temp_State == 1) {
        if (Motor_Stop_Flag_Car == 1 || Motor_Stop_Flag_Car_Kalman == 1) {
            ret = 1;
            Temp_State = 0;
        }
    }
    return ret;
#endif
}

// 车身回正
uint8_t Car_Calibration(uint16_t Speed_Limit, uint16_t Car_ACC) {
    static uint8_t Temp_State = 0;
    static float Temp_Yaw = 0;
    uint8_t ret = 0;
    if (Temp_State == 0) {
        Temp_State++;
        Temp_Yaw = JY_Yaw;
    } else if (Temp_State == 1) {
        uint8_t temp = 0;
        if (Temp_Yaw <= 10 && Temp_Yaw >= -10) {
            temp = Car_Turn(-Temp_Yaw, Speed_Limit, Car_ACC);
        }
        if (Temp_Yaw <= 95 && Temp_Yaw >= 85) {
            temp = Car_Turn(-Temp_Yaw + 90, Speed_Limit, Car_ACC);
        }
        if (Temp_Yaw <= -85 && Temp_Yaw >= -95) {
            temp = Car_Turn(-Temp_Yaw - 90, Speed_Limit, Car_ACC);
        }
        if (Temp_Yaw <= 185 && Temp_Yaw >= 175) {
            temp = Car_Turn(-Temp_Yaw + 180, Speed_Limit, Car_ACC);
        }
        if (Temp_Yaw <= -175 && Temp_Yaw >= -185) {
            temp = Car_Turn(-Temp_Yaw - 180, Speed_Limit, Car_ACC);
        }
        if (Temp_Yaw <= 275 && Temp_Yaw >= 265) {
            temp = Car_Turn(-Temp_Yaw + 270, Speed_Limit, Car_ACC);
        }
        if (Temp_Yaw <= -265 && Temp_Yaw >= -275) {
            temp = Car_Turn(-Temp_Yaw - 270, Speed_Limit, Car_ACC);
        }
        if (Temp_Yaw <= 365 && Temp_Yaw >= 355) {
            temp = Car_Turn(-Temp_Yaw + 360, Speed_Limit, Car_ACC);
        }
        if (Temp_Yaw <= -355 && Temp_Yaw >= -365) {
            temp = Car_Turn(-Temp_Yaw - 360, Speed_Limit, Car_ACC);
        }
        if (temp == 1) {
            ret = 1;
            Temp_State = 0;
            Temp_Yaw = 0;
        }
    }
    return ret;
}

/*
---------------------------------滑轨运动函数---------------------------------
*/

// 滑轨零位归位
void HuaGui_JiaoZhun(void) {
    HuaGui_Motor_State = HuaGui_Motor_State_UP;
    Send_Data[0] = 0x05;
    Send_Data[1] = 0x9A;
    Send_Data[2] = 0x02;
    Send_Data[3] = 0x00;
    Send_Data[4] = 0x6B;
    HAL_UART_Transmit(&huart1, Send_Data, 5, 1000);
    Delay_ms(10);
}

// 滑轨移动到最上端零位置
void HuaGui_UP(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc) {
    HuaGui_Motor_State = HuaGui_Motor_State_UP;
    Motor_SetPosition_A(5, Motor_HuaGui_Pulse_DEFAULT, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

// 滑轨移动到最下端取物位置
void HuaGui_DOWN(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc) {
    HuaGui_Motor_State = HuaGui_Motor_State_DOWN;
    Motor_SetPosition_A(5, Motor_HuaGui_Pulse_GROUND, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

// 滑轨移动到第二个放物位置
void HuaGui_DOWN2(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc) {
    HuaGui_Motor_State = HuaGui_Motor_State_DOWN2;
    Motor_SetPosition_A(5, Motor_HuaGui_Pulse_DOWN2, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

// 滑轨移动到载物台放置位置
void HuaGui_Fang(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc) {
    HuaGui_Motor_State = HuaGui_Motor_State_Fang;
    Motor_SetPosition_A(5, Motor_HuaGui_Pulse_Fang, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

// 滑轨移动到载物台取物位置
void HuaGui_Qu(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc) {
    HuaGui_Motor_State = HuaGui_Motor_State_Qu;
    Motor_SetPosition_A(5, Motor_HuaGui_Pulse_Qu, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

// 滑轨移动到转盘取物位置
void HuaGui_ZhuanPan(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc) {
    HuaGui_Motor_State = HuaGui_Motor_State_ZhuanPan;
    Motor_SetPosition_A(5, Motor_HuaGui_Pulse_ZhuanPan, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}
