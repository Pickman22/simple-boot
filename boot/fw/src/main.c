#include <string.h>
#include "stm32f3xx_hal.h"
#include "mcu.h"
#include "uart.h"
#include "gpio.h"
#include "dbg.h"
#include "flash.h"

extern const uint32_t LD_FLASH_APPL_ADDR_SYM;
extern const uint32_t LD_STACK_ADDR_SYM;

const uint32_t* FLASH_APPL_ADDR = &LD_FLASH_APPL_ADDR_SYM;
const uint32_t* STACK_START_ADDR = &LD_STACK_ADDR_SYM;

typedef void (*appl_exec_t)(void);

void jump_to_appl(void) {
  appl_exec_t appl_exec = (appl_exec_t)(*(FLASH_APPL_ADDR + 1u));
  DBG_DEBUG("Searching for application installed at 0x%X\n\r", FLASH_APPL_ADDR);
  if((uint32_t)STACK_START_ADDR == *FLASH_APPL_ADDR) {
    DBG_DEBUG("Application installed. Attempting jump to address: 0x%X\n\r", (uint32_t)appl_exec);
    __set_MSP((uint32_t)((volatile uint32_t*)STACK_START_ADDR));
    appl_exec();
  } else {
    DBG_ERR("No application installed. Cannot jump!\n\r");
  }
}

#define BLK_SIZE (132)

int main(void)
{
  mcu_init();
  gpio_init();
  uart_init();
  while(1) {
    jump_to_appl();
    gpio_toggle(gpio_led_e);
    HAL_Delay(500);
  }
}

