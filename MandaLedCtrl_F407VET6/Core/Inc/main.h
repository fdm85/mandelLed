/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void SystemClock_Config(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Key1_Pin GPIO_PIN_3
#define Key1_GPIO_Port GPIOE
#define Key0_Pin GPIO_PIN_4
#define Key0_GPIO_Port GPIOE
#define RL_Pin GPIO_PIN_0
#define RL_GPIO_Port GPIOC
#define RM_Pin GPIO_PIN_1
#define RM_GPIO_Port GPIOC
#define RH_Pin GPIO_PIN_2
#define RH_GPIO_Port GPIOC
#define LL_Pin GPIO_PIN_1
#define LL_GPIO_Port GPIOA
#define LM_Pin GPIO_PIN_2
#define LM_GPIO_Port GPIOA
#define LH_Pin GPIO_PIN_3
#define LH_GPIO_Port GPIOA
#define UsrLed1_Pin GPIO_PIN_6
#define UsrLed1_GPIO_Port GPIOA
#define UsrLed2_Pin GPIO_PIN_7
#define UsrLed2_GPIO_Port GPIOA
#define LCD_BL_Pin GPIO_PIN_1
#define LCD_BL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
