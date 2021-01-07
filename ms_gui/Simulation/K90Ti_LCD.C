#include "CPU.H"
#include "K90Ti_LCD.H"

#define _CONST_ const

#include "ASCII.c"
//HZDotReader 输出格式中以code 为const
#define code const
#include "Chinese.c"
#undef code
/*
-------------------------------------------------------------------------
GUI相关全局变量
-------------------------------------------------------------------------
*/
INT8U  GUIEnFont = En_8x16;                //英文字体
INT8U  GUITextMode = GUI_TEXTMODE_NORMAL;  //显示模式
INT8U  GUIZoomMode  = GUI_TEXTMODE_ZOOMx1;


INT8U Char_XSIZE;   //英文字体X宽度
INT8U Char_YSIZE;   //英文字体Y宽度
INT8U Chinese_XSIZE;
INT8U Chinese_YSIZE;

GUI_COLOR BkColor = GUI_BLACK;//背景颜色
GUI_COLOR FgColor = GUI_WHITE;//前景颜色



INT16U PosX = 0;     //当前系统X坐标
INT16U PosY = 0;     //当前系统Y坐标

INT8U NoXY = TRUE;//是否使用PosX PosY的标志   1-->不使用   0-->使用




void SomeNop(void)
{
    INT16U j;
    for(j = 0x0F; j > 0; j--);
}
//获得LCD型号
INT8U LCD_GetLCDType(void)
{
    INT8U temp;
    temp = (0x07 & DCU_DATA_PORT);
    return temp;
}
//设置显存地址
void LCD_SetSRAMAddr(INT32U Addr)
{
    INT8U temp;
    Addr = Addr &0x0003FFFF;//18bit Addr
    temp = Addr & 0x3F;
    DCU_CMD_PORT = DCU_CMD_Set_Addr_L | temp;
    SomeNop();
    Addr = Addr >> 6;
    temp = Addr & 0x3F;
    DCU_CMD_PORT = DCU_CMD_Set_Addr_M | temp;
    SomeNop();
    Addr = Addr >> 6;
    temp = Addr & 0x3F;
    DCU_CMD_PORT = DCU_CMD_Set_Addr_H | temp;
    SomeNop();

}
void LCD_SetBkColor(INT8U BkColor)
{
    INT8U temp;
    temp = BkColor & 0x07;
    DCU_CMD_PORT = DCU_CMD_Set_RGB_0 | temp;//设置数据0 对应的颜色
    SomeNop();
}
void LCD_SetColor(INT8U Color)
{
    INT8U temp;
    temp = Color & 0x07;
    DCU_CMD_PORT = DCU_CMD_Set_RGB_1 | temp;//设置数据1 对应的颜色
    SomeNop();
}
void LCD_Delayus(INT16U us)
{
  INT16U i;
  INT16U j;
  for(i = us; i > 0; i--){ /* delay */
    for(j = 0x0D; j > 0; j--); /* delay */
  }
}
void LCD_DelayMs(INT16U Ms)
{
  INT16U i;
  for(i = 0; i < Ms; i++){ /* delay */
    LCD_Delayus(1000);
  }
}
/*
void LCD_Init(void)
{
    DCU_CMD_PORT = DCU_CMD_Set_State_B;
    SomeNop();


    DCU_CMD_PORT = DCU_CMD_Set_State_A |LCD_VEE_ON_Bit;//VEE_ON
    SomeNop();
    DCU_CMD_PORT = DCU_CMD_Set_State_A |LCD_VEE_ON_Bit;//
    SomeNop();
    DCU_CMD_PORT = DCU_CMD_Set_State_A |LCD_VEE_ON_Bit;//
    SomeNop();
    DCU_CMD_PORT = DCU_CMD_Set_State_A |LCD_VEE_ON_Bit;//
    SomeNop();
    DCU_CMD_PORT = DCU_CMD_Set_State_A |LCD_VEE_ON_Bit;//
    SomeNop();

    LCD_DelayMs(25);
    DCU_CMD_PORT = DCU_CMD_Set_State_A |LCD_VEE_ON_Bit|LCD_DISP_ON_Bit;//VEE_ON + Disp_ON
    SomeNop();
    DCU_CMD_PORT = DCU_CMD_Set_State_A |LCD_VEE_ON_Bit|LCD_DISP_ON_Bit;//
    SomeNop();
    DCU_CMD_PORT = DCU_CMD_Set_State_A |LCD_VEE_ON_Bit|LCD_DISP_ON_Bit;//
    SomeNop();
    DCU_CMD_PORT = DCU_CMD_Set_State_A |LCD_VEE_ON_Bit|LCD_DISP_ON_Bit;//
    SomeNop();
    DCU_CMD_PORT = DCU_CMD_Set_State_A |LCD_VEE_ON_Bit|LCD_DISP_ON_Bit;//
    SomeNop();
}

void LCD_SetBkLight(BOOL Up_nDown)
{


    if(Up_nDown){
        DCU_CMD_PORT = DCU_CMD_Set_Light |X9313_nCS_Bit|X9313_UnD_Bit|X9313_INC_Bit;
        LCD_Delayus(2);
        DCU_CMD_PORT = DCU_CMD_Set_Light |              X9313_UnD_Bit|X9313_INC_Bit;
        LCD_Delayus(2);
        DCU_CMD_PORT = DCU_CMD_Set_Light |              X9313_UnD_Bit;
        LCD_Delayus(2);
        DCU_CMD_PORT = DCU_CMD_Set_Light |              X9313_UnD_Bit|X9313_INC_Bit;
        LCD_Delayus(2);
        DCU_CMD_PORT = DCU_CMD_Set_Light |X9313_nCS_Bit|X9313_UnD_Bit|X9313_INC_Bit;

    }else{
        DCU_CMD_PORT = DCU_CMD_Set_Light |X9313_nCS_Bit|X9313_INC_Bit;
        LCD_Delayus(2);
        DCU_CMD_PORT = DCU_CMD_Set_Light |              X9313_INC_Bit;
        LCD_Delayus(2);
        DCU_CMD_PORT = DCU_CMD_Set_Light ;
        LCD_Delayus(2);
        DCU_CMD_PORT = DCU_CMD_Set_Light |              X9313_INC_Bit;
        LCD_Delayus(2);
        DCU_CMD_PORT = DCU_CMD_Set_Light |X9313_nCS_Bit|X9313_INC_Bit;

    }
}

void LCD_Clear(INT8U BkColor)
{
    INT16U LCD_X;
    INT16U LCD_Y;
//    LCD_SetBkColor(BkColor);
//    LCD_SetColor(0xFF);


//    LCD_SetSRAMAddr(0);
    for(LCD_Y = 0; LCD_Y < LCD_YSIZE; LCD_Y++){
        for(LCD_X = 0; LCD_X < LCD_X_BYTE_SIZE; LCD_X++){
//            SomeNop();
//            DCU_DATA_PORT = 0x00;
            GUI_X_Lock();
            LCDSIM_SetPixelIndex(LCD_X,LCD_Y,BkColor);
            GUI_X_Unlock();

        }
    }
}
*/
void GUI_Clear(void)
{
    INT16U LCD_X;
    INT16U LCD_Y;
    for(LCD_Y = 0; LCD_Y < LCD_YSIZE; LCD_Y++){
        for(LCD_X = 0; LCD_X < LCD_X_BYTE_SIZE; LCD_X++){
            Display_Locate(0x00, LCD_X,LCD_Y);
        }
    }
}
void GUI_Init(void)
{

//    INT8U LCDType;
//    LCD_Init();
//    LCDType = LCD_GetLCDType();
//    if(LCDType==0x01){//黑白 LCD
//        LCD_Clear(0x00);
//    }
    BkColor = GUI_BLUE;
    FgColor = GUI_RED;
    GUI_Clear();
    GUI_SetEnFont(En_8x16);
    GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
    GUI_SetFontZoom(GUI_TEXTMODE_ZOOMx1);
    PosX = 0;
    PosY = 0;
    NoXY = TRUE;

}
/*
*****************************************************************************
* Display_Locate - LCD上指定位置 显示指定数据
* DESCRIPTION: -
*
* @Param x0:0--(640/8-1)   横向坐标  LCD 是横向8个点为一个Byte
* @Param y0:0--(480-1)     纵向坐标
* @Param DisplayData:写入的数据
* @Return :
* ----
*****************************************************************************
*/
/*
void Display_Locate(INT8U DisplayData, INT16U x0, INT16U y0)
{
    INT32U Addr;
    if ((x0 <= (LCD_X_BYTE_SIZE-1))&&(y0 <= (LCD_YSIZE-1))){
        Addr = y0 * LCD_X_BYTE_SIZE + x0;
        LCD_SetSRAMAddr(Addr);
        DCU_DATA_PORT = DisplayData;
        SomeNop();
    }
}
*/
//该表格把字体放大
//
INT16U _CONST_ Changedot[] =
{
0x0000, 0x0003, 0x000c, 0x000f, 0x0030, 0x0033, 0x003c, 0x003f, 0x00c0, 0x00c3,
0x00cc, 0x00cf, 0x00f0, 0x00f3, 0x00fc, 0x00ff, 0x0300, 0x0303, 0x030c, 0x030f,
0x0330, 0x0333, 0x033c, 0x033f, 0x03c0, 0x03c3, 0x03cc, 0x03cf, 0x03f0, 0x03f3,
0x03fc, 0x03ff, 0x0c00, 0x0c03, 0x0c0c, 0x0c0f, 0x0c30, 0x0c33, 0x0c3c, 0x0c3f,
0x0cc0, 0x0cc3, 0x0ccc, 0x0ccf, 0x0cf0, 0x0cf3, 0x0cfc, 0x0cff, 0x0f00, 0x0f03,
0x0f0c, 0x0f0f, 0x0f30, 0x0f33, 0x0f3c, 0x0f3f, 0x0fc0, 0x0fc3, 0x0fcc, 0x0fcf,
0x0ff0, 0x0ff3, 0x0ffc, 0x0fff, 0x3000, 0x3003, 0x300c, 0x300f, 0x3030, 0x3033,
0x303c, 0x303f, 0x30c0, 0x30c3, 0x30cc, 0x30cf, 0x30f0, 0x30f3, 0x30fc, 0x30ff,
0x3300, 0x3303, 0x330c, 0x330f, 0x3330, 0x3333, 0x333c, 0x333f, 0x33c0, 0x33c3,
0x33cc, 0x33cf, 0x33f0, 0x33f3, 0x33fc, 0x33ff, 0x3c00, 0x3c03, 0x3c0c, 0x3c0f,
0x3c30, 0x3c33, 0x3c3c, 0x3c3f, 0x3cc0, 0x3cc3, 0x3ccc, 0x3ccf, 0x3cf0, 0x3cf3,
0x3cfc, 0x3cff, 0x3f00, 0x3f03, 0x3f0c, 0x3f0f, 0x3f30, 0x3f33, 0x3f3c, 0x3f3f,
0x3fc0, 0x3fc3, 0x3fcc, 0x3fcf, 0x3ff0, 0x3ff3, 0x3ffc, 0x3fff, 0xc000, 0xc003,
0xc00c, 0xc00f, 0xc030, 0xc033, 0xc03c, 0xc03f, 0xc0c0, 0xc0c3, 0xc0cc, 0xc0cf,
0xc0f0, 0xc0f3, 0xc0fc, 0xc0ff, 0xc300, 0xc303, 0xc30c, 0xc30f, 0xc330, 0xc333,
0xc33c, 0xc33f, 0xc3c0, 0xc3c3, 0xc3cc, 0xc3cf, 0xc3f0, 0xc3f3, 0xc3fc, 0xc3ff,
0xcc00, 0xcc03, 0xcc0c, 0xcc0f, 0xcc30, 0xcc33, 0xcc3c, 0xcc3f, 0xccc0, 0xccc3,
0xcccc, 0xcccf, 0xccf0, 0xccf3, 0xccfc, 0xccff, 0xcf00, 0xcf03, 0xcf0c, 0xcf0f,
0xcf30, 0xcf33, 0xcf3c, 0xcf3f, 0xcfc0, 0xcfc3, 0xcfcc, 0xcfcf, 0xcff0, 0xcff3,
0xcffc, 0xcfff, 0xf000, 0xf003, 0xf00c, 0xf00f, 0xf030, 0xf033, 0xf03c, 0xf03f,
0xf0c0, 0xf0c3, 0xf0cc, 0xf0cf, 0xf0f0, 0xf0f3, 0xf0fc, 0xf0ff, 0xf300, 0xf303,
0xf30c, 0xf30f, 0xf330, 0xf333, 0xf33c, 0xf33f, 0xf3c0, 0xf3c3, 0xf3cc, 0xf3cf,
0xf3f0, 0xf3f3, 0xf3fc, 0xf3ff, 0xfc00, 0xfc03, 0xfc0c, 0xfc0f, 0xfc30, 0xfc33,
0xfc3c, 0xfc3f, 0xfcc0, 0xfcc3, 0xfccc, 0xfccf, 0xfcf0, 0xfcf3, 0xfcfc, 0xfcff,
0xff00, 0xff03, 0xff0c, 0xff0f, 0xff30, 0xff33, 0xff3c, 0xff3f, 0xffc0, 0xffc3,
0xffcc, 0xffcf, 0xfff0, 0xfff3, 0xfffc, 0xffff,
};
//extern void GUI_X_Lock(void);
//extern void GUI_X_Unlock(void);
//extern void  LCDSIM_SetPixelIndex(int x, int y, int Index);

