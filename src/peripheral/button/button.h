#ifndef __USER_EL_BUTTON_H__
#define __USER_EL_BUTTON_H__

#include "event_loop.h"

typedef struct
{
  uint8_t id;
  el_btn_port_def *port;
  el_btn_pin_def pin;
  el_btn_state_t lastState;
  uint32_t lastEventTime;
} el_btn_t;

typedef struct
{
  el_btn_t *btns[DF_BUTTON_COUNTER];
  uint8_t wp;
} el_btn_group_t;


extern el_btn_state_t __user_el_btn_readPinState(el_btn_port_def *port, el_btn_pin_def pin);

el_ret_t el_button_regist(el_btn_port_def *port, el_btn_pin_def pin, uint8_t id, el_btn_state_t initState);
void el_button_observeState(void);

#endif
