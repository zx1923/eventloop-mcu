#include "buzzer.h"

#ifdef ENABLE_BUZZER_DEVICE

void _resetBuzzerCompareState(fun_params_t p[])
{
  el_buzzer_t *buzzer = (el_buzzer_t *)p[0].param.pointer;
  __user_el_buzzer_setTimCompare(buzzer->htim, buzzer->channel, 0);
  __user_el_buzzer_stop(buzzer->htim, buzzer->channel);
}

el_buzzer_t *el_buzzer_regist(el_tim_def *htim, el_channel_def channel, uint8_t id, uint16_t initState)
{
  el_buzzer_t *buzzer = (el_buzzer_t *)malloc(sizeof(el_buzzer_t));
  buzzer->id = id;
  buzzer->htim = htim;
  buzzer->channel = channel;
  buzzer->compare = initState;
  return buzzer;
}

void el_buzzer_setState(el_buzzer_t *buzzer, uint16_t period, uint32_t duration)
{
  __user_el_buzzer_setTimPeriod(buzzer->htim, period);
  __user_el_buzzer_setTimCompare(buzzer->htim, buzzer->channel, buzzer->compare);
  __user_el_buzzer_start(buzzer->htim, buzzer->channel);
  el_delaySync(duration);
  fun_params_t *params = (fun_params_t *)malloc(sizeof(fun_params_t));
  params[0].param.pointer = (uint32_t)buzzer;
  el_setTimeout(_resetBuzzerCompareState, params, duration);
}
#endif // ENABLE_BUZZER_DEVICE
