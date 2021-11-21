#ifndef __USER_EMITER_H__
#define __USER_EMITER_H__

#define MAX_ET_LISTENERS  128
#include "sys_global.h"

typedef struct
{
  et_type_t eventType;
  void (*handler)(fun_params_t *p);
} et_subs_t;

typedef struct
{
  et_subs_t *subs[MAX_ET_LISTENERS];
  uint8_t rp;
  uint8_t wp;
  uint8_t size;
} et_dep_t;

typedef struct
{
  et_type_t eventType;
  fun_params_t *params;
  uint32_t occuredTime;
} et_body_t;

void ET_Init(void);
el_ret_t ET_AddEventListener(et_type_t eventType, void handler());
el_ret_t ET_EmitEvent(et_type_t eventType, fun_params_t params[]);

#endif
