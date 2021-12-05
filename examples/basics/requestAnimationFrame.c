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
  el_requestAnimationFrame(helloWorld, 30, EL_PARAMS_NULL);
  // 启动事件循环
  el_startLoop();
  // ...
}
