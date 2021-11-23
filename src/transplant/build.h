#ifndef __USER_EL_BUILD_H__
#define __USER_EL_BUILD_H__

#define DF_MAX_TASK_LEN     32
#define DF_EVENT_BUF_LEN    16
#define DF_MAX_LISTENERS    32
// peripheral
#define DF_BUTTON_COUNTER   4

// module/device
#define ENABLE_BUTTON_DEVICE
#define ENABLE_BUZZER_DEVICE

// sensors
#define ENABLE_SENSOR_LIS3DH
#define ENABLE_SENSOR_BMP180
#define ENABLE_SENSOR_BMP280

#endif
