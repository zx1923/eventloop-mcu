#include "buzzer.h"

#ifdef ENABLE_BUZZER_DEVICE

void _resetBuzzerCompareState(fun_params_t p[])
{
  el_buzzer_t *buzzer = (el_buzzer_t *)p[0].param.pointer;
  __user_el_buzzer_setTimCompare(buzzer->htim, buzzer->channel, 0);
}

el_buzzer_t *el_buzzer_regist(el_tim_def *htim, el_channel_def channel, const char *name, uint16_t initState)
{
  el_buzzer_t *buzzer = (el_buzzer_t *)malloc(sizeof(el_buzzer_t));
  buzzer->name = name;
  buzzer->htim = htim;
  buzzer->channel = channel;
  buzzer->compare = initState;
  __user_el_buzzer_start(htim, channel);
  return buzzer;
}

void el_buzzer_setState(el_buzzer_t *buzzer, uint16_t period, uint32_t duration)
{
  __user_el_buzzer_setTimPeriod(buzzer->htim, period);
  __user_el_buzzer_setTimCompare(buzzer->htim, buzzer->channel, buzzer->compare);
  fun_params_t *params = (fun_params_t *)malloc(sizeof(fun_params_t));
  params[0].param.pointer = (uint32_t)buzzer;
  el_setTimeout(_resetBuzzerCompareState, params, duration);
}
#endif // ENABLE_BUZZER_DEVICE
