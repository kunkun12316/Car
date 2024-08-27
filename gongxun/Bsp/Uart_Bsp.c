#include "Bsp.h"
#include <stdio.h>
#include <stdint.h>

extern JY_USART JY901_data;
extern OP_USART OpennMv_Data;

int16_t Car_Turn_angle;

void Usart_Init(void)
{
    //JY901S_Init();
    //OpenMV_Init();
}

//***************************************JY901S初始化************************************************************

JY_USART JY901_data;

struct SAcc stcAcc;
struct SGyro stcGyro;
struct SAngle stcAngle;

void JY901S_Init(void)
{
    JY901S_USART_Init(&JY901_data);

    __HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);

//    if (HAL_UART_Receive_DMA(&huart3, JY901_data.RxBuffer, RXBUFFER_LEN_JY) != HAL_OK)
//        Error_Handler();
}

//接收结构体初始化
void JY901S_USART_Init(JY_USART *Data) {
    for (uint16_t i = 0; i < RXBUFFER_LEN_JY; i++) Data->RxBuffer[i] = 0;
    Data->frame_head = 0x55;
    Data->Rx_flag = 0;
    Data->Rx_len = 0;
}

void JY901_Process() {
    if (JY901_data.Rx_len < RXBUFFER_LEN_JY) return;    //如果位数不对

    for (uint8_t i = 0; i < 4; i++) {
        if (JY901_data.RxBuffer[i * 11] != JY901_data.frame_head) return;    //如果帧头不对
        switch (JY901_data.RxBuffer[i * 11 + 1]) {
            case 0x51:
                memcpy(&stcAcc, &JY901_data.RxBuffer[2 + i * 11], 8);
                for (uint8_t j = 0; j < 3; j++)
                    JY901_data.acc.a[j] = (float) stcAcc.a[j] / 32768 * 16;             //官方加速度解算
                break;
            case 0x52:
                memcpy(&stcGyro, &JY901_data.RxBuffer[2 + i * 11], 8);
                for (uint8_t j = 0; j < 3; j++)
                    JY901_data.w.w[j] = (float) stcGyro.w[j] / 32768 * 2000;                //官方角速度解算
                break;
            case 0x53:
                memcpy(&stcAngle, &JY901_data.RxBuffer[2 + i * 11], 8);
                for (uint8_t j = 0; j < 3; j++)
                    JY901_data.angle.angle[j] = (float) stcAngle.Angle[j] / 32768 * 180;        //官方角度解算
                break;
        }

    }

}

//****************************************OpenMV初始化************************************************************


OP_USART OpennMv_Data;

//结构体初始化
void OP_USART_Init(OP_USART *Data)
{
    for(uint8_t i=0; i < RXBUFFER_LEN_OP; i++)	Data->RxBuffer[i] = 0;
    Data->frame_head[0] = 0x2C;
    Data->frame_head[1] = 0x12;
    Data->frame_tail = 0x5B;
    Data->frame_type_qr = 0x0A;
    Data->frame_type_color = 0x0B;
    Data->Rx_flag = 0;
    Data->Rx_len = 0;
    Data->x = 0;
    Data->y = 0;
    Data->w = 0;
    Data->h = 0;
}

void OpenMvData_Process(uint8_t *RxBuffer)
{
    //判断帧头帧尾
    if(RxBuffer[0] != OpennMv_Data.frame_head[0]) return;
    if(RxBuffer[1] != OpennMv_Data.frame_head[1]) return;
    if(RxBuffer[10] != OpennMv_Data.frame_tail) return;

    OpennMv_Data.x = (RxBuffer[3]<<8)|RxBuffer[2];
    OpennMv_Data.y = (RxBuffer[5]<<8)|RxBuffer[4];
    OpennMv_Data.w = (RxBuffer[7]<<8)|RxBuffer[6];
    OpennMv_Data.h = (RxBuffer[9]<<8)|RxBuffer[8];
}

uint8_t OpenMV_rxbuff;
uint8_t OpenMV_RxFlag = 0;
uint8_t OpenMV_Start_Char = 0x7C;
uint8_t OpenMV_End_Char = 0x7D;

uint8_t OpenMV_RawData_X[2] = {0};
uint8_t OpenMV_RawData_Y[2] = {0};
uint8_t OpenMV_RawData_Z[2] = {0};

int16_t OpenMV_Data_X;
int16_t OpenMV_Data_Y;
int16_t OpenMV_Data_Z;

void OpenMV_Init(void)
{
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart2, &OpenMV_rxbuff, 1);
}

void OpenMV_Receive_Data(uint8_t com_data)
{
    uint8_t i;
    static uint8_t RxCounter = 0; //计数
    static uint8_t RxState = 0;
    if(RxState == 0 && com_data == OpenMV_Start_Char && OpenMV_RxFlag == 0){
        for (int a = 0; a < 2; ++i) {
            OpenMV_RawData_X[a] = 0x00;
        }
        for (int a = 0; a < 2; ++i) {
            OpenMV_RawData_Y[a] = 0x00;
        }
        for (int a = 0; a < 2; ++i) {
            OpenMV_RawData_Z[a] = 0x00;
        }
        RxState = 1;
        RxCounter = 0;
    } else if(RxState == 1){
        if(com_data == 0x6A){
            RxState = 2;
        } else {
            OpenMV_RawData_X[RxCounter++] = com_data;
        }
    } else if (RxState == 2){
        if(com_data == 0x6B){
            RxState = 3;
            RxCounter = 0;
        } else{
            OpenMV_RawData_Y[RxCounter++] = com_data;
        }
    } else if(RxState == 3){
        if(com_data == OpenMV_End_Char){
            RxCounter = 0;
            RxState = 0;
            OpenMV_RxFlag = 1;
        } else{
            OpenMV_RawData_Z[RxCounter++] = com_data;
        }
    }
#if Serial_Debug == 1
    printf("Task_Num:%d %d %d \r\n", OpenMV_RawData_X, OpenMV_RawData_Y, OpenMV_RawData_Z);
#endif
}

void OpenMV_Data_Receive_Proc(void)
{
    if(OpenMV_RxFlag == 1){
        if (OpenMV_RawData_X[0] == 0x01){
            OpenMV_Data_X = OpenMV_RawData_X[1];
        } else{
            OpenMV_Data_X =  -(int16_t)OpenMV_RawData_X[1];
        }

        if (OpenMV_RawData_Y[0] == 0x01){
            OpenMV_Data_Y = OpenMV_RawData_Y[1];
        } else{
            OpenMV_Data_Y =  -(int16_t)OpenMV_RawData_Y[1];
        }

        if (OpenMV_RawData_Z[0] == 0x01){
            OpenMV_Data_Z = OpenMV_RawData_Z[1];
        } else{
            OpenMV_Data_Z =  -(int16_t)OpenMV_RawData_Z[1];
        }
    }
}








//****************************************OpenMV初始化************************************************************
uint8_t Task_num1[3] = {0};
uint8_t Task_num2[3] = {0};
uint8_t OpenMV_Type = 0;//保存OpenMV发来的数据类型 0 = 0x5A是偏移校准，1 = 0x5B是颜色





