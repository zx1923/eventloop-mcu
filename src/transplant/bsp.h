#ifndef __USER_EL_BSP_H__
#define __USER_EL_BSP_H__

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "build.h"

#ifndef NULL
#define NULL  0
#endif // NULL

// BSP delay
#define Bsp_Get_Tick            HAL_GetTick
#define Bsp_Delay_Ms            HAL_Delay
// BSP gpio define
#define Bsp_Gpio_Write          HAL_GPIO_WritePin
#define Bsp_Gpio_Read           HAL_GPIO_ReadPin
// gpio state define
#define BSP_PIN_HIGH            GPIO_PIN_SET
#define BSP_PIN_LOW             GPIO_PIN_RESET

typedef enum 
{
  EL_IO_OK       = 0x00U,
  EL_IO_ERROR    = 0x01U,
  EL_IO_BUSY     = 0x02U,
  EL_IO_TIMEOUT  = 0x03U
} el_io_status_t;

typedef enum
{
  EVENT_NONE = 0,
  EVENT_BTN_PRESS,
  EVENT_BTN_LONG_PRESS,
  EVENT_BTN_RELEASE,
  EVENT_BTN_CLICK,
  EVENT_BTN_DCLICK,
  // ...
  // add custom events here
  // ...
} et_type_t;

// button
#ifdef ENABLE_BUTTON_DEVICE
#define DF_BTN_LONG_PRESS_TIME  1000
#define DF_BTN_DCLICK_DELAY     300
typedef enum
{
  EL_BTN_PRESS = BSP_PIN_LOW,
  EL_BTN_RELEASE = BSP_PIN_HIGH,
} el_btn_state_t;
#define el_btn_port_def         GPIO_TypeDef
#define el_btn_pin_def          uint16_t
#endif // ENABLE_BUTTON_DEVICE

// buzzer
#ifdef ENABLE_BUZZER_DEVICE
#define el_tim_def              TIM_HandleTypeDef
#define el_channel_def          uint16_t
#endif // ENABLE_BUZZER_DEVICE

// i2c device/sensor
#ifdef ENABLE_I2C_SENSOR
#define el_i2c_def              I2C_HandleTypeDef
#define DF_I2C_TIMEOUT          1000
#endif // ENABLE_I2C_SENSOR

#ifdef ENABLE_SENSOR_BMP180_I2C
#define DF_BMP180_ADDRESS_7BIT  0x77
#endif // ENABLE_SENSOR_BMP180_I2C

#endif
