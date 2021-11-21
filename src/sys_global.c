#include "sys_global.h"

static uint8_t sys_task_id = 0;

uint8_t Sys_CreateTaskId(void)
{
  return ++sys_task_id;
}

void Sys_DelaySync(uint32_t ms) 
{
  Bsp_Delay_Ms(ms);
}

