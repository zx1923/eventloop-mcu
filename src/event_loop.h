#ifndef __USER_EVENT_LOOP_H__
#define __USER_EVENT_LOOP_H__

#include "sys_global.h"
#include "emiter.h"

#define MA_TASK_LEN   5
#define EVENT_BUF_LEN   32
#define INTERVAL_NONE 0

typedef enum
{
  IMMEDIATE_N = 0,
  IMMEDIATE_Y,
} task_immediate_t;

typedef enum
{
  EL_NULL = 0,
  EL_IDLE,
  EL_RUNNING,
  EL_DONE,
} el_status_t;

typedef struct
{
  uint8_t taskId;
  void (*handler)(fun_params_t *p);
  fun_params_t *params;
  uint32_t interval;
  uint32_t runAt;
  el_status_t status;
} el_task_t;

typedef struct
{
  el_task_t *buf[MA_TASK_LEN];
  uint8_t wp;    // 写指针
  uint8_t rp;    // 读指针
  uint8_t size;  // 缓冲区的最大长度
} el_task_buf_t;

typedef struct
{
  et_body_t *buf[EVENT_BUF_LEN];
  uint8_t wp;
  uint8_t rp;
  uint8_t size;
} el_event_buf_t;


void El_Init(void);

el_ret_t EL_PushMacroTask(el_task_t *task);
el_ret_t EL_PushMicroTask(el_task_t *task);
el_ret_t EL_PushEvent(et_type_t eventType, fun_params_t params[], uint32_t occuredTime);

void EL_RunTasks(void);

#endif
