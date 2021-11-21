#ifndef __USER_BSP_BUTTON_H__
#define __USER_BSP_BUTTON_H__
#include "platform.h"
#include "framework.h"

void observBtnState(void);
void onBtnPressed(fun_params_t params[]);
void onBtnReleased(fun_params_t params[]);

#endif
