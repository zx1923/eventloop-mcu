#include <stdio.h>
#include "main.h"
#include "eos.h"

void ledBlink(fun_params_t p[])
{
  // 假设已定义 el_led_on 和 el_led_off 两个函数
  el_led_t *led = (el_led_t *)p[0].param.pointer;
  uint32_t onMs = (uint32_t)p[1].param.int32Data;
  if (onMs == 0)
    return;
  el_led_on(led);
  fun_params_t *params = (fun_params_t *)malloc(sizeof(el_led_t));
  memcpy(params, led, sizeof(el_led_t));
  // 点亮一段时间后熄灭LED
  el_setTimeout(el_led_off, onMs, params);
}

int main(void)
{
  // ...
  el_led_t *ledR = el_led_regist(GPIOB, GPIO_PIN_0, "Red", EL_PIN_LOW);
  el_led_t *ledG = el_led_regist(GPIOB, GPIO_PIN_5, "Green", EL_PIN_LOW);
  
  fun_params_t *blinkParamsR = (fun_params_t *)malloc(sizeof(fun_params_t) * 2);
  blinkParamsR[0].param.pointer = (uint32_t)ledR;
  blinkParamsR[1].param.int32Data = 10;
  el_setInterval(ledBlink, 1000, blinkParamsR, IMMEDIATE_N);

  fun_params_t *blinkParamsG = (fun_params_t *)malloc(sizeof(fun_params_t) * 2);
  blinkParamsG[0].param.pointer = (uint32_t)ledG;
  blinkParamsG[1].param.int32Data = 20;
  el_setInterval(ledBlink, 2000, blinkParamsG, IMMEDIATE_N);

  // 启动事件循环
  el_startLoop();
  // ...
}
