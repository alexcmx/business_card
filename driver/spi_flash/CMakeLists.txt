set(STM32CUBEF0_DIR "${CMAKE_SOURCE_DIR}/vendor/STM32CubeF0")

add_library(spi_flash STATIC W25Q32JVSSIQ.c)

target_include_directories(spi_flash PUBLIC
    ${STM32CUBEF0_DIR}/Drivers/CMSIS/Core/Include
    ${STM32CUBEF0_DIR}/Drivers/CMSIS/Device/ST/STM32F0xx/Include
    ${STM32CUBEF0_DIR}/Drivers/STM32F0xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/core
    .)