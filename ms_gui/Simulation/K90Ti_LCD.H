
#define DCU_DATA_PORT          (*((volatile unsigned char *) 0x82010030))
#define DCU_CMD_PORT           (*((volatile unsigned char *) 0x82010032))

#define DCU_CMD_Set_Addr_L  0x00
#define DCU_CMD_Set_Addr_M  0x40
#define DCU_CMD_Set_Addr_H  0x80
#define DCU_CMD_Set_RGB_1   0xC0
#define DCU_CMD_Set_RGB_0   0xC8
#define DCU_CMD_Set_State_A 0xE0
#define DCU_CMD_Set_Light   0xE8
#define DCU_CMD_Scan_Key_n  0xF0
#define DCU_CMD_Set_State_B 0xF8

#define X9313_nCS_Bit 0x04
#define X9313_UnD_Bit 0x02
#define X9313_INC_Bit 0x01

#define LCD_Color_INV_Bit 0x04
#define LCD_VEE_ON_Bit   0x02
#define LCD_DISP_ON_Bit  0x01


#define LCD_XSIZE 640
//LCD是横向8bit为一个字节
#define LCD_X_BYTE_SIZE (LCD_XSIZE)

#define LCD_YSIZE 480

#define GUI_COLOR  unsigned char
//-------------------------------
#define GUI_BLACK         0x000000
#define GUI_BLUE          0xFF0000
#define GUI_GREEN         0x00ff00
#define GUI_CYAN          0xffff00
#define GUI_RED           0x0000FF
#define GUI_MAGENTA       0x8b008b
#define GUI_BROWN         0x2a2aa5
#define GUI_DARKGRAY      0x404040
#define GUI_GRAY          0x808080
#define GUI_LIGHTGRAY     0xd3d3d3
#define GUI_LIGHTBLUE     0xff8080
#define GUI_LIGHTGREEN    0x80ff80
#define GUI_LIGHTCYAN     0x80ffff
#define GUI_LIGHTRED      0x8080ff
#define GUI_LIGHTMAGENTA  0xff80ff
#define GUI_YELLOW        0x00ffff
#define GUI_WHITE         0xffffff
#define GUI_INVALID_COLOR 0xfffffff      /* Invalid color - more than 24 bits */

//-------------------------------------------------------------------------------------
//显示模式
#define GUI_TEXTMODE_NORMAL      (0)
#define GUI_TEXTMODE_REVERSE     (1<<0)
#define GUI_TEXTMODE_UNDERLINE   (1<<1)
#define GUI_TEXTMODE_XOR         (1<<2)

#define GUI_TEXTMODE_ZOOMx1      (0)
#define GUI_TEXTMODE_ZOOMx2      (1<<0)

//对齐方式
#define GUI_TA_LEFT              (0<<4)
#define GUI_TA_HCENTER           (1<<4)
#define GUI_TA_RIGHT             (2<<4)
#define GUI_JUST_TEXT            (1<<7)

//英文的两种字体
#define En_8x16      (0)
#define En_5x8       (1)

//字体宽度定义
#define En_8x16_XSIZE  8
#define En_8x16_YSIZE  16
#define En_5x8_XSIZE   8
#define En_5x8_YSIZE   8
//英文字体宽度定义
extern INT8U Char_XSIZE;
extern INT8U Char_YSIZE;

extern INT8U Chinese_XSIZE;
extern INT8U Chinese_YSIZE;

//中文字体宽度定义
#define _Chinese_XSIZE_  16
#define _Chinese_YSIZE_  16
//-----------------------------------------------------------------------------------



void Display_Locate(INT8U DisplayData, INT16U x0, INT16U y0);
void GUI_Init(void);
void GUI_DispCharAt(INT8U AscciiChar, INT16U x, INT16U y);
void GUI_DispStringAt(INT8U const *Str,INT16U x0,INT16U y0);
void GUI_SetFontZoom(INT8U ZoomMode);
void GUI_GotoXY(INT16U x,INT16U y);
INT8U GUI_SetTextMode(INT8U TextMode);
void GUI_SetEnFont(INT8U EnFont);
void GUI_DispHexAt(INT32U v, INT16U x, INT16U y, INT8U Len);
void LCD_DelayMs(INT16U Ms);
//void SomeNop(void);

//void LCD_SetBkLight(BOOL Up_nDown);
void VBar(INT16U x, INT16U y0, INT16U y1,INT8U percent);
void GUI_DispDecSAt(INT16S v, INT16U x, INT16U y, INT8U Len);
void GUI_DispDecAt(INT16U v, INT16U x, INT16U y, INT8U Len);
void GUI_Clear(void);
void GUI_DispBitAt(INT8U v, INT16U x, INT16U y);

