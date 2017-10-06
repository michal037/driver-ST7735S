# Developing Tips

> explanation: `v1.3; p10` means official datasheet version 1.3 page 10

## # 1
In the documentation pin `A0` is `D/CX`, `SCK` is `SCL`, `CS` is `CSX` and `RESET` is `RESX`.

`v1.3; p23-24`

## # 2
The module is adapted to `4-lines serial interface` because it uses SDA, SCK, CS and A0 pins. Works with standard `MODE_0` SPI and 8-bits word where MSB is shifted out as first.

`v1.3; p36` `v1.3; p44`

## # 3
To active communication between MCU and Driver, pin `CS` must be logic 0 - `GND` volatage. Otherwise must be logic 1 (no communication) - `VCC` voltage.

`v1.3; p44-45`

To send command to the display driver, pin `A0` must be logic 0 - `GND` voltage. To send data or command parameter to the display driver, pin `A0` must be logic 1 - `VCC` voltage.

`v1.3; p24` `v1.3; p44`

To activate the hardware reset, pin `RESET` must be logic 0 - GND voltage. In normal operation must be logic 1 - VCC voltage.

`v1.3; p23`

## # 4
Write mode; Any instruction can be sent in any order to the driver. The serial interface is initialized when `CSX` is high. In this state, `SCL` clock pulse or `SDA` data have no effect. A falling edge on `CSX` enables the serial interface and indicates the start of data transmission.

`v1.3; p44`

When `CSX` is “high”, `SCL` clock is ignored. During the high period of `CSX` the serial interface is initialized. At
the falling edge of `CSX`, `SCL` can be high or low. `SDA` is sampled at the rising edge of `SCL`. `D/CX` is
sampled on 8th rising edge of `SCL`. If `CSX` stays low after the last bit of command/data byte, the serial interface expects the next byte at the next rising edge of `SCL`.

`v1.3; p45`
