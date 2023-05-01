#include "stm32f3xx_hal.h"
#include "mcu.h"
#include "uart.h"
#include "gpio.h"
#include "dbg.h"

int main(void)
{
  mcu_init();
  gpio_init();
  uart_init();
  DBG_DEBUG("system initialzed\n\r");
  while(1) {
    gpio_toggle();
    HAL_Delay(500);
  }
  return 0;
}

