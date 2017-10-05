# Developing Tips

> explanation: `v1.3; p10` means official datasheet version 1.3 page 10

## # 1
In the documentation pin `A0` is `D/CX`, `SCK` is `SCL`, `CS` is `CSX` and `RESET` is `RESX`.

`v1.3; p23-24`

## # 2
The module is adapted to `4-lines serial interface` because it uses SDA, SCK, CS and A0 pins. Works with standard `MODE_0` SPI and 8-bits word where MSB is shifted out as first.

`v1.3; p36` `v1.3; p44`

## # 3
To send command to the display driver, pin `A0` must be logic 0 - GND voltage. To send data or command parameter to the display driver, pin `A0` must be logic 1 - VCC voltage.

`v1.3; p24` `v1.3; p44`

To activate the hardware reset, pin `RESET` must be logic 0 - GND voltage. In normal operation must be logic 1 - VCC voltage.

`v1.3; p23`

To active communication between MCU and Driver, pin `CS` must be logic 0 - GND volatage. Otherwise must be logic 1 (no communication) - VCC voltage.

`v1.3; p44-45`
