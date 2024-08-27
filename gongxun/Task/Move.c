#include "Move.h"
#include "Bsp.h"

uint16_t u10ms;
double Yaw_Compensate = 0;//偏航补偿

uint8_t Counter_Enable = 0;//控制计数器的启用和禁用。
uint16_t Counter = 0;//计数器，用于记录时间或事件次数。
uint16_t Counter_Times = 0;//计数器计数的目标值。

uint8_t Car_Counter_Enable = 0;//控制车载计数器的启用和禁用。
uint16_t Car_Counter_Times = 0;//车载计数器，用于记录车载事件次数。
uint16_t Car_Counter = 0;//车载计数器计数的目标值。

uint16_t Car_Task_Time = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == (&htim6)) {
        if (Counter_Enable == 1) {
            Counter++;
            if (Counter >= Counter_Times) {
                Counter = 0;
                Counter_Enable = 0;
            }
        } else if (Counter_Enable == 0) {
            Counter = 0;
        }

        if (Car_Counter_Enable == 1) {
            Car_Counter++;
            if (Car_Counter >= Car_Counter_Times) {
                Car_Counter = 0;
                Car_Counter_Enable = 0;
            }
        } else if (Car_Counter_Enable == 0) {
            Car_Counter = 0;
        }

        //一秒为周期
        u10ms++;
        if (u10ms >= 100) {
            u10ms = 0;
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            Yaw_Compensate += 0.007;
        }
        Servo_CallBack();
    }
    else if (htim == (&htim7)) {
        //200次一秒
        Car_Task_Time++;
        if(Car_Task_Time >= 50000){
            Car_Task_Time = 0;
        }
    }
}

void Move_Init(void) {
    Servo_Init();
    Motor_Init();
}

