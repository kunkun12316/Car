#include "gm65.h"

uint8_t Gm65_RxFlag = 0;
uint8_t data[19]="t0.txt=\"000+000\"\xff\xff\xff";
uint8_t gm65_buf[8]={0};
uint8_t fssj[] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, 0xAB, 0xCD};
uint8_t GM65_Send_Data[9] = {0x7E,0x00,0x08,0x01,0x00,0x02,0x01,0xAB,0xCD};

uint8_t hydata[7] = {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

void GM65_Init(void)
{
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    HAL_UART_Transmit_IT(&huart2, GM65_Send_Data, 9);
}

void GM65_QR_Send(void)
{
    uint8_t asjdha[] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, 0xAB, 0xCD};
    HAL_UART_Transmit_IT(&huart2, fssj, sizeof(asjdha));
}

uint8_t ASC_TO_INT(uint8_t ASC_Data)
{
    switch (ASC_Data) {
        case 49:
            return 1;
        case 50:
            return 2;
        case 51:
            return 3;
    }
}

void UART_LCD(void)
{
//    __HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);
//    HAL_UART_Receive_IT(&huart6, data, sizeof(data));
}

