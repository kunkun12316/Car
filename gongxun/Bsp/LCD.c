#include "LCD.h"

uint8_t LCD_Data[19] = {0x74, 0x30, 0x2E, 0x74, 0x78, 0x74, 0x3D, 0x22, 0x31, 0x33, 0x31, 0x2B, 0x31, 0x33, 0x33, 0x22, 0xFF, 0xFF, 0xFF};
// 123的排列组合
void LCD_Init(void)
{
    LCD_Data[8] = 0x2B;
    LCD_Data[9] = 0x2B;
    LCD_Data[10] = 0x2B;
    LCD_Data[12] = 0x2B;
    LCD_Data[13] = 0x2B;
    LCD_Data[14] = 0x2B;
    HAL_UART_Transmit(&huart3, LCD_Data, 19, 0xFFFFF);
}
void LCD_SendData(uint16_t temp1, uint16_t temp2)
{
    uint16_t temp = temp1;
    switch (temp)
    {
        case 123:
            LCD_Data[8] = 0x31;
            LCD_Data[9] = 0x32;
            LCD_Data[10] = 0x33;
            break;
        case 132:
            LCD_Data[8] = 0x31;
            LCD_Data[9] = 0x33;
            LCD_Data[10] = 0x32;
            break;
        case 213:
            LCD_Data[8] = 0x32;
            LCD_Data[9] = 0x31;
            LCD_Data[10] = 0x33;
            break;
        case 231:
            LCD_Data[8] = 0x32;
            LCD_Data[9] = 0x33;
            LCD_Data[10] = 0x31;
            break;
        case 312:
            LCD_Data[8] = 0x33;
            LCD_Data[9] = 0x31;
            LCD_Data[10] = 0x32;
            break;
        case 321:
            LCD_Data[8] = 0x33;
            LCD_Data[9] = 0x32;
            LCD_Data[10] = 0x31;
            break;
        default:
            break;
    }
    temp = temp2;
    switch (temp)
    {
        case 123:
            LCD_Data[12] = 0x31;
            LCD_Data[13] = 0x32;
            LCD_Data[14] = 0x33;
            break;
        case 132:
            LCD_Data[12] = 0x31;
            LCD_Data[13] = 0x33;
            LCD_Data[14] = 0x32;
            break;
        case 213:
            LCD_Data[12] = 0x32;
            LCD_Data[13] = 0x31;
            LCD_Data[14] = 0x33;
            break;
        case 231:
            LCD_Data[12] = 0x32;
            LCD_Data[13] = 0x33;
            LCD_Data[14] = 0x31;
            break;
        case 312:
            LCD_Data[12] = 0x33;
            LCD_Data[13] = 0x31;
            LCD_Data[14] = 0x32;
            break;
        case 321:
            LCD_Data[12] = 0x33;
            LCD_Data[13] = 0x32;
            LCD_Data[14] = 0x31;
            break;
        default:
            break;
    }

    HAL_UART_Transmit(&huart3, LCD_Data, 19, 0xFFFFF);
}
