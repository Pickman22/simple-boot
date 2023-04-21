#include "mcu.h"
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_rcc_ex.h"
#include "assert.h"

static void system_clock_config(void);

void mcu_init(void)
{
  system_clock_config();
}

void mcu_deinit()
{

}

static void system_clock_config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {
    .OscillatorType = RCC_OSCILLATORTYPE_HSE,
    .HSEState = RCC_HSE_ON,
    .HSEPredivValue = RCC_HSE_PREDIV_DIV1,
    .HSIState = RCC_HSI_ON,
    .PLL.PLLState = RCC_PLL_ON,
    .PLL.PLLSource = RCC_PLLSOURCE_HSE,
    .PLL.PLLMUL = RCC_PLL_MUL9,
  };
  assert(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK);

  RCC_ClkInitTypeDef RCC_ClkInitStruct = {
    .ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK|  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
    .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
    .AHBCLKDivider = RCC_SYSCLK_DIV1,
    .APB1CLKDivider = RCC_HCLK_DIV1,
    .APB2CLKDivider = RCC_HCLK_DIV1,
  };
  assert(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK);

  RCC_PeriphCLKInitTypeDef PeriphClkInit = {
    .PeriphClockSelection = RCC_PERIPHCLK_TIM1,
    .Tim1ClockSelection = RCC_TIM1CLK_HCLK,
  };
  assert(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK);
}
