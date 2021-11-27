#include <stdio.h>
#include "main.h"
#include "eos.h"

void helloWorld()
{
  printf("hello world!\r\n");
}
int main(void)
{
  // ...
  el_addEventListener(EVENT_SELF_ADD, helloWorld);
  el_emitEvent(EVENT_SELF_ADD, NULL);
  // 启动事件循环
  el_startLoop();
  // ...
}
