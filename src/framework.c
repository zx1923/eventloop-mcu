#include "framework.h"

static uint16_t TickNumber = 0;
static float EfpsTime = 0;

uint16_t _setAsyncTask(void callback(), fun_params_t p[], uint32_t runAt, uint32_t interval, el_ret_t (*pushFn)(el_task_t *))
{
  el_task_t *timeoutTask;
  timeoutTask = (el_task_t *)malloc(sizeof(el_task_t));
  timeoutTask->params = p;
  timeoutTask->handler = callback;
  timeoutTask->interval = interval;
  timeoutTask->runAt = runAt;
  timeoutTask->taskId = Sys_CreateTaskId();
  timeoutTask->status = EL_IDLE;
  pushFn(timeoutTask);
  return timeoutTask->taskId;
}

void FW_Start(void handler(), uint8_t fps)
{
  static uint32_t lastMillis = 0;
  if (fps != NULL)
  {
    EfpsTime = 1000.0 / fps;
  }
  El_Init();
  handler();
  while (1)
  {
    EL_RunTasks();
    TickNumber++;
    if (Sys_GetMillis() - lastMillis < EfpsTime) {
      int32_t waitMs = EfpsTime - (Sys_GetMillis() - lastMillis);
      Sys_Delay(waitMs <= 0 ? 0 : waitMs);
    }
    lastMillis = Sys_GetMillis();
  }
}

float FW_GetFPS()
{
  static uint32_t lastMillis = 0;
  static float fps = 0;
  uint32_t millis = Sys_GetMillis();
  if (millis - lastMillis >= 1000)
  {
    fps = 1.0 * TickNumber / (millis - lastMillis) * 1000;
    TickNumber = 0;
    lastMillis = Sys_GetMillis();
  }
  return fps;
}

uint8_t FW_SetTimeout(void callback(), fun_params_t p[], uint32_t ms)
{
  return _setAsyncTask(callback, p, Sys_GetMillis() + ms, INTERVAL_NONE, EL_PushMacroTask);
}

uint8_t FW_SetInterval(void callback(), fun_params_t p[], uint32_t ms, task_immediate_t immediate)
{
  uint32_t runAt = immediate == IMMEDIATE_N ? Sys_GetMillis() + ms : 0;
  return _setAsyncTask(callback, p, runAt, ms, EL_PushMacroTask);
}

uint8_t FW_NextTick(void callback(), fun_params_t p[])
{
  return _setAsyncTask(callback, p, 0, 0, EL_PushMicroTask);
}

// 异步延时
void FW_Delay(uint32_t ms)
{
  uint32_t endTime = Sys_GetMillis() + ms;
  while (Sys_GetMillis() < endTime)
  {
    EL_RunTasks();
  }
}
