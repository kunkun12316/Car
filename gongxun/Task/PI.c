#include "PI.h"
// @02!003+000#

uint8_t uart4_rxbuff;
uint8_t PI_RawData0[2];
uint8_t PI_RawData1[4];
uint8_t PI_RawData2[4];
uint8_t RxFlag = 0;

uint8_t PI_Data0 = 0;
int16_t PI_Data1;
int16_t PI_Data2;

void PI_Init(void)
{
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart4, &uart4_rxbuff, 1);
}

void PI_Receive_Data(uint8_t com_data)
{
    uint8_t i;
    static uint8_t RxCounter = 0; // 计数
    static uint8_t RxState = 0;
    if (RxState == 0 && com_data == '@' && RxFlag == 0) // @ 帧头
    {
        for (i = 0; i < 2; i++)
        {
            PI_RawData0[i] = 0x00; // 将存放数据数组清零
        }
        for (i = 0; i < 4; i++)
        {
            PI_RawData1[i] = 0x00; // 将存放数据数组清零
        }
        for (i = 0; i < 4; i++)
        {
            PI_RawData2[i] = 0x00; // 将存放数据数组清零
        }
        RxState = 1;
        RxCounter = 0;
    }
    else if (RxState == 1)
    {
        if (com_data == '!')
        {
            RxState = 2;
            RxCounter = 0;
        }
        else
        {
            PI_RawData0[RxCounter++] = com_data;
        }
    }

    else if (RxState == 2)
    {
        if (com_data == '|')
        {
            RxState = 3;
            RxCounter = 0;
        }
        else
        {
            PI_RawData1[RxCounter++] = com_data;
        }
    }
    else if (RxState == 3)
    {
        if (com_data == '#')
        {
            RxCounter = 0;
            RxState = 0;
            RxFlag = 1;
        }
        else
        {
            PI_RawData2[RxCounter++] = com_data;
        }
    }
}
void PI_Data_Receive_Proc(void)
{

    if (RxFlag == 1)
    {
        PI_Data0 = Char_To_Num(PI_RawData0[0]) * 10 + Char_To_Num(PI_RawData0[1]);
        if (PI_RawData1[0] == '+')
            PI_Data1 = Char_To_Num(PI_RawData1[1]) * 100 + Char_To_Num(PI_RawData1[2]) * 10 + Char_To_Num(PI_RawData1[3]);
        else if (PI_RawData1[0] == '-')
            PI_Data1 = -Char_To_Num(PI_RawData1[1]) * 100 - Char_To_Num(PI_RawData1[2]) * 10 - Char_To_Num(PI_RawData1[3]);

        if (PI_RawData2[0] == '+')
            PI_Data2 = Char_To_Num(PI_RawData2[1]) * 100 + Char_To_Num(PI_RawData2[2]) * 10 + Char_To_Num(PI_RawData2[3]);
        else if (PI_RawData2[0] == '-')
            PI_Data2 = -Char_To_Num(PI_RawData2[1]) * 100 - Char_To_Num(PI_RawData2[2]) * 10 - Char_To_Num(PI_RawData2[3]);
        RxFlag = 0;
#if Serial_Debug == 1
        printf("Task_Num:%d %d %d \r\n", PI_Data0, PI_Data1, PI_Data2);
#endif
    }
}
uint8_t Char_To_Num(uint8_t Char_Temp)
{
    uint8_t temp;

    switch (Char_Temp)
    {
        case '0':
            temp = 0;
            break;
        case '1':
            temp = 1;
            break;
        case '2':
            temp = 2;
            break;
        case '3':
            temp = 3;
            break;
        case '4':
            temp = 4;
            break;
        case '5':
            temp = 5;
            break;
        case '6':
            temp = 6;
            break;
        case '7':
            temp = 7;
            break;
        case '8':
            temp = 8;
            break;
        case '9':
            temp = 9;
            break;
    }
    return temp;
}
