#include <stdio.h>
#include "main.h"
#include "eos.h"

void echoCounter()
{
  static uint16_t count = 0;
  printf("Count: %d\r\n", count++);
}

void helloWorld()
{
  printf("hello world!\r\n");
}

int main(void)
{
  // ...

  /**
   * @brief el_nextTick 的回调函数执行早于 el_setTimeout 的回调函数
   * el_nextTick 的回调会被放入微任务队列，el_setTimeout 的回调会被放入宏任务队列
   * 每次 tick 中微任务总是先于宏任务执行
   * 
   * 输出：
   * hello world!
   * Count: 0
   */
  el_setTimeout(NULL, echoCounter, 0, EL_PARAMS_NULL);
  el_nextTick(helloWorld, EL_PARAMS_NULL);
  // 启动事件循环
  el_startLoop();
  // ...
}
