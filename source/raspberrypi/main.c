/*
 * MIT License
 * Copyright (c) from 2017, Michal Kozakiewicz, github.com/michal037
 */

#include <stdio.h>
#include <time.h>
#include <wiringPi.h> /* Can be changed */
#include "st7735s.h"

int main(int argc, char *argv[])
{
    /* For speed test */
    clock_t start, stop;

    printf("BEGIN TEST!\n");

    /* Setup WiringPi */
    wiringPiSetup();

    /* Create display and set as active */
    st7735sType disp1 = st7735s.conf.createDisplay(15000000, 0, 5, -1);

    /* Draw 8 colours */
    start = clock();
        st7735s.draw.fillScreen(255, 0, 0); /* Red */
        st7735s.draw.fillScreen(0, 255, 0); /* Green */
        st7735s.draw.fillScreen(0, 0, 255); /* Blue */
        st7735s.draw.fillScreen(255, 255, 0); /* Yellow */
        st7735s.draw.fillScreen(0, 255, 255); /* Cyan */
        st7735s.draw.fillScreen(255, 0, 255); /* Magenta */
        st7735s.draw.fillScreen(255, 255, 255); /* White */
        st7735s.draw.fillScreen(0, 0, 0); /* Black */
    stop = clock();

    printf("One fillScreen: ~%.1f seconds!\n", (double)((stop - start)/CLOCKS_PER_SEC) / 8);

    /* Destroy active display */
    st7735s.conf.destroyActiveDisplay();

    printf("END TEST!\n");

    return 0;
} /* main */
