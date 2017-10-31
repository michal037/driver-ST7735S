/*
 * MIT License
 * Copyright (c) from 2017, Michal Kozakiewicz, github.com/michal037
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "st7735s.h"

/************************ FOR EASIER PORT ************************/
#include <wiringPi.h> /* Can be changed */
#include <wiringPiSPI.h> /* Can be changed */
struct /* Function pointers structure */
{
    void (* const pinMode)(int pin, int mode); /* DO NOT TOUCH! */
    void (* const digitalWrite)(int pin, int value); /* DO NOT TOUCH! */
    void (* const delay)(unsigned int howLong); /* DO NOT TOUCH! */
    int (* const SPISetup)(int channel, int speed); /* DO NOT TOUCH! */
    int (* const SPIDataRW)(int channel, uint8 *data, int len); /* DO NOT TOUCH! */
} static const gpio = /* DEFINITION */
{ /* Function pointers can be changed */
    pinMode,
    digitalWrite,
    delay,
    wiringPiSPISetup,
    wiringPiSPIDataRW
};
/******************** END FOR EASIER PORT END ********************/

/* st7735s RAW Display Type */
typedef struct
{
    int cs; /* Chip select line */
    int a0; /* Data/Command line */
    int rs; /* Reset line */
    uint8 width; /* Currently used width */
    uint8 height; /* Currently used height */
} st7735sTypeRaw;

static st7735sType ActiveDisplay; /* FILE GLOBAL: Current active display */

/* Public functions declarations */
// Configuration
static st7735sType createDisplay(int SPISpeed, int cs, int a0, int rs);
static void destroyDisplay(st7735sType display);
static void destroyActiveDisplay(void);
static void setActiveDisplay(st7735sType display);
static st7735sType getActiveDisplay(void);
static void HWReset(st7735sType display);
static void setInversion(uint8 state);
static void setGamma(uint8 state);
static void setOrientation(uint8 state);
static void setWindow(uint8 x1, uint8 y1, uint8 x2, uint8 y2);
static void activeRAMWrite(void);
static uint8 getActiveDisplayWidth(void);
static uint8 getActiveDisplayHeight(void);
#if CHOOSE_THE_PIXEL_FORMAT == st7735s_PixelFull /* DO NOT TOUCH THIS LINE! */
    // Drawing functions for 18-bits colour
    static void pushColour(uint8 red, uint8 green, uint8 blue);
    static void drawPx(uint8 x, uint8 y, uint8 red, uint8 green, uint8 blue);
    static void drawFastHLine(uint8 x, uint8 y, uint8 width, uint8 red, uint8 green, uint8 blue);
    static void drawFastVLine(uint8 x, uint8 y, uint8 height, uint8 red, uint8 green, uint8 blue);
    static void drawRect(uint8 x, uint8 y, uint8 width, uint8 height, uint8 red, uint8 green, uint8 blue);
    static void fillRect(uint8 x, uint8 y, uint8 width, uint8 height, uint8 red, uint8 green, uint8 blue);
    static void fillScreen(uint8 red, uint8 green, uint8 blue);
#elif CHOOSE_THE_PIXEL_FORMAT == st7735s_PixelReduced /* DO NOT TOUCH THIS LINE! */
    // Drawing functions for 12-bits colour
    static void pushColour(st7735s_Colour12b stPx, st7735s_Colour12b_2 ndPx);
    static void drawPx(uint8 x, uint8 y, st7735s_Colour12b stPx);
    static void drawFastHLine(uint8 x, uint8 y, uint8 width, st7735s_Colour12b stPx);
    static void drawFastVLine(uint8 x, uint8 y, uint8 height, st7735s_Colour12b stPx);
    static void drawRect(uint8 x, uint8 y, uint8 width, uint8 height, st7735s_Colour12b stPx);
    static void fillRect(uint8 x, uint8 y, uint8 width, uint8 height, st7735s_Colour12b stPx);
    static void fillScreen(st7735s_Colour12b stPx);
#endif

/* Public st7735s functions */
const struct st7735s st7735s = /* DEFINITION */
{
    { /* Configuration Functions */
        createDisplay,
        destroyDisplay,
        destroyActiveDisplay,
        setActiveDisplay,
        getActiveDisplay,
        HWReset,
        setInversion,
        setGamma,
        setOrientation,
        setWindow,
        activeRAMWrite,
        getActiveDisplayWidth,
        getActiveDisplayHeight
    },
    { /* Drawing Functions */
        pushColour,
        drawPx,
        drawFastHLine,
        drawFastVLine,
        drawRect,
        fillRect,
        fillScreen
    }
};