void Display_Locate(INT8U DisplayData, INT16U x0, INT16U y0)
{
//    INT32U Addr;
    INT8U LByte;
    INT8U HByte;
    unsigned char i;
    if ((x0 <= (LCD_X_BYTE_SIZE-1))&&(y0 <= (LCD_YSIZE-1))){
        if (GUIZoomMode == GUI_TEXTMODE_ZOOMx2){
/*
            Addr = y0  * LCD_X_BYTE_SIZE + x0;
            LByte = (INT8U) Changedot[DisplayData];
            HByte = (INT8U) ((Changedot[DisplayData]&0xFF00) >> 8);
            SomeNop();
            LCD_SetSRAMAddr(Addr);
            DCU_DATA_PORT = HByte;
            SomeNop();
            SomeNop();
            SomeNop();
            SomeNop();
            DCU_DATA_PORT = LByte;
            SomeNop();
            SomeNop();
            SomeNop();
            SomeNop();
            Addr = Addr + LCD_X_BYTE_SIZE;//下一行
            LCD_SetSRAMAddr(Addr);
            DCU_DATA_PORT = HByte;
            SomeNop();
            SomeNop();
            SomeNop();
            SomeNop();
            SomeNop();
            DCU_DATA_PORT = LByte;
            SomeNop();
*/
            LByte = (INT8U) Changedot[DisplayData];
            HByte = (INT8U) ((Changedot[DisplayData]&0xFF00) >> 8);

            GUI_X_Lock();
            x0 = x0*8;
            for(i = 0; i < 8; i++){
                if (HByte&(1<<(7-i))){
                    LCDSIM_SetPixelIndex(x0++,y0,FgColor);
                }else {
                    LCDSIM_SetPixelIndex(x0++,y0,BkColor);
                }
            }
            for(i = 0; i < 8; i++){
                if (LByte&(1<<(7-i))){
                    LCDSIM_SetPixelIndex(x0++,y0,FgColor);
                }else {
                    LCDSIM_SetPixelIndex(x0++,y0,BkColor);
                }
            }

            GUI_X_Unlock();

        }else{
//            Addr = y0 * LCD_X_BYTE_SIZE + x0;
//            LCD_SetSRAMAddr(Addr);
//            DCU_DATA_PORT = DisplayData;
//            SomeNop();
/*
            y0 = y0*8*2;
            for(i = 0; i < 8; i++){
                if (DisplayData&(1<<i)){
                    GUI_SetColor(GUI_LIGHTRED);
                    LCD_DrawPixel(y0*2, y0);
                    LCD_DrawPixel(y0*2, y0+1);
                    LCD_DrawPixel(y0*2+1, y0);
                    LCD_DrawPixel(y0*2+1, y0+1);
                    y0 = y0+2;
                }else {
                    GUI_SetColor(GUI_LIGHTGREEN);
                    LCD_DrawPixel(y0*2, y0);
                    LCD_DrawPixel(X*2, Y+1);
                    LCD_DrawPixel(X*2+1, Y);
                    LCD_DrawPixel(X*2+1, Y+1);
                    Y = Y+2;
                }
            }
*/
            GUI_X_Lock();
//            x0 = x0*8;
            for(i = 0; i < 8; i++){
                if (DisplayData&(1<<(7-i))){
//                    GUI_X_Lock();
                    LCDSIM_SetPixelIndex(x0++,y0,FgColor);
//                    GUI_X_Unlock();
                }else {
//                    GUI_X_Lock();
                    LCDSIM_SetPixelIndex(x0++,y0,BkColor);
//                    GUI_X_Unlock();
                }
            }
            GUI_X_Unlock();
        }
    }
}
/*
*****************************************************************************
* Display_ASCII - 显示一个ASCII
* DESCRIPTION: -
*
* @Param X:X轴坐标 0--LCD_X_BYTE_SIZE  (注意边界)
* @Param Y:Y轴坐标 0-- LCD_YSIZE (注意边界)
* @Param Char:ASCII码
* @Return :
* ----
*****************************************************************************
*/
void Display_ASCII(INT8U AscciiChar, INT16U X, INT16U Y)
{
    INT8U i, ImgData;
    INT16U Disp_y = Y;
    if(GUIEnFont == En_5x8){
        for(i = 0; i < 8; i++){
            ImgData = chardot_5x8[i+(AscciiChar-0x20)*8];
            Display_Locate(ImgData, X, Disp_y);
            Disp_y ++;
        }
    } else {
        for(i = 0; i < 16; i++){
            ImgData = chardot_8x16[i+(AscciiChar-0x20)*16];
            Display_Locate(ImgData, X, Disp_y);
            if(GUIZoomMode == GUI_TEXTMODE_ZOOMx1){
                    Disp_y ++;
            }else{
                    Disp_y += 2;
            }
        }
   }
}
void Display_InvASCII(INT8U AscciiChar, INT16U X, INT16U Y)
{
    INT8U i, ImgData;
    INT16U Disp_y = Y;
    if(GUIEnFont == En_5x8){
        for(i = 0; i < 8; i++){
            ImgData = chardot_5x8[i+(AscciiChar-0x20)*8] ^0xFF;
            Display_Locate(ImgData, X, Disp_y);
            Disp_y++;
        }
    } else {
        for(i = 0; i < 16; i++){
            ImgData = chardot_8x16[i+(AscciiChar-0x20)*16] ^0xFF;
            Display_Locate(ImgData, X, Disp_y);
            Disp_y++;
        }
   }
}

