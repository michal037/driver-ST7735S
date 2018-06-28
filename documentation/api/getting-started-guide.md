# Getting started guide

1. Download and install the official [Wiring Pi library](http://wiringpi.com/download-and-install/).
2. Enable support for the [hardware SPI interface](https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md) using [raspi-config](https://www.raspberrypi.org/documentation/configuration/raspi-config.md).
3. Connect the display to the [Raspberry Pi GPIO interface](https://www.raspberrypi.org/documentation/usage/gpio/README.md). You can display the GPIO list with the `gpio readall` command.
   * **VCC** to pin number **1** (3,3V).
   * **GND** to pin number **6** (0V).
   * **CS** to pin number **24** (CE0) or **26** (CE1).
   * **RESET** to any IO pin (3).
   * **A0** to any IO pin (5).
   * **SDA** to pin number **19** (MOSI).
   * **SCK** to pin number **23** (SCLK).
   * **LED** to pin number **17** (3,3V).
4. Download the [source code](/source) to the ST7735S driver from this repository.
5. Configure the `lcdst_init()` function in the [main.c file](/source/main.c).
6. To run the example program, run the `make run` command in the directory with the source files.
7. Read the description of the functions in the [st7735s.h file](/source/st7735s.h) for more details.
