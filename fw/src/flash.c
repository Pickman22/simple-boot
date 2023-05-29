#include <stdbool.h>
#include <assert.h>
#include "flash.h"
#include "stm32f3xx_hal_flash.h"
#include "stm32f3xx_hal_flash_ex.h"
#include "dbg.h"

void flash_init(void) { }

STATIC_FUNC bool flash_phys_address_range_is_ok(uint32_t start_addr, size_t len) {
  bool is_ok = false;
  uint32_t end_addr = 0;
  if(len > 0) {
    end_addr = start_addr + (sizeof(uint32_t) * len) - 1u;
    is_ok = IS_FLASH_PROGRAM_ADDRESS(start_addr) && IS_FLASH_PROGRAM_ADDRESS(end_addr);
  }
  if(false == is_ok) {
    DBG_ERR("Address range 0x%X - 0x%X is not valid for write operation.\n\r", start_addr, end_addr);
  }
  return is_ok;
}

STATIC_FUNC bool flash_address_alignment_is_ok(uint32_t addr) {
  bool is_ok = (0u == (addr % sizeof(uint32_t)));
  if(false == is_ok) {
    DBG_ERR("Alignment not correct. ADDR: 0x%X\n\r", addr);
  }
  return is_ok;
}

STATIC_FUNC bool flash_phys_address_range_and_aligment_is_ok(uint32_t addr, size_t len) {
  bool range_ok = flash_phys_address_range_is_ok(addr, len);
  bool alingment_ok = flash_address_alignment_is_ok(addr);
  return range_ok && alingment_ok;
}

STATIC_FUNC bool flash_is_erased(uint32_t addr, size_t len) {

}

flash_err_t flash_write(uint32_t addr, uint32_t* data, size_t len) {
  flash_err_t ret = flash_err;
  uint32_t phys_addr = addr + FLASH_BASE;
  assert(data); 
  if(HAL_FLASH_Unlock() != HAL_OK) {
    DBG_ERR("Unable to unlock flash. ADDR: 0x%X\n\r", phys_addr);
    ret = flash_unlock_err;
  } else if (false == flash_phys_address_range_is_ok(phys_addr, len)) {
    ret = flash_address_err;
  } else {
    /* Okay to attempt flash programming. */
    for(size_t i = 0; i < len; i++) {
      if(HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, phys_addr, (uint64_t)data[i])) {
        DBG_ERR("Programming error at phys_address 0x%X\n\r", phys_addr);
        ret = flash_write_err;
        break;
      } else {
        phys_addr += sizeof(*data);
      }
    }
    ret = flash_no_err;
  }
  (void)HAL_FLASH_Lock();
  return ret;
}

flash_err_t flash_read(uint32_t addr, uint32_t* data, size_t len) {
  flash_err_t ret = flash_err;
  uint32_t phys_addr = addr + FLASH_BASE;
  uint32_t* flash_data = (uint32_t*)addr;
  assert(data && (len > 0));
  if(false == flash_phys_address_range_and_aligment_is_ok(phys_addr, len)) {
    ret = flash_address_err;
  } else {
    for(size_t i = 0; i < len; i++) {
      data[i] = *flash_data;
      flash_data++;
    }
    ret = flash_no_err;
  }
  return ret;
}

flash_err_t flash_erase(size_t page) {
  return flash_err;
}

uint16_t flash_get_size(void) {
 return *((uint16_t *)FLASH_SIZE_DATA_REGISTER);
}

void flash_deinit(void) { }
