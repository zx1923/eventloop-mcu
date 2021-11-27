# eventloop-mcu
一个在单片机上运行的事件循环模型库，参考了JavaScript的事件循环模型，并做了简化，使其可以在单片机上顺利运行，基于此模型，可以在单片机上实现类似 JavaScript 的异步编程。

## Features

- 多任务，基于异步实现的多任务，类似于运行了 RTOS 操作系统；
- 轻量，核心部分占用的 RAM 小于 1Kb；
- JavaScript like；
- 拥有 setTimeout 、setInterval 、nextTick 和 requestAnimationFrame 函数，使用效果与 JavaScript 类似；
- 拥有异步延时函数，实现了等待，但不影响其他任务执行；
- 事件驱动，基于异步编程，可以通过事件驱动各种应用；
- 移植方便，核心部分只需实现两个函数；

## 移植

移植核心部分只需要实现两个函数：

- **Bsp_Get_Tick**：获取嘀嗒定时器的 tick ，即系统启动后的毫秒时间戳；
- **Bsp_Delay_Ms**：阻塞式延时函数；

eos 要求 **Bsp_Get_Tick** 达到 **1ms** 的精度，所以不管通过什么方式，请确保定时器中断能产生至少 1ms 的嘀嗒周期。

### STM32 HAL

以 stm32 为例，如果工程是通过 STM32CubeMX 创建，默认是开启系统嘀嗒定时器的，且滴答定时器的终端周期是 1ms ，那么在 stm32 hal 项目上移植就非常简单，在 bsp.h 中做一下定义即可：

```c
// stm32_hal/bsp.h
/* Time-delay function and tick timer definition */
#define Bsp_Get_Tick            HAL_GetTick
#define Bsp_Delay_Ms            HAL_Delay
```

### Arduino

Arduino 的移植也非常简单，由于 arduino 标准函数中默认就带了 millis 和 delay 两个函数，所以移植只需要做一下映射定义即可：

```c
// arduino/bsp.h
/* Time-delay function and tick timer definition */
#define Bsp_Get_Tick            millis
#define Bsp_Delay_Ms            delay
```

## 构建配置

与编译构建有关的配置全部定义在 build.h 文件中，文件如下：

```c
#ifndef __USER_EL_BUILD_H__
#define __USER_EL_BUILD_H__

// 选择需要使能的平台，当前开启了 STM32 HAL
#define BSP_USE_ARM_STM32_HAL

// eventloop 相关 buffer 长度定义
#define DF_MAX_TASK_LEN     32
#define DF_EVENT_BUF_LEN    32
#define DF_MAX_LISTENERS    16

// 是否启用下面的基本外设驱动
#define ENABLE_PWM_DRIVER
#define ENABLE_GPIO_DRIVER

// 是否启用按键监听
#define ENABLE_BUTTON_DEVICE
#ifdef ENABLE_BUTTON_DEVICE
#define DF_BUTTON_COUNTER   4
#endif // ENABLE_BUTTON_DEVICE

// ...

#endif
```



## 示例

在单片机中使用事件循环模型只需要引入 **eos.h** 头文件即可：

```c
#include "eos.h"
```

当然，为了能编译成功，还需要将 **eos.h** 所在的目录加入到 **Include Paths** 中。

### 周期任务

使用 **el_setInterval** 定义的任务为周期任务，类似于 JavaScript 中的 **setInterval** 函数。

```c
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
```

### 延时任务

**el_setTimeout** 用于定义一个延时任务，与 JavaScript 中的 **setTimeout** 表现类似，表示在给定时间后尽快执行回调函数，回调函数执行完成后会立即从内存中释放。

```c
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
  el_startLoop(startTestTasks);
  // ...
}

```

### 事件监听

事件监听可以分为同步监听和异步监听，同步监听是立即执行的，异步监听则会在下一个 tick 中处理事件回调。

下面的示例将已监听按钮事件为例进行说明。

#### 同步事件监听

同步事件监听其实是同步的发布订阅模式，当事件被触发后，会直接执行回调函数，不必等到下一个 tick ，这是与异步不同的地方。

假设有 **EVENT_SELF_ADD** 自定义事件，以同步事件的方式触发该事件：

```c
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
```

由代码可以发现，通过 **el_addEventListener** 函数可以定义指定事件的监听回调函数，同步触发的就是直接通过 **el_emitEvent** 函数触发事件回调函数。

#### 异步事件监听

异步事件监听的表现与 JavaScript DOM 事件基本一致，异步事件被一个事件消息队列维护，当事件被触发时，并不会立刻执行事件的回调函数，而是将事件推入事件队列中，在下一个 tick 中依次处理事件队列中的事件，触发对应的回调函数。

与 JavaScript 不同的是，eos 的事件监听只绑定事件和回调，并不绑定实例（触发源），这是出于节省资源的考虑，所以需要在回调函数中通过回调参数来判断事件源。

在 **eos** 中，当系统就绪，准备开始事件循环时，会触发 **EVENT_EL_LOAD** 事件，用户可以监听该事件，以便知道事件循环何时开始。

