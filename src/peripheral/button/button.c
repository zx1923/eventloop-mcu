#include "button.h"
#include "eos.h"
#ifdef ENABLE_BUTTON_EVENT

static el_btn_group_t Buttons;

el_ret_t _pushBtnEvent(el_btn_t *btn, el_btn_state_t state, et_type_t eventType);
el_ret_t _walkButtonsState(el_btn_t *btn);
void _clearDclickEvent(fun_params_t p[]);

el_ret_t _pushBtnEvent(el_btn_t *btn, el_btn_state_t state, et_type_t eventType)
{
  btn->lastState = state;
  btn->lastEventTime = el_getMillis();
  fun_params_t *params = (fun_params_t *)malloc(sizeof(fun_params_t) * 2);
  params[0].param.uint8Data = btn->id;
  params[1].param.uint8Data = btn->lastEventTime;
  return el_pushEvent(eventType, params, btn->lastEventTime);
}

void _clearDclickEvent(fun_params_t p[])
{
  el_btn_t *btn = (el_btn_t *)p[0].param.pointer;
  if (btn != NULL)
  {
    btn->lockEvent = EVENT_NONE;
  }
}

el_ret_t _walkButtonsState(el_btn_t *btn)
{
  uint32_t lastEventTime = btn->lastEventTime;
  // press
  if (btn->lastState == EL_BTN_RELEASE && __user_el_btn_readPinState(btn->port, btn->pin) == EL_BTN_PRESS)
  {
    return _pushBtnEvent(btn, EL_BTN_PRESS, EVENT_BTN_PRESS);
  }

  // long press
  if (btn->lastState == EL_BTN_PRESS && __user_el_btn_readPinState(btn->port, btn->pin) == EL_BTN_PRESS)
  {
    if (el_getMillis() - lastEventTime >= DF_BTN_LONG_PRESS_TIME)
    {
      btn->lockEvent = EVENT_BTN_LONG_PRESS;
      return _pushBtnEvent(btn, EL_BTN_PRESS, EVENT_BTN_LONG_PRESS);
    }
  }

  // release/click
  if (btn->lastState == EL_BTN_PRESS && __user_el_btn_readPinState(btn->port, btn->pin) == EL_BTN_RELEASE)
  {
    el_ret_t res = EL_OK;
    res = _pushBtnEvent(btn, EL_BTN_RELEASE, EVENT_BTN_RELEASE);
    if (btn->lockEvent == EVENT_BTN_LONG_PRESS)
    {
      btn->lockEvent = EVENT_NONE;
      return res;
    }
    // double click
    if (btn->lockEvent == EVENT_BTN_CLICK && el_getMillis() - lastEventTime < DF_BTN_DCLICK_DELAY)
    {
      btn->lockEvent = EVENT_NONE;
      res = _pushBtnEvent(btn, EL_BTN_RELEASE, EVENT_BTN_DCLICK);
      return res;
    }
    btn->lockEvent = EVENT_BTN_CLICK;
    res = _pushBtnEvent(btn, EL_BTN_RELEASE, EVENT_BTN_CLICK);

    // setTimeout => clear dclick handler
    fun_params_t *params = (fun_params_t *)malloc(sizeof(fun_params_t));
    params[0].param.pointer = (uint32_t)btn;
    el_setTimeout(_clearDclickEvent, params, DF_BTN_DCLICK_DELAY);
    return res;
  }
  return EL_EMPTY;
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

#endif // ENABLE_BUTTON_EVENT
