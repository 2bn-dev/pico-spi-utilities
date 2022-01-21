#include "spi.h"

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

void __not_in_flash_func(flash_erase)(spi_inst_t *spi, uint cs_pin, uint32_t addr) {
	cs_select(cs_pin);
	uint8_t cmdbuf[4] = {
                FLASH_CMD_ERASE,
                addr >> 16,
                addr >> 8,
                addr
        };

        spi_write_blocking(spi, cmdbuf, 4);
        //spi_read_blocking(spi, 0, buf, len);
        cs_deselect(cs_pin);
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

void __not_in_flash_func(flash_write)(spi_inst_t *spi, uint cs_pin, uint32_t addr, uint8_t *buf) {
	cs_select(cs_pin);
	uint8_t cmdbuf[4] = {
		FLASH_CMD_PAGE_PROG,
		addr >> 16,
		addr >> 8,
		addr
	};

	spi_write_blocking(spi, cmdbuf, 4);
	spi_write_blocking(spi, buf, 256);
	cs_deselect(cs_pin);
}

void __not_in_flash_func(flash_write_enable)(spi_inst_t *spi, uint cs_pin) {
	cs_select(cs_pin);
	uint8_t cmdbuf[1] = {
		FLASH_CMD_WRITE_EN,
	};

	spi_write_blocking(spi, cmdbuf, 1);
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

void __not_in_flash_func(flash_read_mfr)(spi_inst_t *spi, uint cs_pin, uint8_t *buf){
	cs_select(cs_pin);
	uint8_t cmdbuf[4] = {
		FLASH_CMD_READ_MFR,
		0x00,
		0x00,
		0x00,
	};
	spi_write_blocking(spi, cmdbuf, 4);
	spi_read_blocking(spi, 0, buf, 2);

	cs_deselect(cs_pin);

}

void printbuf(uint8_t buf[FLASH_PAGE_SIZE]) {
	for (int i = 0; i < FLASH_PAGE_SIZE; i++) {
		printf("%c", (char) buf[i]);
	}
}

void printbuf_hex(uint8_t buf[FLASH_PAGE_SIZE]){
	for (int i=0; i < FLASH_PAGE_SIZE; i++){
		printf("%d ", (char) buf[i]);
		if(i%16 ==0){
			printf("\n");
		}
	}
}

