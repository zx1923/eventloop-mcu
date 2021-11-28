#ifndef __USER_EL_BSPINC_H__
#define __USER_EL_BSPINC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "build.h"

#ifdef BSP_USE_STM32_HAL
#include "transplant/stm32_hal/bsp.h"
#endif // BSP_USE_STM32_HAL

#ifdef BSP_USE_STM8_SPL
#include "transplant/stm8_spl/bsp.h"
#endif // BSP_USE_STM8_SPL

#ifdef BSP_USE_ARDUINO
#include "transplant/arduino/bsp.h"
#endif // BSP_USE_ARDUINO

#ifdef __cplusplus
}
#endif

#endif // __USER_EL_BSPINC_H__
