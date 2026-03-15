cmake_minimum_required(VERSION 3.15)

project(stm32-without-cubeide
        LANGUAGES C ASM)

add_compile_definitions(USE_HAL_DRIVER)
add_compile_options(-g -O0) # Global compile flags
add_subdirectory(driver/spi_flash/)

set(SOURCES
    main.c
    usb_device.c
    usbd_conf.c
    usbd_desc.c
    usbd_storage.c
    core/syscalls.c
    core/startup_stm32f072xb.s
    core/system_stm32f0xx.c
    core/stm32f0xx_hal_msp.c
    core/stm32f0xx_it.c
    vendor/STM32CubeF0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    vendor/STM32CubeF0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    vendor/STM32CubeF0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    vendor/STM32CubeF0/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.c
    vendor/STM32CubeF0/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.c
    vendor/STM32CubeF0/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.c
    vendor/STM32CubeF0/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.c)

function(add_firmware_target target_name read_only output_name)
    add_executable(${target_name} ${SOURCES})

    set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "${output_name}")

    target_include_directories(${target_name} PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/core)

    target_compile_definitions(${target_name} PRIVATE
        USB_MSC_READ_ONLY=${read_only})

    target_compile_options(${target_name} PRIVATE
        --specs=nano.specs)

    target_link_options(${target_name} PRIVATE
        -T ${CMAKE_SOURCE_DIR}/core/STM32F072RBTx_FLASH.ld
        -u _printf_float)

    target_link_libraries(${target_name} PRIVATE
        stm32cubef0
        spi_flash)
endfunction()

include(cmake/stm32cubef0.cmake)

add_firmware_target(business_card 0U "business_card.elf")
add_firmware_target(business_card_readonly 1U "business_card_ro.elf")

set(PROGRAMMER openocd)
set(PROGRAMMER_FLAGS -f interface/stlink.cfg -f target/stm32f0x.cfg)

add_custom_target(flash
    COMMAND ${PROGRAMMER} ${PROGRAMMER_FLAGS} -c "program $<TARGET_FILE:business_card> verify reset exit"
    DEPENDS business_card
    VERBATIM)

add_custom_target(flash_readonly
    COMMAND ${PROGRAMMER} ${PROGRAMMER_FLAGS} -c "program $<TARGET_FILE:business_card_readonly> verify reset exit"
    DEPENDS business_card_readonly
    VERBATIM)
