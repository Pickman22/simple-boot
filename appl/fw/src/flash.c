#include <stdbool.h>
#include <assert.h>
#include "flash.h"
#include "stm32f3xx_hal_flash.h"
#include "stm32f3xx_hal_flash_ex.h"
#include "dbg.h"

#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base address of Page 0, 2 Kbytes */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08000800) /* Base address of Page 1, 2 Kbytes */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08001000) /* Base address of Page 2, 2 Kbytes */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x08001800) /* Base address of Page 3, 2 Kbytes */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08002000) /* Base address of Page 4, 2 Kbytes */
#define ADDR_FLASH_PAGE_5     ((uint32_t)0x08002800) /* Base address of Page 5, 2 Kbytes */
#define ADDR_FLASH_PAGE_6     ((uint32_t)0x08003000) /* Base address of Page 6, 2 Kbytes */
#define ADDR_FLASH_PAGE_7     ((uint32_t)0x08003800) /* Base address of Page 7, 2 Kbytes */
#define ADDR_FLASH_PAGE_8     ((uint32_t)0x08004000) /* Base address of Page 8, 2 Kbytes */
#define ADDR_FLASH_PAGE_9     ((uint32_t)0x08004800) /* Base address of Page 9, 2 Kbytes */
#define ADDR_FLASH_PAGE_10    ((uint32_t)0x08005000) /* Base address of Page 10, 2 Kbytes */
#define ADDR_FLASH_PAGE_11    ((uint32_t)0x08005800) /* Base address of Page 11, 2 Kbytes */
#define ADDR_FLASH_PAGE_12    ((uint32_t)0x08006000) /* Base address of Page 12, 2 Kbytes */
#define ADDR_FLASH_PAGE_13    ((uint32_t)0x08006800) /* Base address of Page 13, 2 Kbytes */
#define ADDR_FLASH_PAGE_14    ((uint32_t)0x08007000) /* Base address of Page 14, 2 Kbytes */
#define ADDR_FLASH_PAGE_15    ((uint32_t)0x08007800) /* Base address of Page 15, 2 Kbytes */
#define ADDR_FLASH_PAGE_16    ((uint32_t)0x08008000) /* Base address of Page 16, 2 Kbytes */
#define ADDR_FLASH_PAGE_17    ((uint32_t)0x08008800) /* Base address of Page 17, 2 Kbytes */
#define ADDR_FLASH_PAGE_18    ((uint32_t)0x08009000) /* Base address of Page 18, 2 Kbytes */
#define ADDR_FLASH_PAGE_19    ((uint32_t)0x08009800) /* Base address of Page 19, 2 Kbytes */
#define ADDR_FLASH_PAGE_20    ((uint32_t)0x0800A000) /* Base address of Page 20, 2 Kbytes */
#define ADDR_FLASH_PAGE_21    ((uint32_t)0x0800A800) /* Base address of Page 21, 2 Kbytes  */
#define ADDR_FLASH_PAGE_22    ((uint32_t)0x0800B000) /* Base address of Page 22, 2 Kbytes  */
#define ADDR_FLASH_PAGE_23    ((uint32_t)0x0800B800) /* Base address of Page 23, 2 Kbytes */
#define ADDR_FLASH_PAGE_24    ((uint32_t)0x0800C000) /* Base address of Page 24, 2 Kbytes */
#define ADDR_FLASH_PAGE_25    ((uint32_t)0x0800C800) /* Base address of Page 25, 2 Kbytes */
#define ADDR_FLASH_PAGE_26    ((uint32_t)0x0800D000) /* Base address of Page 26, 2 Kbytes */
#define ADDR_FLASH_PAGE_27    ((uint32_t)0x0800D800) /* Base address of Page 27, 2 Kbytes */
#define ADDR_FLASH_PAGE_28    ((uint32_t)0x0800E000) /* Base address of Page 28, 2 Kbytes */
#define ADDR_FLASH_PAGE_29    ((uint32_t)0x0800E800) /* Base address of Page 29, 2 Kbytes */
#define ADDR_FLASH_PAGE_30    ((uint32_t)0x0800F000) /* Base address of Page 30, 2 Kbytes */
#define ADDR_FLASH_PAGE_31    ((uint32_t)0x0800F800) /* Base address of Page 31, 2 Kbytes */

#define FLASH_START ((uint32_t)0x08000000)
#define FLASH_END ((uint32_t)0x0800FFFF)
#define FLASH_LAST_PAGE (31u)
#define FLASH_PAGE_N(n) (FLASH_START + ((n) * FLASH_PAGE_SIZE))

// static uint32_t flash_ram_buff[FLASH_PAGE_SIZE] = {0};

STATIC_FUNC uint32_t flash_page_to_phys_address(flash_page_t page_num) {
  uint32_t addr = 0;
  if(page_num < flash_page_num) {
    addr = (uint32_t)FLASH_BASE + (uint32_t)FLASH_PAGE_SIZE * (uint32_t)page_num;
    if(addr > ADDR_FLASH_PAGE_31) {
      DBG_ERR("Flash page address 0x%X not valid.\n\r", addr);
      addr = 0;
    }
  } else {
    DBG_ERR("Invalid flash page number: %u\n\r", page_num);
  }
  return addr;
}

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

// STATIC_FUNC bool flash_is_erased(uint32_t addr, size_t len) {
//   return false;
// }

STATIC_FUNC flash_err_t flash_unlock(void) {
  flash_err_t ret = flash_err;
  if(HAL_FLASH_Unlock() != HAL_OK) {
    DBG_ERR("Unable to unlock flash.\n\r");
    ret = flash_unlock_err;
  } else {
    ret = flash_no_err;
  }
  return ret;
}

STATIC_FUNC void flash_lock(void) {
  (void)HAL_FLASH_Lock();
}

void flash_init(void) { }

flash_err_t flash_write(uint32_t addr, uint32_t* data, size_t len) {
  flash_err_t ret = flash_err;
  uint32_t phys_addr = addr + FLASH_BASE;
  assert(data); 
  if(flash_no_err != flash_unlock()) {
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
  flash_lock();
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

flash_err_t flash_erase_page(flash_page_t page) {
  flash_err_t ret = flash_err;
  FLASH_EraseInitTypeDef flash_erase_config;
  uint32_t page_err;
  uint32_t phys_addr;
  if(page > flash_page_num) {
    ret = flash_address_err;
  } else if (flash_no_err != flash_unlock()) {
    ret = flash_unlock_err;
    DBG_ERR("Flash erase fialed. Cannot unlock.\n\r");
  } else {
    phys_addr = flash_page_to_phys_address((size_t)page);
    DBG_DEBUG("Erasing page: %u at physical address: 0x%X\n\r", page, phys_addr);
    flash_erase_config = (FLASH_EraseInitTypeDef) {
      .TypeErase = FLASH_TYPEERASE_PAGES,
      .NbPages = 1u,
      .PageAddress = phys_addr,
    };
    if(HAL_OK != HAL_FLASHEx_Erase(&flash_erase_config, &page_err)) {
      DBG_ERR("Flash erase error at address: 0x%X\n\r", page_err);
      ret = flash_erase_err;
    } else {
      DBG_DEBUG("Flash page %u erased.\n\r", page);
      ret = flash_no_err;
    }
    flash_lock();
  }
  return ret;
}

uint16_t flash_get_size(void) {
 return *((uint16_t *)FLASH_SIZE_DATA_REGISTER);
}

void flash_deinit(void) { }
