#include "event_loop.h"
#include <string.h>

static el_task_buf_t MacroTasksBuffer;
static el_task_buf_t MicroTasksBuffer;
static el_event_buf_t EventQueueBuffer;

el_ret_t _pushTask(el_task_buf_t *taskBuf, el_task_t *task);
el_task_t *_shiftTask(el_task_buf_t *taskBuf);
el_ret_t _callTaskHandler(el_task_t *task);
el_ret_t _walkTaskBuf(el_task_buf_t *taskBuf);
el_ret_t _pushEvent(el_event_buf_t *eventBuf, et_body_t *eventBody);
et_body_t *_shiftEvent(el_event_buf_t *eventBuf);

el_ret_t _pushTask(el_task_buf_t *taskBuf, el_task_t *task)
{
  if (taskBuf->size >= MA_TASK_LEN)
  {
    return EL_FULL;
  }
  taskBuf->buf[taskBuf->wp++] = task;
  taskBuf->wp = taskBuf->wp >= MA_TASK_LEN ? 0 : taskBuf->wp;
  taskBuf->size++;
  return EL_OK;
}

el_task_t *_shiftTask(el_task_buf_t *taskBuf)
{
  if (taskBuf->size <= 0)
  {
    return NULL;
  }
  el_task_t *task = taskBuf->buf[taskBuf->rp++];
  taskBuf->rp = taskBuf->rp >= MA_TASK_LEN ? 0 : taskBuf->rp;
  taskBuf->size--;
  return task;
}

el_ret_t _pushEvent(el_event_buf_t *eventBuf, et_body_t *eventBody)
{
  if (eventBuf->size >= EVENT_BUF_LEN)
  {
    return EL_FULL;
  }
  eventBuf->buf[eventBuf->wp++] = eventBody;
  eventBuf->wp = eventBuf->wp >= EVENT_BUF_LEN ? 0 : eventBuf->wp;
  eventBuf->size++;
  return EL_OK;
}

et_body_t *_shiftEvent(el_event_buf_t *eventBuf)
{
  if (eventBuf->size <= 0)
  {
    return NULL;
  }
  et_body_t *eventBody = eventBuf->buf[eventBuf->rp++];
  eventBuf->rp = eventBuf->rp >= EVENT_BUF_LEN ? 0 : eventBuf->rp;
  eventBuf->size--;
  return eventBody;
}

el_ret_t _callTaskHandler(el_task_t *task)
{
  if (task->status == EL_NULL)
  {
    return EL_ERR;
  }
  if (Sys_GetMillis() < task->runAt)
  {
    EL_PushMacroTask(task);
    return EL_OK;
  }
  task->status = EL_RUNNING;
  task->handler(task->params);
  task->status = EL_DONE;

  // 周期任务
  if (task->interval > 0)
  {
    task->status = EL_IDLE;
    task->runAt = Sys_GetMillis() + task->interval;
    EL_PushMacroTask(task);
    return EL_OK;
  }

  free(task->params);
  free(task);
  return EL_OK;
}

el_ret_t _walkTaskBuf(el_task_buf_t *taskBuf)
{
  el_task_t *task = _shiftTask(taskBuf);
  if (task == NULL)
  {
    return EL_EMPTY;
  }
  return _callTaskHandler(task);
}

el_ret_t _walkEventBuf(el_event_buf_t *eventBuf)
{
  et_body_t *eventBody = _shiftEvent(eventBuf);
  if (eventBody == NULL)
  {
    return EL_EMPTY;
  }
  el_ret_t res = ET_EmitEvent(eventBody->eventType, eventBody->params);
  free(eventBody);
  return res;
}

void El_Init()
{
  // macro
  MacroTasksBuffer.rp = MacroTasksBuffer.wp = MacroTasksBuffer.size;
  // micro
  MicroTasksBuffer.rp = MicroTasksBuffer.wp = MicroTasksBuffer.size;
  // events
  EventQueueBuffer.rp = EventQueueBuffer.wp = EventQueueBuffer.size;
}

el_ret_t EL_PushMacroTask(el_task_t *task)
{
  return _pushTask(&MacroTasksBuffer, task);
}

el_ret_t EL_PushMicroTask(el_task_t *task)
{
  return _pushTask(&MicroTasksBuffer, task);
}

el_ret_t EL_PushEvent(et_type_t eventType, fun_params_t params[], uint32_t occuredTime)
{
  et_body_t *eventBody = (et_body_t *)malloc(sizeof(et_body_t));
  eventBody->eventType = eventType;
  eventBody->params = params;
  eventBody->occuredTime = occuredTime;
  return _pushEvent(&EventQueueBuffer, eventBody);
}

void EL_RunTasks(void)
{
  uint8_t miSize = MicroTasksBuffer.size;
  uint8_t maSize = MacroTasksBuffer.size;
  uint8_t etSize = EventQueueBuffer.size;
  // micro
  while (EL_EMPTY != _walkTaskBuf(&MicroTasksBuffer) && miSize--)
    ;
  // events
  while (EL_EMPTY != _walkEventBuf(&EventQueueBuffer) && etSize--)
    ;
  // macro
  while (EL_EMPTY != _walkTaskBuf(&MacroTasksBuffer) && maSize--)
    ;
}
