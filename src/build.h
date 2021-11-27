#ifndef __USER_EL_BUILD_H__
#define __USER_EL_BUILD_H__

// bsp
#define BSP_USE_ARM_STM32_HAL

#define DF_MAX_TASK_LEN     16
#define DF_EVENT_BUF_LEN    16
#define DF_MAX_LISTENERS    8
// peripheral
#define DF_BUTTON_COUNTER   4

// module/device
#define ENABLE_BUTTON_DEVICE
#define ENABLE_BUZZER_DEVICE

// i2c sensor
#define ENABLE_I2C_SENSOR

// sensors
#ifdef ENABLE_I2C_SENSOR
#define ENABLE_SENSOR_BMP180_I2C
#define ENABLE_SENSOR_LIS3DH_I2C
#define ENABLE_SENSOR_BMP280_I2C
#endif // ENABLE_I2C_SENSOR

#ifdef BSP_USE_ARM_STM32_HAL
#include "transplant/stm32_hal/bsp.h"
#endif // BSP_USE_ARM_STM32_HAL

#endif
