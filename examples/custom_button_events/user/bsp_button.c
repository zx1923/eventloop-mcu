#include "bsp_button.h"
#include "bsp_led.h"

void observBtnState(void)
{
  static uint32_t lastEventTime = 0;
  static uint8_t lastState = PIN_HIGH;
  
  // press 条件
  if (lastState == PIN_HIGH && Bsp_Btn1_Read() == PIN_LOW)
  {
    lastState = PIN_LOW;
    fun_params_t *params = (fun_params_t *)malloc(sizeof(fun_params_t));
    params[0].param.uint8Data = 16 + 1;
    lastEventTime = Sys_GetMillis();
    EL_PushEvent(ET_BTN_PRESS, params, lastEventTime);
    return;
  }

  // releas 条件
  if (lastState == PIN_LOW && Bsp_Btn1_Read() == PIN_HIGH)
  {
    lastState = PIN_HIGH;
    fun_params_t *params = (fun_params_t *)malloc(sizeof(fun_params_t));
    params[0].param.uint8Data = 16 + 1;
    lastEventTime = Sys_GetMillis();
    EL_PushEvent(ET_BTN_RELEASE, params, lastEventTime);
    return;
  }
}

void onBtnPressed(fun_params_t params[])
{
  uint8_t btnId = (uint8_t)params[0].param.uint8Data;
  printf("[%0.8d] Btn%x pressed\r\n", Sys_GetMillis(), btnId);
}

void onBtnReleased(fun_params_t params[])
{
  static uint8_t ledState = PIN_HIGH;
  uint8_t btnId = (uint8_t)params[0].param.uint8Data;
  printf("[%0.8d] Btn%x released\r\n", Sys_GetMillis(), btnId);
  ledState = !ledState;
  LedG_SetState(ledState);
}
