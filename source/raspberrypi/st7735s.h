/*
 * MIT License
 * Copyright (c) from 2017, Michal Kozakiewicz, github.com/michal037
 *
 * ST7735S driver
 * https://github.com/michal037/driver-ST7735S
 */

#ifndef LIB_ST7735S_H
#define LIB_ST7735S_H

#ifdef __cplusplus
extern "C" {
#endif

//////////////////// IMPORTANT CONFIGURATION
 #ifndef TRUE
 #define TRUE 1
 #endif
 #ifndef FALSE
 #define FALSE 0
 #endif
 #warning Please answer the question! 'IS_SYSTEM_LITTLE_ENDIAN' => TRUE/FALSE
 /* If you do not know, use is_big_endian() from st7735s.c */
 #define IS_SYSTEM_LITTLE_ENDIAN TRUE /* Change here! */
 /* TESTED BELOW
  *
  * Raspberry Pi Zero (Not W) v.1.3 is Little-Endian;
  */
//////////////////// ENF OF IMPORTANT CONFIGURATION

#ifndef uint8
#define uint8 unsigned char
#endif

/* st7735s Structure for 12-bits colour; Size of one copy is 2-Bytes */
typedef union
{
    #if IS_SYSTEM_LITTLE_ENDIAN == TRUE /* DO NOT TOUCH THIS LINE! */
        #warning You have chosen that system is Little-Endian!
        struct {uint8 g:4, r:4, r2:4, b:4;} rgbx; /* Little-Endian System */
    #elif IS_SYSTEM_LITTLE_ENDIAN == FALSE /* DO NOT TOUCH THIS LINE! */
        #warning You have chosen that system is Big-Endian!
        struct {uint8 r:4, g:4, b:4, r2:4;} rgbx; /* Big-Endian System */
        /* I am not sure if this is correct. If not, try the line below. */
        //struct {uint8 b:4, xr2:4, r:4, g:4;} rgbx; /* Big-Endian System */
    #else
        #error Makro 'IS_SYSTEM_LITTLE_ENDIAN' is not specified!
    #endif
    uint8 onePx[2];
} st7735s_Colour12b;

/* st7735s Display Type; Hidden from the user */
typedef void * st7735sType;

/* st7735s Public Functions */
struct st7735s
{
    struct /* Configuration Functions */
    {
        st7735sType (* const createDisplay)(int SPISpeed, int cs, int a0, int rs);
        void (* const destroyDisplay)(st7735sType display);
        void (* const destroyActiveDisplay)(void);
        void (* const setActiveDisplay)(st7735sType display);
        st7735sType (* const getActiveDisplay)(void);
        void (* const HWReset)(st7735sType display);
        void (* const setInversion)(uint8 state);
        void (* const setGamma)(uint8 state);
        void (* const setOrientation)(uint8 state);
        void (* const setWindow)(uint8 x1, uint8 y1, uint8 x2, uint8 y2);
        void (* const activeRAMWrite)(void);
    } conf;
    struct /* Drawing Functions for 18-bits colour */
    {
        void (* const drawPx)(uint8 x, uint8 y, uint8 red, uint8 green, uint8 blue);
        void (* const pushColour)(uint8 red, uint8 green, uint8 blue);
        void (* const drawFastVLine)(uint8 x, uint8 y, uint8 height, uint8 red, uint8 green, uint8 blue);
        void (* const drawFastHLine)(uint8 x, uint8 y, uint8 width, uint8 red, uint8 green, uint8 blue);
        void (* const fillRect)(uint8 x, uint8 y, uint8 width, uint8 height, uint8 red, uint8 green, uint8 blue);
        void (* const fillScreen)(uint8 red, uint8 green, uint8 blue);
        void (* const drawRect)(uint8 x, uint8 y, uint8 width, uint8 height, uint8 red, uint8 green, uint8 blue);
    } draw;
};
extern const struct st7735s st7735s;

#ifdef __cplusplus
}
#endif

#endif /* LIB_ST7735S_H */
