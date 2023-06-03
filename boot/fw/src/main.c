#include "stm32f3xx_hal.h"
#include "mcu.h"
#include "uart.h"
#include "gpio.h"
#include "dbg.h"
#include "flash.h"

#define MAGIC_WORD (0xDEADBEEF)

static bool check_flash_page_is_erased(flash_page_t page)
{
  uint32_t page_size = 0x800;
  uint32_t start_addr = 0x08000000 + ((uint32_t)page * 0x800);
  uint32_t end_addr = start_addr + page_size;
  bool is_erased = true;
  for(uint32_t* data = (uint32_t*)start_addr; data < (uint32_t*)end_addr; data++) {
    if(*data != 0xFFFFFFFF) {
      DBG_ERR("Flash page not actually erased!\n\r");
      is_erased = false;
      break;
    }
  }
  return is_erased;
}

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
  uint32_t flash_data = MAGIC_WORD;
  uint16_t flash_size = flash_get_size();
  bool is_erased;

  DBG_INFO("Flash size: 0x%X\n\r", flash_size);
  flash_err_t ret = flash_erase_page(flash_page_31);
  if(ret != flash_no_err) {
    DBG_ERR("Flash erase error!\n\r");
    is_erased = false;
  } else {
    is_erased = check_flash_page_is_erased(flash_page_31);
    if(is_erased) {
      DBG_DEBUG("Flash page actually erased.\n\r");
    } else {
      DBG_ERR("Flash not erased. Will not attempt to write!\n\r");
    }
  }

  if(is_erased) {
    ret = flash_write(0xF800, &flash_data, 1u);
    if(flash_no_err != ret) {
      DBG_ERR("Flash write error.\n\r");
    } else {
      DBG_DEBUG("Flash write successful!\n\r");
      flash_data = *(uint32_t*)0x0800F800;
      DBG_DEBUG("Reading written flash data: 0x%X\n\r", flash_data);
      if(flash_data == MAGIC_WORD) {
        DBG_DEBUG("Flash actually written with magic word.\n\r");
      } else {
        DBG_ERR("Magic pattern does not match.\n\r");
      }
    }
  }

  while(1) {
    uart_get_data(&data);
    if(data.len > 0) {
      DBG_DEBUG("Got data: %s\n\r", data.data);
    }
  }
  return 0;
}

