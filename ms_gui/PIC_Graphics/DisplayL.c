#include <string.h>

#include "DisplayL.h"

//XWindow  * _xwin;
WORD_VAL   _color; // Color
SHORT      _clipRgn; // Clipping region control
SHORT      _clipLeft; // Clipping region borders
SHORT      _clipTop; // Clipping region borders
SHORT      _clipRight; // Clipping region borders
SHORT      _clipBottom; // Clipping region borders

#if 0
	#define PMPDelay()                     Nop(); Nop();
	#define SetIndex(index)                PMADDR=0x0000;PMDIN1=0;PMPDelay();PMDIN1=index;PMPDelay();
	#define SetAddress(addr2,addr1,addr0)  SetIndex(0x20);WriteData(0,addr0);SetIndex(0x21);WriteData(addr2,addr1);SetIndex(0x22);
	#define WriteData(byte1,byte0)         PMADDR=0x8001;PMDIN1=byte1;PMPDelay();PMDIN1=byte0;PMPDelay();
#endif

/*********************************************************************
* Function:  void  DelayMs(WORD time)
*
* PreCondition: none
*
* Input: time - delay in ms
*
* Output: none
*
* Side Effects: none
*
* Overview: delays execution on time specified in ms
*
* Note: delay is defined for 16MIPS
*
********************************************************************/
void  DelayMs( WORD time )
{
//	usleep( 1000 * time );
    SIM_Delay(1 * time);
}

void initDevice( void )
{
//init Device
}

void ResetDevice( void )
{

}

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates
*
* Output: none
*
* Side Effects: none
*
* Overview: puts pixel
*
* Note: none
*
********************************************************************/
void GUI_X_Lock(void);
void GUI_X_Unlock(void);
void  LCDSIM_SetPixelIndex(int x, int y, int Index);
void PutPixel(SHORT x, SHORT y)
{
	DWORD_VAL address;
    if(_clipRgn)
    {
        if(x<_clipLeft)
            return;
        if(x>_clipRight)
            return;
        if(y<_clipTop)
            return;
        if(y>_clipBottom)
            return;
    }
    //y = GetMaxY() - y; this will invert the display

//    XSetForeground( _xwin->display, _xwin->gc, RGB16TO32( _color.Val ) );
//  	XDrawPoint( _xwin->display, _xwin->window, _xwin->gc, x, y );
    //GUI_SetColor(RGB16TO32( _color.Val ));
    //LCD_DrawPixel(x, y );
    GUI_X_Lock();    
    LCDSIM_SetPixelIndex(x,y,_color.Val);
    GUI_X_Unlock();

}

/*********************************************************************
* Function: WORD GetPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates
*
* Output: pixel color
*
* Side Effects: none
*
* Overview: return pixel color at x,y position
*
* Note: none
*
********************************************************************/
WORD GetPixel(SHORT x, SHORT y)
{
	return 0;
}


