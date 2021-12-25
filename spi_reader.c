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

#define FLASH_CMD_READ          0x03
#define FLASH_CMD_STATUS        0x05

#define FLASH_STATUS_BUSY_MASK  0x01

#define SPI_RX_PIN              0
#define SPI_TX_PIN              3
#define SPI_SCK_PIN             2
#define SPI_CSN_PIN             1



static inline void cs_select(uint cs_pin) {
	asm volatile("nop \n nop \n nop"); // FIXME
	gpio_put(cs_pin, 0);
	asm volatile("nop \n nop \n nop"); // FIXME
}

static inline void cs_deselect(uint cs_pin) {
	asm volatile("nop \n nop \n nop"); // FIXME
	gpio_put(cs_pin, 1);
	asm volatile("nop \n nop \n nop"); // FIXME
}

void __not_in_flash_func(flash_read)(spi_inst_t *spi, uint cs_pin, uint32_t addr, uint8_t *buf, size_t len) {
	cs_select(cs_pin);
	uint8_t cmdbuf[4] = {
		FLASH_CMD_READ,
		addr >> 16,
		addr >> 8,
		addr
	};

	spi_write_blocking(spi, cmdbuf, 4);
	spi_read_blocking(spi, 0, buf, len);
	cs_deselect(cs_pin);
}


void __not_in_flash_func(flash_wait_done)(spi_inst_t *spi, uint cs_pin) {
	uint8_t status;
	do {
		cs_select(cs_pin);
		uint8_t buf[2] = {FLASH_CMD_STATUS, 0};
		spi_write_read_blocking(spi, buf, buf, 2);
		cs_deselect(cs_pin);
		status = buf[1];
	} while (status & FLASH_STATUS_BUSY_MASK);
}

void printbuf(uint8_t buf[FLASH_PAGE_SIZE]) {
	for (int i = 0; i < FLASH_PAGE_SIZE; i++) {
		printf("%c", (char) buf[i]);
	}
}


int main(){
	stdio_usb_init();
	while(!stdio_usb_connected()){
		sleep_ms(100);
	}
	sleep_ms(1000);
	spi_init(spi0, 1*1000*1000);

	gpio_set_function(SPI_RX_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_TX_PIN, GPIO_FUNC_SPI);

	gpio_init(SPI_CSN_PIN);
	gpio_put(SPI_CSN_PIN, 1);
	gpio_set_dir(SPI_CSN_PIN, GPIO_OUT);


	uint8_t page_buf[FLASH_PAGE_SIZE];    
	uint32_t target_addr = 0;

	for(target_addr = 0; target_addr < FLASH_TOTAL_SIZE; target_addr += FLASH_PAGE_SIZE){
		memset(&page_buf, 0x0, FLASH_PAGE_SIZE);
		flash_read(spi0, SPI_CSN_PIN, target_addr, page_buf, FLASH_PAGE_SIZE);
		printbuf(page_buf);
	}


	return 0;

}



