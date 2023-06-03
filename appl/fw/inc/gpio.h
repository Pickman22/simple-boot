#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

typedef enum {
  gpio_led_e = 0,
  gpio_uart_rx_e,
  gpio_uart_tx_e,
  gpio_num_e,
  gpio_invalid_e = gpio_num_e,
} gpio_t;

void gpio_init(void);

void gpio_set(gpio_t gpio);

void gpio_clear(gpio_t gpio);

void gpio_toggle(gpio_t gpio);

bool gpio_get(gpio_t gpio);

void gpio_deinit(void);

#endif
