#include <stdio.h>
#include "main.h"
#include "eos.h"

void onLongPress(fun_params_t p[])
{
  char *btnName = (char *)params[0].param.stringData;
  printf("[%s] long press\r\n");
}

void onClick(fun_params_t p[])
{
  char *btnName = (char *)params[0].param.stringData;
  printf("[%s] clicked\r\n");
}

int main(void)
{
  // ...
  
  el_btn_t *btn1 = el_button_regist(GPIOB, GPIO_PIN_0, "Btn1", EL_PIN_LOW);
  el_btn_t *btn2 = el_button_regist(GPIOB, GPIO_PIN_1, "Btn2", EL_PIN_HIGH);
  el_addEventListener(EVENT_BTN_LONG_PRESS, onLongPress);
  el_addEventListener(EVENT_BTN_CLICK, onClick);
  
  // 启动事件循环
  el_startLoop();
  // ...
}
