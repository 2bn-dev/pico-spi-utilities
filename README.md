# Pico SPI Utilities

This is a set of utilities that allow you to read, write or erase SPI flash chips using a Raspberry Pi Pico (RP2040) chip.

While the pins are configurable in the code, you must use specific pins that correlate to hardware SPI functionality within the RP2040. This was built for Winbond flash, no guarauntees it will work correctly on other stuff at the moment.


### Building

```
mkdir build
cd build
cmake ..
make -j 8
```




### Using
After flashing appropriate UF2 file


##### spi_reader
spi_reader will wait for a UART connection over USB and then dump the flash in raw binary format into the UART. You should monitor the size of the output file to determine when it's done. Also, there is no mechanism to confirm the chip is actually communicating, so if your chip is miswired you will receive a file full of 00's.

```
(stty raw; cat > firmware.output.file.bin) < /dev/ttyACM0
```

```
hexdump -C firmware.output.file.bin | less
```

##### spi_eraser
spi_eraser will erase the entire flash 1 sector (4096 kb) at a time. It will wait for a serial connection and then proceed to flash.


```
minicom -b 115200 -D /dev/ttyACM0
```
You should see messages in the minicom session as it erases.


##### spi_writer
