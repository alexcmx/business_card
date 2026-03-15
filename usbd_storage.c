#include "usbd_storage.h"

#include "main.h"

#define STORAGE_LUN_NBR 1U
#define STORAGE_BLK_SIZ 512U
#define STORAGE_BLK_NBR (W25Q32JVSSIQ_FLASH_SIZE / STORAGE_BLK_SIZ)
#define STORAGE_MAX_XFER_BLOCKS (UINT16_MAX / STORAGE_BLK_SIZ)

static int8_t STORAGE_Init(uint8_t lun);
static int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady(uint8_t lun);
static int8_t STORAGE_IsWriteProtected(uint8_t lun);
static int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun(void);

static int8_t STORAGE_Inquirydata[] = {
    0x00, 0x80, 0x02, 0x02, (STANDARD_INQUIRY_DATA_LEN - 5), 0x00, 0x00, 0x00,
    'C', 'o', 'd', 'e', 'x', ' ', ' ', ' ',
    'S', 'P', 'I', ' ', 'F', 'l', 'a', 's',
    'h', ' ', 'M', 'S', 'C', ' ', ' ', ' ',
    '1', '.', '0', '0'
};

USBD_StorageTypeDef USBD_DISK_fops = {
    STORAGE_Init,
    STORAGE_GetCapacity,
    STORAGE_IsReady,
    STORAGE_IsWriteProtected,
    STORAGE_Read,
    STORAGE_Write,
    STORAGE_GetMaxLun,
    STORAGE_Inquirydata,
};

static int8_t STORAGE_Init(uint8_t lun)
{
    UNUSED(lun);
    return 0;
}

static int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
    UNUSED(lun);
    *block_num = STORAGE_BLK_NBR;
    *block_size = STORAGE_BLK_SIZ;
    return 0;
}

static int8_t STORAGE_IsReady(uint8_t lun)
{
    UNUSED(lun);
    return 0;
}

static int8_t STORAGE_IsWriteProtected(uint8_t lun)
{
    UNUSED(lun);
    return (USB_MSC_READ_ONLY != 0U) ? 1 : 0;
}

static int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    uint16_t remaining = blk_len;

    UNUSED(lun);

    while (remaining > 0U) {
        const uint16_t transfer_blocks = (remaining > STORAGE_MAX_XFER_BLOCKS) ? STORAGE_MAX_XFER_BLOCKS : remaining;

        W25Q32JVSSIQ_read_data(&spi_handle,
                               blk_addr * STORAGE_BLK_SIZ,
                               buf,
                               (uint16_t)(transfer_blocks * STORAGE_BLK_SIZ));
        blk_addr += transfer_blocks;
        buf += transfer_blocks * STORAGE_BLK_SIZ;
        remaining = (uint16_t)(remaining - transfer_blocks);
    }

    return 0;
}

static int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    uint16_t remaining = blk_len;

    UNUSED(lun);

    if (USB_MSC_READ_ONLY != 0U) {
        UNUSED(buf);
        UNUSED(blk_addr);
        UNUSED(blk_len);
        return -1;
    }

    while (remaining > 0U) {
        const uint16_t transfer_blocks = (remaining > STORAGE_MAX_XFER_BLOCKS) ? STORAGE_MAX_XFER_BLOCKS : remaining;

        W25Q32JVSSIQ_write_data(&spi_handle,
                                blk_addr * STORAGE_BLK_SIZ,
                                buf,
                                (uint16_t)(transfer_blocks * STORAGE_BLK_SIZ));
        blk_addr += transfer_blocks;
        buf += transfer_blocks * STORAGE_BLK_SIZ;
        remaining = (uint16_t)(remaining - transfer_blocks);
    }

    return 0;
}

static int8_t STORAGE_GetMaxLun(void)
{
    return (int8_t)(STORAGE_LUN_NBR - 1U);
}