/************************ PRIVATE FUNCTIONS ************************/
/*
 * is_big_endian()
 *
 * Result:
 *   0: No, System is Little-Endian!
 *   1: Yes, System is Big-Endian!
 */
int is_big_endian(void)
{ // Add: #include <stdint.h>
    // GUIDE
    // |Big-Endian---------|
    // |0x01|0x00|0x00|0x00|
    // |MSB-|----|----|LSB-|
    // |-READ-->>|-READ-->>|
    // |0---|1---|2---|3---|
    // =====================
    // |Little-Endian------|
    // |0x00|0x00|0x00|0x01|
    // |LSB-|----|----|MSB-|
    // |<<--READ-|<<--READ-|
    // |0---|1---|2---|3---|
    union
    {
        uint32_t i;
        char c[4];
    } e = {0x01000000};

    printf("System is %s-Endian!\n", e.c[0] ? "Big" : "Little");

    return e.c[0]; /* Output: [1:Big], [0:Little] Endian */
} /* is_big_endian */

/*
 * safe_Malloc()
 *
 * Parameters:
 *   INPUT: size - size of memory to allocate
 *
 * Result:
 *   Pointer to memory. If cause error, close program.
 */
static inline void *safe_Malloc(size_t size)
{
    void *mem = malloc(size);

    if(mem == NULL)
    {
        fprintf(stderr, "Out of RAM memory!\n");
        exit(EXIT_FAILURE);
    }

    return mem;
} /* safe_Malloc */

/*
 * SPIWrite()
 *
 * Parameters:
 *   INPUT: data - pointer to data for write
 */
static inline void SPIWrite(uint8 data)
{
    gpio.SPIDataRW(((st7735sTypeRaw*)ActiveDisplay)->cs, &data, 1);
} /* SPIWrite */

/*
 * writeData()
 *
 * Parameters:
 *   INPUT: data - pointer to data for write
 */
static inline void writeData(uint8 data)
{
    gpio.digitalWrite(((st7735sTypeRaw*)ActiveDisplay)->a0, HIGH);
    gpio.SPIDataRW(((st7735sTypeRaw*)ActiveDisplay)->cs, &data, 1);
} /* writeData */

/*
 * writeCommand()
 *
 * Parameters:
 *   INPUT: data - pointer to data for write
 */
static inline void writeCommand(uint8 data)
{
    gpio.digitalWrite(((st7735sTypeRaw*)ActiveDisplay)->a0, LOW);
    gpio.SPIDataRW(((st7735sTypeRaw*)ActiveDisplay)->cs, &data, 1);
} /* writeCommand */
/******************** END PRIVATE FUNCTIONS END ********************/

#define DISPCMD_MEMORYWRITE 0x2C /* DO NOT TOUCH! */

/************************ PUBLIC FUNCTIONS ************************/
static st7735sType createDisplay(int SPISpeed, int cs, int a0, int rs)
{
    /* Malloc memory for result structure */
    st7735sTypeRaw *result =
                        (st7735sTypeRaw *) safe_Malloc(sizeof(st7735sTypeRaw));

    /* Last created display is now active */
    setActiveDisplay(result);

    /* Write data to structure */
    result->cs = cs;
    result->a0 = a0;
    result->rs = rs;
    result->width = 128; /* Default */
    result->height = 160; /* Default */

    /* Setup SPI interface */
    if(gpio.SPISetup(result->cs, SPISpeed) == -1)
    {
        /* If error, close program. */
        fprintf(stderr, "Failed to setup Hardware SPI!\n");
        exit(EXIT_FAILURE);
    }

    /* Reset pin as OUTPUT; disable -> 'HIGH' */
    if(result->rs != -1)
    { /* If reset pin is connected */
        gpio.pinMode(result->rs, OUTPUT);
        gpio.digitalWrite(result->rs, HIGH);
        /* Hardware restart before initialize display */
        HWReset(result);
    }

    /* Data/Command pin as OUTPUT; logic level is not important NOW */
    gpio.pinMode(result->a0, OUTPUT);
    gpio.digitalWrite(result->a0, HIGH);

    /* Initialzie display */
    writeCommand(0x01); /* Software Reset */
    gpio.delay(150); /* 150ms (min: 120ms) */
    writeCommand(0x11); /* Sleep Out */
    gpio.delay(150); /* 150ms (min: 120ms) */
    setGamma(2); /* Optional: Set Builtin Gamma: [1/2/4/8]; 2 = the darkest */
    setOrientation(0); /* [Default:0] Must be refreshed! */
    /* Because the software reset does not refresh it.
     * See the 'Reset Table' in the documentation for this. */
    #if CHOOSE_THE_PIXEL_FORMAT == st7735s_PixelReduced /* DO NOT TOUCH THIS LINE! */
        writeCommand(0x3A); /* Choose the pixel format; Send 12-bits per Pixel */
        writeData(0x03);
    #elif CHOOSE_THE_PIXEL_FORMAT == st7735s_PixelFull /* DO NOT TOUCH THIS LINE! */
        writeCommand(0x3A); /* Choose the pixel format; Send 18-bits per Pixel */
        writeData(0x06); /* Default */
    #endif
    writeCommand(0x29); /* Display On */
    gpio.delay(100); /* 100ms before start */

    return (st7735sType) result;
} /* createDisplay */

