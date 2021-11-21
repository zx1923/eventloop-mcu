#ifndef __USER_EL_FRAMEWORK_H__
#define __USER_EL_FRAMEWORK_H__

#include "bsp.h"
#include "sys_global.h"
#include "event_loop.h"
#include "emiter.h"

uint8_t FW_SetTimeout(void callback(), fun_params_t p[], uint32_t ms);
uint8_t FW_SetInterval(void callback(), fun_params_t p[], uint32_t ms, task_immediate_t immediate);
uint8_t FW_NextTick(void callback(), fun_params_t p[]);
void FW_Start(void handler(), uint8_t fps);
void FW_LedState(fun_params_t p[]);
void FW_Delay(uint32_t ms);
float FW_GetFPS(void);

#endif

