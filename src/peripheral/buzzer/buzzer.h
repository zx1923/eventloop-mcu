#ifndef __USER_EL_BUZZER_H__
#define __USER_EL_BUZZER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "eos.h"
#ifdef ENABLE_BUZZER_DEVICE
typedef struct
{
  const char *name;
  el_tim_def *htim;
  el_channel_def channel;
  uint16_t compare;
} el_buzzer_t;

extern void __user_el_buzzer_start(el_tim_def *tim, el_channel_def channel);
extern void __user_el_buzzer_stop(el_tim_def *tim, el_channel_def channel);
extern void __user_el_buzzer_setTimCompare(el_tim_def *tim, el_channel_def channel, uint16_t value);
extern void __user_el_buzzer_setTimPeriod(el_tim_def *tim, uint16_t period);

el_buzzer_t *el_buzzer_regist(el_tim_def *htim, el_channel_def channel, const char *name, uint16_t initState);
void el_buzzer_setState(el_buzzer_t *buzzer, uint16_t period, el_time_t duration);

#endif

#ifdef __cplusplus
}
#endif // ENABLE_BUZZER_DEVICE

#endif // __USER_EL_BUZZER_H__
