/*
 * MIT License
 * Copyright (c) 2018, Michal Kozakiewicz, github.com/michal037
 *
 * Version: rpi-v1.1
 * Standard: GCC-C11
 */

#include <stdio.h>
#include <wiringPi.h>
#include "st7735s.h"

int main(int argc, char *argv[])
{
	lcdst_t *myDisplay = NULL;
	
	/* Setup the WiringPi library */
	wiringPiSetup();
	
	/* Initialize the display and receive the pointer */
	myDisplay = lcdst_init(15000000, 0, 9, 8);
	
	/* Fill the entire screen with black color */
	lcdst_drawScreen(0, 0, 0);
	
	/* Draw the pixels */
	lcdst_drawPx(49, 70, 255, 255, 255);
	lcdst_drawPx(55, 70, 255, 255, 255);
	
	/* Draw the lines */
	lcdst_drawHLine(0, 49, 128, 0, 0, 255);
	lcdst_drawVLine(117, 0, 160, 255, 255, 0);
	
	/* Draw the rectangles */
	lcdst_drawRect(9, 9, 15, 15, 255, 255, 255);
	lcdst_drawRect(9, 29, 15, 15, 255, 255, 255);
	lcdst_drawFRect(29, 29, 15, 15, 150, 0, 255);
	
	/* Push the raw data */
	lcdst_setWindow(50, 120, 100, 150);
	for(int i=0; i<1000 ;i++) lcdst_pushPx(255, 255, 0);
	
	/* Reset the specified display and clear the previously assigned memory */
	printf("Pointer: %p\n", myDisplay);
	//lcdst_uninit(myDisplay);
	
	return 0;
} /* main */
