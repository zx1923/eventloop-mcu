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
  /**
   * @brief 一秒钟后执行 helloWorld 函数
   * 
   * 输出：
   * hello world!
   */
  el_setTimeout(helloWorld, 1000, NULL);
  // 启动事件循环
  el_startLoop();
  // ...
}
