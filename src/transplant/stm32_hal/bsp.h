#ifndef __USER_EL_BSP_H__
#define __USER_EL_BSP_H__

#include "stm32f4xx_hal.h"
#include "sys_global.h"

#ifndef NULL
#define NULL  0
#endif // NULL

/* Time-delay function and tick timer definition */
#define Bsp_Get_Tick            HAL_GetTick
#define Bsp_Delay_Ms            HAL_Delay

typedef enum
{
  EVENT_NONE = 0,
  EVENT_EL_LOAD,
#ifdef ENABLE_BUTTON_DEVICE
  EVENT_BTN_PRESS,
  EVENT_BTN_LONG_PRESS,
  EVENT_BTN_RELEASE,
  EVENT_BTN_CLICK,
  EVENT_BTN_DCLICK,
#endif // ENABLE_BUTTON_DEVICE
  EVENT_SELF_ADD,
  // ...
  // Add other event enumeration values here
  // ...
} et_type_t;

/**
 * @brief Enabling the GPIO driver
 * el_btn_port_def:   If not(like arduino), it can be defined as NULL
 * el_btn_pin_def:    Data type of pin number
 */
#ifdef ENABLE_GPIO_DRIVER
#define el_btn_port_def         GPIO_TypeDef
#define el_btn_pin_def          uint16_t
#endif // ENABLE_GPIO_DRIVER

/**
 * @brief Enabling the user button driver
 * 
 * DF_BTN_LONG_PRESS_TIME:  Wait time to trigger the button long press event
 * DF_BTN_DCLICK_DELAY:     Double-click time trigger time
 */
#ifdef ENABLE_BUTTON_DEVICE
#define DF_BTN_LONG_PRESS_TIME  1000
#define DF_BTN_DCLICK_DELAY     300
#endif // ENABLE_BUTTON_DEVICE

/* Enable buzzer driver */
#ifdef ENABLE_BUZZER_DEVICE
#define el_tim_def              TIM_HandleTypeDef
#define el_channel_def          uint16_t
#endif // ENABLE_BUZZER_DEVICE

/* Enabling i2c protocol */
#ifdef ENABLE_I2C_SENSOR
#define el_i2c_def              I2C_HandleTypeDef
#define DF_I2C_TIMEOUT          1000
#endif // ENABLE_I2C_SENSOR

/* Enables the bmp180 sensor, using the i2c driver protocol */
#ifdef ENABLE_SENSOR_BMP180_I2C
#define DF_BMP180_ADDRESS_7BIT  0x77
#endif // ENABLE_SENSOR_BMP180_I2C

#endif // __USER_EL_BSP_H__
