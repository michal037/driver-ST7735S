/*
 * MIT License
 * Copyright (c) from 2017, Michal Kozakiewicz, github.com/michal037
 */

#include <stdlib.h>
#include <stdio.h>
#include "st7735s.h"

/************************ FOR EASIER PORT ************************/
#include <wiringPi.h> /* Can be changed */
#include <wiringPiSPI.h> /* Can be changed */
struct /* Function pointers structure */
{ /* DO NOT TOUCH! */
    void (* const pinMode)(int pin, int mode);
    void (* const digitalWrite)(int pin, int value);
    void (* const delay)(unsigned int howLong);
    void (* const delayMicroseconds)(unsigned int howLong);
    int (* const SPISetup)(int channel, int speed);
    int (* const SPIDataRW)(int channel, unsigned char *data, int len);
} static const gpio = /* DEFINITION */
{ /* Function pointers can be changed */
    pinMode,
    digitalWrite,
    delay,
    delayMicroseconds,
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
} st7735sTypeRaw;

static st7735sType ActiveDisplay; /* FILE GLOBAL: Current active display */

/* Public functions declarations */
static st7735sType createDisplay(int SPISpeed, int cs, int a0, int rs);
static void destroyDisplay(st7735sType display);
static void destroyActiveDisplay(void);
static void setActiveDisplay(st7735sType display);
static st7735sType getActiveDisplay(void);
static void HWReset(st7735sType display);
static void fillColour(int colour);
static void drawPx(int x, int y, int colour);

/* Public st7735s functions */
const struct st7735s st7735s = /* DEFINITION */
{
    { /* Configuration Functions */
        createDisplay,
        destroyDisplay,
        destroyActiveDisplay,
        setActiveDisplay,
        getActiveDisplay,
        HWReset
    },
    { /* Drawing Functions */
        fillColour,
        drawPx
    }
};


/************************ PRIVATE FUNCTIONS ************************/
/*
 * Safe Free
 * Use with macro:
 *   #define safeFree(p) __safeFree((void**)&(p))
 */
#define safeFree(p) __safeFree((void**)&(p))
static inline void __safeFree(void **pp)
{
    if(pp != NULL && *pp != NULL)
    {
        free(*pp);
        *pp = NULL;
    }
} /* __safeFree */

/*
 * Safe Malloc()
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
 *   INPUT: len - lenght of data for write
 */
static inline void SPIWrite(unsigned char *data, int len)
{
    gpio.SPIDataRW(((st7735sTypeRaw*)ActiveDisplay)->cs, data, len);
} /* SPIWrite */

/*
 * writeData()
 *
 * Parameters:
 *   INPUT: data - pointer to data for write
 *   INPUT: len - lenght of data for write
 */
static inline void writeData(unsigned char *data, int len)
{
    gpio.digitalWrite(((st7735sTypeRaw*)ActiveDisplay)->a0, HIGH);
    gpio.SPIDataRW(((st7735sTypeRaw*)ActiveDisplay)->cs, data, len);
} /* writeData */

/*
 * writeCommand()
 *
 * Parameters:
 *   INPUT: data - pointer to data for write
 *   INPUT: len - lenght of data for write
 */
static inline void writeCommand(unsigned char *data, int len)
{
    gpio.digitalWrite(((st7735sTypeRaw*)ActiveDisplay)->a0, LOW);
    gpio.SPIDataRW(((st7735sTypeRaw*)ActiveDisplay)->cs, data, len);
} /* writeCommand */
/******************** END PRIVATE FUNCTIONS END ********************/


/************************ PUBLIC FUNCTIONS ************************/
static st7735sType createDisplay(int SPISpeed, int cs, int a0, int rs)
{
    /* Malloc memory for result structure */
    st7735sTypeRaw *result = (st7735sTypeRaw *) safe_Malloc(sizeof(st7735sTypeRaw));

    /* Last created display is now active */
    setActiveDisplay(result);

    /* Write data to structure */
    result->cs = cs;
    result->a0 = a0;
    result->rs = rs;

    /* Setup SPI interface */
    if(gpio.SPISetup(result->cs, SPISpeed) == -1)
    {
        fprintf(stderr, "Failed to setup Hardware SPI!\n");
        exit(EXIT_FAILURE);
    }

    /* Reset pin as OUTPUT; disable = HIGH */
    if(result->rs != -1)
    { /* If reset pin is not connected */
        gpio.pinMode(result->rs, OUTPUT);
        gpio.digitalWrite(result->rs, HIGH);

        /* Hardware restart before initialize display */
        HWReset(result);
    }

    /* Data/Command pin as OUTPUT; logic level is not important NOW */
    gpio.pinMode(result->a0, OUTPUT);
    gpio.digitalWrite(result->a0, HIGH);



    return (st7735sType) result;
} /* createDisplay */

static void destroyDisplay(st7735sType display)
{
    st7735sTypeRaw *disp = display;

    if(disp == NULL) return;

    HWReset(disp); /* Hardware Restart */

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

    /* If reset pin is not connected: return */
    if(disp->rs == -1) return;

    gpio.digitalWrite(disp->cs, LOW); /* Chip select active */

    gpio.digitalWrite(disp->rs, HIGH); /* Disable reset */
    gpio.digitalWrite(disp->rs, LOW); /* Active reset */
    gpio.delay(10); /* wait 10ms */
    gpio.digitalWrite(disp->rs, HIGH); /* Disable reset */
    gpio.delay(150); /* wait 150ms */

    gpio.digitalWrite(disp->cs, HIGH); /* Chip select disable */
} /* HWReset */

//////////////////////////////////////////////////

static void fillColour(int colour)
{

} /* fillColour */

static void drawPx(int x, int y, int colour)
{

} /* drawPx */
/******************** END PUBLIC FUNCTIONS END ********************/