static void destroyDisplay(st7735sType display)
{
    st7735sTypeRaw *disp = display;

    if(disp == NULL) return;
    HWReset(disp); /* Optional Hardware Restart */
    free(disp); /* Free display struct */
} /* destroyDisplay */

static void destroyActiveDisplay(void)
{
    destroyDisplay(ActiveDisplay); /* Destroy current active display */
    ActiveDisplay = NULL; /* Clear variable */
} /* destroyActiveDisplay */

static void setActiveDisplay(st7735sType display)
{ ActiveDisplay = display; } /* setActiveDisplay */

static st7735sType getActiveDisplay(void)
{ return ActiveDisplay; } /* getActiveDisplay */

static void HWReset(st7735sType display)
{
    st7735sTypeRaw *disp = display;

    /* If reset pin is not connected: exit function */
    if(disp->rs == -1) return;

    gpio.digitalWrite(disp->rs, HIGH); /* Disable reset */
    gpio.digitalWrite(disp->rs, LOW); /* Active reset */
    gpio.delay(10); /* Wait 10ms to apply */
    gpio.digitalWrite(disp->rs, HIGH); /* Disable reset */
    gpio.delay(150); /* Wait 150ms to reload configuration */
} /* HWReset */

static void setInversion(uint8 state)
{
    /* Display Inversion On / Off */
    writeCommand(state ? 0x21 : 0x20);
} /* setInversion */

static void setGamma(uint8 state)
{
    switch(state)
    { /* [GS_Pin = 0] Checked on hardware! */
        default: state = 1; break; /* GS_Pin=1: 2.2; GS_Pin=0: 1.0 */
        case 2: break;             /* GS_Pin=1: 1.8; GS_Pin=0: 2.5 */
        case 4: break;             /* GS_Pin=1: 2.5; GS_Pin=0: 2.2 */
        case 8: break;             /* GS_Pin=1: 1.0; GS_Pin=0: 1.8 */
    }
    writeCommand(0x26); /* Builtin Gamma Set */
    writeData(state);
} /* setGamma */

static void setOrientation(uint8 state)
{
    st7735sTypeRaw *disp = ActiveDisplay;

    writeCommand(0x36); /* Memory Data Access Control */
    if(state)
    {
        writeData(0xA0); /* [MY + MV]; Landscape */
        setWindow(0, 0, 159, 127); /* Must be refreshed */
        disp->width = 160;
        disp->height = 128;
    } else {
        writeData(0); /* Default configuration; Portrait */
        setWindow(0, 0, 127, 159); /* Must be refreshed */
        disp->width = 128;
        disp->height = 160;
    }
} /* setOrientation */

static void setWindow(uint8 x1, uint8 y1, uint8 x2, uint8 y2)
{
    writeCommand(0x2A); /* Column Address Set */
    writeData(0); SPIWrite(x1); SPIWrite(0); SPIWrite(x2);
    writeCommand(0x2B); /* Row Address Set */
    writeData(0); SPIWrite(y1); SPIWrite(0); SPIWrite(y2);
} /* setWindow */

static void activeRAMWrite(void)
{
    writeCommand(DISPCMD_MEMORYWRITE);
    gpio.digitalWrite(((st7735sTypeRaw*)ActiveDisplay)->a0, HIGH);
} /* activeRAMWrite */

static uint8 getActiveDisplayWidth(void)
{
    return ((st7735sTypeRaw*)ActiveDisplay)->width;
} /* getActiveDisplayWidth */

static uint8 getActiveDisplayHeight(void)
{
    return ((st7735sTypeRaw*)ActiveDisplay)->height;
} /* getActiveDisplayHeight */

//////////////////////////////////////////////////

#if CHOOSE_THE_PIXEL_FORMAT == st7735s_PixelFull /* DO NOT TOUCH THIS LINE! */

