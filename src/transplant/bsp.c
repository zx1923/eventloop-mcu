#include "bsp.h"

#ifdef ENABLE_BUTTON_EVENT
el_btn_state_t __user_el_btn_readPinState(el_btn_port_def *port, el_btn_pin_def pin)
{
  return (el_btn_state_t)HAL_GPIO_ReadPin(port, pin);
}
#endif // ENABLE_BUTTON_EVENT

