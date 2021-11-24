#ifndef __USER_EL_ASYNC_H__
#define __USER_EL_ASYNC_H__

#include "transplant/bsp.h"
#include "transplant/sys_global.h"
#include "event_loop.h"

#define INTERVAL_NONE 0

uint8_t el_setTimeout(void callback(), fun_params_t params[], uint32_t ms);
uint8_t el_setInterval(void callback(), fun_params_t params[], uint32_t ms, task_immediate_t immediate);
uint8_t el_nextTick(void callback(), fun_params_t params[]);
void el_startLoop(void handler(), uint8_t fps);
float el_getFps(void);

#endif
