#include "button.h"

static el_btn_group_t Buttons;

el_ret_t _walkButtonsState(el_btn_t *btn)
{
  // press
  if (btn->lastState == EL_BTN_RELEASE && __user_el_btn_readPinState(btn->port, btn->pin) == EL_BTN_PRESS)
  {
    btn->lastState = EL_BTN_PRESS;
    btn->lastEventTime = el_getMillis();
    fun_params_t *params = (fun_params_t *)malloc(sizeof(fun_params_t));
    params[0].param.uint8Data = btn->id;
    return el_pushEvent(EVENT_BTN_PRESS, params, btn->lastEventTime);
  }

  // release
  if (btn->lastState == EL_BTN_PRESS && __user_el_btn_readPinState(btn->port, btn->pin) == EL_BTN_RELEASE)
  {
    btn->lastState = EL_BTN_RELEASE;
    btn->lastEventTime = el_getMillis();
    fun_params_t *params = (fun_params_t *)malloc(sizeof(fun_params_t));
    params[0].param.uint8Data = btn->id;
    return el_pushEvent(EVENT_BTN_RELEASE, params, btn->lastEventTime);
  }
  return EL_OK;
}

el_ret_t el_button_regist(el_btn_port_def *port, el_btn_pin_def pin, uint8_t id, el_btn_state_t initState)
{
  if (Buttons.wp >= DF_BUTTON_COUNTER)
  {
    return EL_FULL;
  }
  el_btn_t *btn = (el_btn_t *)malloc(sizeof(el_btn_t));
  btn->id = id;
  btn->port = port;
  btn->pin = pin;
  btn->lastState = initState;
  btn->lastEventTime = 0;
  Buttons.btns[Buttons.wp++] = btn;
  return EL_OK;
}

void el_button_observeState()
{
  if (Buttons.wp <= 0)
  {
    return;
  }
  for (uint8_t i = 0; i < Buttons.wp; i++)
  {
    _walkButtonsState(Buttons.btns[i]);
  }
}

