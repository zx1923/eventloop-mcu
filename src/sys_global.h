#ifndef __USER_SYS_GLOBAL_H__
#define __USER_SYS_GLOBAL_H__

#include <stdlib.h>
#include "bsp.h"
#define Sys_Delay             HAL_Delay
#define Sys_GetMillis         HAL_GetTick

typedef enum
{
  EL_OK = 1,
  EL_ERR,
  EL_BUSY,
  EL_FULL,
  EL_EMPTY,
} el_ret_t;

typedef union
{
  uint8_t uint8Data;
  int32_t int32Data;
  double doubleData;
  const char* stringData;
} params_union_t;

typedef struct
{
  params_union_t param;
} fun_params_t;

void Sys_DelaySync(uint32_t ms);
uint8_t Sys_CreateTaskId(void);

#endif
