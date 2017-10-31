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
#define st7735s_PixelFull 18 /* DO NOT TOUCH THIS LINE! */
#define st7735s_PixelReduced 12 /* DO NOT TOUCH THIS LINE! */

//////////////////// SETUP 1
    #define CHOOSE_THE_PIXEL_FORMAT st7735s_PixelFull /* Change here! -> st7735s_PixelFull or st7735s_PixelReduced */

//////////////////// SETUP 2
#if CHOOSE_THE_PIXEL_FORMAT == st7735s_PixelReduced /* DO NOT TOUCH THIS LINE! */
#warning Please answer the question! 'IS_SYSTEM_LITTLE_ENDIAN' -> TRUE/FALSE
    #define IS_SYSTEM_LITTLE_ENDIAN TRUE /* Change here! -> TRUE or FALSE */
#endif
/* If you do not know, use is_big_endian() from st7735s.c */
/* TESTED BELOW
 *
 * Raspberry Pi Zero (Not W) v.1.3 is Little-Endian;
 */
//////////////////// ENF OF IMPORTANT CONFIGURATION

#ifndef uint8
#define uint8 unsigned char
#endif

#if CHOOSE_THE_PIXEL_FORMAT == st7735s_PixelReduced /* DO NOT TOUCH THIS LINE! */
/* Structure for 12-bits colour; Size of one copy is 2-Bytes */
typedef union
{
    #if IS_SYSTEM_LITTLE_ENDIAN == TRUE /* DO NOT TOUCH THIS LINE! */
        #warning You have chosen that system is Little-Endian!
        struct {uint8 g:4, r:4, r2:4, b:4;} rgbr2; /* Little-Endian System */
    #elif IS_SYSTEM_LITTLE_ENDIAN == FALSE /* DO NOT TOUCH THIS LINE! */
        #warning You have chosen that system is Big-Endian!
        struct {uint8 r:4, g:4, b:4, r2:4;} rgbr2; /* Big-Endian System */
    #else
        #error YOU NEED TO SET THE ANSWER FOR 'IS_SYSTEM_LITTLE_ENDIAN'!
    #endif
    uint8 stPx[2];
} st7735s_Colour12b;

/* Structure for 12-bits colour; Size of one copy is 1-Byte */
typedef union
{
    #if IS_SYSTEM_LITTLE_ENDIAN == TRUE /* DO NOT TOUCH THIS LINE! */
        struct {uint8 b2:4, g2:4;} g2b2; /* Little-Endian System */
    #elif IS_SYSTEM_LITTLE_ENDIAN == FALSE /* DO NOT TOUCH THIS LINE! */
        struct {uint8 g2:4, b2:4;} g2b2; /* Big-Endian System */
    #endif
    uint8 ndPx;
} st7735s_Colour12b_2;
#endif

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
        uint8 (* const getActiveDisplayWidth)(void);
        uint8 (* const getActiveDisplayHeight)(void);
    } conf;
    #if CHOOSE_THE_PIXEL_FORMAT == st7735s_PixelFull /* DO NOT TOUCH THIS LINE! */
        struct /* Drawing Functions for 18-bits colour */
        {
            void (* const pushColour)(uint8 red, uint8 green, uint8 blue);
            void (* const drawPx)(uint8 x, uint8 y, uint8 red, uint8 green, uint8 blue);
            void (* const drawFastHLine)(uint8 x, uint8 y, uint8 width, uint8 red, uint8 green, uint8 blue);
            void (* const drawFastVLine)(uint8 x, uint8 y, uint8 height, uint8 red, uint8 green, uint8 blue);
            void (* const drawRect)(uint8 x, uint8 y, uint8 width, uint8 height, uint8 red, uint8 green, uint8 blue);
            void (* const fillRect)(uint8 x, uint8 y, uint8 width, uint8 height, uint8 red, uint8 green, uint8 blue);
            void (* const fillScreen)(uint8 red, uint8 green, uint8 blue);
        } draw;
    #elif CHOOSE_THE_PIXEL_FORMAT == st7735s_PixelReduced /* DO NOT TOUCH THIS LINE! */
        struct /* Drawing Functions for 12-bits colour */
        {
            void (* const pushColour)(st7735s_Colour12b stPx, st7735s_Colour12b_2 ndPx);
            void (* const drawPx)(uint8 x, uint8 y, st7735s_Colour12b stPx);
            void (* const drawFastHLine)(uint8 x, uint8 y, uint8 width, st7735s_Colour12b stPx);
            void (* const drawFastVLine)(uint8 x, uint8 y, uint8 height, st7735s_Colour12b stPx);
            void (* const drawRect)(uint8 x, uint8 y, uint8 width, uint8 height, st7735s_Colour12b stPx);
            void (* const fillRect)(uint8 x, uint8 y, uint8 width, uint8 height, st7735s_Colour12b stPx);
            void (* const fillScreen)(st7735s_Colour12b stPx);
        } draw;
    #else
    #warning YOU NEED TO SET THE ANSWER FOR 'CHOOSE_THE_PIXEL_FORMAT' -> st7735s_PixelFull OR st7735s_PixelReduced
    #endif
};
extern const struct st7735s st7735s;

/* Predefined colours for 12-bits pixel */
#define st7735s_COLOR_WHITE ((st7735s_Colour12b){{15,15,15,15}})
#define st7735s_COLOR_BLACK ((st7735s_Colour12b){{0,0,0,0}})
#define st7735s_COLOR_WHITE_NDPX ((st7735s_Colour12b){{15,15}})
#define st7735s_COLOR_BLACK_NDPX ((st7735s_Colour12b){{0,0}})

/* Inversion States */
#define st7735s_InversionOn 1
#define st7735s_InversionOff 0 /* Preconfigured */

/* Gamma States */
#define st7735s_Gamma1 1 /* 1.0 */
#define st7735s_Gamma2 2 /* 2.5 : Preconfigured */
#define st7735s_Gamma3 4 /* 2.2 */
#define st7735s_Gamma4 8 /* 1.8 */

/* Orientation States */
#define st7735s_Orientation_Portrait 0 /* Preconfigured */
#define st7735s_Orientation_Landscape 1

#ifdef __cplusplus
}
#endif
#endif /* LIB_ST7735S_H */
