#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "uart.h"
#include "stm32f3xx_hal.h"
#include "dbg.h"
#include <stdatomic.h>

/* 1ms timeout. */
#define UART_TIMEOUT (1)

/* Priority definitions. */
#define UART_IRQ_PRIO (5)
#define UART_IRQ_SUBPRIO (0)

#define UART_DMA_RX_PRIO (6)
#define UART_DMA_RX_SUBPRIO (0)

#define UART_DMA_TX_PRIO (6)
#define UART_DMA_TX_SUBPRIO (1)

UART_HandleTypeDef uart_init_config;
static DMA_HandleTypeDef uart_dma_config;
static DMA_HandleTypeDef uart_rx_dma_config;
static uint8_t uart_rx_dma_buffer[255] = {0};
static atomic_bool uart_isr_new_data_flag = false;
static uart_data_t uart_rx_data;

static void uart_isr_idle_cb(void);
static void enable_uart_dma_isr(void);
static void dma_config(void);
static void dma_rx_cplt_cb(DMA_HandleTypeDef* dma);
static void dma_rx_half_cplt_cb(DMA_HandleTypeDef* dma);
static void dma_rx_error_cb(DMA_HandleTypeDef* dma);
static void dma_rx_abort_cb(DMA_HandleTypeDef* dma);
static void process_new_data_isr(void);
static void copy_rx_data(uart_data_t* const from, uart_data_t* to);

void uart_init(void)
{
  uart_init_config = (UART_HandleTypeDef){
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
  assert(HAL_UART_Init(&uart_init_config) == HAL_OK);
  uart_isr_new_data_flag = false;
  uart_rx_data = (uart_data_t){
    .data = NULL,
    .len = 0
  };

  dma_config();
  enable_uart_dma_isr();
  HAL_UART_Receive_DMA(&uart_init_config, uart_rx_dma_buffer, sizeof(uart_rx_dma_buffer));
  DBG_DEBUG("uart initialized\n\r");
}

void uart_rx(uint8_t* buffer, uint16_t sz)
{
  HAL_StatusTypeDef ret;
  assert(buffer);
  assert(sz > 0);
  ret = HAL_UART_Receive(&uart_init_config, buffer, sz, UART_TIMEOUT); 
  assert(ret == HAL_OK);
}

void uart_tx(uint8_t* buffer, uint16_t sz)
{
  HAL_StatusTypeDef ret;
  assert(buffer);
  assert(sz > 0);
  ret = HAL_UART_Transmit(&uart_init_config, buffer, sz, UART_TIMEOUT);
  assert(ret == HAL_OK);
}

void uart_deinit(void)
{

}

void uart_get_data(uart_data_t* data)
{
  assert(data);
  if(uart_isr_new_data_flag) {
    uart_isr_new_data_flag = false;
    copy_rx_data(&uart_rx_data, data);
    memset((void*)uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer));
    HAL_UART_Receive_DMA(&uart_init_config, uart_rx_dma_buffer, sizeof(uart_rx_dma_buffer));
  } else {
    data->len = 0;
  }
}

static void copy_rx_data(uart_data_t* const from, uart_data_t* to)
{
  size_t bytes_to_copy = (from->len > to->len) ? to->len : from->len;
  for(size_t idx = 0; idx < bytes_to_copy; idx++) {
    to->data[idx] = from->data[idx];
  }
}

void _putchar(char character)
{
  uart_tx((uint8_t*)&character, (uint16_t)sizeof(character));
}

void uart_isr_cb(void)
{
  HAL_UART_IRQHandler(&uart_init_config);
  if(__HAL_UART_GET_FLAG(&uart_init_config, UART_FLAG_IDLE)) {
    uart_isr_idle_cb();
    __HAL_UART_CLEAR_IDLEFLAG(&uart_init_config);
  }
}

void uart_dma_isr_cb(void) {
  HAL_DMA_IRQHandler(&uart_dma_config);
}

void uart_rx_dma_isr_cb(void) {
  HAL_DMA_IRQHandler(&uart_rx_dma_config);
}

void uart_isr_idle_cb(void)
{
  process_new_data_isr();
}

static void dma_rx_cplt_cb(DMA_HandleTypeDef* dma)
{
  process_new_data_isr();
}

static void dma_rx_half_cplt_cb(DMA_HandleTypeDef* dma)
{
  process_new_data_isr();
}

static void process_new_data_isr(void)
{
  size_t len;
  len = (size_t)(sizeof(uart_rx_dma_buffer) - __HAL_DMA_GET_COUNTER(&uart_rx_dma_config));
  if(len > 0) {
    HAL_UART_DMAStop(&uart_init_config); 
    uart_rx_data.len = len;
    uart_rx_data.data = uart_rx_dma_buffer;
    uart_isr_new_data_flag = true;
  }
}

static void dma_rx_error_cb(DMA_HandleTypeDef* dma)
{
  assert(false);
}

static void dma_rx_abort_cb(DMA_HandleTypeDef* dma)
{
  assert(false);
}

static void enable_uart_dma_isr(void)
{
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

static void dma_config(void)
{
  uart_dma_config = (DMA_HandleTypeDef){
    .Instance = DMA1_Channel7,
    .Init.Direction = DMA_MEMORY_TO_PERIPH,
    .Init.MemInc = DMA_MINC_ENABLE,
    .Init.PeriphInc = DMA_PINC_DISABLE,
    .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
    .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
    .Init.Mode = DMA_NORMAL,
    .Init.Priority = DMA_PRIORITY_LOW,
  };
  assert(HAL_DMA_Init(&uart_dma_config) == HAL_OK);

  uart_rx_dma_config = (DMA_HandleTypeDef){
    .Instance = DMA1_Channel6,
    .Init.Direction = DMA_PERIPH_TO_MEMORY,
    .Init.MemInc = DMA_MINC_ENABLE,
    .Init.PeriphInc = DMA_PINC_DISABLE,
    .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
    .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
    .Init.Mode = DMA_NORMAL,
    .Init.Priority = DMA_PRIORITY_HIGH,
  };
  assert(HAL_DMA_Init(&uart_rx_dma_config) == HAL_OK);

  uart_rx_dma_config.XferCpltCallback = dma_rx_cplt_cb;
  uart_rx_dma_config.XferHalfCpltCallback = dma_rx_half_cplt_cb;
  uart_rx_dma_config.XferErrorCallback = dma_rx_error_cb;
  uart_rx_dma_config.XferAbortCallback = dma_rx_abort_cb;

  __HAL_LINKDMA(&uart_init_config, hdmatx, uart_dma_config);
  __HAL_LINKDMA(&uart_init_config, hdmarx, uart_rx_dma_config);

  /* Enable idle line detection interrupt. */
  __HAL_UART_ENABLE_IT(&uart_init_config, UART_IT_IDLE);

  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, UART_DMA_TX_PRIO, UART_DMA_TX_SUBPRIO);
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, UART_DMA_RX_PRIO, UART_DMA_RX_SUBPRIO);
  HAL_NVIC_SetPriority(USART2_IRQn, UART_IRQ_PRIO, UART_IRQ_SUBPRIO);
}
