#include "uart.h"
#include "assert.h"
#include "stm32f3xx_hal.h"
#include "dbg.h"

#define UART_TIMEOUT (100)

static UART_HandleTypeDef uart_init_conf;

void uart_init(void)
{
  uart_init_conf = (UART_HandleTypeDef){
    .Instance = USART2,
    .Init = {
      .BaudRate = 230400,
      .WordLength = UART_WORDLENGTH_8B,
      .StopBits = UART_STOPBITS_1,
      .Parity = UART_PARITY_NONE,
      .HwFlowCtl = UART_HWCONTROL_NONE,
      .Mode = UART_MODE_TX_RX,
    },
  };
  assert(HAL_UART_Init(&uart_init_conf) != HAL_OK);
  printf("uart initialized\n\r");
}

void uart_rx(uint8_t* buffer, uint16_t sz)
{
  HAL_StatusTypeDef ret;
  assert(buffer);
  assert(sz > 0);
  ret = HAL_UART_Receive(&uart_init_conf, buffer, sz, UART_TIMEOUT); 
  assert(ret == HAL_OK);
}

void uart_tx(uint8_t* buffer, uint16_t sz)
{
  HAL_StatusTypeDef ret;
  assert(buffer);
  assert(sz > 0);
  ret = HAL_UART_Transmit(&uart_init_conf, buffer, sz, UART_TIMEOUT);
  assert(ret == HAL_OK);
}

void uart_deinit(void)
{

}

void _putchar(char character)
{
  uart_tx((uint8_t*)&character, (uint16_t)sizeof(character));
}
