/*
 * MIT License
 * Copyright (c) 2018, Michal Kozakiewicz, github.com/michal037
 *
 * Version: 2.0.0
 * Standard: GCC-C11
 */

#ifndef _LIBRARY_ST7735S_
#define _LIBRARY_ST7735S_
#ifdef __cplusplus
extern "C" {
#endif

/* Pixel sizes */
#define ST7735S_PIXEL_FULL 1
#define ST7735S_PIXEL_REDUCED 0

/*
 * This setting determines the number of bits per pixel.
 * Choose the above pixel size, enter it in the configuration.
 * Without change, default value are selected.
 ******************************** CONFIGURATION *******************************/
#define ST7735S_CFG_PIXEL ST7735S_PIXEL_FULL
/**************************** END CONFIGURATION END ***************************/

/* Type simplification; The 8-bit unsigned integer */
#ifndef uint8
#define uint8 unsigned char
#endif
 
/* The data type for one display */
typedef struct
{
	int cs, a0, rs;
	uint8 width, height;
} lcdst_t;

/*
 * Initialize the display and create a data structure for it.
 * The last initialized display is active.
 *
 * Parameters:
 *   spiSpeed - Speed of the SPI interface.
 *   cs - Chip selection pin.
 *   a0 - Data/Command pin.
 *   rs - Optional reset pin. If you do not use it, enter -1.
 *
 * Return: Pointer to the structure with display data.
 *
 */
lcdst_t *lcdst_init(int spiSpeed, int cs, int a0, int rs);

/*
 * Reset the specified display and clear the previously assigned memory.
 *
 * Parameters:
 *   display - Pointer to the structure with display data.
 *
 * Return: void
 */
void lcdst_uninit(lcdst_t *display);

/*
 * Perform a hardware reset on a specified display.
 *
 * Parameters:
 *   display - Pointer to the structure with display data.
 *
 * Return: void
 */
void lcdst_hardwareReset(lcdst_t *display);

/*
 * Set the pointer to structure with the display data as active.
 *
 * Parameters:
 *   display - Pointer to the structure with display data.
 *
 * Return: void
 */
void lcdst_setActiveDisplay(lcdst_t *display);

/*
 * Get the pointer to structure with the display data,
 * which is currently active.
 *
 * Parameters: none
 * Return: Pointer to the structure with display data.
 *
 */
lcdst_t *lcdst_getActiveDisplay(void);

/*
 * Get the width of the currently active display.
 *
 * Parameters: none
 * Return: The width of the currently active display.
 *
 */
uint8 lcdst_getWidth(void);

/*
 * Get the height of the currently active display.
 *
 * Parameters: none
 * Return: The height of the currently active display.
 *
 */
uint8 lcdst_getHeight(void);

/*
 * Set the orientation of the currently active display.
 *
 * Parameters:
 *   orientation - Choose one orientation: 0/1/2/3.
 *
 * Return: void
 */
void lcdst_setOrientation(uint8 orientation);

/*
 * Set the gamma correction for the currently active display.
 *
 * Parameters:
 *   state - Choose one gamma correction: 0/1/2/3.
 *
 * Return: void
 */
void lcdst_setGamma(uint8 state);

/*
 * Set the color inversion for the currently active display.
 *
 * Parameters:
 *   state - Choose one: 0 = 0FF; 1 = ON.
 *
 * Return: void
 */
void lcdst_setInversion(uint8 state);

/*
 * Set the drawing area on the currently active display.
 *
 * Parameters:
 *   x1 - The X parameter of the first point.
 *   y1 - The Y parameter of the first point.
 *   x2 - The X parameter of the second point.
 *   y2 - The Y parameter of the second point.
 *
 * Return: Confirmation of the occurrence or non-occurrence of an error.
 * 0 - The error did not occur; 1 - The error occurred.
 *
 */
uint8 lcdst_setWindow(uint8 x1, uint8 y1, uint8 x2, uint8 y2);

/*
 * Set the currently active display to RAM modification mode.
 *
 * Parameters: none
 * Return: void
 */
void lcdst_activateRamWrite(void);

/*
 * Send the raw pixel color to the currently active display.
 *
 * Parameters:
 *   r - The intensity of the red color on a scale from 0 to 255.
 *   g - The intensity of the green color on a scale from 0 to 255.
 *   b - The intensity of the blue color on a scale from 0 to 255.
 *
 * Return: void
 */
void lcdst_pushPx(uint8 r, uint8 g, uint8 b);

/*
 * Send two raw pixel colors to the currently active display.
 * This is a function for a pixel with a reduced number of bits.
 *
 * Parameters:
 *   r - The intensity of the red color for the first pixel
 *       on a scale from 0 to 15.
 *   g - The intensity of the green color for the first pixel
 *       on a scale from 0 to 15.
 *   b - The intensity of the blue color for the first pixel
 *       on a scale from 0 to 15.
 *   rr - The intensity of the red color for the second pixel
 *       on a scale from 0 to 15.
 *   gg - The intensity of the green color for the second pixel
 *       on a scale from 0 to 15.
 *   bb - The intensity of the blue color for the second pixel
 *       on a scale from 0 to 15.
 *
 * Return: void
 */
void lcdst_pushRPx(uint8 r, uint8 g, uint8 b, uint8 rr, uint8 gg, uint8 bb);

/*
 * Draw one pixel on the currently active display.
 * The color intensity scale for a normal pixel is from 0 to 255.
 * The color intensity scale for the reduced pixel is from 0 to 15.
 *
 * Parameters:
 *   x - The X parameter of the pixel.
 *   y - The Y parameter of the pixel.
 *   r - The intensity of the red color.
 *   g - The intensity of the green color.
 *   b - The intensity of the blue color.
 *
 * Return: void
 */
void lcdst_drawPx(uint8 x, uint8 y, uint8 r, uint8 g, uint8 b);

/*
 * Draw a horizontal line on the currently active display.
 * The color intensity scale for a normal pixel is from 0 to 255.
 * The color intensity scale for the reduced pixel is from 0 to 15.
 *
 * Parameters:
 *   x - Parameter X of the left end of the line.
 *   y - Parameter Y of the left end of the line.
 *   l - The length of the line.
 *   r - The intensity of the red color.
 *   g - The intensity of the green color.
 *   b - The intensity of the blue color.
 *
 * Return: void
 */
void lcdst_drawHLine(uint8 x, uint8 y, uint8 l, uint8 r, uint8 g, uint8 b);

/*
 * Draw a vertical line on the currently active display.
 * The color intensity scale for a normal pixel is from 0 to 255.
 * The color intensity scale for the reduced pixel is from 0 to 15.
 *
 * Parameters:
 *   x - The X parameter of the upper end of the line.
 *   y - The Y parameter of the upper end of the line.
 *   l - The length of the line.
 *   r - The intensity of the red color.
 *   g - The intensity of the green color.
 *   b - The intensity of the blue color.
 *
 * Return: void
 */
void lcdst_drawVLine(uint8 x, uint8 y, uint8 l, uint8 r, uint8 g, uint8 b);

/*
 * Draw a rectangle on the currently active display.
 * The color intensity scale for a normal pixel is from 0 to 255.
 * The color intensity scale for the reduced pixel is from 0 to 15.
 *
 * Parameters:
 *   x - Parameter X of the upper left corner of the rectangle.
 *   y - Parameter Y of the upper left corner of the rectangle.
 *   w - The width of the rectangle.
 *   h - The height of the rectangle.
 *   r - The intensity of the red color.
 *   g - The intensity of the green color.
 *   b - The intensity of the blue color.
 *
 * Return: void
 */
void lcdst_drawRect(uint8 x, uint8 y, uint8 w, uint8 h,
					uint8 r, uint8 g, uint8 b);

/*
 * Draw a filled rectangle on the currently active display.
 * The color intensity scale for a normal pixel is from 0 to 255.
 * The color intensity scale for the reduced pixel is from 0 to 15.
 *
 * Parameters:
 *   x - Parameter X of the upper left corner of the rectangle.
 *   y - Parameter Y of the upper left corner of the rectangle.
 *   w - The width of the rectangle.
 *   h - The height of the rectangle.
 *   r - The intensity of the red color.
 *   g - The intensity of the green color.
 *   b - The intensity of the blue color.
 *
 * Return: void
 */
void lcdst_drawFRect(uint8 x, uint8 y, uint8 w, uint8 h,
					uint8 r, uint8 g, uint8 b);

/*
 * Fill the entire screen with one color of the currently active display.
 * The color intensity scale for a normal pixel is from 0 to 255.
 * The color intensity scale for the reduced pixel is from 0 to 15.
 *
 * Parameters:
 *   r - The intensity of the red color.
 *   g - The intensity of the green color.
 *   b - The intensity of the blue color.
 *
 * Return: void
 */
void lcdst_drawScreen(uint8 r, uint8 g, uint8 b);

#ifdef __cplusplus
}
#endif
#endif /* _LIBRARY_ST7735S_ */
