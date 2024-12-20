/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../Bsp/Bsp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */


  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

    uint32_t temp_flag = 0;
    uint32_t temp;
    temp_flag = __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE);
    if ((temp_flag != RESET)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        HAL_UART_DMAStop(&huart3);                                            //关闭DMA，防止冲�?
        temp = hdma_usart3_rx.Instance->NDTR;                        //获取DMA中未传输的数据个�?
        JY901_data.Rx_len = RXBUFFER_LEN - temp;                                            //获得�?共传输的个数
        JY901_Process();
        LED2_TOGGLE();
    }
    HAL_UART_Receive_DMA(&huart3, JY901_data.RxBuffer, RXBUFFER_LEN);


    //配置只接收加速度和角�?
    /*
      if(JY_rx_buffer[0] == 0x55 && JY_rx_buffer[1] == 0x51 && JY_rx_buffer[11] == 0x55 && JY_rx_buffer[12] == 0x53)
      {
          Angle_speed_temp_x = (float)(JY_rx_buffer[3] << 8 | JY_rx_buffer[2])/32768 * 16;
          Angle_speed_temp_y = (float)(JY_rx_buffer[5] << 8 | JY_rx_buffer[4])/32768 * 16;
          Angle_speed_temp_z = (float)(JY_rx_buffer[7] << 8 | JY_rx_buffer[6])/32768 * 16;

          Angle_temp_x = (float)(JY_rx_buffer[14] << 8 | JY_rx_buffer[13]) / 32768 * 180;
          Angle_temp_y = (float)(JY_rx_buffer[16] << 8 | JY_rx_buffer[15]) / 32768 * 180;
          Angle_temp_z = (float)(JY_rx_buffer[18] << 8 | JY_rx_buffer[17]) / 32768 * 180;

          Last_Yaw = Yaw;
          Yaw += (float)Yaw_Compensate;
          JY_Rx_Flag = 1;
      }

      HAL_UART_Receive_DMA(&huart3, JY_rx_buffer, 22);
  */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */

  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */

  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

#define BUFFER_SIZE 15
char uart4_buffer[BUFFER_SIZE];
uint8_t uart4_index = 0;
uint8_t usart_rev_flag = 0; // 状态变量，表示是否开始接收


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

    uint8_t tem = 0;
    if (huart->Instance == USART1) {
        tem = Motor_RxBuff;
        Motor_Receive_Data(tem);

        HAL_UART_Receive_IT(&huart1, &Motor_RxBuff, 1);
    }

    if (huart->Instance == UART4) {
//        char rxChar = uart4_rxbuff;
//
//        // 存储接收到的字符
//        if (rxChar == '@') {
//            uart4_buffer[0] = rxChar; // 将 '@' 放到缓冲区开头
//            uart4_index = 1; // 重置索引，从1开始接收后续字符
//            usart_rev_flag = 1; // 设置接收 状态为真
//        } else if (usart_rev_flag && uart4_index < BUFFER_SIZE - 1) {
//            uart4_buffer[uart4_index++] = rxChar; // 添加字符到缓冲区
//            uart4_buffer[uart4_index] = '\0'; // 末尾添加字符串结束符
//        }
//
//        if (rxChar == '#' && uart4_index > 0) {
//            printf("len_str : %d \n", strlen(uart4_buffer));
//            printf("Received string: %s\n", uart4_buffer); // 输出当前字符串
//
//            PI_Receive_Data(uart4_buffer);
//
//            // 重置索引以接收下一个字符串
//            usart_rev_flag = 0;
//            uart4_index = 0;
//        }
//
//        tem = uart4_rxbuff;
//        printf("%d ",tem);

//        char uart_data = uart4_rxbuff;
//        printf("%c ", uart_data);

//         将数据存入缓冲区
        uart_buffer[write_pos] = uart4_rxbuff;
        write_pos = (write_pos + 1) % OP_BUFFER_SIZE;  // 更新写入位置，且实现环形

// //PI_Receive_Data(uart4_rxbuff);

        HAL_UART_Receive_IT(&huart4, &uart4_rxbuff, 1);
    }
    if (huart->Instance == USART2) {
        printf("Qr Read !\n");
        if (memcmp(gm65_buf, hydata, 7) == 0) {
            printf("Qr Start Read !\n");
            HAL_UART_Receive(&huart2, gm65_buf, 8, 0xfff);
            Gm65_RxFlag = 1;
        }
    }
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart2) {
        HAL_UART_Receive_IT(&huart2, gm65_buf, 7);
    }
}

/* USER CODE END 1 */
