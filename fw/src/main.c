#include "mcu.h"
#include "uart.h"
#include "gpio.h"
#include "dbg.h"

int main(void)
{
  mcu_init();
  gpio_init();
  uart_init();
  printf("system initialized\n\r");

  while(1) {

  }

  return 0;
}

