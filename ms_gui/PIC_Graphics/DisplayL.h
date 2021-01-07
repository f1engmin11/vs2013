#ifndef DISPLAYL_H_
#define DISPLAYL_H_

//#include "../xinit.h"
//#include "../types.h"

#include "GenericTypeDefs.h"  // for BYTE, SHORT, WORD,... types
#include "GraphicsConfig.h"   // for USE_BITMAP_FLASH
#include "Primitive.h"		  // for FLASH_BYTE

/*********************************************************************
* Overview: Additional hardware-accelerated functions can be implemented
*           in the driver. These definitions exclude the PutPixel()-based
*           functions in the primitives layer (Primitive.c file) from compilation.
*********************************************************************/

// Define this to implement Font related functions in the driver.
//#define USE_DRV_FONT

// Define this to implement Line function in the driver.
//#define USE_DRV_LINE

// Define this to implement Circle function in the driver.
//#define USE_DRV_CIRCLE

// Define this to implement FillCircle function in the driver.
//#define USE_DRV_FILLCIRCLE

// Define this to implement Bar function in the driver.
//#define USE_DRV_BAR

// Define this to implement ClearDevice function in the driver.
//#define USE_DRV_CLEARDEVICE

// Define this to implement PutImage function in the driver.
//#define USE_DRV_PUTIMAGE

/*********************************************************************
* Overview: Horizontal and vertical screen size.
*********************************************************************/

// Defines the horizontal screen size. Dependent on the display glass used.
#define SCREEN_HOR_SIZE    320
// Defines the vertical screen size. Dependent on the display glass used.
#define SCREEN_VER_SIZE    240

// Memory pitch for line
#define LINE_MEM_PITCH     320
// Video buffer offset
#define BUF_MEM_OFFSET    //(DWORD)0x18000 

//// Definitions for reset pin
//#define RST_TRIS_BIT       TRISCbits.TRISC1
//#define RST_LAT_BIT        LATCbits.LATC1
//
//// Definitions for CS pin
//#define CS_TRIS_BIT        TRISDbits.TRISD8
//#define CS_LAT_BIT         LATDbits.LATD8
//
//// Definitions for wait pin
//#define WAIT_TRIS_BIT      TRISCbits.TRISC2
//#define WAIT_PORT_BIT      PORTCbits.RC2
//
//// Definitions for FLASH CS pin 
//#define CS_FLASH_LAT_BIT   LATDbits.LATD9
//#define CS_FLASH_TRIS_BIT  TRISDbits.TRISD9
//
//// Clipping region control codes
//#define CLIP_DISABLE       0 
//#define CLIP_ENABLE        1
//
//// HIT1270 commands
//#define SET_DATA_POINTER   0x50
//#define SRAM_WRITE         0x51
//#define STAMP_ICON         0x52
//#define FILL_BLOCK         0x53
//
//#define FLASH_WRITE        0x60
//#define FLASH_READ         0x61


/*********************************************************************
* Overview: Clipping region control codes to be used with SetClip(...)
*           function. 
*********************************************************************/
#define CLIP_DISABLE       0 	// Disables clipping.
#define CLIP_ENABLE        1	// Enables clipping.

/*********************************************************************
* Overview: Some basic colors definitions.
*********************************************************************/

//#define RGBA16(a,r,g,b)    (WORD)( ( ((a)&0xF) << 12 ) | ( ((r)&0xF) << 8 ) | ( ((g)&0xF) << 4  ) | ( ((b)&0xF) << 0 ) )
//#define RGBA16(a,r,g,b)    (WORD)( ( ((a)&0x1) << 15 ) | ( ((r)&0x1F) << 10 ) | ( ((g)&0x1F) << 5  ) | ( ((b)&0x1F) << 0 ) )
#define RGBA16(a,r,g,b)		(WORD)( ((((r)&0xFF)>>3)<<11) | ((((g)&0xFF)>>2)<<5) | ((((b)&0xFF)>>3)<<0) )
#define RGB16(r,g,b)       	RGBA16(0,r,g,b)