void Display_ASCII_UnderLine(INT8U AscciiChar, INT16U X, INT16U Y)
{
    INT8U i, ImgData;
    INT16U Disp_y = Y;
    if(GUIEnFont == En_5x8){
        for(i = 0; i < 7; i++){
            ImgData = chardot_5x8[i+(AscciiChar-0x20)*8];
            Display_Locate(ImgData, X, Disp_y);
            Disp_y++;
        }
        ImgData = 0xFF;//下划线
        Display_Locate(ImgData, X, Disp_y);
    } else {
        for(i = 0; i < 15; i++){
            ImgData = chardot_8x16[i+(AscciiChar-0x20)*16];
            Display_Locate(ImgData, X, Disp_y);
            Disp_y++;
        }
        ImgData = 0xFF;//下划线
        Display_Locate(ImgData, X, Disp_y);

   }

}


/*
----------------------------------------------------------------------------------------------
//移动当前Y坐标
----------------------------------------------------------------------------------------------
*/
void GUI_GotoY(INT16U y)
{
    if (y > LCD_YSIZE) {
        PosY = LCD_YSIZE-1;
    } else {
        PosY = y;
    }
}
/*
----------------------------------------------------------------------------------------------
//移动当前Y坐标
----------------------------------------------------------------------------------------------
*/
void GUI_GotoX(INT16U x)
{
    if (x > LCD_X_BYTE_SIZE) {
        PosX = LCD_X_BYTE_SIZE;
    } else {
        PosX = x;
    }
}
/*
*****************************************************************************
* GUI_GotoXY - 系统光标移动到指定位置
* DESCRIPTION: -
*
* @Param y:Y轴坐标
* @Param x:X轴坐标
* @Return :
* ----
*****************************************************************************
*/
void GUI_GotoXY(INT16U x,INT16U y)
{
    GUI_GotoX(x);
    GUI_GotoY(y);
}
//设置背景颜色
void GUI_SetBkColor(GUI_COLOR Color)
{
    BkColor = Color;
}
//设置前景颜色
void GUI_SetColor(GUI_COLOR Color)
{
    FgColor = Color;
}


