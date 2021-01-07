#ifndef __COMMON_H__

/************************* ***********************************
  REVISION LOG ENTRY
  Revision By: houhh
  Revised on 2005-5-28 17:36:41
  Comments: 通常调用的头文件,用于包含UCGUI中的各种函数调用的头文件...
  *************************************************************/

#include "Windows.h"

//#include "GUI.h"
/************************************************************
*
*                    GUI_KEY_...
*
*************************************************************
These ID values are basically meant to be used with widgets

*/

#define GUI_KEY_BACKSPACE         8         /* ASCII: BACKSPACE Crtl-H */
#define GUI_KEY_TAB               9         /* ASCII: TAB       Crtl-I */
#define GUI_KEY_ENTER             13        /* ASCII: ENTER     Crtl-M */
#define GUI_KEY_LEFT              16
#define GUI_KEY_UP                17
#define GUI_KEY_RIGHT             18
#define GUI_KEY_DOWN              19
#define GUI_KEY_HOME              23
#define GUI_KEY_END               24
#define GUI_KEY_SHIFT             25
#define GUI_KEY_CONTROL           26
#define GUI_KEY_ESCAPE            27        /* ASCII: ESCAPE    0x1b   */
#define GUI_KEY_INSERT            29
#define GUI_KEY_DELETE            30



/*********************************************************************
*
*                   General configuration of LCD
*
**********************************************************************
*/

#define LCD_XSIZE      (320)   /* X-resolution of LCD, Logical coor. */
#define LCD_YSIZE      (240)   /* Y-resolution of LCD, Logical coor. */

#define LCD_BITSPERPIXEL (16)

//--------------------------------------------
#define LCD_XMAG 1
#define LCD_YMAG 1

//#ifndef LCD_VXSIZE
#define LCD_VXSIZE      (LCD_XSIZE*LCD_XMAG) /* virtual x-size */
//#endif
//#ifndef LCD_VYSIZE
#define LCD_VYSIZE      (LCD_YSIZE*LCD_YMAG) /* virtual y-size */
//#endif
//#ifndef LCD_NUM_COLORS
#define LCD_NUM_COLORS (1L<<LCD_BITSPERPIXEL)
//#endif
//#ifndef LCD_SWAP_RB
#define LCD_SWAP_RB 0
//#endif
  #define LCD_NUM_DISPLAYS 1

#if LCD_BITSPERPIXEL == 8
    #define LCD_FIXEDPALETTE 8666
#elif LCD_BITSPERPIXEL == 12
    #define LCD_FIXEDPALETTE 444
#elif LCD_BITSPERPIXEL == 15
    #define LCD_FIXEDPALETTE 555
#elif LCD_BITSPERPIXEL == 16
    #define LCD_FIXEDPALETTE 565
#else
    #define LCD_FIXEDPALETTE 0
#endif




#include "LCD.h"
//#include "GUI_X_SIM.h"

#include "SIM_X.h"
#include "SIM.h"
#include "LCDSIM.h"
#include "Branding.h"
#include "LCD_SIM.h"
//#include "HZ.h"


#define __COMMON_H__

#endif //__COMMON_H__