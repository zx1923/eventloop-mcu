/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "eos.h"
/* USER CODE END Includes */

// ...

void onBtnPressed(fun_params_t params[])
{
  uint8_t btnId = (uint8_t)params[0].param.uint8Data;
  printf("[%0.8d] Btn%x pressed\r\n", el_getMillis(), btnId);
}

void onBtnReleased(fun_params_t params[])
{
  uint8_t btnId = (uint8_t)params[0].param.uint8Data;
  printf("[%0.8d] Btn%x released\r\n", el_getMillis(), btnId);
}

void helloWorld() {
  printf("[%0.8d] hello world\r\n", Sys_GetMillis());
}

void startTestTasks()
{
  fun_params_t *blinkParams = (fun_params_t *)malloc(sizeof(fun_params_t) * 2);
  blinkParams[0].param.int32Data = 20;
  blinkParams[1].param.int32Data = 0;
  // 周期任务：每隔 2000ms LED_R 点亮 50ms
  el_setInterval(LedR_Blink, blinkParams, 2000, IMMEDIATE_N);
  // 一次性任务：在下一个 tick 打印出 hello world
  el_nextTick(helloWorld, EL_PARAMS_NULL);
  // 周期任务：每 10ms 执行 observBtnState 函数观察按钮状态
  el_setInterval(NULL, observBtnState, EL_PARAMS_NULL, 10, IMMEDIATE_Y);

  // 监听按钮 press 和 release 事件
  el_addEventListener(EVENT_BTN_PRESS, onBtnPressed);
  el_addEventListener(EVENT_BTN_RELEASE, onBtnReleased);
}
/* USER CODE END 0 */

// ...

int main(void)
{
  // ...
  // 启动任务，不设置帧率，将 startTestTasks 作为入口函数，按最高频率进行事件循环
  el_startLoop(startTestTasks, NULL);
  // ...
}