static void pushColour(uint8 red, uint8 green, uint8 blue)
{
    SPIWrite(red); SPIWrite(green); SPIWrite(blue);
} /* pushColour */

static void drawPx(uint8 x, uint8 y, uint8 red, uint8 green, uint8 blue)
{
    /* Check x and y */
    if((x >= ((st7735sTypeRaw*)ActiveDisplay)->width) ||
       (y >= ((st7735sTypeRaw*)ActiveDisplay)->height)) return;

    /* Setup window, active ram writing and send colour */
    setWindow(x, y, x, y);
    activeRAMWrite();
    SPIWrite(red); SPIWrite(green); SPIWrite(blue);
} /* drawPx */

static void drawFastHLine(uint8 x, uint8 y, uint8 width, uint8 red, uint8 green,
                          uint8 blue)
{
    /* Check x and y */
    if((x >= ((st7735sTypeRaw*)ActiveDisplay)->width) ||
       (y >= ((st7735sTypeRaw*)ActiveDisplay)->height)) return;

    /* Check width */
    if((x+width-1) >= ((st7735sTypeRaw*)ActiveDisplay)->width)
    {
        width = ((st7735sTypeRaw*)ActiveDisplay)->width - x;
    }

    setWindow(x, y, x+width-1, y);
    activeRAMWrite();
    while(width--)
    {
        SPIWrite(red); SPIWrite(green); SPIWrite(blue);
    }
} /* drawFastHLine */

static void drawFastVLine(uint8 x, uint8 y, uint8 height, uint8 red,
                          uint8 green, uint8 blue)
{
    /* Check x and y */
    if((x >= ((st7735sTypeRaw*)ActiveDisplay)->width) ||
       (y >= ((st7735sTypeRaw*)ActiveDisplay)->height)) return;

    /* Check height */
    if((y+height-1) >= ((st7735sTypeRaw*)ActiveDisplay)->height)
    {
        height = ((st7735sTypeRaw*)ActiveDisplay)->height - y;
    }

    setWindow(x, y, x, y+height-1);
    activeRAMWrite();
    while(height--)
    {
        SPIWrite(red); SPIWrite(green); SPIWrite(blue);
    }
} /* drawFastVLine */

static void drawRect(uint8 x, uint8 y, uint8 width, uint8 height, uint8 red,
                     uint8 green, uint8 blue)
{
        drawFastHLine(x, y, width, red, green, blue);
        drawFastHLine(x, y+height-1, width, red, green, blue);
        drawFastVLine(x, y, height, red, green, blue);
        drawFastVLine(x+width-1, y, height, red, green, blue);
} /* drawRect */

static void fillRect(uint8 x, uint8 y, uint8 width, uint8 height, uint8 red,
                     uint8 green, uint8 blue)
{
    /* Check x and y */
    if((x >= ((st7735sTypeRaw*)ActiveDisplay)->width) ||
       (y >= ((st7735sTypeRaw*)ActiveDisplay)->height)) return;

    /* Check height */
    if((y+height-1) >= ((st7735sTypeRaw*)ActiveDisplay)->height)
    {
        height = ((st7735sTypeRaw*)ActiveDisplay)->height - y;
    }
    /* Check width */
    if((x+width-1) >= ((st7735sTypeRaw*)ActiveDisplay)->width)
    {
        width = ((st7735sTypeRaw*)ActiveDisplay)->width - x;
    }

    setWindow(x, y, x+width-1, y+height-1);
    activeRAMWrite();
    while(height--) while(width--)
    {
        SPIWrite(red); SPIWrite(green); SPIWrite(blue);
    }
} /* drawFillRect */

static void fillScreen(uint8 red, uint8 green, uint8 blue)
{
    fillRect(0, 0, ((st7735sTypeRaw*)ActiveDisplay)->width,
             ((st7735sTypeRaw*)ActiveDisplay)->height, red, green, blue);
} /* fillScreen */

#elif CHOOSE_THE_PIXEL_FORMAT == st7735s_PixelReduced /* DO NOT TOUCH THIS LINE! */

static void pushColour(st7735s_Colour12b stPx, st7735s_Colour12b_2 ndPx)
{
    SPIWrite(stPx.stPx[0]); SPIWrite(stPx.stPx[1]); SPIWrite(ndPx.ndPx);
} /* pushColour */

