#ifndef __USBD_STORAGE_H_
#define __USBD_STORAGE_H_

#include "usbd_msc.h"

#ifndef USB_MSC_READ_ONLY
#define USB_MSC_READ_ONLY 0U
#endif

extern USBD_StorageTypeDef USBD_DISK_fops;

#endif
