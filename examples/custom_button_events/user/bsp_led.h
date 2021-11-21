#ifndef __USER_BSP_LED_H__
#define __USER_BSP_LED_H__
#include "platform.h"
#include "framework.h"

void LedR_On(void);
void LedR_Off(void);
void LedR_Blink(fun_params_t params[]);
void LedG_SetState(uint8_t state);

#endif

