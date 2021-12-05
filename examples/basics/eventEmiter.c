#include <stdio.h>
#include "main.h"
#include "eos.h"

void onLoad()
{
  printf("eventloop loaded\r\n");
}

void helloWorld()
{
  printf("hello world!\r\n");
}

int main(void)
{
  // ...
  el_addEventListener(EVENT_EL_LOAD, onLoad);
  el_nextTick(helloWorld, EL_PARAMS_NULL);
  // 启动事件循环
  el_startLoop();
  // ...
}
