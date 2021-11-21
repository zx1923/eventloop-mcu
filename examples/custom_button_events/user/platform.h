#ifndef __USER_PLATFORM_H__
#define __USER_PLATFORM_H__

#include "stdio.h"
#include "stm32f4xx_hal.h"

#define PIN_HIGH			GPIO_PIN_SET
#define PIN_LOW				GPIO_PIN_RESET

#define LED_R_PORT		GPIOB
#define LED_R_PIN			GPIO_PIN_0
#define LED_G_PORT		GPIOB
#define LED_G_PIN			GPIO_PIN_5

#define BTN_1_PORT		GPIOB
#define BTN_1_PIN			GPIO_PIN_1
#define BTN_2_PORT		GPIOB
#define BTN_2_PIN			GPIO_PIN_2

#define Bsp_Delay_Ms		HAL_Delay
#define Bsp_Gpio_Write	HAL_GPIO_WritePin
#define Bsp_Gpio_Read		HAL_GPIO_ReadPin

// Led Green Set State
#define Bsp_LedR_Write_High()		HAL_GPIO_WritePin(LED_R_PORT, LED_R_PIN, GPIO_PIN_SET)
#define Bsp_LedR_Write_Low()		HAL_GPIO_WritePin(LED_R_PORT, LED_R_PIN, GPIO_PIN_RESET)
#define Bsp_LedG_Write_High()		HAL_GPIO_WritePin(LED_G_PORT, LED_G_PIN, GPIO_PIN_SET)
#define Bsp_LedG_Write_Low()		HAL_GPIO_WritePin(LED_G_PORT, LED_G_PIN, GPIO_PIN_RESET)

// button read state
#define Bsp_Btn1_Read()					HAL_GPIO_ReadPin(BTN_1_PORT, BTN_1_PIN)
#define Bsp_Btn2_Read()					HAL_GPIO_ReadPin(BTN_2_PORT, BTN_2_PIN)

#endif
