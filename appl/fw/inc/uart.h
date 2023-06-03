#ifndef UART_H
#define UART_H

#include "printf.h"
#include <stdint.h>

typedef struct {
  uint8_t* data;
  size_t len;
} uart_data_t;

void uart_init(void);

void uart_rx(uint8_t* buffer, uint16_t sz);

void uart_tx(uint8_t* buffer, uint16_t sz);

void uart_get_data(uart_data_t* data);

void uart_deinit(void);

#endif
