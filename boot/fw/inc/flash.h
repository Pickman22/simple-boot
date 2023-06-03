#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
  flash_no_err,
  flash_err,
  flash_unlock_err,
  flash_write_err,
  flash_erase_err,
  flash_read_err,
  flash_address_err,
} flash_err_t;

typedef enum {
  flash_page_0,
  flash_page_1,
  flash_page_2,
  flash_page_3,
  flash_page_4,
  flash_page_5,
  flash_page_6,
  flash_page_7,
  flash_page_8,
  flash_page_9,
  flash_page_10,
  flash_page_11,
  flash_page_12,
  flash_page_13,
  flash_page_14,
  flash_page_15,
  flash_page_16,
  flash_page_17,
  flash_page_18,
  flash_page_19,
  flash_page_20,
  flash_page_21,
  flash_page_22,
  flash_page_23,
  flash_page_24,
  flash_page_25,
  flash_page_26,
  flash_page_27,
  flash_page_28,
  flash_page_29,
  flash_page_30,
  flash_page_31,
  flash_page_num,
  flash_page_invalid = flash_page_num,
} flash_page_t;

void flash_init(void);

flash_err_t flash_write(uint32_t addr, uint32_t* data, size_t len);

flash_err_t flash_read(uint32_t addr, uint32_t* data, size_t len);

flash_err_t flash_erase_page(flash_page_t page);

uint16_t flash_get_size(void);

void flash_deinit(void);

#endif
