cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gcc-arm-none-eabi.cmake")

project(Matrix_Led_2x2 C ASM)

add_compile_definitions(STM32F103xB)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    main.c
    startup_stm32f103xb.s
    system_stm32f1xx.c
    stm32f1xx_hal.c
    stm32f1xx_hal_gpio.c
    stm32f1xx_hal_rcc.c
    stm32f1xx_hal_cortex.c
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    .
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc
)

# ... (Giữ nguyên các đoạn khai báo nguồn và include bên trên) ...

# 1. ÉP FILE .LD CHẠY RIÊNG BIỆT BẰNG LỆNH CHUẨN CỦA CMAKE
target_link_options(${PROJECT_NAME} PRIVATE "-TD:/EmbeddedC/Matrix_Led_2x2/STM32F103XX_FLASH.ld")

# 2. TÁCH BIỆT LỆNH OBJCOPY THÀNH MỘT TARGET RIÊNG (Không nhồi nhét chung dòng với Linker nữa)
add_custom_target(generate_bin ALL
    COMMAND arm-none-eabi-objcopy -O binary "$<TARGET_FILE:${PROJECT_NAME}>" "${CMAKE_BINARY_DIR}/app_firmware.bin"
    DEPENDS ${PROJECT_NAME}
    COMMENT "Generating binary flash image: build/app_firmware.bin"
)