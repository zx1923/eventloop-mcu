#include "event_loop.h"

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
  if (taskBuf->size >= DF_MAX_TASK_LEN)
  {
    free(task->params);
    free(task);
    return EL_FULL;
  }
  taskBuf->buf[taskBuf->wp++] = task;
  taskBuf->wp = taskBuf->wp >= DF_MAX_TASK_LEN ? 0 : taskBuf->wp;
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
  taskBuf->rp = taskBuf->rp >= DF_MAX_TASK_LEN ? 0 : taskBuf->rp;
  taskBuf->size--;
  return task;
}

el_ret_t _pushEvent(el_event_buf_t *eventBuf, et_body_t *eventBody)
{
  if (eventBuf->size >= DF_EVENT_BUF_LEN)
  {
    free(eventBody->params);
    free(eventBody);
    return EL_FULL;
  }
  eventBuf->buf[eventBuf->wp++] = eventBody;
  eventBuf->wp = eventBuf->wp >= DF_EVENT_BUF_LEN ? 0 : eventBuf->wp;
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
  eventBuf->rp = eventBuf->rp >= DF_EVENT_BUF_LEN ? 0 : eventBuf->rp;
  eventBuf->size--;
  return eventBody;
}

el_ret_t _callTaskHandler(el_task_t *task)
{
  if (task->status == EL_NULL)
  {
    return EL_ERR;
  }
  if (task->status == EL_CLEAR)
  {
    free(task->params);
    free(task);
    return EL_OK;
  }
  if (el_getMillis() < task->runAt)
  {
    el_pushMacroTask(task);
    return EL_OK;
  }
  task->status = EL_RUNNING;
  task->handler(task->params);
  task->status = EL_DONE;

  // 周期任务
  if (task->interval > 0)
  {
    task->status = EL_IDLE;
    task->runAt = el_getMillis() + task->interval;
    el_pushMacroTask(task);
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
  el_ret_t res = el_emitEvent(eventBody->eventType, eventBody->params);
  free(eventBody);
  return res;
}

el_ret_t el_pushMacroTask(el_task_t *task)
{
  return _pushTask(&MacroTasksBuffer, task);
}

el_ret_t el_pushMicroTask(el_task_t *task)
{
  return _pushTask(&MicroTasksBuffer, task);
}

el_ret_t el_pushEvent(et_type_t eventType, fun_params_t params[], uint32_t occuredTime)
{
  et_body_t *eventBody = (et_body_t *)malloc(sizeof(et_body_t));
  eventBody->eventType = eventType;
  eventBody->params = params;
  eventBody->occuredTime = occuredTime;
  return _pushEvent(&EventQueueBuffer, eventBody);
}

el_bool_t el_isEventQueueValid()
{
  return EventQueueBuffer.size < DF_EVENT_BUF_LEN ? EL_TRUE : EL_FALSE;
}

el_bool_t el_isMacroTaskQueueValid(void)
{
  return MacroTasksBuffer.size < DF_MAX_TASK_LEN ? EL_TRUE : EL_FALSE;
}

el_bool_t el_isMicroTaskQueueValid(void)
{
  return MicroTasksBuffer.size < DF_MAX_TASK_LEN ? EL_TRUE : EL_FALSE;
}

void el_runTasks()
{
  static uint8_t step = 0;
  static uint8_t size = 0;

  if (step > 2)
    step = 0;
  switch (step)
  {
  case 0:
    if (size == 0)
      size = MicroTasksBuffer.size;
    _walkTaskBuf(&MicroTasksBuffer);
    size--;
    if (size == 0)
      step++;
    break;
  case 1:
    if (size == 0)
      size = EventQueueBuffer.size;
    _walkEventBuf(&EventQueueBuffer);
    size--;
    if (size == 0)
      step++;
    break;
  case 2:
    if (size == 0)
      size = MacroTasksBuffer.size;
    _walkTaskBuf(&MacroTasksBuffer);
    size--;
    if (size == 0)
      step++;
    break;
  };
}
