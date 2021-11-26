#include "eos.h"

uint8_t _createTaskId(void)
{
  static uint8_t taskId = 0;
  return ++taskId;
}

el_ret_t _clearAsyncTask(el_task_t *taskInstance)
{
  if (taskInstance != NULL) {
    taskInstance->status = EL_CLEAR;
    return EL_OK;
  }
  return EL_ERR;
}

el_task_t *_setAsyncTask(void callback(), fun_params_t p[], uint32_t runAt, uint32_t interval, el_ret_t (*pushFn)(el_task_t *))
{
  el_task_t *timeoutTask;
  timeoutTask = (el_task_t *)malloc(sizeof(el_task_t));
  timeoutTask->params = p;
  timeoutTask->handler = callback;
  timeoutTask->interval = interval;
  timeoutTask->runAt = runAt;
  timeoutTask->taskId = _createTaskId();
  timeoutTask->status = EL_IDLE;
  pushFn(timeoutTask);
  return timeoutTask;
}

void el_startLoop(void handler())
{
  handler();
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

el_task_t *el_requestAnimationFrame(void callback(), fun_params_t params[], uint8_t fps)
{

}
