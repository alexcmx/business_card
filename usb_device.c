#include "usb_device.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage.h"

USBD_HandleTypeDef hUsbDeviceFS;

void MX_USB_DEVICE_Init(void)
{
    USBD_Init(&hUsbDeviceFS, &MSC_Desc, 0U);
    USBD_RegisterClass(&hUsbDeviceFS, USBD_MSC_CLASS);
    USBD_MSC_RegisterStorage(&hUsbDeviceFS, &USBD_DISK_fops);
    USBD_Start(&hUsbDeviceFS);
}
