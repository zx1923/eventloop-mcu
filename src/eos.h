#ifndef __USER_EL_FRAMEWORK_H__
#define __USER_EL_FRAMEWORK_H__

// #include "transplant/bsp.h"
#include "build.h"
#include "sys_global.h"
#include "core/event_loop.h"
#include "core/async.h"
#include "core/emiter.h"

#ifdef ENABLE_BUTTON_DEVICE
#include "peripheral/button/button.h"
#endif // ENABLE_BUTTON_DEVICE

#ifdef ENABLE_BUZZER_DEVICE
#include "peripheral/buzzer/buzzer.h"
#endif // ENABLE_BUZZER_DEVICE

#ifdef ENABLE_LED_DEVICE
#include "peripheral/led/led.h"
#endif // ENABLE_LED_DEVICE

#ifdef ENABLE_SENSOR_BMP180_I2C
#include "peripheral/bmp180/bmp180.h"
#endif // ENABLE_SENSOR_BMP180_I2C

#endif