**eos** 中的所有事件默认都是异步事件，**EVENT_EL_LOAD** 也不例外，当第一轮 tick 的微任务执行完毕后，就会开始执行 **EVENT_EL_LOAD** 的回调函数。

```c
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
  el_nextTick(helloWorld, NULL);
  // 启动事件循环
  el_startLoop();
  // ...
}

```

由于在每一个 tick 中，微任务会先被执行，所以 HelloWorld 函数比 onLoad 先执行，上面的例子最终输出为：

```shell
hello world!
eventloop loaded
```

#### 自定义事件

与 JavaScript 类似，你也可以定义自己的事件，然后在需要的时候触发该事件的回调。自定义事件时只需要在 

**bsp.h** 文件中扩展 **et_type_t** 枚举类型，见：

```c
// bsp.h
// ...
typedef enum
{
  EVENT_NONE = 0,
  EVENT_EL_LOAD,
#ifdef ENABLE_BUTTON_DEVICE
  EVENT_BTN_PRESS,
  EVENT_BTN_LONG_PRESS,
  EVENT_BTN_RELEASE,
  EVENT_BTN_CLICK,
  EVENT_BTN_DCLICK,
#endif // ENABLE_BUTTON_DEVICE
  // ...
  // Add other event enumeration values here
  // ...
} et_type_t;
// ...
```

需要注意，请确保枚举值 **EVENT_NONE 始终等于 0** ，不要删除该枚举值。

### nextTick

**eos** 中也有 **nextTick** 函数，其名称为 **el_nextTick** ，与 JavaScript（nodejs环境）的 **process.nextTick** 功能类似。**el_nextTick** 中的回调函数总是会在下一次 tick 中优先执行。

```c
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
  el_setTimeout(echoCounter, 0, NULL);
  el_nextTick(helloWorld, NULL);
  // 启动事件循环
  el_startLoop(startTestTasks);
  // ...
}
```

### requestAnimationFrame

**eos** 中也支持 **requestAnimationFrame** 函数，其名称为 **el_requestAnimationFrame** ，使用效果与 web 端的 **requestAnimationFrame** 函数类似，但原理不同。

在单片机中，**el_requestAnimationFrame** 可以用于需要周期性刷新显示的场景，比如需要让一块屏幕按照给定的帧率进行刷新时，就可以通过该函数设置刷新函数以及期望的帧率。

下面的示例代码，表示以每秒 30 帧的速度执行 helloWorld 函数：

```c
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
  el_requestAnimationFrame(helloWorld, 30, NULL);
  // 启动事件循环
  el_startLoop();
  // ...
}
```

> 要让 el_requestAnimationFrame 设置的回调函数顺利执行，还需要在 MCU 的 tickTimer 中断函数中调用 el_onIncTick 函数，由于这不是该段代码的主要目的，所以不在这里展开。

### 监听用户输入

#### 按钮点击事件

**eso** 的外设库中实现了一个简单的按钮驱动，使用轮训的方式处理用户点击，支持的事件有：

- **EVENT_BTN_PRESS**：按钮被按下时触发；
- **EVENT_BTN_RELEASE**：按钮被释放后触发；
- **EVENT_BTN_LONG_PRESS**：按钮被按下并保持一段时间后触发；
- **EVENT_BTN_CLICK**：按钮完成一次单击后触发；
- **EVENT_BTN_DCLICK**：按钮完成一次双击后触发；

假设项目构建时已经启用了按键监听，并且启用了 GPIO 驱动，且在 bsp.c 中完成了对应的函数实现，例如对于按键而言，由于其依赖 GPIO 驱动，所以需要实现 GPIO 读写函数，对应在 stm32_hal 的 bsp.c 中为如下（已实现）：

```c
// stm32_hal/bsp.c
#include "bsp.h"

// ...

// Define gpio read and write functions
#ifdef ENABLE_GPIO_DRIVER
extern void __user_el_gpio_writePin(el_btn_port_def *port, el_btn_pin_def pin, el_pin_set_t state)
{
  HAL_GPIO_WritePin(port, pin, (GPIO_PinState)state);
}

extern el_pin_set_t __user_el_gpio_readPin(el_btn_port_def *port, el_btn_pin_def pin)
{
  return (el_pin_set_t)HAL_GPIO_ReadPin(port, pin);
}
#endif // ENABLE_GPIO_DRIVER

// ...
```

实现对按钮的长按和单击监听，示例如下：

```c
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
```

当用户对注册的按钮进行操作时，就会触发 EVENT_BTN_LONG_PRESS 或 EVENT_BTN_CLICK 事件，通过回调函数即可处理该事件。

按键的事件回调会传入两个参数，分别是：

- 按键名，即按键注册时传入的按键名；
- 触发时间，即触发该事件时的毫秒时间戳；

回调函数的参数会通过一个 fun_params_t 类型的指针传入，回调函数对入参进行结构，拿到参数即可。
