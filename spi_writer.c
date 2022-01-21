#include "spi.h"

int main(){
	stdio_usb_init();
	while(!stdio_usb_connected()){
		sleep_ms(100);
	}
	sleep_ms(1000);
	spi_init(spi0, 12*1000*1000);

	gpio_set_function(SPI_RX_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_TX_PIN, GPIO_FUNC_SPI);

	gpio_init(SPI_CSN_PIN);
	gpio_put(SPI_CSN_PIN, 1);
	gpio_set_dir(SPI_CSN_PIN, GPIO_OUT);

	uint8_t mfr_buf[2];
	flash_read_mfr(spi0, SPI_CSN_PIN, mfr_buf);
	if(mfr_buf[0] != 0xEF){
		while(true){
			printf("FATAL ERROR, mfr: %02x, %02x\r\n", mfr_buf[0], mfr_buf[1]);
			sleep_ms(1000);
		}
		return 0;
	}
	
	uint8_t page_buf[FLASH_PAGE_SIZE+1];
	uint32_t target_addr = 0;

	for(target_addr = 0; target_addr < FLASH_TOTAL_SIZE; target_addr += FLASH_PAGE_SIZE){
		memset(&page_buf, 0x0, FLASH_PAGE_SIZE);
		fread(&page_buf, sizeof(uint8_t), FLASH_PAGE_SIZE, stdin);

		if(target_addr % FLASH_SECTOR_SIZE == 0){
			flash_write_enable(spi0, SPI_CSN_PIN);
			flash_erase(spi0, SPI_CSN_PIN, target_addr);
			flash_wait_done(spi0, SPI_CSN_PIN);
			printf("flash_erase: %d\r\n", target_addr);
		}

		flash_write_enable(spi0, SPI_CSN_PIN);
		flash_write(spi0, SPI_CSN_PIN, target_addr, page_buf);
		flash_wait_done(spi0, SPI_CSN_PIN);
		printf("flash_write: %d\r\n", target_addr);
	}

	return 0;

}