#define RGBA32(a,r,g,b)    	(DWORD)( ( ((a)&0xFF) << 24 ) | ( ((r)&0xFF) << 16 ) | ( ((g)&0xFF) << 8  ) | ( ((b)&0xFF) << 0 ) )
#define RGBA16TO32(a,c)		RGBA32(a, ((((c)>>11)&0x1F)<<3)+7, ((((c)>>5)&0x3F)<<2)+3, ((((c)>>0)&0x3F)<<3)+7 )
#define RGB16TO32(c)		RGBA16TO32(0,c)

#define RGB32(r,g,b)	   	RGBA32(0,r,g,b)      

#define RGBA			   	RGBA16

#define RED                	RGBA(0,255,0,0)
#define GREEN              	RGBA(0,0,255,0)
#define BLUE               	RGBA(0,0,0,255)

#define BLACK              	RGBA(0,0,0,0)
#define WHITE              	RGBA(0,255,255,255)

#define YELLOW             	RGBA(0,255,255,255)
#define CYAN               	(WORD)0b0000010000010000
#define MAGENTA            	(WORD)0b1000000000010000
#define BROWN              	(WORD)0b1111110000000000

#define BRIGHTBLUE         (WORD)0x001F
#define BRIGHTGREEN        (WORD)0x07E0 //0b0000 0111 1110 0000
#define BRIGHTCYAN         (WORD)0b0000011111111111
#define BRIGHTRED          0xF800 //(WORD)0b1111100000000000
#define BRIGHTMAGENTA      (WORD)0b1111100000011111
#define BRIGHTYELLOW       0xFFE0 //(WORD)0b1111111111100000

#define LIGHTBLUE          0x821F//(WORD)0b1000010000011111
#define LIGHTGREEN         (WORD)0b1000011111110000
#define LIGHTCYAN          (WORD)0b1000011111111111
#define LIGHTRED           (WORD)0b1111110000010000
#define LIGHTMAGENTA       (WORD)0b1111110000011111
#define LIGHTGRAY          0x8410//(WORD)0b1000010000010000

#define DARKGRAY           0x4208//(WORD)0b0100001000001000

#define GRAY0       	   RGBA(0,256-(1*(256/8)),256-(1*(256/8)), 256-(1*(256/8)))
#define GRAY1         	   RGBA(0,256-(2*(256/8)),256-(2*(256/8)), 256-(2*(256/8)))
#define GRAY2              RGBA(0,256-(3*(256/8)),256-(3*(256/8)), 256-(3*(256/8)))
#define GRAY3              RGBA(0,256-(4*(256/8)),256-(4*(256/8)), 256-(4*(256/8)))
#define GRAY4              RGBA(0,256-(5*(256/8)),256-(5*(256/8)), 256-(5*(256/8)))
#define GRAY5              RGBA(0,256-(6*(256/8)),256-(6*(256/8)), 256-(6*(256/8)))
#define GRAY6	           RGBA(0,256-(7*(256/8)),256-(7*(256/8)), 256-(7*(256/8)))
#define GRAY7	           RGBA(0,256-(8*(256/8))+1,256-(8*(256/8))+1, 256-(8*(256/8))+1)

// Color
extern WORD_VAL _color;

/*********************************************************************
* Overview: Clipping region control an//(WORD)0b1000010000010000d border settings.
*
*********************************************************************/
// Clipping region enable control
extern SHORT _clipRgn;
// Left clipping region border
extern SHORT _clipLeft;
// Top clipping region border
extern SHORT _clipTop;
// Right clipping region border
extern SHORT _clipRight;
// Bottom clipping region border
extern SHORT _clipBottom;

