cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(spi-reader)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

#include(../tinyusb-multitool/CMakeLists.txt)


add_executable(spi_reader
	spi_reader.c
)

add_executable(spi_eraser
	spi_eraser.c
)


# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(
	spi_reader
       	pico_stdlib 
	hardware_spi
	pico_time
	pico_platform
	pico_stdio
	pico_bootsel_via_double_reset
	)

target_link_libraries(
	spi_eraser
	pico_stdlib
	hardware_spi
	pico_time
	pico_platform
	pico_stdio
	pico_bootsel_via_double_reset
	)

pico_enable_stdio_usb(spi_reader 1)
pico_enable_stdio_uart(spi_reader 0)

pico_enable_stdio_usb(spi_eraser 1)
pico_enable_stdio_uart(spi_eraser 0)



# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(spi_reader)
pico_add_extra_outputs(spi_eraser)


