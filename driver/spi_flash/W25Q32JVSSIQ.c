#include "W25Q32JVSSIQ.h"

static uint8_t W25Q32JVSSIQ_sector_buffer[W25Q32JVSSIQ_SECTOR_SIZE];

static void W25Q32JVSSIQ_select(W25Q32JVSSIQ_handle_t * dev)
{
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
}

static void W25Q32JVSSIQ_deselect(W25Q32JVSSIQ_handle_t * dev)
{
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

static void W25Q32JVSSIQ_wait_while_busy(W25Q32JVSSIQ_handle_t * dev)
{
    uint8_t cmd = W25Q32JVSSIQ_CMD_READ_STATUS_REG1;
    uint8_t status;

    do {
        W25Q32JVSSIQ_select(dev);
        HAL_SPI_Transmit(dev->spi_handle, &cmd, 1, HAL_MAX_DELAY);
        HAL_SPI_Receive(dev->spi_handle, &status, 1, HAL_MAX_DELAY);
        W25Q32JVSSIQ_deselect(dev);
    } while ((status & 0x01U) != 0U);
}

void W25Q32JVSSIQ_init(W25Q32JVSSIQ_handle_t * dev)
{
    W25Q32JVSSIQ_deselect(dev);
}


static void W25Q32JVSSIQ_write_enable(W25Q32JVSSIQ_handle_t * dev){
    uint8_t cmd = W25Q32JVSSIQ_CMD_WRITE_ENABLE;

    W25Q32JVSSIQ_select(dev);
    HAL_SPI_Transmit(dev->spi_handle, &cmd, 1, HAL_MAX_DELAY);
    W25Q32JVSSIQ_deselect(dev);
}

static void W25Q32JVSSIQ_write_disable(W25Q32JVSSIQ_handle_t * dev){
    uint8_t cmd = W25Q32JVSSIQ_CMD_WRITE_DISABLE;

    W25Q32JVSSIQ_select(dev);
    HAL_SPI_Transmit(dev->spi_handle, &cmd, 1, HAL_MAX_DELAY);
    W25Q32JVSSIQ_deselect(dev);
}


void W25Q32JVSSIQ_read_data(W25Q32JVSSIQ_handle_t * dev, uint32_t address, uint8_t * buffer, uint16_t size){
    uint8_t cmd[4] = {
        W25Q32JVSSIQ_CMD_READ_DATA,
        (uint8_t)((address >> 16) & 0xFFU),
        (uint8_t)((address >> 8) & 0xFFU),
        (uint8_t)(address & 0xFFU)
    };

    W25Q32JVSSIQ_select(dev);
    HAL_SPI_Transmit(dev->spi_handle, cmd, sizeof(cmd), HAL_MAX_DELAY);
    HAL_SPI_Receive(dev->spi_handle, buffer, size, HAL_MAX_DELAY);
    W25Q32JVSSIQ_deselect(dev);
}

void W25Q32JVSSIQ_page_program(W25Q32JVSSIQ_handle_t * dev, uint32_t address, uint8_t * data, uint16_t size){
    uint8_t cmd[4] = {
        W25Q32JVSSIQ_CMD_PAGE_PROGRAM,
        (uint8_t)((address >> 16) & 0xFFU),
        (uint8_t)((address >> 8) & 0xFFU),
        (uint8_t)(address & 0xFFU)
    };

    W25Q32JVSSIQ_write_enable(dev);
    W25Q32JVSSIQ_select(dev);
    HAL_SPI_Transmit(dev->spi_handle, cmd, sizeof(cmd), HAL_MAX_DELAY);
    HAL_SPI_Transmit(dev->spi_handle, data, size, HAL_MAX_DELAY);
    W25Q32JVSSIQ_deselect(dev);
    W25Q32JVSSIQ_wait_while_busy(dev);
}

void W25Q32JVSSIQ_sector_erase(W25Q32JVSSIQ_handle_t * dev, uint32_t address){
    uint8_t cmd[4] = {
        W25Q32JVSSIQ_CMD_SECTOR_ERASE,
        (uint8_t)((address >> 16) & 0xFFU),
        (uint8_t)((address >> 8) & 0xFFU),
        (uint8_t)(address & 0xFFU)
    };

    W25Q32JVSSIQ_write_enable(dev);
    W25Q32JVSSIQ_select(dev);
    HAL_SPI_Transmit(dev->spi_handle, cmd, sizeof(cmd), HAL_MAX_DELAY);
    W25Q32JVSSIQ_deselect(dev);
    W25Q32JVSSIQ_wait_while_busy(dev);
}

void W25Q32JVSSIQ_write_data(W25Q32JVSSIQ_handle_t * dev, uint32_t address, uint8_t * data, uint16_t size)
{
    uint32_t current_address = address;
    uint16_t remaining = size;

    while (remaining > 0U) {
        const uint32_t sector_base = current_address & ~(W25Q32JVSSIQ_SECTOR_SIZE - 1U);
        const uint32_t sector_offset = current_address - sector_base;
        uint32_t chunk_size = W25Q32JVSSIQ_SECTOR_SIZE - sector_offset;
        uint32_t program_offset;

        if (chunk_size > remaining) {
            chunk_size = remaining;
        }

        W25Q32JVSSIQ_read_data(dev, sector_base, W25Q32JVSSIQ_sector_buffer, W25Q32JVSSIQ_SECTOR_SIZE);
        for (program_offset = 0U; program_offset < chunk_size; ++program_offset) {
            W25Q32JVSSIQ_sector_buffer[sector_offset + program_offset] = data[program_offset];
        }

        W25Q32JVSSIQ_sector_erase(dev, sector_base);

        for (program_offset = 0U; program_offset < W25Q32JVSSIQ_SECTOR_SIZE; program_offset += W25Q32JVSSIQ_PAGE_SIZE) {
            W25Q32JVSSIQ_page_program(dev,
                                      sector_base + program_offset,
                                      &W25Q32JVSSIQ_sector_buffer[program_offset],
                                      W25Q32JVSSIQ_PAGE_SIZE);
        }

        current_address += chunk_size;
        data += chunk_size;
        remaining = (uint16_t)(remaining - chunk_size);
    }
}
