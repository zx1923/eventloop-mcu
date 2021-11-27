#include "led.h"
#include <string.h>

#ifdef ENABLE_LED_DEVICE

el_led_t *el_led_regist(el_btn_port_def *port, el_btn_pin_def pin, const char *name, el_pin_set_t onPinSet)
{
  el_led_t *led = (el_led_t *)malloc(sizeof(el_led_t));
  led->name = name;
  led->port = port;
  led->pin = pin;
  led->onPinSet = onPinSet;
  led->status = EL_LED_STATUS_UNKNOWN;
  return led;
}

void el_led_on(el_led_t *led)
{
  __user_el_gpio_writePin(led->port, led->pin, led->onPinSet);
  led->status = EL_LED_STATUS_ON;
}

void el_led_off(el_led_t *led)
{
  __user_el_gpio_writePin(led->port, led->pin, (el_pin_set_t)!led->onPinSet);
  led->status = EL_LED_STATUS_OFF;
}

void el_led_blink(el_led_t *led, uint32_t onMs)
{
  if (onMs == 0)
    return;
  el_led_on(led);
  fun_params_t *params = (fun_params_t *)malloc(sizeof(el_led_t));
  memcpy(params, led, sizeof(el_led_t));
  el_setTimeout(el_led_off, onMs, params);
}

#endif // ENABLE_LED_DEVICE
