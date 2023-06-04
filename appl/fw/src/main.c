#include "stm32f3xx_hal.h"
#include "mcu.h"
#include "uart.h"
#include "gpio.h"
#include "dbg.h"
#include "flash.h"

int main(void)
{
  mcu_init();
  gpio_init();
  uart_init();
  DBG_DEBUG("Appl system initialzed\n\r");
  while(1) {
    DBG_DEBUG("Hello from appl!\n\r");
    HAL_Delay(1000);
  }
  return 0;
}

