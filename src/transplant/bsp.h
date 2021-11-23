#ifndef __USER_EL_BSP_H__
#define __USER_EL_BSP_H__

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "build.h"

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

#ifdef ENABLE_BUTTON_EVENT
// #define EVENT_BTN_CLICK         EVENT_BTN_RELEASE
#define DF_BTN_LONG_PRESS_TIME  1000
#define DF_BTN_DCLICK_DELAY     300
typedef enum
{
  EL_BTN_PRESS = BSP_PIN_LOW,
  EL_BTN_RELEASE = BSP_PIN_HIGH,
} el_btn_state_t;
#define el_btn_port_def         GPIO_TypeDef
#define el_btn_pin_def          uint16_t
#endif // ENABLE_BUTTON_EVENT

#endif