//将物块放置到转盘上
uint8_t Get_Loads(uint8_t Dir, uint8_t From, uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc) {
    uint8_t Get_Loads_Return = 0; //获取负载返回
    static Get_Loads_States Get_Loads_State = Stop_State;
    Counter_Times = 115;
    if (Get_Loads_State == Stop_State) {
        HuaGui_UP(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
        Counter_Enable = 1;
        Get_Loads_State = Get_JiaoZhun_State;
    } else if (Get_Loads_State == Get_JiaoZhun_State) {
        if (Stop_Flag_HuaGui == 1 || Counter_Enable == 0) {
            Get_Loads_State = Ready_State;
            Counter_Enable = 0;
        }
    } else if (Get_Loads_State == Ready_State) {
        uint8_t temp2, temp3;
        if (Dir == 0)
            ZaiWu_Turn(ZaiWu_Mid);
        else if (Dir == 1)
            ZaiWu_Turn(ZaiWu_Left);
        else if (Dir == 2)
            ZaiWu_Turn(ZaiWu_Right);
        temp2 = JiaZhua_Turn(JiaZhua_Open);
        temp3 = HuaGui_Turn(HuaGui_OUT);
        if (temp2 == 1 && temp3 == 1 && Stop_Flag_HuaGui == 1) {
            if (From == 0) {
                HuaGui_DOWN(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
                Counter_Enable = 1;
                Get_Loads_State = Get_State1;
            } else if (From == 1) {
                HuaGui_ZhuanPan(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
                Get_Loads_State = Get_State1_5;
            }
        }
    } else if (Get_Loads_State == Get_State1_5) {
        if (Stop_Flag_HuaGui == 1) {
            Get_Loads_State = Get_State2;
            Counter_Enable = 0;
        }
    } else if (Get_Loads_State == Get_State1) {
        if (Stop_Flag_HuaGui == 1 || Counter_Enable == 0) {
            Get_Loads_State = Get_State2;
            Counter_Enable = 0;
        }
    } else if (Get_Loads_State == Get_State2) {
        if (Dir == 0)
            ZaiWu_Turn(ZaiWu_Mid);
        else if (Dir == 1)
            ZaiWu_Turn(ZaiWu_Left);
        else if (Dir == 2)
            ZaiWu_Turn(ZaiWu_Right);

        uint8_t temp1 = JiaZhua_Turn(JiaZhua_Close);
        if (temp1 == 1) {
            Get_Loads_State = Get_State3;
            HuaGui_UP(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
            Counter_Enable = 1;
        }
    } else if (Get_Loads_State == Get_State3) {
        if (Stop_Flag_HuaGui == 1 || Counter_Enable == 0) {
            Get_Loads_State = Get_State4;
            Get_Loads_Return = 2;
            Counter_Enable = 0;
        }
    } else if (Get_Loads_State == Get_State4) {
        uint8_t temp2 = 0;
        if (Dir == 0)
            temp2 = ZaiWu_Turn(ZaiWu_Mid);
        else if (Dir == 1)
            temp2 = ZaiWu_Turn(ZaiWu_Left);
        else if (Dir == 2)
            temp2 = ZaiWu_Turn(ZaiWu_Right);

        uint8_t temp1 = HuaGui_Turn(HuaGui_IN);
        if (temp1 == 1 && temp2 == 1) {
            Get_Loads_State = Get_State5;
            HuaGui_Fang(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
        }
    } else if (Get_Loads_State == Get_State5) {
        if (Stop_Flag_HuaGui == 1) {
            Get_Loads_State = Get_State6;
        }
    } else if (Get_Loads_State == Get_State6) {
        uint8_t temp1 = JiaZhua_Turn(JiaZhua_Open);
        if (temp1 == 1) {
            Get_Loads_Return = 3;
            Get_Loads_State = Get_State7;
            HuaGui_UP(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
            Counter_Enable = 1;
        }
    } else if (Get_Loads_State == Get_State7) {
        if (Stop_Flag_HuaGui == 1 || Counter_Enable == 0) {
            Get_Loads_State = Get_State8;
            Counter_Enable = 0;
        }
    } else if (Get_Loads_State == Get_State8) {
        uint8_t temp1 = ZaiWu_Turn(ZaiWu_Mid);
        uint8_t temp2 = HuaGui_Turn(HuaGui_OUT);
        if (temp1 == 1 && temp2 == 1) {
            Get_Loads_State = End_State;
        }
    } else if (Get_Loads_State == End_State) {
        Get_Loads_State = Stop_State;
        Get_Loads_Return = 1;
    }
    return Get_Loads_Return;
}

//将转盘上的物块放置到置物台上
uint8_t Put_Loads(uint8_t Dir, uint8_t Tar, uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc) {
    Counter_Times = 115;
    uint8_t Put_Loads_Return = 0;
    static Put_Loads_States Put_Loads_State = Put_State;

    if (Put_Loads_State == Put_State) {
        HuaGui_UP(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
        Counter_Enable = 1;
        Put_Loads_State = Put_JiaoZhun_State;
    } else if (Put_Loads_State == Put_JiaoZhun_State) {
        if (Stop_Flag_HuaGui == 1 || Counter_Enable == 0) {
            Put_Loads_State = Put_Ready_State;
            Counter_Enable = 0;
        }
    } else if (Put_Loads_State == Put_Ready_State) {
        if (Stop_Flag_HuaGui == 1) {
            uint8_t temp1, temp2, temp3;
            if (Dir == 0)
                temp1 = ZaiWu_Turn(ZaiWu_Mid);
            else if (Dir == 1)
                temp1 = ZaiWu_Turn(ZaiWu_Left);
            else if (Dir == 2)
                temp1 = ZaiWu_Turn(ZaiWu_Right);
            temp2 = JiaZhua_Turn(JiaZhua_Open);
            temp3 = HuaGui_Turn(HuaGui_IN);
            if (temp1 == 1 && temp2 == 1 && temp3 == 1) {
                Put_Loads_State = Put_State1;
                HuaGui_Qu(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
            }
        }
    } else if (Put_Loads_State == Put_State1) {
        if (Stop_Flag_HuaGui == 1) {
            Put_Loads_State = Put_State2;
        }
    } else if (Put_Loads_State == Put_State2) {
        uint8_t temp1 = JiaZhua_Turn(JiaZhua_Close);
        if (temp1 == 1) {
            Put_Loads_State = Put_State3;
            HuaGui_UP(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
            Counter_Enable = 1;
        }
    } else if (Put_Loads_State == Put_State3) {
        if (Stop_Flag_HuaGui == 1 || Counter_Enable == 0) {
            Put_Loads_State = Put_State4;
            Counter_Enable = 0;
        }
    } else if (Put_Loads_State == Put_State4) {
        uint8_t temp1 = HuaGui_Turn(HuaGui_OUT);
        if (temp1 == 1) {
            if (Tar == 0) {
                HuaGui_DOWN(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
                Counter_Enable = 1;
                Put_Loads_State = Put_State5;
            } else if (Tar == 1) {
                HuaGui_DOWN2(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
                Put_Loads_State = Put_State5_5;
            }
        }
    } else if (Put_Loads_State == Put_State5_5) {
        uint8_t temp1 = ZaiWu_Turn(ZaiWu_Mid);

        if (Stop_Flag_HuaGui == 1 && temp1 == 1) {
            Put_Loads_State = Put_State6;
        }
    } else if (Put_Loads_State == Put_State5) {
        uint8_t temp1 = ZaiWu_Turn(ZaiWu_Mid);

        if (Stop_Flag_HuaGui == 1 && temp1 == 1 || Counter_Enable == 0) {
            Put_Loads_State = Put_State6;
            Counter_Enable = 0;
        }
    } else if (Put_Loads_State == Put_State6) {
        uint8_t temp1 = JiaZhua_Turn(JiaZhua_Open);
        if (temp1 == 1) {
            Put_Loads_State = Put_State7;
            HuaGui_UP(Motor_HuaGui_Speed, Motor_HuaGui_Acc);
            Counter_Enable = 1;
            Delay_ms(200);
            Put_Loads_Return = 2;
        }
    } else if (Put_Loads_State == Put_State7) {
        if (Stop_Flag_HuaGui == 1 || Counter_Enable == 0) {
            Put_Loads_State = Put_End_State;
            Counter_Enable = 0;
        }
    } else if (Put_Loads_State == Put_End_State) {
        Put_Loads_State = Put_State;
        Put_Loads_Return = 1;
    }
    return Put_Loads_Return;
}
