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


	uint8_t page_buf[FLASH_PAGE_SIZE];    
	uint32_t target_addr = 0;

	for(target_addr = 0; target_addr < FLASH_TOTAL_SIZE ; target_addr += FLASH_PAGE_SIZE){
		//printf("\r\nAddr %08x\r\n", target_addr);
		memset(&page_buf, 0x0, FLASH_PAGE_SIZE);
		flash_wait_done(spi0, SPI_CSN_PIN);
		flash_read(spi0, SPI_CSN_PIN, target_addr, page_buf, FLASH_PAGE_SIZE);
		flash_wait_done(spi0, SPI_CSN_PIN);
		printbuf(page_buf);
	}


	return 0;

}



