set(STM32CUBEF0_DIR "${CMAKE_SOURCE_DIR}/vendor/STM32CubeF0")
set(HAL_ROOT_DIR "${STM32CUBEF0_DIR}/Drivers/STM32F0xx_HAL_Driver")
set(HAL_SOURCE_DIR "${HAL_ROOT_DIR}/Src")
set(HAL_INCLUDE_DIR "${HAL_ROOT_DIR}/Inc")

set(HAL_SOURCES
    "${HAL_SOURCE_DIR}/stm32f0xx_hal.c"
    "${HAL_SOURCE_DIR}/stm32f0xx_hal_rcc.c"
    "${HAL_SOURCE_DIR}/stm32f0xx_hal_rcc_ex.c"
    "${HAL_SOURCE_DIR}/stm32f0xx_hal_spi.c"
    "${HAL_SOURCE_DIR}/stm32f0xx_hal_pcd.c"
    "${HAL_SOURCE_DIR}/stm32f0xx_hal_pcd_ex.c"
    "${HAL_SOURCE_DIR}/stm32f0xx_hal_cortex.c"
    "${HAL_SOURCE_DIR}/stm32f0xx_hal_gpio.c"
    "${HAL_SOURCE_DIR}/stm32f0xx_hal_dma.c"
    "${HAL_SOURCE_DIR}/stm32f0xx_ll_usb.c"
    "${HAL_SOURCE_DIR}/stm32f0xx_ll_crs.c")

add_library(stm32cubef0 STATIC ${HAL_SOURCES})

target_include_directories(stm32cubef0 PUBLIC
    ${STM32CUBEF0_DIR}/Drivers/CMSIS/Core/Include
    ${STM32CUBEF0_DIR}/Drivers/CMSIS/Device/ST/STM32F0xx/Include
    ${STM32CUBEF0_DIR}/Drivers/STM32F0xx_HAL_Driver/Inc
    ${STM32CUBEF0_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${STM32CUBEF0_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc
    ${CMAKE_SOURCE_DIR}/core)
