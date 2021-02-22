/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l4xx_hal.h"

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
#define PWR_SIM_Pin GPIO_PIN_0
#define PWR_SIM_GPIO_Port GPIOA
#define RTS_SIM_Pin GPIO_PIN_1
#define RTS_SIM_GPIO_Port GPIOA
#define RI_SIM_Pin GPIO_PIN_4
#define RI_SIM_GPIO_Port GPIOA
#define RI_SIM_EXTI_IRQn EXTI4_IRQn
#define SPEAKER_PIN_Pin GPIO_PIN_5
#define SPEAKER_PIN_GPIO_Port GPIOA
#define ALLOW_DETACH_PIN_Pin GPIO_PIN_6
#define ALLOW_DETACH_PIN_GPIO_Port GPIOA
#define ALLOW_DETACH_PIN_EXTI_IRQn EXTI9_5_IRQn
#define DETACH_PIN_Pin GPIO_PIN_7
#define DETACH_PIN_GPIO_Port GPIOA
#define DETACH_PIN_EXTI_IRQn EXTI9_5_IRQn
#define STAT_SIM_Pin GPIO_PIN_0
#define STAT_SIM_GPIO_Port GPIOB
#define PIN_PWR_Pin GPIO_PIN_11
#define PIN_PWR_GPIO_Port GPIOA
#define PIN_WAKEUP_Pin GPIO_PIN_12
#define PIN_WAKEUP_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
