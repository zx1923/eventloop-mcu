#ifndef __USER_EL_BUTTON_H__
#define __USER_EL_BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "eos.h"
#ifdef ENABLE_BUTTON_DEVICE
#ifndef ENABLE_GPIO_DRIVER
#error "Please enable the GPIO driver!"
#endif // !ENABLE_GPIO_DRIVER

typedef enum
{
  EL_BTN_PRESS,
  EL_BTN_RELEASE,
} el_btn_status_t;

typedef struct
{
  const char* name;
  el_btn_port_def *port;
  el_btn_pin_def pin;
  el_btn_status_t lastStatus;
  el_pin_set_t pressPinSet;
  uint32_t lastEventTime;
  et_type_t lockEvent;
} el_btn_t;

typedef struct
{
  el_btn_t *btns[DF_BUTTON_COUNTER];
  uint8_t wp;
} el_btn_group_t;

extern el_pin_set_t __user_el_gpio_readPin(el_btn_port_def *port, el_btn_pin_def pin);

el_btn_t *el_button_regist(el_btn_port_def *port, el_btn_pin_def pin, const char* name, el_pin_set_t pressPinSet);
el_ret_t el_button_postEvent(el_btn_t *btn);
void el_button_scan(void);

#endif

#ifdef __cplusplus
}
#endif

#endif // ENABLE_BUTTON_DEVICE