static void drawPx(uint8 x, uint8 y, st7735s_Colour12b stPx)
{
    /* Check x and y */
    if((x >= ((st7735sTypeRaw*)ActiveDisplay)->width) ||
       (y >= ((st7735sTypeRaw*)ActiveDisplay)->height)) return;

    /* Setup window, active ram writing and send colour */
    setWindow(x, y, x, y);
    activeRAMWrite();
    SPIWrite(stPx.stPx[0]); SPIWrite(stPx.stPx[1]);
} /* drawPx */

static void drawFastHLine(uint8 x, uint8 y, uint8 width, st7735s_Colour12b stPx)
{
    st7735s_Colour12b_2 ndPx;

    /* Check x and y */
    if((x >= ((st7735sTypeRaw*)ActiveDisplay)->width) ||
       (y >= ((st7735sTypeRaw*)ActiveDisplay)->height)) return;

    /* Check width */
    if((x+width-1) >= ((st7735sTypeRaw*)ActiveDisplay)->width)
    {
        width = ((st7735sTypeRaw*)ActiveDisplay)->width - x;
    }

    /* Complement the pixels */
    stPx.rgbr2.r2 = stPx.rgbr2.r;
    ndPx.g2b2.g2 =  stPx.rgbr2.g;
    ndPx.g2b2.b2 =  stPx.rgbr2.b;

    setWindow(x, y, x+width-1, y);
    activeRAMWrite();
    while(width--)
    {
        SPIWrite(stPx.stPx[0]); SPIWrite(stPx.stPx[1]); SPIWrite(ndPx.ndPx);
    }
} /* drawFastHLine */

static void drawFastVLine(uint8 x, uint8 y, uint8 height, st7735s_Colour12b stPx)
{
    st7735s_Colour12b_2 ndPx;

    /* Check x and y */
    if((x >= ((st7735sTypeRaw*)ActiveDisplay)->width) ||
       (y >= ((st7735sTypeRaw*)ActiveDisplay)->height)) return;

    /* Check height */
    if((y+height-1) >= ((st7735sTypeRaw*)ActiveDisplay)->height)
    {
        height = ((st7735sTypeRaw*)ActiveDisplay)->height - y;
    }

    /* Complement the pixels */
    stPx.rgbr2.r2 = stPx.rgbr2.r;
    ndPx.g2b2.g2 =  stPx.rgbr2.g;
    ndPx.g2b2.b2 =  stPx.rgbr2.b;

    setWindow(x, y, x, y+height-1);
    activeRAMWrite();
    while(height--)
    {
        SPIWrite(stPx.stPx[0]); SPIWrite(stPx.stPx[1]); SPIWrite(ndPx.ndPx);
    }
} /* drawFastVLine */

static void drawRect(uint8 x, uint8 y, uint8 width, uint8 height, st7735s_Colour12b stPx)
{
        drawFastHLine(x, y, width, stPx);
        drawFastHLine(x, y+height-1, width, stPx);
        drawFastVLine(x, y, height, stPx);
        drawFastVLine(x+width-1, y, height, stPx);
} /* drawRect */

static void fillRect(uint8 x, uint8 y, uint8 width, uint8 height, st7735s_Colour12b stPx)
{
    st7735s_Colour12b_2 ndPx;

    /* Check x and y */
    if((x >= ((st7735sTypeRaw*)ActiveDisplay)->width) ||
       (y >= ((st7735sTypeRaw*)ActiveDisplay)->height)) return;

    /* Check height */
    if((y+height-1) >= ((st7735sTypeRaw*)ActiveDisplay)->height)
    {
        height = ((st7735sTypeRaw*)ActiveDisplay)->height - y;
    }
    /* Check width */
    if((x+width-1) >= ((st7735sTypeRaw*)ActiveDisplay)->width)
    {
        width = ((st7735sTypeRaw*)ActiveDisplay)->width - x;
    }

    /* Complement the pixels */
    stPx.rgbr2.r2 = stPx.rgbr2.r;
    ndPx.g2b2.g2 =  stPx.rgbr2.g;
    ndPx.g2b2.b2 =  stPx.rgbr2.b;

    setWindow(x, y, x+width-1, y+height-1);
    activeRAMWrite();
    while(height--) while(width--)
    {
        SPIWrite(stPx.stPx[0]); SPIWrite(stPx.stPx[1]); SPIWrite(ndPx.ndPx);
    }
} /* drawFillRect */

static void fillScreen(st7735s_Colour12b stPx)
{
    fillRect(0, 0, ((st7735sTypeRaw*)ActiveDisplay)->width,
             ((st7735sTypeRaw*)ActiveDisplay)->height, stPx);
} /* fillScreen */

#endif
/******************** END PUBLIC FUNCTIONS END ********************/
