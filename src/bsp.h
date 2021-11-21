#ifndef __USER_BSP_H__
#define __USER_BSP_H__

#include "stdio.h"
#include "stm32f4xx_hal.h"

#define Bsp_Delay_Ms		HAL_Delay

typedef enum
{
  ET_BTN_PRESS = 1,
  ET_BTN_RELEASE,
  ET_BTN_CLICK,
  ET_BTN_DCLICK,
} et_type_t;

#endif
