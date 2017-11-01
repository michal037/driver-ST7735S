# Getting started guide

1. Connect the display module to the *GPIO*. Look at [module pins](https://github.com/michal037/driver-ST7735S/blob/master/README.md).
2. Add `st7735s.c` and `st7735s.h` files to your project.
3. Include a `st7735s.h` header to your program.
4. Initialize *GPIO* `wiringPiSetup()` - Raspberry Pi
5. At the beginning of the program create display `st7735s.conf.createDisplay()` and receive the display pointer.
6. Now you can use the drawing functions.
7. At the end of the program free memory allocated before, use `st7735s.conf.destroyDisplay(yourDisplay)`.

**See also API and examples.**
