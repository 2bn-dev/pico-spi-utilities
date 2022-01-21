#define PICO_STDIO_DEFAULT_CRLF 0
#define PICO_STDIO_ENABLE_CRLF_SUPPORT 0

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include "pico/stdio.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#include "pico/stdio_usb.h"


#define FLASH_PAGE_SIZE         256
#define FLASH_SECTOR_SIZE       4096
#define FLASH_PAGE_PER_KB       1024/FLASH_PAGE_SIZE
#define FLASH_TOTAL_SIZE        16*1024*FLASH_PAGE_PER_KB*FLASH_PAGE_SIZE

#define FLASH_CMD_PAGE_PROG	0x02
#define FLASH_CMD_READ          0x03
#define FLASH_CMD_STATUS        0x05
#define FLASH_CMD_WRITE_EN	0x06
#define FLASH_CMD_ERASE         0x20
#define FLASH_CMD_READ_MFR	0x90

#define FLASH_STATUS_BUSY_MASK  0x01

#define SPI_RX_PIN              0
#define SPI_TX_PIN              3
#define SPI_SCK_PIN             2
#define SPI_CSN_PIN             1



static inline void cs_select(uint cs_pin);
static inline void cs_deselect(uint cs_pin);
void __not_in_flash_func(flash_erase)(spi_inst_t *spi, uint cs_pin, uint32_t addr);
void __not_in_flash_func(flash_read)(spi_inst_t *spi, uint cs_pin, uint32_t addr, uint8_t *buf, size_t len);
void __not_in_flash_func(flash_write)(spi_inst_t *spi, uint cs_pin, uint32_t addr, uint8_t *buf);
void __not_in_flash_func(flash_write_enable)(spi_inst_t *spi, uint cs_pin);
void __not_in_flash_func(flash_wait_done)(spi_inst_t *spi, uint cs_pin);
void __not_in_flash_func(flash_read_mfr)(spi_inst_t *spi, uint cs_pin, uint8_t *buf);
void printbuf(uint8_t buf[FLASH_PAGE_SIZE]);
void printbuf_hex(uint8_t buf[FLASH_PAGE_SIZE]);

