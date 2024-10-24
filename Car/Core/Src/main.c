/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../Bsp/Bsp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
    RetargetInit(&huart5);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_USART3_UART_Init();
  MX_UART4_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
    Bsp_Init();

    printf("Start OK!\r\n");
    uint8_t time = 0;
    uint8_t flag = 0;

//Task_Num_0

    printf("Tar send!\r\n");

    static uint8_t flag_huagui = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

        //printf("Yaw : %f\n",Yaw);

//        printf(" x : %.2f", JY901_data.angle.angle[0]);
//        printf(" y : %.2f", JY901_data.angle.angle[1]);
//        printf(" z : %.2f \n", JY901_data.angle.angle[2]);

//        printf(" SP_x : %.2f", JY901_data.acc.a[0]);
//        printf(" SP_y : %.2f", JY901_data.acc.a[1]);
//        printf(" SP_z : %.2f \r\n", JY901_data.acc.a[2]);

//        if (HuaGui_Init_State == 0) {
//            static uint8_t num = 0;
//            if (num == 0 && Running_Flag == 0) {
//                //滑轨向外
//                printf("HuaGui OUT!\n");
//                Task_Num_0 = 4;
//                Task_Num_1 = 1;
//                num = 1;
//            }
//            else if (num == 1 && Running_Flag == 0) {
//                //出启动区
//                Task_Num_0 = 1;
//                Task_Num_1 = -21;
//                Task_Num_2 = 65;
//                num = 2;
//            }
//            else if (num == 1 && Running_Flag == 0) {
//                printf("Jia wukuai!\n");
//                Task_Num_0 = 5;
//                Task_Num_1 = 1;
//                Task_Num_2 = 0;
//                num = 2;
//            }
//            else if (num == 2 && Running_Flag == 0) {
//                //行驶到物料
//                Task_Num_0 = 6;
//                Task_Num_1 = 1;
//                Task_Num_2 = 0;
//                num = 3;
//            }
//            else if (num == 3 && Running_Flag == 0) {
//                //行驶到物料
//                Task_Num_0 = 1;
//                Task_Num_1 = 10;
//                Task_Num_2 = 83;
//                num = 4;
//            }
//            else if (num == 4 && Running_Flag == 0) {
//                //车身角度回正
//                Task_Num_0 = 7;
//                num = 5;
//            } else if (num == 5 && Running_Flag == 0) {
//                //到拐角
//                Task_Num_0 = 1;
//                Task_Num_1 = -5;
//                Task_Num_2 = 42;
//                num = 6;
//            } else if (num == 6 && Running_Flag == 0) {
//                Task_Num_0 = 2;
//                Task_Num_1 = 90;
//                num = 7;
//            } else if (num == 7 && Running_Flag == 0) {
//                Task_Num_0 = 1;
//                Task_Num_1 = 10;
//                Task_Num_2 = 75;
//                num = 8;
//            } else if (num == 8 && Running_Flag == 0) {
//                Task_Num_0 = 7;
//                num = 9;
//            }
//        }

        Key_Proc();
        Process_OP_Data();
        PI_Data_Receive_Proc();
        HuaGui_Init_Proc();
        Control_Proc();
        process_kalman_filter();

//        if (HuaGui_Init_State == 0) {
////            printf("HuaGui Task 0!\n");
////            JiaZhua_Turn(JiaZhua_Close);
////            HuaGui_Turn(HuaGui_OUT);
////            ZaiWu_Turn(ZaiWu_Right);
////            flag_huagui = 1;
//
//
//
//            HuaGui_Init_State = 20;
//        } else if (HuaGui_Init_State == 10 && Running_Flag == 0) {
//
//            PI_Data0 = 10;
//            PI_Data1 = 0;
//            PI_Data2 = 0;
//
//            HuaGui_Init_State = 20;
//        } else if (HuaGui_Init_State == 20 && Running_Flag == 0) {
//
//            PI_Data0 = 1;
//            PI_Data1 = -30;
//            PI_Data2 = 100;
//
//            HuaGui_Init_State = 30;
//        } else if (HuaGui_Init_State == 30 && Running_Flag == 0) {
//            PI_Data0 = 1;
//            PI_Data1 = 40;
//            PI_Data2 = 50;
//
//            HuaGui_Init_State = 40;
//        } else if (HuaGui_Init_State == 40 && Running_Flag == 0){
//            PI_Data0 = 2;
//            PI_Data1 = 90;
//            PI_Data2 = 0;
//
//            HuaGui_Init_State = 50;
//        }else if (HuaGui_Init_State == 50 && Running_Flag == 0){
//            PI_Data0 = 7;
//            PI_Data1 = 0;
//            PI_Data2 = 0;
//            HuaGui_Init_State = 60;
//        }else if (HuaGui_Init_State == 60 && Running_Flag == 0){
//            PI_Data0 = 2;
//            PI_Data1 = 90;
//            PI_Data2 = 0;
//            HuaGui_Init_State = 70;
//
//        }else if (HuaGui_Init_State == 70 && Running_Flag == 0) {
//            PI_Data0 = 7;
//            PI_Data1 = 0;
//            PI_Data2 = 0;
//            HuaGui_Init_State = 100;
//        }

//        Delay_ms(100);

    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