/*********************************************************************
* Function:  void ResetDevice()
*
* Overview: Initializes LCD module.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void initDevice( void );
void ResetDevice( void );

/*********************************************************************
* Macros:  GetMaxX()
*
* Overview: Returns maximum horizontal coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: Maximum horizontal coordinate.
*
* Side Effects: none
*
********************************************************************/
#define GetMaxX() (SCREEN_HOR_SIZE-1)

/*********************************************************************
* Macros:  GetMaxY()
*
* Overview: Returns maximum vertical coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: Maximum vertical coordinate.
*
* Side Effects: none
*
********************************************************************/
#define GetMaxY() (SCREEN_VER_SIZE-1)

/*********************************************************************
* Macros:  SetColor(color)
*
* Overview: Sets current drawing color.
*
* PreCondition: none
*
* Input: color - Color coded in 5:6:5 RGB format.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetColor(color) _color.Val = color;

/*********************************************************************
* Macros:  GetColor()
*
* Overview: Returns current drawing color.
*
* PreCondition: none
*
* Input: none
*
* Output: Color coded in 5:6:5 RGB format.
*
* Side Effects: none
*
********************************************************************/
#define GetColor() _color.Val

/*********************************************************************
* Macros:  SetActivePage(page)
*
* Overview: Sets active graphic page.
*
* PreCondition: none
*
* Input: page - Graphic page number.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetActivePage(page)

/*********************************************************************
* Macros: SetVisualPage(page)
*
* Overview: Sets graphic page to display.
*
* PreCondition: none
*
* Input: page - Graphic page number
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetVisualPage(page)

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* Overview: Puts pixel with the given x,y coordinate position.
*
* PreCondition: none
*
* Input: x - x position of the pixel.
*		 y - y position of the pixel.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y);

/*********************************************************************
* Function: WORD GetPixel(SHORT x, SHORT y)
*
* Overview: Returns pixel color at the given x,y coordinate position.
*
* PreCondition: none
*
* Input: x - x position of the pixel.
*		 y - y position of the pixel.
*
* Output: pixel color
*
* Side Effects: none
*
********************************************************************/
WORD GetPixel(SHORT x, SHORT y);

/*********************************************************************
* Macros: SetClipRgn(left, top, right, bottom)
*
* Overview: Sets clipping region.
*
* PreCondition: none
*
* Input: left - Defines the left clipping region border.
*		 top - Defines the top clipping region border.
*		 right - Defines the right clipping region border.
*	     bottom - Defines the bottom clipping region border.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetClipRgn(left,top,right,bottom) _clipLeft=left; _clipTop=top; _clipRight=right; _clipBottom=bottom;

/*********************************************************************
* Macros: GetClipLeft()
*
* Overview: Returns left clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Left clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipLeft() _clipLeft

/*********************************************************************
* Macros: GetClipRight()
*
* Overview: Returns right clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Right clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipRight() _clipRight

/*********************************************************************
* Macros: GetClipTop()
*
* Overview: Returns top clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Top clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipTop() _clipTop

/*********************************************************************
* Macros: GetClipBottom()
*
* Overview: Returns bottom clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Bottom clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipBottom() _clipBottom

/*********************************************************************
* Macros: SetClip(control)
*
* Overview: Enables/disables clipping.
*
* PreCondition: none
*
* Input: control - Enables or disables the clipping.
*			- 0: Disable clipping
*			- 1: Enable clipping
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetClip(control) _clipRgn=control;

/*********************************************************************
* Macros: IsDeviceBusy()
*
* Overview: Returns non-zero if LCD controller is busy 
*           (previous drawing operation is not completed).
*
* PreCondition: none
*
* Input: none
*
* Output: Busy status.
*
* Side Effects: none
*
********************************************************************/
#define IsDeviceBusy()  0

/*********************************************************************
* Macros: SetPalette(colorNum, color)
*
* Overview:  Sets palette register.
*
* PreCondition: none
*
* Input: colorNum - Register number.
*        color - Color.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetPalette(colorNum, color)

#endif /*DISPLAYL_H_*/
