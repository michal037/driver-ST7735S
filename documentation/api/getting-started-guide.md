# Getting started guide

1. Enable support for the [hardware SPI interface](https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md) using [raspi-config](https://www.raspberrypi.org/documentation/configuration/raspi-config.md).
2. Download and install the official [Wiring Pi library](http://wiringpi.com/download-and-install/).
3. Connect the display to the [Raspberry Pi GPIO interface](https://www.raspberrypi.org/documentation/usage/gpio/README.md). You can display the GPIO list with the `gpio readall` command.
   1. VCC to pin number 1 (3,3V).
   2. GND to pin number 6 (0V).
   3. CS to pin number 24 (CE0) or 26 (CE1).
   4. RESET to any IO pin (3).
   5. A0 to any IO pin (5).
   6. SDA to pin number 19 (MOSI).
   7. SCK to pin number 23 (SCLK).
   8. LED to pin number 17 (3,3V).
4. 
