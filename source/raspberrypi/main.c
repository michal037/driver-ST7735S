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
    st7735sType disp1 = st7735s.conf.createDisplay(50000, 0, 5, 6);



    /* Destroy active display */
    st7735s.conf.destroyActiveDisplay();

    printf("END TEST!\n");

    return 0;
} /* main */
