/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCDInfo1.C
Purpose     : Routines returning info at runtime
---------------------------END-OF-HEADER------------------------------
*/

//#include <stddef.h>           /* needed for definition of NULL */
//#include "LCD.h"		          /* interface definitions */
//#include "LCD_Private.h"      /* private modul definitions & config */
#include "COMMON.H"

int LCD_GetXSize(void)        { return LCD_XSIZE; }
int LCD_GetYSize(void)        { return LCD_YSIZE; }
int LCD_GetVXSize(void)       { return LCD_VXSIZE; }
int LCD_GetVYSize(void)       { return LCD_VYSIZE; }
int LCD_GetBitsPerPixel(void) { return LCD_BITSPERPIXEL; }
U32 LCD_GetNumColors(void)    { return LCD_NUM_COLORS; }
int LCD_GetYMag(void)         { return LCD_YMAG; }
int LCD_GetXMag(void)         { return LCD_XMAG; }
int LCD_GetFixedPalette(void) {
  #if LCD_SWAP_RB == 0
    return LCD_FIXEDPALETTE;
  #else
    return (-1)*LCD_FIXEDPALETTE;
  #endif
}

I32  LCD_GetDevCap(int Index) {
    switch (Index) {
    case LCD_DEVCAP_NUMCOLORS:
        return LCD_NUM_COLORS;
    case LCD_DEVCAP_XSIZE:
        return LCD_GetXSize();
    case LCD_DEVCAP_YSIZE:
        return LCD_GetYSize();
    case LCD_DEVCAP_VXSIZE:
        return LCD_VXSIZE;
    case LCD_DEVCAP_VYSIZE:
        return LCD_VYSIZE;
//    case LCD_DEVCAP_CONTROLLER:
//        return LCD_CONTROLLER;
    case LCD_DEVCAP_BITSPERPIXEL:
        return LCD_BITSPERPIXEL;
    }
    if ((Index >= LCD_DEVCAP_COLOR) && (Index < LCD_DEVCAP_COLOR+LCD_NUM_COLORS)) {
//        return LCD_Index2Color(Index-LCD_DEVCAP_COLOR);
    }
    return 0;
}

int LCD_GetNumDisplays(void)        { return LCD_NUM_DISPLAYS; }