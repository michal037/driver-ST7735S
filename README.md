# Sitronix ST7735S display driver
[![ST7735S front](https://raw.githubusercontent.com/michal037/driver-ST7735S/master/photos/front_small.png)](https://raw.githubusercontent.com/michal037/driver-ST7735S/master/photos/front.png)


# Parameters
* **Display size:** 1.8 inch
* **Resolution:** 128(H) x 160(V) px
* **Colors:** 262144
* **Driver:** ST7735S
* **Protocol:** [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus)
* **Logic:** [CMOS](https://en.wikipedia.org/wiki/CMOS) and [TTL](https://en.wikipedia.org/wiki/Transistor%E2%80%93transistor_logic) compatible
* **Technology:** [LCD](https://en.wikipedia.org/wiki/Liquid-crystal_display) [TFT](https://en.wikipedia.org/wiki/Thin-film_transistor)
* **Operating temperature:** -30 to +85 C
* **Additional:** Built-in [SD](https://en.wikipedia.org/wiki/Secure_Digital) card slot
* **Dimensions:** 58,0 x 34,5 mm


# Pins
## LCD
| Pin | Name | Description |
| :---: | :---: | --- |
| 1 | `VCC` | Power supply |
| 2 | `GND` | Power ground |
| 3 | `CS` | Chip select (SPI) |
| 4 | `RESET` | Reset pin |
| 5 | `A0` | Data/Command switch |
| 6 | `SDA` | Master Output Slave Input (SPI) |
| 7 | `SCK` | Serial Clock (SPI) |
| 8 | `LED` | Backlight |

## SD
| Pin | Name | Description |
| :---: | :---: | --- |
| 1 | `SD_CS` | Chip select (SPI) |
| 2 | `SD_MOSI` | Master Output Slave Input (SPI) |
| 3 | `SD_MISO` | Master Input Slave Output (SPI) |
| 4 | `SD_SCK` | Serial Clock (SPI) |


# License
:page_facing_up: [MIT License](https://raw.githubusercontent.com/michal037/driver-ST7735S/master/LICENSE)

Copyright (c) 2017 Michal Kozakiewicz, [github.com/michal037](https://github.com/michal037)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