/*
*****************************************************************************
* GUI_SetTextMode - 设置文字显示模式
* DESCRIPTION: -
*
* @Param TextMode:模式代码
* @Return :模式代码
* ----
*****************************************************************************
*/

INT8U GUI_SetTextMode(INT8U TextMode)
{
    GUITextMode = TextMode;
    return GUITextMode;
}

/*
*****************************************************************************
* GUI_SetEnFont - 设置显示的英文字体
* DESCRIPTION: -
* 目前支持两种字体8x16 5x8
* @Param EnFont:字体代码
* @Return :
* ----
*****************************************************************************
*/
void GUI_SetEnFont(INT8U EnFont)
{
  if (EnFont == En_5x8){
    GUIEnFont = En_5x8;
    Char_XSIZE = En_5x8_XSIZE;
    Char_YSIZE = En_5x8_YSIZE;
  } else {
    GUIEnFont = En_8x16;
    Char_XSIZE = En_8x16_XSIZE;
    Char_YSIZE = En_8x16_YSIZE;
  }
}
void GUI_SetFontZoom(INT8U ZoomMode)
{
    GUIZoomMode = ZoomMode;

    if(GUIZoomMode == GUI_TEXTMODE_ZOOMx1){
        if (GUIEnFont == En_5x8){
            Char_XSIZE = En_5x8_XSIZE;
            Char_YSIZE = En_5x8_YSIZE;
        } else if (GUIEnFont == En_8x16){
            Char_XSIZE = En_8x16_XSIZE;
            Char_YSIZE = En_8x16_YSIZE;
        }
        Chinese_XSIZE = _Chinese_XSIZE_;
        Chinese_YSIZE = _Chinese_YSIZE_;
    }else{
        if (GUIEnFont == En_5x8){
            Char_XSIZE = 2*En_5x8_XSIZE;
            Char_YSIZE = 2*En_5x8_YSIZE;
        } else if (GUIEnFont == En_8x16){
            Char_XSIZE = 2*En_8x16_XSIZE;
            Char_YSIZE = 2*En_8x16_YSIZE;
        }
        Chinese_XSIZE = 2*_Chinese_XSIZE_;
        Chinese_YSIZE = 2*_Chinese_YSIZE_;

    }
}
/*
*****************************************************************************
* GUI_DispCharAt - 指定位置显示一个ASCII码
* DESCRIPTION: - 字体使用是系统设定的字体
*
* @Param x:X轴坐标
* @Param y:Y轴坐标
* @Param c:ASCII码
* @Return :
* ----
*****************************************************************************
*/
void GUI_DispCharAt(INT8U AscciiChar, INT16U x, INT16U y)
{
    switch (GUITextMode) {
        case GUI_TEXTMODE_NORMAL :
             Display_ASCII(AscciiChar, x, y);
             break;
        case GUI_TEXTMODE_REVERSE :
             Display_InvASCII(AscciiChar, x, y);
             break;
        case GUI_TEXTMODE_UNDERLINE :
             Display_ASCII_UnderLine(AscciiChar, x, y);
             break;
        default :
             break;
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#if 1
void GUI_DispChar(INT16U AscciiChar)
{
    GUI_DispCharAt(AscciiChar, PosX, PosY);
    PosX += 1;
}
#endif

/*
*****************************************************************************
* Display_Chinese - 指定位置显示一个16x16的汉字
* DESCRIPTION: -
*
* @Param x0:X轴坐标0--(192-16)
* @Param y0:Y轴坐标0--(8-2)
* @Param UniCode:汉字内码
* @Return :
* ----
*****************************************************************************
*/
void Display_Chinese(INT16U UniCode, INT16U x0, INT16U y0)
{
  INT16U CodeID;
  INT8U i, ImgData;
  INT16U Disp_y = y0;
  // 在字库里搜索汉字
    for(CodeID=0; CodeID < hzNum; CodeID++){
        // 找到 指定汉字的 Index 后, 跳出循环
        if ( (hzIndex[2*CodeID] == UniCode/256) && (hzIndex[2*CodeID+1] == UniCode%256) ){
            break;
        }else if(CodeID == (hzNum-1)){//未检索到的汉字,显示个"##"提示吧
            GUI_DispCharAt('#',x0,y0);
            GUI_DispCharAt('#',x0+Char_XSIZE,y0);
            return;
        }
    }
//----------------------------------
    switch (GUITextMode) {
        case GUI_TEXTMODE_NORMAL :
//-------------------------------------------------------------------------
            for(i = 0; i < 16; i++){
                ImgData = hzdot[i*2+CodeID*32];
                Display_Locate(ImgData, x0, Disp_y);//左边
                ImgData = hzdot[i*2+1+CodeID*32];
                Display_Locate(ImgData, x0+Chinese_XSIZE/2, Disp_y);//右边
                if(GUIZoomMode == GUI_TEXTMODE_ZOOMx1){
                        Disp_y ++;
                }else{
                        Disp_y += 2;
                }
            }
//-------------------------------------------------------------------------
             break;
        case GUI_TEXTMODE_REVERSE :
//-------------------------------------------------------------------------
            for(i = 0; i < 16; i++){
                ImgData = hzdot[i*2+CodeID*32] ^0xFF;
                Display_Locate(ImgData, x0, Disp_y);//左边
                ImgData = hzdot[i*2+1+CodeID*32] ^ 0xFF;
                Display_Locate(ImgData, x0+1, Disp_y);//右边
                Disp_y++;
            }
//-------------------------------------------------------------------------
             break;
        case GUI_TEXTMODE_UNDERLINE :
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
            for(i = 0; i < 15; i++){
                ImgData = hzdot[i*2+CodeID*32];
                Display_Locate(ImgData, x0, Disp_y);//左边
                ImgData = hzdot[i*2+1+CodeID*32];
                Display_Locate(ImgData, x0+1, Disp_y);//右边
                Disp_y++;
            }
//-------------------------------------------------------------------------
            ImgData = 0xFF;
            Display_Locate(ImgData, x0, Disp_y);
            ImgData = 0xFF;
            Display_Locate(ImgData, x0+1, Disp_y);

//-------------------------------------------------------------------------
             break;
        default :
             break;
    }
}
#if 1
void GUI_DispCEOL(void)
{
    INT16U i;
    for(i = PosX; i <= LCD_X_BYTE_SIZE-1; i += 1) {
        GUI_DispCharAt(' ', i, PosY);
    }
}
#endif
/*
*****************************************************************************
* GUI_DispStringAt - 指定位置显示一个字符串
* DESCRIPTION: -
* 支持中英混合 当中英混合时英文字体强制为8x16 如果单单英文 按照设定字体
* @Param x0:X轴坐标
* @Param y0:Y轴坐标
* @Param s:字符串
* @Return :
* ----
*****************************************************************************
*/
//void GUI_DispStringAt(U8 _CONST_ *s,U8 x0,U8 y0)
void GUI_DispStringAt(INT8U _CONST_ *Str,INT16U x0,INT16U y0)
{
    INT8U i;
    if (NoXY == FALSE){//没有提供坐标,则使用系统坐标
        x0 = PosX;
        y0 = PosY;
    }
    i = 0;
//    if(GUI_TEXTMODE_ZOOMx2 ==GUIZoomMode){
//        x0 = x0/2;
//        y0 = y0/2;
//    }
    while(Str[i]!='\0'){
        if (Str[i] < 0x80) {//是ASCII
             if(Str[i] == '\n'){
                    x0 = 0;
                    y0 += Char_YSIZE;
             } else {
                if (x0 > (LCD_X_BYTE_SIZE-1)) {//换行
                    x0 = 0;
                    y0 += Char_YSIZE;
                }
                switch (GUITextMode) {
                    case GUI_TEXTMODE_NORMAL :
                         Display_ASCII(Str[i], x0, y0);
                         break;
                    case GUI_TEXTMODE_REVERSE :
                         Display_InvASCII(Str[i], x0, y0);
                         break;
                    case GUI_TEXTMODE_UNDERLINE :
                         Display_ASCII_UnderLine(Str[i], x0, y0);
                         break;
                    default :
                         break;
                }
                x0 += Char_XSIZE;
             }
        } else {//是中文
            if (x0 > LCD_X_BYTE_SIZE-1){
                x0 = 0;
                y0 += Chinese_YSIZE;
            }
            Display_Chinese(Str[i]*256+Str[i+1],x0,y0);//中文
            i++;
            x0 += Chinese_XSIZE;
       }
       Str++;
    }
    if (NoXY == FALSE){
        PosX = x0;
        PosY = y0;
    }

}
#if 0
/*
*****************************************************************************
* GUI_DispString - 在当前系统坐标下显示字符串
* DESCRIPTION: -
* 除了坐标不能指定外 功能跟GUI_DispStringAt一致
* @Param s:字符串
* @Return :
* ----
*****************************************************************************
*/
void GUI_DispString(INT8U _CONST_ *Str)
{
    NoXY = FALSE;
    GUI_DispStringAt(Str,PosX,PosY);
    NoXY = TRUE;
}

void GUI_DispStringLen(INT8U _CONST_ *Str, INT16U Len)
{
    INT8U i;
    i = 0;
    while(Str[i]!='\0'){
        if (i >= Len ){
            return;
        }
        if (Str[i] < 0x80) {//是ASCII
             if(Str[i] == '\n'){
//                    PosX = 0;
//                    PosY += 2;
             } else {
               if (PosX > (LCD_X_BYTE_SIZE-1)) {//换行
                    PosX = 0;
//                    PosY += 2;
                }
                switch (GUITextMode) {
                    case GUI_TEXTMODE_NORMAL :
                         Display_ASCII(Str[i], PosX, PosY);
                         break;
                    case GUI_TEXTMODE_REVERSE :
                         Display_InvASCII(Str[i], PosX, PosY);
                         break;
                    case GUI_TEXTMODE_UNDERLINE :
                         Display_ASCII_UnderLine(Str[i], PosX, PosY);
                         break;
                    default :
                         break;
                }
                PosX += 1;
             }
        } else {//是中文
            if (PosX > LCD_X_BYTE_SIZE-1){
                PosX = 0;
                PosY += Chinese_YSIZE;
            }
            Display_Chinese(Str[i]*256+Str[i+1],PosX,PosY);//中文
            i++;
            PosX += Chinese_XSIZE;
       }
       i++;
    }

}
/*
void GUI_DrawCursor(INT16U x, INT16U y)
{
    if(GUITextMode == GUI_TEXTMODE_NORMAL){
        Display_Locate(0xFF,x,y);
        Display_Locate(0xFF,x,y+1);
    } else if (GUITextMode == GUI_TEXTMODE_REVERSE){
        Display_Locate(0x00,x,y);
        Display_Locate(0x00,x,y+1);
    } else {
        Display_Locate(0xFF,x,y);
        Display_Locate(0xFF,x,y+1);
    }
}

void GUI_CursorOn(U8 x, U8 y)
{
    Display_Locate(0xFF,x,y);
    Display_Locate(0xFF,x,y+1);
}

void GUI_CursorOFF(U8 x, U8 y)
{
    Display_Locate(0x00,x,y);
    Display_Locate(0x00,x,y+1);
}
*/
#endif

/*
*****************************************************************************
* GUI_DispHexAt - 显示一个数据的十六进制值
* DESCRIPTION: -
* 最大长度4个
* @Param v:数据
* @Param x:X轴坐标
* @Param y:Y轴坐标  XY均是起点坐标 也就是数据最高字节坐标
* @Param Len:长度1--4
* @Return :
*
*****************************************************************************
*/
void GUI_DispHexAt(INT32U v, INT16U x, INT16U y, INT8U Len)
{
    INT8U i;
    INT8U HexData;
    if(Len > 8){//限制范围
        Len = 8;
    }
    for(i = 0; i < Len; i++){
        HexData = v&0x0F;
        v = v >>4;
        if(HexData < 0x0A){
            GUI_DispCharAt(HexData+'0',x+Char_XSIZE*(Len-1-i),y);
        }else{
            GUI_DispCharAt(HexData-0x0A+'A',x+Char_XSIZE*(Len-1-i),y);
        }
    }
}
//显示 bit的数据
void GUI_DispBitAt(INT8U v, INT16U x, INT16U y)
{
    INT8U i;

    for(i = 0; i < 8; i++){
        if(v & 0x80){
            GUI_DispCharAt('1',x+Char_XSIZE*i,y);
        }else{
            GUI_DispCharAt('0',x+Char_XSIZE*i,y);
        }
        v = v << 1;
    }
}

/*
*****************************************************************************
* HBar - 显示一个垂直的进度条
* DESCRIPTION: -
* 附加有百分比显示
* @Param y0:进度条起点X轴坐标   0-->LCD_YSIZE
* @Param y1:进度条结束点X坐标   0-->LCD_YSIZE  必须大于y0  百分比显示于该坐标之后
* @Param x:进度条X轴坐标        0--LCD_X_BYTE_SIZE
* @Param percent:当前百分值     0-->100
* @Return :
*
*****************************************************************************
*/
void VBar(INT16U x, INT16U y0, INT16U y1,INT8U percent)
{
    INT8U U8Temp;
    INT16U Ypos;
    float Center;
    if(percent > 100){//防止上溢出
        percent = 100;
    }
    Center = (y1-y0);
    Center *= percent;
    Center /= 100;
//    U8Temp = (x1-x0)*percent/100;//这个计算做法在430上能用，但C51下似乎必须用浮点算
    U8Temp = (INT8U)Center;
    Display_Locate(0xFF, x, y0);
    Display_Locate(0xFF, x, y1);
    for(Ypos = 1; Ypos < U8Temp; Ypos++){
        Display_Locate(0xBD, x, y0+Ypos);
    }
    for(Ypos = y0+U8Temp+1; Ypos < y1; Ypos++){
        Display_Locate(0x81, x, Ypos);
    }
}

//INT8U _CONST_ BarCode0[]={0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00};
//INT8U _CONST_ BarCode1[]={0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};

INT8U _CONST_ BarCode0[]={0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE};
void HBar(INT16U y, INT16U x0, INT16U x1,INT8U percent)
{

    INT8U U8Temp;
    INT8U U8A;
    INT8U U8B;
    INT16U i;
    INT8U Width;
    float Center;

    Center = (x1-x0)*8;
    Center *= percent;
    Center /= 100;
//    U8Temp = (x1-x0)*percent/100;//这个计算做法在430上能用，但C51下似乎必须用浮点算
    U8Temp = (INT8U)Center;
    U8A = U8Temp / 8;
    U8B = U8Temp % 8;
/*
    if(U8A){
        for(i = 0; i < U8A; i++){
            for(Width = 0; Width < 8; Width++){
                Display_Locate(0xFF, x0 + i, y+Width);
            }
        }

    }else{
        for(Width = 0; Width < 8; Width++){
            Display_Locate(0x80, x0, y+Width);
        }
    }
*/
//------------------
    for(i = 0; i < U8A; i++){
        Display_Locate(0xFF, x0+i, y);
        Display_Locate(0x00, x0+i, y+1);
        for(Width = 2; Width < 6; Width++){
            Display_Locate(0xFF, x0+i, y+Width);
        }
        Display_Locate(0x00, x0+i, y+6);
        Display_Locate(0xFF, x0+i, y+7);
    }

    if (U8A == x1-x0){
        U8Temp = BarCode0[U8B] | 0x01;
    }else if(U8A == 0){
        U8Temp = BarCode0[U8B] | 0x80;
    }else{
        U8Temp = 0xFF;
    }
    i = x0+ U8A;
    Display_Locate(0xFF, i, y);
    Display_Locate(0x00, i, y+1);
    for(Width = 2; Width < 6; Width++){
        Display_Locate(U8Temp, i, y+Width);
    }
    Display_Locate(0x00, i, y+6);
    Display_Locate(0xFF, i, y+7);


//------------------
    for(i = x0+U8A+1; i < x1-1; i++){
        Display_Locate(0xFF, i, y);
        Display_Locate(0x00, i, y+1);
        for(Width = 2; Width < 6; Width++){
            Display_Locate(0x00, i, y+Width);
        }
        Display_Locate(0x00, i, y+6);
        Display_Locate(0xFF, i, y+7);
    }

    if (U8A != x1-x0){
        i = x1-1;
        Display_Locate(0xFF, i, y);
        Display_Locate(0x01, i, y+1);
        for(Width = 2; Width < 6; Width++){
            Display_Locate(0x01, i, y+Width);
        }
        Display_Locate(0x01, i, y+6);
        Display_Locate(0xFF, i, y+7);
    }
/*
    if(U8A){
        for(Width = 0; Width < 8; Width++){
            Display_Locate(0x80, x0, y+Width);
        }
    }
    U8Temp = (INT8U)Center;
    U8Temp = U8Temp % 8;
    if(U8Temp){

    }

    for(i = x0+U8Temp+1; i < x1; i++){
        Display_Locate(0xFF, i, y);
        Display_Locate(0x00, i, y+1);
        for(Width = 2; Width < 6; Width++){
            Display_Locate(0x00, i, y+Width);
        }
        Display_Locate(0x00, i, y+6);
        Display_Locate(0xFF, i, y+7);
    }
*/
}
/*
*****************************************************************************
* GUI_DispDecAt - 显示十进制数值
* DESCRIPTION: -
* 处理长度最多5个数字(因为INT16S--->+- 32768)
* @Param v:显示的数据
* @Param x:X轴坐标
* @Param y:Y轴坐标  XY 均是起点位置坐标 也就是数值最高位的坐标
* @Param Len:指定的显示长度1--5内正负号跟随于最高位的数字前面 更高的用空白显示
5位时:

+12345
   +12
  -235

* @Return :
*
*****************************************************************************
*/
void GUI_DispDecSAt(INT16S v, INT16U x, INT16U y, INT8U Len)
{
#define IsPositive  0
#define IsNegative  1
#define IsZero      2
    INT8U i;
    INT8U CharBuf[5];
    INT8U HighByte;
    INT8U Flag;
    Len += 1;//预留 + -符号
    Flag = 0;
    if(v < 0){
        v = -v;
        Flag = IsNegative;
    }else if (v == 0){
        Flag = IsZero;
    }else{
        Flag = IsPositive;
    }
    HighByte = 0;
    for(i = 0; i < 5; i++){
        CharBuf[i] = (INT8U)(v%10);
        v = v/10;
        if(CharBuf[i]){
            HighByte = i;
        }
    }
    //第0位无论如何也显示
    i = 0;
    GUI_DispCharAt(CharBuf[i]+'0',x+((Len-1)-i)*Char_XSIZE,y);
    for(i = 1; i < Len; i++){
        if(i > HighByte){
            GUI_DispCharAt(' ',x+((Len-1)-i)*Char_XSIZE,y);
        }else{
            GUI_DispCharAt(CharBuf[i]+'0',x+((Len-1)-i)*Char_XSIZE,y);
        }
    }
    if(IsZero == Flag){
        GUI_DispCharAt(' ',x+((Len-2)-HighByte)*Char_XSIZE,y);
    }else if(IsPositive == Flag){
        GUI_DispCharAt('+',x+((Len-2)-HighByte)*Char_XSIZE,y);
    }else{
        GUI_DispCharAt('-',x+((Len-2)-HighByte)*Char_XSIZE,y);
    }


}
/*
*****************************************************************************
* GUI_DispDecAt - 显示十进制数值
* DESCRIPTION: -
* 处理长度最多5个数字(因为INT16S--->+- 32768)
* @Param v:显示的数据
* @Param x:X轴坐标
* @Param y:Y轴坐标  XY 均是起点位置坐标 也就是数值最高位的坐标
* @Param Len:指定的显示长度1--5内 高位用空格填充
5位时:

12345
   12
  235

* @Return :
*
*****************************************************************************
*/
void GUI_DispDecAt(INT16U v, INT16U x, INT16U y, INT8U Len)
{
    INT8U i;
    INT8U CharBuf[5];
    INT8U HighByte;

    HighByte = 0;
    for(i = 0; i < 5; i++){
        CharBuf[i] = (INT8U)(v%10);
        v = v/10;
        if(CharBuf[i]){
            HighByte = i;
        }
    }
    //第0位无论如何也显示
    i = 0;
    GUI_DispCharAt(CharBuf[i]+'0',x+((Len-1)-i)*Char_XSIZE,y);
    for(i = 1; i < Len; i++){
        if(i > HighByte){
            GUI_DispCharAt(' ',x+((Len-1)-i)*Char_XSIZE,y);
        }else{
            GUI_DispCharAt(CharBuf[i]+'0',x+((Len-1)-i)*Char_XSIZE,y);
        }
    }

}

/*
*****************************************************************************
* GUI_DrawIcon - 制定位置显示一个图标 图标数据区的开始2个字节指明该图标大小
* DESCRIPTION: -
*
* @Param x0:X轴坐标
* @Param y0:Y轴坐标 注意边界!
* @Param Ico:图标数据
* @Return :
* ----
*****************************************************************************
*/
void GUI_DrawIcon(INT8U _CONST_ *Ico,INT16U x0,INT16U y0)
{
    INT16U i;
    INT16U j;
    INT16U Count;
    INT16U ICO_XLen;
    INT16U ICO_YLen;

    ICO_XLen = Ico[0];//X Byte len
    ICO_YLen = Ico[1];//Y Byte len

    Count = 2;
    for(j = 0; j < ICO_YLen; j++){
        if(y0 + j < LCD_YSIZE){
            for(i = 0; i < ICO_XLen; i++){
                if(x0 + i < LCD_X_BYTE_SIZE){
                    Display_Locate(Ico[Count++],x0+i,y0+j);
                }else{
                    Count++;//如果越界 只是丢弃数据不显示
                }
            }
        }else{
//            Count++;//如果越界 只是丢弃数据不显示
            return;
        }
    }
}

void GUI_Test(void)
{
    INT8U x;
    INT8U y;
    INT16U Key;
    GUI_Init();
//    for(y = 0; y <= 100;y++){
//        Display_Locate(0xFF, 0,y);
//    }
//    Display_ASCII('A',0,0);
//    Display_ASCII('B',1,0);
//    Display_ASCII('C',1,8);
/*
    for(x = 0; x < 8; x++){
        y = (1<<x);
        GUI_DispBitAt(y, 0,0);
        GUI_DispHexAt(y, 0,Char_YSIZE*4,2);
    }
    for(y = 0; y <= 100;y++){
        for(x = 0; x <= 100;x++){
            //VBar(2,10,210,x);
            HBar(10, 2,22,x);
            LCD_DelayMs(300);
        }
        LCD_DelayMs(300);
    }
*/
//    GUI_SetFontZoom(GUI_TEXTMODE_ZOOMx2);
//    GUI_DispStringAt("ABFC",0,0);
//    GUI_DispStringAt("String测试",0,Char_YSIZE*7);
//    Display_Locate(0x01, 0,0);
//    Display_Locate(0x02, 1,1);
//    Display_Locate(0xFF, 0,1);
//    for(x = 0; x < 8; x++){
//        Display_Locate((1<<x), 0,0);
//    }
    GUI_SetFontZoom(GUI_TEXTMODE_ZOOMx1);

    GUI_DispStringAt("String",0,Char_YSIZE*13);

//    Display_Chinese(0xD5EF,10,0);
    GUI_DispStringAt("String测试中..",0,Char_YSIZE*2);
    GUI_DispStringAt("AString测试中..",10,480-32);
    Display_Locate(0xFF, 0,0);
    while(1){
        for(y = 0; y <= 100;y++){
            GUI_DispDecAt(y,0,0,3);
            SIM_Delay(1);
            Key = GUI_GetKey();
            if(Key){
                GUI_DispHexAt(Key,0,48,4);
            }

        }
    }

//    Display_Locate(0xFF, 0,1);


    Display_Locate(0xFF, LCD_X_BYTE_SIZE-1,0);

    Display_Locate(0xFF, 0,(LCD_YSIZE-1));
    Display_Locate(0xFF, LCD_X_BYTE_SIZE-1,LCD_YSIZE-1);

    for(x = 0; x < (LCD_XSIZE/8); x++){
        Display_Locate(0xAA, x,0);
        Display_Locate(0xAA, x,1);
        Display_Locate(0xAA, x,2);
    }


}
