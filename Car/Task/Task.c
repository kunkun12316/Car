#include "Task.h"

void Task_Proc()
{
    static uint8_t task_num = 0;

    if (Running_Flag == 0)
    {
        if(Motor_Stop_Flag_Car == 1 && Last_Motor_Stop_Flag_Car == 0 && Motor_Stop_Flag_Car_Kalman == 1)
        {
            if (task_num == 0)
            {
                Task_Num_0 = 1;
                Task_Num_1 = -21;
                Task_Num_2 = 0;
                task_num = 1;
            } else if (task_num == 1)
            {
                Task_Num_0 = 1;
                Task_Num_1 = 0;
                Task_Num_2 = 65;
                task_num = 2;
            } else if (task_num == 2)
            {
                Task_Num_0 = 1;
                Task_Num_1 = 10;
                Task_Num_2 = 83;
                task_num = 3;
            } else if (task_num == 3)
            {

            }
        }
    }

    if(Motor_Stop_Flag_Car == 1 && Last_Motor_Stop_Flag_Car == 0 && Motor_Stop_Flag_Car_Kalman == 1)
    {

    }
}