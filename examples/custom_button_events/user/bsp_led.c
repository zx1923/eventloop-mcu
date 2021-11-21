#include "bsp_led.h"

void LedR_Off()
{
	Bsp_LedR_Write_High();
}

void LedR_On()
{
	Bsp_LedR_Write_Low();
}

void LedR_Blink(fun_params_t p[])
{
  uint16_t waitMs = (uint16_t)(p[0].param.int32Data);
  LedR_On();
  FW_SetTimeout(LedR_Off, NULL, waitMs);
}

void LedG_SetState(uint8_t state)
{
	Bsp_Gpio_Write(LED_G_PORT, LED_G_PIN, state);
}
