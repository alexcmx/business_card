#ifndef SPI_FLASH_H
#define SPI_FLASH_H
#include "stm32f0xx.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_spi.h"

// Commands
#define W25Q32JVSSIQ_CMD_WRITE_ENABLE 0x06
#define W25Q32JVSSIQ_CMD_VOLATILE_SR_WRITE_ENABLE 0x50
#define W25Q32JVSSIQ_CMD_WRITE_DISABLE 0x04
#define W25Q32JVSSIQ_CMD_RELEASE_POWER_DOWN 0xAB
#define W25Q32JVSSIQ_CMD_READ_DATA 0x03
#define W25Q32JVSSIQ_CMD_FAST_READ 0x0B
#define W25Q32JVSSIQ_CMD_PAGE_PROGRAM 0x02
#define W25Q32JVSSIQ_CMD_SECTOR_ERASE 0x20
#define W25Q32JVSSIQ_CMD_BLOCK_ERASE_32K 0x52
#define W25Q32JVSSIQ_CMD_BLOCK_ERASE_64K 0xD8
#define W25Q32JVSSIQ_CMD_CHIP_ERASE 0xC7
#define W25Q32JVSSIQ_CMD_CHIP_ERASE_2 0x60
#define W25Q32JVSSIQ_CMD_READ_STATUS_REG1 0x05
#define W25Q32JVSSIQ_CMD_READ_STATUS_REG2 0x35
#define W25Q32JVSSIQ_CMD_READ_STATUS_REG3 0x15
#define W25Q32JVSSIQ_CMD_WRITE_STATUS_REG1 0x01 
#define W25Q32JVSSIQ_CMD_WRITE_STATUS_REG2 0x31
#define W25Q32JVSSIQ_CMD_WRITE_STATUS_REG3 0x11
#define W25Q32JVSSIQ_CMD_READ_JEDEC_ID 0x9F

#define W25Q32JVSSIQ_PAGE_SIZE 256U
#define W25Q32JVSSIQ_SECTOR_SIZE 4096U
#define W25Q32JVSSIQ_FLASH_SIZE (4U * 1024U * 1024U)

typedef struct{
    SPI_HandleTypeDef * spi_handle;
    GPIO_TypeDef * cs_port;
    uint16_t cs_pin;
} W25Q32JVSSIQ_handle_t;

void W25Q32JVSSIQ_init(W25Q32JVSSIQ_handle_t * dev);
void W25Q32JVSSIQ_read_data(W25Q32JVSSIQ_handle_t * dev, uint32_t address, uint8_t * buffer, uint16_t size);
void W25Q32JVSSIQ_page_program(W25Q32JVSSIQ_handle_t * dev, uint32_t address, uint8_t * data, uint16_t size);
void W25Q32JVSSIQ_sector_erase(W25Q32JVSSIQ_handle_t * dev, uint32_t address);
void W25Q32JVSSIQ_block_erase_32k(W25Q32JVSSIQ_handle_t * dev, uint32_t address);
void W25Q32JVSSIQ_block_erase_64k(W25Q32JVSSIQ_handle_t * dev, uint32_t address);
void W25Q32JVSSIQ_chip_erase(W25Q32JVSSIQ_handle_t * dev);
void W25Q32JVSSIQ_write_data(W25Q32JVSSIQ_handle_t * dev, uint32_t address, uint8_t * data, uint16_t size);
#endif // SPI_FLASH_H
