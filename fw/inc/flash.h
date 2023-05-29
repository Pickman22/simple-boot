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

void flash_init(void);

flash_err_t flash_write(uint32_t addr, uint32_t* data, size_t len);

flash_err_t flash_read(uint32_t addr, uint32_t* data, size_t len);

flash_err_t flash_erase(size_t page);

uint16_t flash_get_size(void);

void flash_deinit(void);

#endif
