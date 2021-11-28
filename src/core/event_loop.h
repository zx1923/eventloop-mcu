#ifndef __USER_EL_EVENT_LOOP_H__
#define __USER_EL_EVENT_LOOP_H__

#include "sys_global.h"
#include "emiter.h"

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
  EL_CLEAR,
} el_status_t;

typedef struct
{
  el_status_t status;
  uint8_t taskId;
  void (*handler)(fun_params_t *p);
  fun_params_t *params;
  el_time_t interval;
  el_time_t runAt;
} el_task_t;

typedef struct
{
  el_task_t *buf[DF_MAX_TASK_LEN];
  uint8_t wp;    // 写指针
  uint8_t rp;    // 读指针
  uint8_t size;  // 缓冲区的最大长度
} el_task_buf_t;

typedef struct
{
  et_body_t *buf[DF_EVENT_BUF_LEN];
  uint8_t wp;
  uint8_t rp;
  uint8_t size;
} el_event_buf_t;

el_ret_t el_pushMacroTask(el_task_t *task);
el_ret_t el_pushMicroTask(el_task_t *task);
el_ret_t el_pushEvent(et_type_t eventType, fun_params_t params[], el_time_t occuredTime);
el_bool_t el_isEventQueueValid(void);
el_bool_t el_isMacroTaskQueueValid(void);
el_bool_t el_isMicroTaskQueueValid(void);
void el_runTasks(void);

#endif
