/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"
#include "conversions.h"
#include "DS3231.h"
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define led_Pin GPIO_PIN_13
#define led_GPIO_Port GPIOC
#define kn1_Pin GPIO_PIN_4
#define kn1_GPIO_Port GPIOA
#define gerk1_Pin GPIO_PIN_0
#define gerk1_GPIO_Port GPIOB
#define gerk2_Pin GPIO_PIN_1
#define gerk2_GPIO_Port GPIOB
#define proj_Pin GPIO_PIN_10
#define proj_GPIO_Port GPIOB
#define out5v_Pin GPIO_PIN_11
#define out5v_GPIO_Port GPIOB
#define en1_Pin GPIO_PIN_12
#define en1_GPIO_Port GPIOB
#define dir1_Pin GPIO_PIN_13
#define dir1_GPIO_Port GPIOB
#define step1_Pin GPIO_PIN_14
#define step1_GPIO_Port GPIOB
#define datLight_Pin GPIO_PIN_15
#define datLight_GPIO_Port GPIOB
#define en2_Pin GPIO_PIN_9
#define en2_GPIO_Port GPIOA
#define dir2_Pin GPIO_PIN_10
#define dir2_GPIO_Port GPIOA
#define step2_Pin GPIO_PIN_11
#define step2_GPIO_Port GPIOA
#define kn2_Pin GPIO_PIN_12
#define kn2_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
