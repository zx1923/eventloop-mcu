#ifndef __USER_BSP_H__
#define __USER_BSP_H__

#include "stdio.h"
#include "stm32f4xx_hal.h"  // 根据自己使用的MCU类型进行修改

// 自定义事件枚举
typedef enum
{
  ET_BTN_PRESS = 1,
  ET_BTN_RELEASE,
  ET_BTN_CLICK,
  ET_BTN_DCLICK,
} et_type_t;

#endif
