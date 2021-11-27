#include "eos.h"

static el_task_t AnimationFrameTask = {EL_NULL};
static uint16_t AnimationFrameTime = 0;

uint8_t _createTaskId(void)
{
  static uint8_t taskId = 0;
  return ++taskId;
}

el_ret_t _clearAsyncTask(el_task_t *taskInstance)
{
  if (taskInstance != NULL)
  {
    taskInstance->status = EL_CLEAR;
    return EL_OK;
  }
  return EL_ERR;
}

el_task_t *_setAsyncTask(void callback(), fun_params_t p[], uint32_t runAt, uint32_t interval, el_ret_t (*pushFn)(el_task_t *))
{
  el_task_t *task;
  task = (el_task_t *)malloc(sizeof(el_task_t));
  task->params = p;
  task->handler = callback;
  task->interval = interval;
  task->runAt = runAt;
  task->taskId = _createTaskId();
  task->status = EL_IDLE;
  return pushFn(task) == EL_FULL ? NULL : task;
}

void el_startLoop()
{
  el_pushEvent(EVENT_EL_LOAD, NULL, el_getMillis());
  while (1)
  {
    el_runTasks();
  }
}

el_task_t *el_setTimeout(void callback(), uint32_t ms, fun_params_t p[])
{
  return _setAsyncTask(callback, p, el_getMillis() + ms, INTERVAL_NONE, el_pushMacroTask);
}

el_task_t *el_setInterval(void callback(), uint32_t ms, fun_params_t p[], task_immediate_t immediate)
{
  uint32_t runAt = immediate == IMMEDIATE_N ? el_getMillis() + ms : 0;
  return _setAsyncTask(callback, p, runAt, ms, el_pushMacroTask);
}

el_ret_t el_clearTimeout(el_task_t *taskInstance)
{
  return _clearAsyncTask(taskInstance);
}

el_ret_t el_clearInterval(el_task_t *taskInstance)
{
  return _clearAsyncTask(taskInstance);
}

el_task_t *el_nextTick(void callback(), fun_params_t p[])
{
  return _setAsyncTask(callback, p, 0, 0, el_pushMicroTask);
}

el_ret_t el_requestAnimationFrame(void callback(), uint8_t fps, fun_params_t params[])
{
  if (fps == 0)
  {
    return EL_ERR;
  }
  AnimationFrameTask.status = EL_IDLE;
  AnimationFrameTask.handler = callback;
  AnimationFrameTask.params = params;
  AnimationFrameTime = 1000 / fps;
  return EL_OK;
}

void el_onIncTick()
{
  if (AnimationFrameTime == 0 || AnimationFrameTask.status != EL_IDLE)
  {
    return;
  }
  if (el_getMillis() % AnimationFrameTime == 0)
  {
    AnimationFrameTask.status = EL_RUNNING;
    AnimationFrameTask.handler(AnimationFrameTask.params);
    AnimationFrameTask.status = EL_IDLE;
  }
}

void el_delay(uint32_t ms)
{
  uint32_t endTime = el_getMillis() + ms;
  while (el_getMillis() < endTime)
  {
    el_runTasks();
  }
}
