#ifndef MAIN_H
#define MAIN_H

#include "stm32f0xx.h"
#include "W25Q32JVSSIQ.h"

#define FLASH_CS_PIN GPIO_PIN_4
#define FLASH_CS_PORT GPIOA

extern W25Q32JVSSIQ_handle_t spi_handle;

#endif
