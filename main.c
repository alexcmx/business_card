#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "stm32f0xx_hal_spi.h"
#include "stm32f0xx_ll_crs.h"
#include "usb_device.h"

SPI_HandleTypeDef flash_spi;
W25Q32JVSSIQ_handle_t spi_handle;

static void MX_GPIO_Init(void);
void clock_init()
{
  __HAL_RCC_PWR_CLK_ENABLE();

  RCC_OscInitTypeDef osc_init = {0};
  osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  osc_init.HSI48State = RCC_HSI48_ON;
  
  osc_init.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)
  {
    while(1);
  }

  RCC_ClkInitTypeDef clock_init = {0};
  clock_init.ClockType = RCC_CLOCKTYPE_SYSCLK | 
                         RCC_CLOCKTYPE_PCLK1 | 
                         RCC_CLOCKTYPE_HCLK;
  clock_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  clock_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clock_init.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&clock_init, FLASH_LATENCY_1) != HAL_OK)
  {
    while(1);
  }
  
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
   PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    while(1);
  }

  __HAL_RCC_CRS_CLK_ENABLE();
  LL_CRS_ConfigSynchronization(LL_CRS_SYNC_DIV_1,
                               LL_CRS_SYNC_SOURCE_USB,
                               LL_CRS_SYNC_POLARITY_RISING,
                               0x22U);
  LL_CRS_EnableAutoTrimming();
  LL_CRS_EnableFreqErrorCounter();
}

void spi_config()
{
  flash_spi.Instance               = SPI1;
  flash_spi.Init.Mode              = SPI_MODE_MASTER;
  flash_spi.Init.Direction         = SPI_DIRECTION_2LINES;
  flash_spi.Init.DataSize          = SPI_DATASIZE_8BIT;
  flash_spi.Init.CLKPolarity       = SPI_POLARITY_LOW;
  flash_spi.Init.CLKPhase          = SPI_PHASE_1EDGE;
  flash_spi.Init.NSS               = SPI_NSS_SOFT;
  flash_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  flash_spi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  flash_spi.Init.TIMode            = SPI_TIMODE_DISABLE;
  flash_spi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  flash_spi.Init.CRCPolynomial     = 7;
  flash_spi.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
  flash_spi.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&flash_spi) != HAL_OK)
  {
    while(1);
  }
}
void main(void)
{
  HAL_Init();
  clock_init();
  SystemCoreClockUpdate();
  MX_GPIO_Init();
  spi_config();
  spi_handle.spi_handle = &flash_spi;
  spi_handle.cs_port = FLASH_CS_PORT;
  spi_handle.cs_pin = FLASH_CS_PIN;
  W25Q32JVSSIQ_init(&spi_handle);
  MX_USB_DEVICE_Init();
  while(1){
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    HAL_Delay(500);
  }
}

static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = FLASH_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(FLASH_CS_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}
