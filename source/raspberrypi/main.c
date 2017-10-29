/*
 * MIT License
 * Copyright (c) from 2017, Michal Kozakiewicz, github.com/michal037
 */

#include <stdio.h>
#include <wiringPi.h> /* Can be changed */
#include "st7735s.h"

int main(int argc, char *argv[])
{
    printf("BEGIN TEST!\n");

    /* Setup WiringPi */
    wiringPiSetup();

    /* Create display, set as active */
    st7735sType disp1 = st7735s.conf.createDisplay(15000000, 0, 5, -1);

        while(1)
        {
            st7735s.draw.fillScreen(0, 0, 0);
            st7735s.draw.fillScreen(255, 255, 255);
            st7735s.draw.fillScreen(255, 0, 0);
            st7735s.draw.fillScreen(0, 255, 0);
            st7735s.draw.fillScreen(0, 0, 255);
            st7735s.draw.fillScreen(255, 0, 255);
            st7735s.draw.fillScreen(255, 255, 0);
            st7735s.draw.fillScreen(0, 255, 255);
        }

    /* Destroy active display */
    st7735s.conf.destroyActiveDisplay();

    printf("END TEST!\n");

    return 0;
} /* main */
