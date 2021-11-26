#ifndef __USER_EL_ASYNC_H__
#define __USER_EL_ASYNC_H__

#include "transplant/bsp.h"
#include "transplant/sys_global.h"
#include "event_loop.h"

#define INTERVAL_NONE 0

el_task_t *el_setTimeout(void callback(), uint32_t ms, fun_params_t params[]);
el_ret_t el_clearTimeout(el_task_t *taskInstance);
el_task_t *el_setInterval(void callback(), uint32_t ms, fun_params_t params[], task_immediate_t immediate);
el_ret_t el_clearInterval(el_task_t *taskInstance);
el_task_t *el_nextTick(void callback(), fun_params_t params[]);
el_ret_t el_requestAnimationFrame(void callback(), uint8_t fps, fun_params_t params[]);
void el_startLoop(void handler());
void el_delay(uint32_t ms);
void el_onIncTick(void);

#endif
