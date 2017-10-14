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

/* st7735s Display Type; Hidden from the user */
typedef void * st7735sType;

/* st7735s Public Functions */
struct st7735s
{
    /* Configuration Functions */
    struct
    {
        st7735sType (* const createDisplay)(int SPISpeed, int cs, int a0, int rs);
        void (* const destroyDisplay)(st7735sType display);
        void (* const destroyActiveDisplay)(void);
        void (* const setActiveDisplay)(st7735sType display);
        st7735sType (* const getActiveDisplay)(void);
        void (* const HWReset)(st7735sType display);
    } conf;

    /* Drawing Functions */
    struct
    {
        void (* const fillColour)(int colour);
        void (* const drawPx)(int x, int y, int colour);
    } draw;
};
extern const struct st7735s st7735s;

#ifdef __cplusplus
}
#endif

#endif /* LIB_ST7735S_H */
