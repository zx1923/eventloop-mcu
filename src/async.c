#include "eos.h"

static uint16_t TickNumber = 0;
static float EfpsTime = 0;

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

void el_startLoop(void handler(), uint8_t fps)
{
  static uint32_t lastMillis = 0;
  if (fps != NULL)
  {
    EfpsTime = 1000.0 / fps;
  }
  handler();
  while (1)
  {
    el_runTasks();
    TickNumber++;
    if (el_getMillis() - lastMillis < EfpsTime) {
      int32_t waitMs = EfpsTime - (el_getMillis() - lastMillis);
      el_delaySync(waitMs <= 0 ? 0 : waitMs);
    }
    lastMillis = el_getMillis();
  }
}

float el_getFps()
{
  static uint32_t lastMillis = 0;
  static float fps = 0;
  uint32_t millis = el_getMillis();
  if (millis - lastMillis >= 1000)
  {
    fps = 1.0 * TickNumber / (millis - lastMillis) * 1000;
    TickNumber = 0;
    lastMillis = el_getMillis();
  }
  return fps;
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
