#include "gpio.h"
#include "stm32f3xx_hal.h"

#define GREEN_LED_PIN (GPIO_PIN_5)
#define GREEN_LED_PORT (GPIOA)

#define UART_GPIO_PORT (GPIOA)
#define UART_TX_PIN (GPIO_PIN_2)
#define UART_RX_PIN (GPIO_PIN_3)

// Configuration table not correct. Create a typedef struct
// that can also contain the port the pin belongs to and
// initial value for output pins.
static GPIO_InitTypeDef gpio_config_table[] = {
  [gpio_led_e] = {
    .Pin = GREEN_LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW,
  },
  [gpio_uart_rx_e] = {
    .Pin = UART_TX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF7_USART2,
  },
  [gpio_uart_tx_e] = {
    .Pin = UART_TX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF7_USART2,
  },
  [gpio_num_e] = {0},
};

static void gpio_config(void);

void gpio_init(void)
{
  gpio_config();
}

void gpio_set(gpio_t gpio)
{

}

void gpio_clear(gpio_t gpio)
{

}

void gpio_toggle(gpio_t gpio)
{

}

void gpio_get(gpio_t gpio)
{

}

void gpio_deinit(void);

static void gpio_config(void)
{
  GPIO_InitTypeDef led_gpio_config = {
    .Pin = GREEN_LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW,
  };

  GPIO_InitTypeDef uart_rx_gpio_config = {
    .Pin = UART_TX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF7_USART2,
  };

  GPIO_InitTypeDef uart_tx_gpio_config = {
    .Pin = UART_RX_PIN,
    .Mode = GPIO_MODE_AF_OD,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF7_USART2,
  };

  HAL_GPIO_Init(GREEN_LED_PORT, &led_gpio_config);
  HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_PIN_RESET);

  HAL_GPIO_Init(UART_GPIO_PORT, &uart_rx_gpio_config);
  HAL_GPIO_Init(UART_GPIO_PORT, &uart_tx_gpio_config);
}
