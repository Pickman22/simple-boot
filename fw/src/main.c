#include "stm32f3xx_hal.h"
#include "mcu.h"
#include "uart.h"
#include "gpio.h"
#include "dbg.h"

int main(void)
{
  uint8_t buffer[256] = {0};
  uart_data_t data = {
    .len = 0,
    .data = buffer,
  };

  mcu_init();
  gpio_init();
  uart_init();
  DBG_DEBUG("system initialzed\n\r");
  while(1) {
    uart_get_data(&data);
    if(data.len > 0) {
      DBG_DEBUG("Got data: %s\n\r", data.data);
    }
  }
  return 0;
}

