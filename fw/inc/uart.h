#ifndef UART_H
#define UART_H

#include "printf.h"
#include <stdint.h>

void uart_init(void);

void uart_rx(uint8_t* buffer, uint16_t sz);

void uart_tx(uint8_t* buffer, uint16_t sz);

void uart_deinit(void);


#endif
