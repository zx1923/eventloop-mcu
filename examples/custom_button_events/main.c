/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "user/bsp_led.h"
#include "user/bsp_button.h"
#include "framework.h"
/* USER CODE END Includes */

// ...

void helloWorld() {
  printf("[%0.8d] hello world\r\n", Sys_GetMillis());
}

void startTestTasks()
{
  fun_params_t *blinkParams = (fun_params_t *)malloc(sizeof(fun_params_t) * 2);
  blinkParams[0].param.int32Data = 50;
  blinkParams[1].param.int32Data = 0;
  // 周期任务：每隔 2000ms LED_R 点亮 50ms
  FW_SetInterval(LedR_Blink, blinkParams, 2000, IMMEDIATE_N);
  // 一次性任务：在下一个 tick 打印出 hello world
  FW_NextTick(helloWorld, NULL);
  // 周期任务：每 10ms 执行 observBtnState 函数观察按钮状态
  FW_SetInterval(observBtnState, NULL, 10, IMMEDIATE_Y);

  // 监听按钮 press 和 release 事件
  ET_AddEventListener(ET_BTN_PRESS, onBtnPressed);
  ET_AddEventListener(ET_BTN_RELEASE, onBtnReleased);
}
/* USER CODE END 0 */

// ...

int main(void)
{
  // ...
  // 启动任务，不设置帧率，将 startTestTasks 作为入口函数，按最高频率进行事件循环
  FW_Start(startTestTasks, NULL);
  // ...
}
