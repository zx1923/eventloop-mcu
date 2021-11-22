#include "sys_global.h"

uint32_t el_getMillis(void)
{
  return EL_GetTickMillis();
}

void el_delaySync(uint32_t ms)
{
  EL_Delay(ms);
}
