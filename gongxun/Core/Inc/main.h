/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#define Serial_Debug 0

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE
#define KEY3_Pin GPIO_PIN_4
#define KEY3_GPIO_Port GPIOE
#define BUZ_Pin GPIO_PIN_5
#define BUZ_GPIO_Port GPIOE
#define BUSY_Pin GPIO_PIN_6
#define BUSY_GPIO_Port GPIOE
#define LED_1_Pin GPIO_PIN_0
#define LED_1_GPIO_Port GPIOF
#define LED_2_Pin GPIO_PIN_1
#define LED_2_GPIO_Port GPIOF
#define KEY_1_Pin GPIO_PIN_2
#define KEY_1_GPIO_Port GPIOF
#define KEY_2_Pin GPIO_PIN_3
#define KEY_2_GPIO_Port GPIOF
#define KEY_3_Pin GPIO_PIN_4
#define KEY_3_GPIO_Port GPIOF
#define KEY_4_Pin GPIO_PIN_5
#define KEY_4_GPIO_Port GPIOF
#define PI_1_Pin GPIO_PIN_6
#define PI_1_GPIO_Port GPIOF
#define PI_2_Pin GPIO_PIN_7
#define PI_2_GPIO_Port GPIOF
#define PI_3_Pin GPIO_PIN_8
#define PI_3_GPIO_Port GPIOF
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOF
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOF
#define K_WK_UP_Pin GPIO_PIN_0
#define K_WK_UP_GPIO_Port GPIOA
#define PI_RX_Pin GPIO_PIN_1
#define PI_RX_GPIO_Port GPIOA
#define OpenMV_TX_Pin GPIO_PIN_2
#define OpenMV_TX_GPIO_Port GPIOA
#define OpenMV_RX_Pin GPIO_PIN_3
#define OpenMV_RX_GPIO_Port GPIOA
#define Servo_A_Pin GPIO_PIN_6
#define Servo_A_GPIO_Port GPIOA
#define Servo_B_Pin GPIO_PIN_7
#define Servo_B_GPIO_Port GPIOA
#define Servo_C_Pin GPIO_PIN_0
#define Servo_C_GPIO_Port GPIOB
#define Servo_D_Pin GPIO_PIN_1
#define Servo_D_GPIO_Port GPIOB
#define Motor_Z_Pin GPIO_PIN_9
#define Motor_Z_GPIO_Port GPIOE
#define JY_TX_Pin GPIO_PIN_10
#define JY_TX_GPIO_Port GPIOB
#define JY_RX_Pin GPIO_PIN_11
#define JY_RX_GPIO_Port GPIOB
#define Motor_A_ENA_Pin GPIO_PIN_8
#define Motor_A_ENA_GPIO_Port GPIOD
#define Motor_B_ENA_Pin GPIO_PIN_9
#define Motor_B_ENA_GPIO_Port GPIOD
#define Motor_C_ENA_Pin GPIO_PIN_10
#define Motor_C_ENA_GPIO_Port GPIOD
#define Motor_D_ENA_Pin GPIO_PIN_11
#define Motor_D_ENA_GPIO_Port GPIOD
#define Motor_Z_ENA_Pin GPIO_PIN_12
#define Motor_Z_ENA_GPIO_Port GPIOD
#define Motor_A_DIR_Pin GPIO_PIN_2
#define Motor_A_DIR_GPIO_Port GPIOG
#define Motor_B_DIR_Pin GPIO_PIN_3
#define Motor_B_DIR_GPIO_Port GPIOG
#define Motor_C_DIR_Pin GPIO_PIN_4
#define Motor_C_DIR_GPIO_Port GPIOG
#define Motor_D_DIR_Pin GPIO_PIN_5
#define Motor_D_DIR_GPIO_Port GPIOG
#define Motor_Z_DIR_Pin GPIO_PIN_6
#define Motor_Z_DIR_GPIO_Port GPIOG
#define Motor_A_Pin GPIO_PIN_6
#define Motor_A_GPIO_Port GPIOC
#define Motor_B_Pin GPIO_PIN_7
#define Motor_B_GPIO_Port GPIOC
#define Motor_C_Pin GPIO_PIN_8
#define Motor_C_GPIO_Port GPIOC
#define Motor_D_Pin GPIO_PIN_9
#define Motor_D_GPIO_Port GPIOC
#define Motor_TX_Pin GPIO_PIN_9
#define Motor_TX_GPIO_Port GPIOA
#define Motor_RX_Pin GPIO_PIN_10
#define Motor_RX_GPIO_Port GPIOA
#define PI_TX_Pin GPIO_PIN_10
#define PI_TX_GPIO_Port GPIOC
#define Debug_TX_Pin GPIO_PIN_12
#define Debug_TX_GPIO_Port GPIOC
#define Debug_RX_Pin GPIO_PIN_2
#define Debug_RX_GPIO_Port GPIOD
#define GM65_RX_Pin GPIO_PIN_9
#define GM65_RX_GPIO_Port GPIOG
#define GM65_TX_Pin GPIO_PIN_14
#define GM65_TX_GPIO_Port GPIOG
#define OLED_SCL_Pin GPIO_PIN_8
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_9
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
