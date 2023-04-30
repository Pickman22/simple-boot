#include "gpio.h"
#include "stm32f3xx_hal.h"
#include "assert.h"

#define GREEN_LED_PIN (GPIO_PIN_13)
#define GREEN_LED_PORT (GPIOB)

#define UART_GPIO_PORT (GPIOA)
#define UART_TX_PIN (GPIO_PIN_2)
#define UART_RX_PIN (GPIO_PIN_3)

typedef struct {
  GPIO_InitTypeDef hal_gpio_config;
  GPIO_TypeDef* hal_gpio_port;
  GPIO_PinState init_val;
} gpio_config_t;

static gpio_config_t gpio_config_table[gpio_num_e] = {
  [gpio_led_e] = {
    .init_val = GPIO_PIN_RESET,
    .hal_gpio_port = GREEN_LED_PORT,
    .hal_gpio_config = {
      .Pin = GREEN_LED_PIN,
      .Mode = GPIO_MODE_OUTPUT_PP,
      .Pull = GPIO_NOPULL,
      .Speed = GPIO_SPEED_FREQ_LOW,
    },
  },
  [gpio_uart_rx_e] = {
      .init_val = GPIO_PIN_RESET,
      .hal_gpio_port = UART_GPIO_PORT,
      .hal_gpio_config = {
        .Pin = UART_TX_PIN,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_HIGH,
        .Alternate = GPIO_AF7_USART2,
      },
  },
  [gpio_uart_tx_e] = {
      .init_val = GPIO_PIN_RESET,
      .hal_gpio_port = UART_GPIO_PORT,
      .hal_gpio_config = {
        .Pin = UART_TX_PIN,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_HIGH,
        .Alternate = GPIO_AF7_USART2,
      },
  },
};

static void gpio_config(void);
static bool gpio_is_output(gpio_config_t config);

void gpio_init(void)
{
  gpio_config();
}

void gpio_set(gpio_t gpio)
{
  assert(gpio < gpio_num_e);
  HAL_GPIO_WritePin(gpio_config_table[gpio].hal_gpio_port,
                    gpio_config_table[gpio].hal_gpio_config.Pin,
                    GPIO_PIN_SET);
}

void gpio_clear(gpio_t gpio)
{
  assert(gpio < gpio_num_e);
  HAL_GPIO_WritePin(gpio_config_table[gpio].hal_gpio_port,
                    gpio_config_table[gpio].hal_gpio_config.Pin,
                    GPIO_PIN_RESET);
}

void gpio_toggle(gpio_t gpio)
{
  assert(gpio < gpio_num_e);
  HAL_GPIO_TogglePin(gpio_config_table[gpio].hal_gpio_port,
                    gpio_config_table[gpio].hal_gpio_config.Pin);

}

bool gpio_get(gpio_t gpio)
{
  assert(gpio < gpio_num_e);
  return (bool)HAL_GPIO_ReadPin(gpio_config_table[gpio].hal_gpio_port,
                   gpio_config_table[gpio].hal_gpio_config.Pin);

}

void gpio_deinit(void)
{

}

static void gpio_config(void)
{
  for(uint8_t idx = 0; idx < gpio_num_e; idx++) {
    HAL_GPIO_Init(gpio_config_table[idx].hal_gpio_port, &gpio_config_table[idx].hal_gpio_config);

    if(gpio_is_output(gpio_config_table[idx])) {
      HAL_GPIO_WritePin(gpio_config_table[idx].hal_gpio_port,
                        gpio_config_table[idx].hal_gpio_config.Pin,
                        gpio_config_table[idx].init_val);
    }
  }
    /* Consider locking pin configuration. */
}

static bool gpio_is_output(gpio_config_t config) {
    return ((config.hal_gpio_config.Mode == GPIO_MODE_OUTPUT_PP) ||
            (config.hal_gpio_config.Mode == GPIO_MODE_OUTPUT_OD));
}