#ifdef USE_BITMAP_FLASH
/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
{
	register FLASH_BYTE * bmpData;
	register BYTE         stretchX, stretchY;
	register WORD         x, y;
	WORD                  sizeX, sizeY;
	WORD 				  pallete[2];
	BYTE 				  color;
	BYTE 				  mask;

	// Move pointer to size information
    bitmap += 2;
    // Read image size
    sizeY   = *((FLASH_WORD*)bitmap);
    bitmap += 2;
    sizeX   = *((FLASH_WORD*)bitmap);
    bitmap += 2;

    // Read pallete
    for( color = 0; color < sizeof(pallete)/sizeof(pallete[0]); color++ )
    {
        pallete[color]  = *((FLASH_WORD*)bitmap);
        bitmap         += 2;
    }

    for(y=0; y<sizeY; y++)
    {
        bmpData = bitmap;
        for(stretchY = 0; stretchY<stretch; stretchY++)
        {
            bitmap = bmpData;
            mask   = 0;
            for(x=0; x<sizeX; x++)
            {
                // Read 8 pixels from flash
                if(mask == 0)
                {
                    color = *bitmap++;
                    mask  = 0x80;
                }

                // Set color
                SetColor( pallete[ (mask&color) ? 1 : 0 ] );
                // Shift to the next pixel
                mask >>= 1;

                // Write pixel to screen
                for(stretchX=0; stretchX<stretch; stretchX++)
                {
                    PutPixel( left + (x * stretch) + stretchX, top + (y * stretch) + stretchY );
                }
           }
        }
    }
}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 16 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
{
	register FLASH_BYTE * bmpData;
	register BYTE         stretchX, stretchY;
	register WORD         x, y;
	WORD                  sizeX, sizeY;
	WORD 				  pallete[16];
	BYTE 				  color;

    // Move pointer to size information
    bitmap += 2;
    // Read image size
    sizeY   = *((FLASH_WORD*)bitmap);
    bitmap += 2;
    sizeX   = *((FLASH_WORD*)bitmap);
    bitmap += 2;

    // Read pallete
    for( color = 0; color < sizeof(pallete)/sizeof(pallete[0]); color++ )
    {
        pallete[color]  = *((FLASH_WORD*)bitmap);
        bitmap         += 2;
    }

    for(y=0; y<sizeY; y++)
    {
        bmpData = bitmap;
        for(stretchY = 0; stretchY<stretch; stretchY++)
        {
            bitmap = bmpData;
            for(x=0; x<sizeX; x++)
            {

				// Read 2 pixels from flash
				if( (x&1) == 0 )
				{
					color = *bitmap++;
				}
                // Set color
                SetColor( pallete[ color & 0x0F ] );
                color >>= 4;

                // Write pixel to screen
                for(stretchX=0; stretchX<stretch; stretchX++)
                {
                    PutPixel( left + (x * stretch) + stretchX, top + (y * stretch) + stretchY );
                }
            }
        }
    }
}

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 256 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
{
	register FLASH_BYTE * bmpData;
	register BYTE         stretchX, stretchY;
	register WORD         x, y;
	WORD                  sizeX, sizeY;
	WORD 				  pallete[256];
	WORD 				  color;

    // Move pointer to size information
    bitmap += 2;
    // Read image size
    sizeY   = *((FLASH_WORD*)bitmap);
    bitmap += 2;
    sizeX   = *((FLASH_WORD*)bitmap);
    bitmap += 2;

    // Read pallete
    for( color = 0; color < sizeof(pallete)/sizeof(pallete[0]); color++ )
    {
        pallete[color]  = *((FLASH_WORD*)bitmap);
        bitmap         += 2;
    }

    for(y=0; y<sizeY; y++)
    {
        bmpData = bitmap;
        for(stretchY = 0; stretchY<stretch; stretchY++)
        {
            bitmap = bmpData;
            for(x=0; x<sizeX; x++)
            {
                // Read pixels from flash & Set color
                SetColor( pallete[ (*bitmap++)&0xFF ] );

                // Write pixel to screen
                for(stretchX=0; stretchX<stretch; stretchX++)
                {
                    PutPixel( left + (x * stretch) + stretchX, top + (y * stretch) + stretchY );
                }
            }
        }
    }
}

/*********************************************************************
* Function: void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs hicolor image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
{
	register FLASH_WORD * flashAddress, * tempFlashAddress;
	register BYTE        stretchX, stretchY;
	register WORD        x, y;
	WORD                 sizeX, sizeY;

    // Move pointer to size information
    flashAddress = (FLASH_WORD*)bitmap + 1;

    // Read image size
    sizeY = *flashAddress++;
    sizeX = *flashAddress++;

    for(y=0; y<sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY<stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            for(x=0; x<sizeX; x++)
            {
            	//Read pixels from flash & Set color
                SetColor( *flashAddress++ );

                // Write pixel to screen
                for(stretchX=0; stretchX<stretch; stretchX++)
                {
                    PutPixel( left + (x * stretch) + stretchX, top + (y * stretch) + stretchY );
                }
            }
        }
    }
}
#endif



/*********************************************************************
* Function: void PutImage(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage(SHORT left, SHORT top, void* bitmap, BYTE stretch)
{
	FLASH_BYTE* flashAddress;
	BYTE colorDepth;
	WORD colorTemp;

    // Save current color
    colorTemp = _color.Val;

    switch(*((SHORT*)bitmap))
    {
        case FLASH:
            // Image address
            flashAddress = ((BITMAP_FLASH*)bitmap)->address;
            // Read color depth
            colorDepth = *(flashAddress+1);
            // Draw picture
            switch(colorDepth)
            {
                case 1:
                    PutImage1BPP(left, top, flashAddress, stretch);
                    break;
                case 4:
                    PutImage4BPP(left, top, flashAddress, stretch);
                    break;
                case 8:
                    PutImage8BPP(left, top, flashAddress, stretch);
                    break;
                case 16:
                    PutImage16BPP(left, top, flashAddress, stretch);
                    break;
            }
            break;
        default:
            break;
    }

    // Restore current color
    _color.Val = colorTemp;
}

