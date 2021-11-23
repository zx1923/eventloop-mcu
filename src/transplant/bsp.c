#include "bsp.h"

#ifdef ENABLE_BUTTON_DEVICE
__weak el_btn_state_t __user_el_btn_readPinState(el_btn_port_def *port, el_btn_pin_def pin)
{
  return (el_btn_state_t)HAL_GPIO_ReadPin(port, pin);
}
#endif // ENABLE_BUTTON_DEVICE

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

