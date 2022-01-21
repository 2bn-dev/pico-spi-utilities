#include "spi.h"


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


	uint32_t target_addr = 0;

	for(target_addr = 0; target_addr < FLASH_TOTAL_SIZE; target_addr += FLASH_SECTOR_SIZE){
		flash_erase(spi0, SPI_CSN_PIN,  target_addr);
		flash_wait_done(spi0, SPI_CSN_PIN);
		printf("Flash Erase: %d, %d\n", target_addr, FLASH_SECTOR_SIZE);
	}

	printf("\nFlash Erase [theoretically] done\n");
	return 0;

}



