#include "bsp.h"

// Define gpio read and write functions
#ifdef ENABLE_GPIO_DRIVER
extern void __user_el_gpio_writePin(el_gpio_port_def *port, el_gpio_pin_def pin, el_pin_set_t state)
{
  HAL_GPIO_WritePin(port, pin, (GPIO_PinState)state);
}

extern el_pin_set_t __user_el_gpio_readPin(el_gpio_port_def *port, el_gpio_pin_def pin)
{
  return (el_pin_set_t)HAL_GPIO_ReadPin(port, pin);
}
#endif // ENABLE_GPIO_DRIVER

#ifdef ENABLE_BUZZER_DEVICE
__weak void __user_el_buzzer_setTimCompare(el_tim_def *tim, el_channel_def channel, uint16_t value)
{
  __HAL_TIM_SetCompare(tim, channel, value);
}

__weak void __user_el_buzzer_setTimPeriod(el_tim_def *tim, uint16_t period)
{
  __HAL_TIM_SetAutoreload(tim, period);
}

__weak void __user_el_buzzer_start(el_tim_def *tim, el_channel_def channel)
{
  HAL_TIM_PWM_Start(tim, channel);
}

__weak void __user_el_buzzer_stop(el_tim_def *tim, el_channel_def channel)
{
  HAL_TIMEx_PWMN_Stop(tim, channel);
}
#endif // ENABLE_BUZZER_DEVICE

// Define i2c read and write functions
#ifdef ENABLE_I2C_SENSOR
__weak el_io_status_t __user_el_i2c_read_bytes(el_i2c_def *hi2c, uint16_t devAddress, uint16_t memAddress, uint8_t *pData, uint16_t pLen)
{
  return (el_io_status_t)HAL_I2C_Mem_Read(hi2c, devAddress, memAddress, I2C_MEMADD_SIZE_8BIT, pData, pLen, DF_I2C_TIMEOUT);
}
__weak el_io_status_t __user_el_i2c_write_bytes(el_i2c_def *hi2c, uint16_t devAddress, uint16_t memAddress, uint8_t *pData, uint16_t pLen)
{
  return (el_io_status_t)HAL_I2C_Mem_Write(hi2c, devAddress, memAddress, I2C_MEMADD_SIZE_8BIT, pData, pLen, DF_I2C_TIMEOUT);
}
#endif // ENABLE_I2C_SENSOR

