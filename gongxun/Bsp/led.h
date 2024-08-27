#ifndef _LED_H__
#define _LED_H__

#include "main.h"
#include "gpio.h"

#define LED1(x) do{x ? HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET) : HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET) ; }while(0)
#define LED2(x) do{x ? HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET) : HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET) ; }while(0)

#define LED1_TOGGLE() do{HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);}while(0)
#define LED2_TOGGLE() do{HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);}while(0)

#endif