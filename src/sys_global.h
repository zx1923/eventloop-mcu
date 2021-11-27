#ifndef __USER_EL_SYS_GLOBAL_H__
#define __USER_EL_SYS_GLOBAL_H__

#include <stdio.h>
#include <stdlib.h>
#include "build.h"

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
  EL_IO_OK       = 0x00U,
  EL_IO_ERROR    = 0x01U,
  EL_IO_BUSY     = 0x02U,
  EL_IO_TIMEOUT  = 0x03U
} el_io_status_t;

typedef enum
{
  EL_PIN_LOW = 0,
  EL_PIN_HIGH,
} el_pin_set_t;

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
