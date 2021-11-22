#ifndef __USER_EL_SYS_GLOBAL_H__
#define __USER_EL_SYS_GLOBAL_H__

#include <stdlib.h>
#include "bsp.h"
#define EL_Delay             HAL_Delay
#define EL_GetTickMillis     HAL_GetTick

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

uint32_t el_getMillis(void);
void el_delaySync(uint32_t ms);

#endif
