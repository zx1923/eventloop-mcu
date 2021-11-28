#ifndef __USER_EL_LED_H__
#define __USER_EL_LED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "eos.h"
#ifdef ENABLE_LED_DEVICE
#ifndef ENABLE_GPIO_DRIVER
#error "Please enable the GPIO driver!"
#endif // !ENABLE_GPIO_DRIVER

typedef enum
{
  EL_LED_STATUS_UNKNOWN,
  EL_LED_STATUS_ON,
  EL_LED_STATUS_OFF,
} el_led_status_t;

typedef struct
{
  const char *name;
  el_gpio_port_def *port;
  el_gpio_pin_def pin;
  el_pin_set_t onPinSet;
  el_led_status_t status;
} el_led_t;

extern void __user_el_gpio_writePin(el_gpio_port_def *port, el_gpio_pin_def pin, el_pin_set_t state);

el_led_t *el_led_regist(el_gpio_port_def *port, el_gpio_pin_def pin, const char *name, el_pin_set_t onPinSet);
void el_led_on(el_led_t *led);
void el_led_off(el_led_t *led);
void el_led_blink(el_led_t *led, el_time_t onMs);

#ifdef ENABLE_PWM_DRIVER
extern void __user_el_pwm_setValue(el_gpio_port_def *port, el_gpio_pin_def pin, el_pin_set_t value);
#endif // ENABLE_PWM_DRIVER

#endif // ENABLE_LED_DEVICE

#ifdef __cplusplus
}
#endif

#endif // __USER_EL_LED_H__
