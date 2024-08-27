#include "Control_Logic.h"
#include "Bsp.h"

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

    if(Running_Flag == 0 && Running_Task_Flag == 0){
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


/*
 * 前往二维码区
 * 1.车辆行驶到二维码检测区域
 * 2.扫描二维码，结果保存为task[0],task[1]两个数组
 */
void move_to_qr(void) {
    static uint8_t number = 0;
    Running_Task_Flag = 1;
    if(number == 0)
    {
        //x偏移-21单位
        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = -21;
        Car_Task_Data_2 = 0;
        number = 1;
        Car_Task_Time = 0;
    } else if(number == 1 && Car_Task_Time >= 400){
        //y轴偏移65个单位
        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = 0;
        Car_Task_Data_2 = 65;
        number = 2;
        Car_Task_Time = 0;
        Running_Task_Flag = 0;
    }

}

void qr_task(void) {

    //扫码
}

/*
 * 前往原料区
 * 1.车辆行驶到原料区域
 * 2.第一次到达原料区任务
 */
void move_to_material(void) {
    static uint8_t number = 0;
    Running_Task_Flag = 1;
    if(number == 0){
        //将扫码到的数据发送给OpenMV
        number = 1;
    } else if(number == 1){
        //滑轨朝外
        Car_Task_Data_0 = 4;
        Car_Task_Data_1 = 1;
        Car_Task_Data_2 = 0;
        number = 2;
        Car_Task_Time = 0;
    } else if(number == 2)
    {
        //X,Y轴都偏移
        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = 10;
        Car_Task_Data_2 = 83;
        number = 0;
        Car_Task_Time = 0;
        Running_Task_Flag = 0;
    }


}

void material_first_times_task(void) {
    /*
     * OpenMV任务
     * 1.先校准
     * 2.然后判断颜色
     * 3.逐次将检测到的颜色代号，发给下位机
     */

    Running_Task_Flag = 1;
    static uint8_t number = 0;
    static uint8_t fre = 0; //滑轨夹物块的次数
    static uint8_t temp_color[3] = {0};//需要设计一个函数解析OpenMV发过来的数据中，是哪个颜色
    int16_t temp1,temp2 = 0;//保存OpenMV偏移数据
    if(number == 0){
        //接收OpenMV返回数据，否则一直等待
        //校准，移动x，y

        if(OpenMV_Type == 0){
            number = 3;
        } else if(OpenMV_Type == 1){
            number = 2;
        }
        number = 1;
        fre++;
    }
    else if (number == 1){
        if(fre <= 2){
            Car_Task_Data_0 = 5;
            Car_Task_Data_1 = temp_color[fre];
            Car_Task_Data_2 = 1;
            number = 2;
            Car_Task_Time = 0;
        } else {
            Running_Task_Flag = 0;
        }
    } else if(number == 3){

    }
}

/*
 * 前往粗加工区
 * 1.车辆行驶到暂存区域
 * 2.第一次到达暂存区任务
 */
void move_to_temp(void) {
    static uint8_t number = 0;

    if(number == 0){
        //车身回正
        Car_Task_Data_0 = 7;
        Car_Task_Data_1 = 0;
        Car_Task_Data_2 = 0;
        Car_Task_Time = 0;
        number = 1;
    } else if (number == 1 && Car_Task_Time >= 200){
        //X,Y偏移
        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = -5;
        Car_Task_Data_2 = 42;
        number = 2;
        Car_Task_Time = 0;
    } else if(number == 2 && Car_Task_Time >= 200){
        //车身旋转90°
        Car_Task_Data_0 = 2;
        Car_Task_Data_1 = 90;
        Car_Task_Data_2 = 0;
        number = 3;
        Car_Task_Time = 0;
    } else if(number == 3 && Car_Task_Time >= 200){
        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = 10;
        Car_Task_Data_2 = 75;
        number = 4;
        Car_Task_Time = 0;
    } else if(number == 4 && Car_Task_Time >= 200){
        //车身回正
        Car_Task_Data_0 = 7;
        Car_Task_Data_1 = 0;
        Car_Task_Data_2 = 0;
        Car_Task_Time = 0;
        number = 0;
        Running_Task_Flag = 0;
    }

}

void temp_first_times_task(void) {
    /*
     * OpenMV任务
     * 1.圆心校准
     */
    static uint8_t number = 0;

    //发送给OpenMV圆心校准的s指令

    int16_t temp1,temp2 = 0;//保存OpenMV偏移数据
    if(number == 0){//需要等待OpenMV返回数据并根据状态判断
        Car_Task_Data_0 = 1;
        Car_Task_Data_1 = temp1;
        Car_Task_Data_2 = temp2;
        number = 1;
        Car_Task_Time = 0;
    } else if(number == 1 && Car_Task_Time >= 400){
//        Car_Task_Data_0 =
    }


}

/*
 * 前往暂存区
 * 1.车辆行驶到存放区域
 * 2.第一次到达装载区任务
 */
void move_to_store(void) {

}

void store_first_times_task(void) {

}

/*
 * 返回原料区
 * 1.返回原料区
 * 2.第二次到达原料区任务
 */
void back_to_material(void) {

}

void material_second_times_task(void) {

}

/*
 * 第二次到达粗加工区
 * 1.第二次到达暂存区任务
 */
void temp_second_times_task(void) {

}

/*
 *第二次到达装载区任务
 * 1.第二次到达装载区任务
 */
void store_second_times_task(void) {

}

/*
 * 返回起点
 * 1.返回起点
 */

void back_to_begin(void) {

}



