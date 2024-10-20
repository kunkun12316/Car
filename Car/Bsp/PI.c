#include "PI.h"
// @02!003+000#

uint8_t uart4_rxbuff;
char PI_RawData0[3];
char PI_RawData1[5];
char PI_RawData2[5];
uint8_t RxFlag = 0;

uint8_t PI_Data0 = 0;
int16_t PI_Data1;
int16_t PI_Data2;


volatile uint8_t uart_buffer[OP_BUFFER_SIZE];  // 环形缓冲区
volatile uint8_t write_pos = 0;  // 缓冲区写入位置
volatile uint8_t read_pos = 0;   // 缓冲区读取位置

void PI_Init(void) {
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart4, &uart4_rxbuff, 1);
}

//void PI_Receive_Data(char com_data[]) {
//    uint8_t i;
//    static uint8_t RxCounter = 0; // 计数
//    static uint8_t RxState = 0;
//
//    if (RxState == 0 && com_data[0] == '@' && RxFlag == 0) // @ 帧头
//    {
//        // 清空数据存储
//        memset(PI_RawData0, 0, sizeof(PI_RawData0));
//        memset(PI_RawData1, 0, sizeof(PI_RawData1));
//        memset(PI_RawData2, 0, sizeof(PI_RawData2));
//
//        // 提取数据
//        PI_RawData0[0] = com_data[1]; // @ 后两位
//        PI_RawData0[1] = com_data[2];
//        PI_RawData0[2] = '\0'; // 添加字符串结束符
//
//        PI_RawData1[0] = com_data[4]; // ! 后四位
//        PI_RawData1[1] = com_data[5];
//        PI_RawData1[2] = com_data[6];
//        PI_RawData1[3] = com_data[7];
//        PI_RawData1[4] = '\0'; // 添加字符串结束符
//
//        PI_RawData2[0] = com_data[9]; // | 后四位
//        PI_RawData2[1] = com_data[10];
//        PI_RawData2[2] = com_data[11];
//        PI_RawData2[3] = com_data[12];
//        PI_RawData2[4] = '\0'; // 添加字符串结束符
//
////        printf("PI_RawData0 : %s \n",PI_RawData0);
////        printf("PI_RawData1 : %s \n",PI_RawData1);
////        printf("PI_RawData2 : %s \n",PI_RawData2);
//
//
////        for (int j = 0; j < 14; ++j) {
////            printf("Rev %d : %c \n",j,com_data[j]);
////        }
//
//        RxFlag = 1;
//    }
//}

void Process_OP_Data(void) {
    if (read_pos != write_pos) {  // 读取到所有未处理的数据
        // 从缓冲区读取数据
        uint8_t data_buf = uart_buffer[read_pos];
        read_pos = (read_pos + 1) % OP_BUFFER_SIZE;  // 更新读取位置，环形读取

        printf("Read Pos : %d  ____  Write Pos : %d \n",read_pos,write_pos);

        // 调用数据解析函数
        PI_Receive_Data(data_buf);
    }
}



void PI_Receive_Data(uint8_t com_data) {
    static uint8_t RxCounter = 0;  // 计数
    static uint8_t RxState = 0;    // 状态机
    if (RxState == 0 && com_data == '@' && RxFlag == 0) {
        // 帧头检测到，清空数据
        memset(PI_RawData1, 0, sizeof(PI_RawData1));
        memset(PI_RawData2, 0, sizeof(PI_RawData2));
        memset(PI_RawData0, 0, sizeof(PI_RawData0));
        RxState = 1;
        RxCounter = 0;
    } else if (RxState == 1) {
        if (com_data == '!') {
            RxState = 2;
            RxCounter = 0;
        } else {
            PI_RawData0[RxCounter++] = com_data;
        }
    } else if (RxState == 2) {
        if (com_data == '|') {
            RxState = 3;
            RxCounter = 0;
        } else {
            PI_RawData1[RxCounter++] = com_data;
        }
    } else if (RxState == 3) {
        if (com_data == '#') {
            // 完整帧接收完毕，设置标志位，主循环处理数据
            RxFlag = 1;
            RxCounter = 0;
            RxState = 0;
        } else {
            PI_RawData2[RxCounter++] = com_data;
        }
    }
}



void PI_Data_Receive_Proc(void) {

    if (RxFlag == 1) {
        PI_Data0 = Char_To_Num(PI_RawData0[0]) * 10 + Char_To_Num(PI_RawData0[1]);
        if (PI_RawData1[0] == '+')
            PI_Data1 =
                    Char_To_Num(PI_RawData1[1]) * 100 + Char_To_Num(PI_RawData1[2]) * 10 + Char_To_Num(PI_RawData1[3]);
        else if (PI_RawData1[0] == '-')
            PI_Data1 =
                    -Char_To_Num(PI_RawData1[1]) * 100 - Char_To_Num(PI_RawData1[2]) * 10 - Char_To_Num(PI_RawData1[3]);

        if (PI_RawData2[0] == '+')
            PI_Data2 =
                    Char_To_Num(PI_RawData2[1]) * 100 + Char_To_Num(PI_RawData2[2]) * 10 + Char_To_Num(PI_RawData2[3]);
        else if (PI_RawData2[0] == '-')
            PI_Data2 =
                    -Char_To_Num(PI_RawData2[1]) * 100 - Char_To_Num(PI_RawData2[2]) * 10 - Char_To_Num(PI_RawData2[3]);
        RxFlag = 0;
#if Serial_Debug == 1
        printf("Task_Num:%d %d %d \r\n", PI_Data0, PI_Data1, PI_Data2);
#endif
    }
}

uint8_t Char_To_Num(char Char_Temp) {
    uint8_t temp;

    switch (Char_Temp) {
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
