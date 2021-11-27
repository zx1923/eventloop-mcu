#include <stdio.h>
#include "main.h"
#include "eos.h"

void echoCounter()
{
  static uint16_t count = 0;
  printf("Count: %d\r\n", count++);
}

int main(void)
{
  // ...
    
  /**
   * @brief 每隔 1 秒执行一次 echoCounter 函数
   * 
   * 依次输出：
   * Count: 0
   * Count: 1
   * Count: 2
   * ...
   */
  el_setInerval(echoCounter, 1000, NULL, IMMEDIATE_N);
  // 启动事件循环
  el_startLoop();
  // ...
}