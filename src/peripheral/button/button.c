#include "button.h"

#ifdef ENABLE_BUTTON_DEVICE

static el_btn_group_t Buttons;

el_ret_t _pushBtnEvent(el_btn_t *btn, el_btn_status_t status, et_type_t eventType);
void _clearDclickEvent(fun_params_t p[]);
el_btn_status_t _readBtnStatus(el_btn_t *btn);

el_ret_t _pushBtnEvent(el_btn_t *btn, el_btn_status_t status, et_type_t eventType)
{
  if (el_isEventQueueValid() == EL_FALSE)
  {
    return EL_FULL;
  }
  btn->lastStatus = status;
  btn->lastEventTime = el_getMillis();
  fun_params_t *params = (fun_params_t *)malloc(sizeof(fun_params_t) * 2);
  params[0].param.stringData = btn->name;
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

el_btn_status_t _readBtnStatus(el_btn_t *btn)
{
  return __user_el_gpio_readPin(btn->port, btn->pin) == btn->pressPinSet ? EL_BTN_PRESS : EL_BTN_RELEASE;
}

el_ret_t el_button_postEvent(el_btn_t *btn)
{
  uint32_t lastEventTime = btn->lastEventTime;
  // press
  if (btn->lastStatus == EL_BTN_RELEASE && _readBtnStatus(btn) == EL_BTN_PRESS)
  {
    return _pushBtnEvent(btn, EL_BTN_PRESS, EVENT_BTN_PRESS);
  }

  // long press
  if (btn->lastStatus == EL_BTN_PRESS && _readBtnStatus(btn) == EL_BTN_PRESS)
  {
    if (el_getMillis() - lastEventTime >= DF_BTN_LONG_PRESS_TIME)
    {
      btn->lockEvent = EVENT_BTN_LONG_PRESS;
      return _pushBtnEvent(btn, EL_BTN_PRESS, EVENT_BTN_LONG_PRESS);
    }
  }

  // release/click
  if (btn->lastStatus == EL_BTN_PRESS && _readBtnStatus(btn) == EL_BTN_RELEASE)
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
    el_setTimeout(_clearDclickEvent, DF_BTN_DCLICK_DELAY, params);
    return res;
  }
  return EL_EMPTY;
}

el_btn_t *el_button_regist(el_btn_port_def *port, el_btn_pin_def pin, const char* name, el_pin_set_t pressPinSet)
{
  if (Buttons.wp >= DF_BUTTON_COUNTER)
  {
    return NULL;
  }
  el_btn_t *btn = (el_btn_t *)malloc(sizeof(el_btn_t));
  btn->name = name;
  btn->port = port;
  btn->pin = pin;
  btn->lastStatus = EL_BTN_RELEASE;
  btn->pressPinSet = pressPinSet;
  btn->lastEventTime = 0;
  Buttons.btns[Buttons.wp++] = btn;
  return btn;
}

void el_button_scan()
{
  if (Buttons.wp <= 0)
  {
    return;
  }
  for (uint8_t i = 0; i < Buttons.wp; i++)
  {
    el_button_postEvent(Buttons.btns[i]);
  }
}

#endif // ENABLE_BUTTON_DEVICE
