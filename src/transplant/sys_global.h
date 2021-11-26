#ifndef __USER_EL_SYS_GLOBAL_H__
#define __USER_EL_SYS_GLOBAL_H__

#include <stdlib.h>
#include "bsp.h"

#define el_getMillis    Bsp_Get_Tick
#define el_delaySync    Bsp_Delay_Ms

typedef enum
{
  EL_OK = 1,
  EL_ERR,
  EL_BUSY,
  EL_FULL,
  EL_EMPTY,
} el_ret_t;

typedef enum
{
  EL_FALSE = 0,
  EL_TRUE,
} el_bool_t;

typedef union
{
  uint8_t uint8Data;
  int32_t int32Data;
  double doubleData;
  const char* stringData;
  uint32_t pointer;
} params_union_t;

typedef struct
{
  params_union_t param;
} fun_params_t;

#endif
