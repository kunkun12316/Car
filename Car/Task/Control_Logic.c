#include "Control_Logic.h"
#include "../Bsp/Bsp.h"

/*
 * 初始化()
 * 前往二维码区
 * 前往原料区
 * 前往粗加工区
 * 前往暂存区
 * 返回原料区
 * 前往粗加工区
 * 前往暂存区
 * 返回起点
 */

uint8_t Car_Task_Data_0 = 0;
int16_t Car_Task_Data_1 = 0;
int16_t Car_Task_Data_2 = 0;

uint8_t Running_Task_Flag = 0;


void Tack_mode_Proc(void) {
    static uint8_t mode = 0;

    if (Running_Flag == 0 && Running_Task_Flag == 0) {
        mode++;
    }

    if (Running_Flag == 0 && Running_Task_Flag == 0) {
        switch (mode) {
            case 0:
                move_to_qr();
                break;
            case 1:
                qr_task();
                break;
            case 2:
                move_to_material();
                break;
            case 3:
                material_first_times_task();
                break;
            case 4:
                move_to_temp();
                break;
            case 5:
                temp_first_times_task();
                break;
            case 6:
                move_to_store();
                break;
            case 7:
                store_first_times_task();
                break;
            case 8:
                back_to_material();
                break;
            case 9:
                material_second_times_task();
                break;
            case 10:
                move_to_temp();
                break;
            case 11:
                temp_second_times_task();
                break;
            case 12:
                move_to_store();
                break;
            case 13:
                store_second_times_task();
                break;
            case 14:
                back_to_begin();
                break;
        }
    }
}

void Tack_mode_Proc_Che(void) {
    static uint8_t temp = 0;
    if (temp == 0) {
//移动到扫码区
        printf("Task : %d\r\n",temp);
        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = -21;
        Car_Task_Data_2 = 0;
        temp = 1;

    } else if (temp == 1 && Motor_Stop_Flag_Car == 1 && Running_Flag == 0) {
        printf("Task : %d\r\n",temp);

        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = 0;
        Car_Task_Data_2 = 65;
        temp = 2;

    } else if (temp == 2 && Motor_Stop_Flag_Car == 1 && Running_Flag == 0) {
//移动到原料区
        printf("Task : %d\r\n",temp);

        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = 10;
        Car_Task_Data_2 = 0;
        temp = 3;

    } else if (temp == 3 && Motor_Stop_Flag_Car == 1 && Running_Flag == 0) {

        printf("Task : %d\r\n",temp);

        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = 0;
        Car_Task_Data_2 = 83;
        temp = 4;

    } else if (temp == 4 && Motor_Stop_Flag_Car == 1 && Running_Flag == 0) {
//移动到粗加工区
        printf("Task : %d\r\n",temp);

        Car_Task_Data_0 = 7;
        Car_Task_Data_1 = 0;
        Car_Task_Data_2 = 0;
        temp = 5;

    } else if (temp == 5 && Motor_Stop_Flag_Car == 1 && Running_Flag == 0) {
        printf("Task : %d\r\n",temp);

        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = -5;
        Car_Task_Data_2 = 0;
        temp = 6;

    } else if (temp == 6 && Motor_Stop_Flag_Car == 1 && Running_Flag == 0) {
        printf("Task : %d\r\n",temp);

        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = 0;
        Car_Task_Data_2 = 42;
        temp = 7;

    } else if (temp == 7 && Motor_Stop_Flag_Car == 1 && Running_Flag == 0) {
        printf("Task : %d\r\n",temp);

        Car_Task_Data_0 = 2;
        Car_Task_Data_1 = 90;
        Car_Task_Data_2 = 0;
        temp = 8;

    } else if (temp == 8 && Motor_Stop_Flag_Car == 1 && Running_Flag == 0) {
        printf("Task : %d\r\n",temp);

        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = 10;
        Car_Task_Data_2 = 0;
        temp = 9;

    } else if (temp == 9 && Motor_Stop_Flag_Car == 1 && Running_Flag == 0) {
        printf("Task : %d\r\n",temp);

        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = 0;
        Car_Task_Data_2 = 75;
        temp = 10;

    } else if (temp == 10 && Motor_Stop_Flag_Car == 1 && Running_Flag == 0) {
        printf("Task : %d \r\n",temp);

        Car_Task_Data_0 = 7;
        Car_Task_Data_1 = 0;
        Car_Task_Data_2 = 0;
        temp = 100;
        printf("Task End!!!\r\n");
    }
}

