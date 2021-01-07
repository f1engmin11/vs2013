
/***************************************************************
  REVISION LOG ENTRY
  Revision By: ...
  Revised on 2005-05-30 20:25:27
  Comments: LCDSIM.c LCDģ����ʾ��...

  1. �޸ĺ���LCDSIM_FillRect��BPP���ڰ�λʱ��BUG, �����Ӽ�������, ��߾���ֱ����ٶ�.
  2. �޸ĺ���LCDSIM_SetPixelIndex, ��BPP���ڰ�λʱ��������ɫ��������һ����ͬ, �����������������
     RGB��ɫֵ��ת��, �������������ھ�����估���ƴ���������ͬ��ɫʱ���ٶ�.
  3. ������LCDSIM_SetPixelIndex�м�¼������صı����Ĵ����. ��ɫ��ļ�¼����Ҳ���, ������.

 ***************************************************************/


#include "Common.h"

extern int	xPosLCD, yPosLCD;
extern int	rgbTransparent;
extern HWND	hWndMain;
extern char LCDCaption[200];
extern int	LCDWidth, LCDHeight;
extern HANDLE	SimFileMap;


#define LCDColorBlackNum	4
#define LCDColorWhiteNum	4
#define SPACEBEFORELCDM		0x400		//��ʾӳ����ʼ��ַ��LCD��ʾ�ڴ�֮��Ŀռ�...	

int		LCDSIM_aLCDColorBlack[LCDColorBlackNum];
int		LCDSIM_aLCDColorWhite[LCDColorWhiteNum] = {0xffffff, 0xffffff, 0xffffff, 0xffffff}; 
int		XSize = 0, YSize = 0, VYSize = 0, VXSize = 0;

LPVOID	pFix;				//������ڴ�ӳ��ʼ��ַ...
LPVOID	pSMemFix;			//������ڴ�ӳ��ʼ��ַ...
LPVOID	pSMemVar;			//���������LCD�Դ����ʼӳ���ַpSMemFix+0x1000[ΪSPACEBEFORELCDM]...
LPVOID	paaPixel;			//���������LCD�Դ����ʼӳ���ַ...		
LPVOID	pBitmapInfo;		//ӳ����LCD����ʾλͼ֮��ɫ���ӳ����ʼ��ַpSMemFix+0x28...

DWORD	FixedPalette = 0;	//��ɫ�������õ�ɫ����Ŀ...
int		BPP = 8;			//һ��������ռλ��...
int		NumColors = 0;		
int		LUT_ModifyCnt = 0;
int		BytesPerLine = 0;
int		ModifyCnt = 0;


LCD_tMouseState	mouseMessage;



void	SIM_SetLCDPos(int x, int y)
{
	xPosLCD = x;
	yPosLCD = y;
}


///////////////////////////////////////////////////////////////////////
//
// ������       : SIM_SetTransColor
// ��������     : ����͸��ɫ
// ����         : int Color
// ����ֵ       : 
//
///////////////////////////////////////////////////////////////////////
int	SIM_SetTransColor(int Color)
{
	int OldrgbTransparent = rgbTransparent;
	rgbTransparent = Color;
	return OldrgbTransparent;
}


///////////////////////////////////////////////////////////////////////
//
// ������       : SIM_SetLCDColorBlack
// ��������     : ���ñ���ɫ.������������.
// ����         : unsigned int Index
// ����         : int Color
// ����ֵ       : 
//
///////////////////////////////////////////////////////////////////////
int	SIM_SetLCDColorBlack(unsigned int Index, int Color)
{
	int ret = 0;
	if(Index >=4)	return ret;
	ret = LCDSIM_aLCDColorBlack[Index];
	LCDSIM_aLCDColorBlack[Index] = Color;
	return ret;
}


///////////////////////////////////////////////////////////////////////
//
// ������       : SIM_SetLCDColorWhite
// ��������     : ����ǰ��ɫ....
// ����         : unsigned int Index
// ����         : int Color
// ����ֵ       : 
//
///////////////////////////////////////////////////////////////////////
int	SIM_SetLCDColorWhite(unsigned int Index, int Color)
{
	int ret = 0;
	if(Index >=4)	return ret;
	ret = LCDSIM_aLCDColorWhite[Index];
	LCDSIM_aLCDColorWhite[Index] = Color;
	return ret;

}


///////////////////////////////////////////////////////////////////////
//
// ������       : LCDSIM_Init
// ��������     : �˺��������ʼ��LCD��ʹ�õ��ڴ�ӳ��...����һ�����ô��ɫ��.
//				: һ��������ʾ��Ļ..
// ����         : void
// ����ֵ       : char*  
//
///////////////////////////////////////////////////////////////////////
char*  LCDSIM_Init(void)
{
	char*	lpret = 0;
	int*	lptemp = 0;
	int		i = 0;
	int		lpCount = 0;
	do{
		InitSMem();
		if(pSMemFix == 0)	break;
		XSize = LCD_GetXSize() * LCD_GetXMag();
		YSize = LCD_GetYSize() * LCD_GetYMag();
		VXSize = LCD_GetVXSize();
		VYSize = LCD_GetVYSize();
		BPP = LCD_GetBitsPerPixel();
		FixedPalette = LCD_GetFixedPalette();
		NumColors = LCD_GetNumColors();

		pFix = pSMemFix;
		lptemp = (int*) pSMemFix;
		for(i = 0; i < SPACEBEFORELCDM; i++)
		{
			*lptemp++ = 0;
		}
		strcpy((char*)pSMemFix, "emWin GSC Simulation");
		lpCount = (XSize * YSize & 0xfffffff0)/4;	//����Ϊ��4����...
		lptemp = (int*) pSMemVar;
		for(i = 0; i < lpCount; i++)
		{
			*lptemp++ = 0;
		}
		lpret = (char*)lptemp;
		lpCount = XSize * YSize & 3;				//��ʼ��������4���ֽڵ��ڴ�...
		for(i  = 0; i < lpCount; i++)
		{
			*((char*)lpret)++ = 0;
		//	__asm mov lpret, 0;
		}
		lpret = 0;
		pBitmapInfo = (char*)pSMemFix + 0x100; 
		paaPixel = pSMemVar;
									
		// 2005-6-5 15:18:39
		lptemp =(int*)((char*)pSMemFix+0x20);			//pSMemFix+0x20��ʼ���δ�����¶���...
		*lptemp++ = XSize;
		*lptemp++ = YSize;
		*lptemp++ = VXSize;
		*lptemp++ = VYSize;
		*lptemp++ = FixedPalette;						//�˴��ݿ���...
		*lptemp++ = BPP;
		*lptemp++ = NumColors;
		
		if(BPP <= 8){
			BytesPerLine = (XSize + 3) & 0xFFFFFFFC;	//һ�ж��ٸ��ֽ�,BPPΪ8��һ���ֽڱ�ʾһ������
														//����,���ĸ��ֱ�ʾһ������,+3�Ǳ�ʾLCD��ʾ�������ױ��п�...
			for(i = 0; i < NumColors; i++)				//�˴���ʼ����ɫ��...
			{
			//	LCDSIM_SetLUTEntry(i, LCD_L0_Index2Color(i));
			}
		}
		else{
			BytesPerLine = (XSize* 4 + 3)  & 0xFFFFFFFC;	//һ�ж��ٸ��ֽ�,BPPΪ8��һ���ֽڱ�ʾһ������
		//	BytesPerLine = (XSize + 3) * 4 + 3;
		}
		return 0;
	}while(0);
	lpret = "Could not alloc Server data ...";
	return lpret;
}



///////////////////////////////////////////////////////////////////////
//
// ������       : InitSMem
// ��������     : ��ʼ��LCD��ʹ�õ��ڴ�ӳ��, �����Ǵ�, ����Ҫע�����
//				: һ����, ��UCGUIӦ�ó����Ѿ����е��������ģ����Ҳ�Ѿ�����,
//				: ���Щʱ�ص�ԭ����UCGUIӦ�ó�����ٿ�һ���µ�, ��ô�������һ��
//				: ���³�ʼ��, InitSMem�ᱻ�ٴε���, ��ʱ�����ٴ�ӳ��...
// ����         : void
// ����ֵ       : void  
//
///////////////////////////////////////////////////////////////////////
void  InitSMem(void)
{
	//HANDLE SimFileMap = 0;
	if(SimFileMap != 0)	return;
	SimFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, 1, "emWinLCDMap");
	if(SimFileMap == 0){
		SimFileMap = CreateFileMapping((HANDLE)0xffffffff, 0, PAGE_READWRITE, 0, 0x401000, "emWinLCDMap");
	}
	pSMemFix = MapViewOfFile(SimFileMap, SECTION_MAP_WRITE | SECTION_MAP_READ, 0, 0, 0x401000);
	pSMemVar = (int*)pSMemFix + 0x400;
	//if(SimFileMap)	CloseHandle(SimFileMap);
}


/* 2005-6-2 23:58:54
#if !defined (LCD_PHYSCOLORS)
  #if   (LCD_BITSPERPIXEL == 1) | (LCD_BITSPERPIXEL == 2) | (LCD_BITSPERPIXEL == 4)
    #ifndef LCD_FIXEDPALETTE
      #define LCD_FIXEDPALETTE LCD_BITSPERPIXEL
    #endif
  #endif
#endif // defined (LCD_PHYSCOLORS) 
*/
/* 2005-6-2 23:01:21
#if !defined (LCD_FIXEDPALETTE)
  #if !defined (LCD_PHYSCOLORS)
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
  #else
    #define LCD_FIXEDPALETTE 0
  #endif
#endif*/

//�������ע�����ݿ��Կ���LCD_BITSPERPIXEL����8ʱֻ���������,444/555/565

/* 2005-6-10 0:20:02
LCD_COLOR LCD_Index2Color_444(int Index) 
{
	unsigned int r,g,b;
	r = Index & 0xf;
	g = (Index >> 4) & 0xf;
	b = ((unsigned)Index >> 8) & 0xf;
	r = r * 17;
	g = g * 17;
	b = b * 17;
	return r + (g<<8) + (((U32)b)<<16);
}*/

//-FixedPalette��ʾRGB��,R��B�Ի�...
//#include "LCD_Protected.h"

///////////////////////////////////////////////////////////////////////
//
// ������       : Convert_Index16IntoIndex32
// ��������     : ����ɫ����ֵתΪRGB��ɫֵ, ��FixedPaletteΪ444ʱ, ����ʾ
//				: 4096����ɫ, ������ֵ��Ϊ0-4095, �����غ�����ָ���Ĳ���RGB, ��
//				: �Ǵ�����ֵ, д�뵽LCD��ʾ�ڴ���, ҪתΪFixedPalette��ӦRGBֵ.
//				: FixedPaletteֵ��ͬ, ת��Ҳ��С�Ĳ���...
//				: gui\ConvertColor\����ļ����ļ�ר�����ڴ˵�...
//				: ����ת����RGB�Ļ���ԭ��------
// ����         : int Index
// ����ֵ       : LCD_COLOR 
//
///////////////////////////////////////////////////////////////////////

// ����ת����RGB�Ļ���ԭ��------����, ����RGB��ֵ, ��FixedPaletteָ����ÿ��λ��ռ��
// λ��, ��Ϊ444���4λ. FixedPalette��ָ���ܱ�ʾ��RGB�ķ�Χ�ֱ�Ϊ(R,G,B)1111/1111/1111,
// ��16(R)/16(G)/16(B). ������ֵIndexΪ444ʱ, ����RGB��ֵ��������ʱ��, �ɴ˼��ɷ���, 
// R,G,B��ֵҪ��ʾ����ɫ��Χ��0x00-0xff, Ϊ�������޵�0-16����������ʾ0x00-0xff��Щ��ɫ
// ֵ, ��ô��Ȼֻ��������������ʾ, ��R��˵, �����ȡֵΪ(17*0, 17*1, 17*2, 17*3,.....17*15=248), 
// ��˵Ļ�, ���ܲ�������޵�16������ֵ�½�0xff����ɫ���̶ȵı�ʾ��(�Ƚ�ƽ��)...

// ͬ��, ����FixedPaletteΪ555ʱ, ��Rȡֵ���Ϊ(8*0, 8*1, 8*2, 8*3,.....8*31=243)
// ͬ��, ����FixedPaletteΪ565ʱ, ��Bȡֵ���Ϊ(4*0, 4*1, 4*2, 4*3,.....4*63=252)
// ֪����һ��, ��������º��������ͷǳ�����...
LCD_COLOR LCD_Index2Color_444(int Index);
LCD_COLOR LCD_Index2Color_555(int Index);
LCD_COLOR LCD_Index2Color_M555(int Index);
LCD_COLOR LCD_Index2Color_565(int Index);

LCD_COLOR Convert_Index16IntoIndex32(int Index)
{
//	int convertColor = 0;
	int r = 0, g = 0, b = 0;
	LCD_COLOR convertColor = 0;
	switch(FixedPalette){
	case 444:
		convertColor = LCD_Index2Color_444(Index);
		break;
	case 555:
		convertColor = LCD_Index2Color_555(Index);
		break;
	case -555:
		convertColor = LCD_Index2Color_M555(Index);
		break;
	case 565:
		convertColor = LCD_Index2Color_565(Index);
		break;
	case -565:
		convertColor = LCD_Index2Color_M555(Index);
		break;
	case -444:	//�޴���ת��...
		break;
	}
// 2005-8-21 16:32:22
	r = (convertColor & 0xff) << 16;
	g = (convertColor & 0xff00);
	b = (convertColor & 0xff0000)>>16;
	convertColor = r | g | b;
	return convertColor;

/* 2005-6-4 12:33:06
	return INDEX2COLOR(Index);		//��һ����Ե���ǰ���N��...
*/
}


///////////////////////////////////////////////////////////////////////
//
// ������       : LCDSIM_SetPixelIndex
// ��������     : ������Ļ��㺯��...
// ����         : int x[��x����]
// ����         : int y[��y����]
// ����         : int Index[��ɫ����ֵ]
// ����ֵ       : void  
//
///////////////////////////////////////////////////////////////////////
void  LCDSIM_SetPixelIndex(int x, int y, int Index)
{
	static int preIndex = 0, preLUT = 0, curLUT = 0, curColor = 0;
	int pixPos = 0;
	char* lptemp = 0;
	if(paaPixel == 0)	return;
	if(BPP <= 8){
		pixPos = y * BytesPerLine + x;
		lptemp = (char*)paaPixel + pixPos;
		*lptemp = Index;
	}
	else{
// 2005-8-27 13:04:52
		lptemp = (char*)pFix + 0x40;	
		curLUT = *((int*)lptemp);
		if(curLUT != preLUT){	//�ı��˵�ɫ��ʱ, һ��Ҫ���¸�������ת��ɫ...
			preLUT = curLUT;
			preIndex = -1;
		}
		if(preIndex != Index){
			curColor = Convert_Index16IntoIndex32(Index);
		}
		pixPos = y * BytesPerLine + x * 4;
		lptemp = (char*)paaPixel + pixPos;
		*(int*)lptemp = curColor;
		preIndex = Index;
	}
	ModifyCnt++;
	//ӳ���Ƿ�ɹ�����...
	if(pFix != 0){
		lptemp = (char*)((char*)pFix + 0x3c);
		*((int*)lptemp) = ModifyCnt;
		// 2005-8-27 13:04:52, ��ǰ��дһ���ַ���ȥ, Ӧ��дһINT��ȥ...
		//*lptemp = ModifyCnt;
	}
}

//�����ֻ��0xff		0xff		0...0xff(256��)
//			0xff		0...0xff	0xff(256��)
//			0...0xff	0xff		0xff(256��)

///////////////////////////////////////////////////////////////////////
//
// ������       : LCDSIM_SetSubPixel
// ��������     : �˺�����Ҫ��Ϊ�˴���RGBʱ, ����Ϊ��ǿɫ, ��һ��Ϊ0...0xff
//				: valueȡֵΪ256��, ��ɫֵ��x%3 << 3������, ��x��3���ĵ㶼��ʾ
//				: ͬһ����ɫ(һ����ֱ��), ��ɫֵΪValue | ~(0xff << (x%3 << 3))
// ����         : int x
// ����         : int y
// ����         : U8 Value[0--255]
// ����ֵ       : void 
//
///////////////////////////////////////////////////////////////////////
void LCDSIM_SetSubPixel(int x, int y, U8 Value)
{
	int pixPos = 0;
	char* lptemp = 0;

	if(BPP <= 0)	return;
	//(0xff << (x%3 << 3))��˷���...
	Value = Value | ~(0xff << (x%3 << 3));
	if(BPP > 8){
		pixPos = y * BytesPerLine + x * 4;
		(char*)paaPixel += pixPos;
		*(int*)paaPixel = Value;
	}
	ModifyCnt++;
	//ӳ���Ƿ�ɹ�����...
	if(pFix != 0){
		lptemp = (char*)((char*)pFix + 0x3c);
		*lptemp = ModifyCnt;
	}
}

//����һ����ɫ���ںڰ�֮��ĵĶԱȶ�...

///////////////////////////////////////////////////////////////////////
//
// ������       : FilterColor
// ��������     : ��������������õ�ɫ��(LCDSIM_SetLUTEntry)ʱ�õ���, ��Ҫ����
//				: ��8λ��8λ��������ʾһ�����ص�ʱ. Ҫ������Ӧ��ɫֵ, ֻ����
//				: ���ںڰ�֮��һ������ֵ, �����Ϊ0xff, ��Ϊ0x0,��ôcolor�����Ϊ��ɫֵ,
//				: �����Ϊ0x80, ��Ϊ0x10, ��ô[color]_R=(0x80-0x10)*color/255, ����B,Gԭ����ͬ
//				: ����, �ڵ���LCDSIM_SetLUTEntry(i, LCD_L0_Index2Color(i))�������, color��
//				: ʵ��һ��RGB��ɫ, ��FixedPaletteΪ111ʱ, ��ɫ����ֻ��Ҫ������ɫ, ����Ҫ256��
//				: ѡ��, ����Ϊ(0x000000[0], 0x0000ff[1], 0x00ff00[2], 0x00ffff[3], 0xff0000[4], 0xff00ff[5], 0xffff00[6], 0xffffff[7] )
//				: ����ɫ������(0-7, 8-15, 16-24, ....248-255)����FixedPaletteΪ111��˵���ظ���...
//				: ���Գ�ʼ����ɫ��ʱ, Ҫ�ȵ���LCD_L0_Index2Color(i)��IndexתΪ��ɫ, ����ڰ׳ɱ�
//				: ���ۺ�. �����˾ͿɶԸ�����ʵ��ʹ�õ�ɫ��Ϊ2��,4��,8��,16��,256���һ���ֽڱ�ʾ�������
//				: �õ���ɫ�������.
// ����         : LCD_COLOR color[RGB��ɫֵ]
// ����         : LCD_COLOR colorBlack
// ����         : LCD_COLOR colorWhite
// ����ֵ       : LCD_COLOR 
//
///////////////////////////////////////////////////////////////////////
LCD_COLOR FilterColor(LCD_COLOR color, LCD_COLOR colorBlack, LCD_COLOR colorWhite)
{
	int temp1 = 0, temp2 = 0, temp3 = 0;
//	temp2 = 0xff * 0xcc / 0xff;
	temp1 = (colorWhite & 0xff - colorBlack & 0xff) * (color & 0xff) / 0xff; //+ colorBlack & 0xff;
	temp2 = ((colorWhite & 0xff00 - colorBlack & 0xff00) >> 8) * ((color & 0xff00) >> 8) / 0xff;// + colorBlack & 0xff00;
	temp3 = ((colorWhite & 0xff0000 - colorBlack & 0xff0000) >> 16) * ((color & 0xff0000) >> 16) / 0xff;// + colorBlack & 0xff0000;
	return temp1 | temp2 << 8 | temp3 << 16;
}


///////////////////////////////////////////////////////////////////////
//
// ������       : LCDSIM_SetLUTEntry
// ��������     : ��ʼ�����ص�Ϊ8λ������ʱ, Ҫ�õ���ɫ������ʾλͼ�����.��ɫ��Ϊ256��.
//				: ֻ�������õ�8λ��ʾһ����ʱ��������ɫ��.���������,��ɫ��ֵ���ظ�����.
// ����         : U8 Pos
// ����         : LCD_COLOR color
// ����ֵ       : void  
//
///////////////////////////////////////////////////////////////////////
void  LCDSIM_SetLUTEntry(U8 Pos, LCD_COLOR color)
{
	char* lptemp = 0;
	if(BPP == 0)	return;
	color = FilterColor(color, LCDSIM_aLCDColorBlack[0], LCDSIM_aLCDColorWhite[0]);
	lptemp = (char*)pBitmapInfo + 0x28 + Pos * 4;
	*(char*)lptemp++ = (color & 0xff0000) >> 16;
	*(char*)lptemp++ = (color & 0xff00) >> 8;
	*(char*)lptemp = color & 0xff;
	ModifyCnt++;
	LUT_ModifyCnt++;
	if(pFix != 0){
		lptemp = (char*)pFix + 0x3c;
		*lptemp = ModifyCnt;
		lptemp = (char*)pFix + 0x40;	
		// 2005-8-27 13:17:57
		//*lptemp = LUT_ModifyCnt;
		*((int*)lptemp) = LUT_ModifyCnt;
	}
}


///////////////////////////////////////////////////////////////////////
//
// ������       : LCDSIM_Index2Color
// ��������     : ����ת����RGB��ɫ, �����ص�ռ8λ(��8λ����)�ʹ���8λ�����
//				: ��UCGUI�ٷ��ṩ��ת����������BUG(����8λʱ), �����ڴ˺���һ��
//				: �����ֻ��8λ������µ���, ���Բ��ᵼ�³���.��Ϊ���ȵ��õ���LCDSIM_GetPixelColor.
//				: ����8�����, ��������ֱ���ڵ�ɫ��ȥȡ, ����8λ����Ҫ����һ��ת��.
// ����         : int Index
// ����ֵ       : int 
//
///////////////////////////////////////////////////////////////////////
int LCDSIM_Index2Color(int Index)
{
	int getColor = 0;

	if(BPP == 0)	return getColor;
	else if(BPP > 8){
		getColor = Convert_Index16IntoIndex32(Index);
	}
	else if(BPP <= 8){
		getColor = *((char*)pBitmapInfo + 0x28 + Index*4) << 16 | *((char*)pBitmapInfo + 0x29 + Index*4) << 8 | *((char*)pBitmapInfo + 0x2a + Index*4);
	}
	return getColor;
}


///////////////////////////////////////////////////////////////////////
//
// ������       : LCDSIM_GetPixelColor
// ��������     : ȡָ�����ص���ɫ, 
// ����         : int x
// ����         : int y
// ����ֵ       : int 
//
///////////////////////////////////////////////////////////////////////
int LCDSIM_GetPixelColor(int x, int y)
{
	int Color = 0;
	if(paaPixel == 0)	return Color;
	if(BPP > 8){
		Color = *((int*)paaPixel + y * BytesPerLine + x * 4);		//ֱ������ʾ�ڴ���ȡ, ��ֵ��Ϊ�õ�RGBֵ.
	}
	else if(BPP <= 8){
		Color = *((char*)paaPixel + BytesPerLine * y + x * 4);
		Color = LCDSIM_Index2Color(LCDSIM_GetPixelIndex(x, y));
	}
	return Color;
}

//�˺���������ʵ�ַ���


///////////////////////////////////////////////////////////////////////
//
// ������       : COLORREF2Index
// ��������     : Convert_Index16IntoIndex32���෴���ܺ���, �������ص����8λ�����
//				: ��ת��ָ��RGBֵ��Index...
// ����         : LCD_COLOR color
// ����ֵ       : int 
//
///////////////////////////////////////////////////////////////////////
int COLORREF2Index(LCD_COLOR color)
{
	char r = 0, b = 0, g = 0;
	int x = 0, y = 0, z = 0;
	x = FixedPalette / 100;
	y = (FixedPalette % 10) %10;
	z = FixedPalette % 10;
	b  = (color >> 16) >> (8 - x);
	g  = ((color >> 8) & 8) >> (8 - y);
	r  = (color & 0xff) >> (8 - z);
	return r | g << y | b << (y+z);

/* 2005-6-4 12:31:02
	int Index = 0;
	switch(FixedPalette){
	case 444:
		Index = LCD_Color2Index_444(Index);
		break;
	case 555:
		Index = LCD_Color2Index_555(Index);
		break;
	case -555:
		Index = LCD_Color2Index_M555(Index);
		break;
	case 565:
		Index = LCD_Color2Index_565(Index);
		break;
	case -565:
		Index = LCD_Color2Index_M555(Index);
		break;
	case -444:	//�޴���ת��...
		break;
	}
	return Index;*/
/* 2005-6-4 12:33:06
	return COLOR2INDEX(color);		//��һ����Ե���ǰ���N��...
*/
}


///////////////////////////////////////////////////////////////////////
//
// ������       : LCDSIM_GetPixelIndex
// ��������     : ��ȡָ�����ص���ɫ����.
// ����         : int x
// ����         : int y
// ����ֵ       : int  
//
///////////////////////////////////////////////////////////////////////
int  LCDSIM_GetPixelIndex(int x, int y)
{
	int Index = 0;
	if(BPP == 0 || paaPixel == 0)	return Index;
	else if(BPP > 8){
		Index = COLORREF2Index(*((char*)paaPixel + y * BytesPerLine + x*4));
	}
	else if(BPP <= 8){
		Index = *((char*)paaPixel + y * BytesPerLine + x);
	}
	return Index;
}

/* 2005-6-4 14:11:24
//MOSE��Ϣ�ṹ...
typedef struct {
  int x,y;
  unsigned char KeyStat;
} LCD_tMouseState;*/

int  LCDSIM_GetMouseState(LCD_tMouseState *pState)
{
	pState->x = *(int*)((char*)pFix + 0x50);
	pState->y = *(int*)((char*)pFix + 0x54);
	pState->KeyStat = *((char*)pFix + 0x58);
	return 0;
}

void LCDSIM_SetMouseState(int x, int y, int KeyStat)
{
	int* lptemp = 0;
	int Mag_x = 1, Mag_y = 1;
	if(pFix == 0)	return;
	lptemp = (int*)((char*)pFix + 0x50);
	if(LCD_GetXMag() > 0) Mag_x = LCD_GetXMag();
	if(LCD_GetYMag() > 0) Mag_y = LCD_GetYMag();
	*lptemp++ = x / Mag_x;
	*lptemp++ = y / Mag_y;
	*(char*)lptemp = KeyStat;
	mouseMessage.x = x / Mag_x;
	mouseMessage.y = y / Mag_y;
	mouseMessage.KeyStat = KeyStat;
	NotifyMouseState(mouseMessage);
}


///////////////////////////////////////////////////////////////////////
//
// ������       : NotifyMouseState
// ��������     : ������Ϣ��GUI�ڲ�, ����GUI�ڲ����¼�����...
// ����         : LCD_tMouseState mouseState
// ����ֵ       : void 
//
///////////////////////////////////////////////////////////////////////
LCD_tMouseState _State;
void NotifyMouseState(LCD_tMouseState mouseState)
{
	if(mouseState.KeyStat == 0){
//		GUI_TOUCH_StoreState(-1, -1);
        //_State.x = -1;
        //_State.y = -1;
        _State.KeyStat = 0;
        
	}
	else{
//		GUI_TOUCH_StoreState(mouseState.x, mouseState.y);
        _State.x = mouseState.x;
        _State.y = mouseState.y;   
        _State.KeyStat = mouseState.KeyStat;
	}
//	GUI_MOUSE_StoreState((const GUI_PID_STATE*)&mouseState);
}

void GUI_TOUCH_GetState(LCD_tMouseState *state)
{
    *state = _State;
}

///////////////////////////////////////////////////////////////////////
//
// ������       : LCDSIM_CheckMouseState
// ��������     : ��lcd���ں����еĶ�ʱ���е���, ������ʱ��Ӧ�ⲿMOUSE�¼�,
//				: ����MOUSE���������ס�Ի�������ƶ�...
// ����         : void
// ����ֵ       : void 
//
///////////////////////////////////////////////////////////////////////
void LCDSIM_CheckMouseState(void)
{
	int x = 0, y = 0;
	char KeyStat = 0;
	x = *(int*)((char*)pFix + 0x50);
	y = *(int*)((char*)pFix + 0x54);
	KeyStat = *((char*)pFix + 0x58);
	if(x | y | KeyStat){
		mouseMessage.x = x;
		mouseMessage.y = y;
		mouseMessage.KeyStat = KeyStat;
		NotifyMouseState(mouseMessage);
	}
}

void LCDSIM_RLUT_SetPixelIndex(int x, int y, int Index)
{

}

int  LCDSIM_RLUT_GetPixelIndex(int x, int y)
{
	return 0;
}

//���һ������...
//#define FASTTING	1	//�����Ƿ����, ʹ�ô�����, ע����ֵ������...
void  LCDSIM_FillRect(int x0, int y0, int x1, int y1, int Index)
{
	int step = 1, color = 0;
	int line  = 0, comlum = 0;
	char* lptemp = 0;
	if(BPP == 0)	return;
	if(x1 > XSize)	x1 = XSize - 1;
	if(y1 > YSize)	y1 = YSize - 1;
	if(x0 < 0)	x1 = 1;
	if(y1 < 0)	y1 = 1;
	if(paaPixel == 0)	return;
	if(BPP > 8){
		step = 4;
		color = Convert_Index16IntoIndex32(Index);
	}
	else{
		color = Index;
	}
	comlum = (x1 - x0);
	lptemp = (char*)paaPixel + BytesPerLine * y0 + x0 * step;

	//���ο���Ƿ�Ϊ����...
#ifdef FASTTING
	if(comlum == XSize){
		comlum = comlum*(y1-y0);
		_asm{
			mov eax, color
			mov ecx, comlum
			mov edi, lptemp
		}
		if(step == 1) __asm rep stosb
		else __asm rep stosd
		return;
	}
#endif

// 2005-8-27 14:35:38 ʹ�ô�����ָ������ٶ�...
	for(line = y0; line < (y1 - y0); line++){
#ifdef FASTTING
		_asm{
			mov eax, color
			mov ecx, comlum
			mov edi, lptemp
		}
		if(step == 1) __asm rep stosb
		else __asm rep stosd
		lptemp += BytesPerLine; 
#else
		// 2005-8-27 14:54:02
		for(comlum = x0; comlum < (x1 - x0); comlum++){
			lptemp = (char*)paaPixel + BytesPerLine * line + comlum * step;
			if(step == 4) *((int*)lptemp) = color;
			else *lptemp = color;
		}
#endif
	}
}

int   LCDSIM_GetModifyCnt(void)
{
//	return ModifyCnt;
	return *((char*)pFix + 0x3c);	//��ucgui-view�ĳ�����, Ҫ��ӳ������ȡ����,һ��Ҫע����һ��,
									//��ģ�������������...
}

int   LCDSIM_GetModifyCntInfo(void)
{
//	return LUT_ModifyCnt;
	return *((char*)pFix + 0x40);	//��ucgui-view�ĳ�����, Ҫ��ӳ������ȡ����,һ��Ҫע����һ��,

}


///////////////////////////////////////////////////////////////////////
//
// ������       : LCDSIM_CheckInit
// ��������     : ����Ƿ���Ҫ���³�ʼ��ӳ��, ���й�LCD��ʾ����Ҫ�õ���һЩ��ϢXSize/bpp��...
//				: ���ڹص�һ��GUIӦ�ú�. ���¿�һ��GUIӦ��ʱ, ��UCGUIVIEW�����Ƿ�Ҫ���³�ʼ��...
// ����         : HWND hwnd
// ����ֵ       : void 
//
///////////////////////////////////////////////////////////////////////
void LCDSIM_CheckInit(HWND hwnd)
{
	/* 2005-6-5 12:22:11
	XSize = LCD_GetXSize() * LCD_GetXMag();
	YSize = LCD_GetYSize() * LCD_GetYMag();
	VXSize = LCD_GetVXSize();
	VYSize = LCD_GetVYSize();
	NumColors = LCD_GetNumColors();*/
	int		ret = 0;
	int*	lptemp = 0;
	int		newBPP = BPP, newVXSize = VXSize, newVYSize = VYSize, newXSize = XSize, newYSize = YSize;
	unsigned int newFixedPalette = FixedPalette;
	if(pSMemFix !=0){
		lptemp =(int*)((char*)pSMemFix+0x20);		//pSMemFix+0x20��ʼ���δ�����¶���...
		newXSize = *lptemp++;
		newYSize = *lptemp++;
		newVXSize = *lptemp++;
		newVYSize = *lptemp++;
		newFixedPalette = *lptemp++;				//�˴��ݿ���...
		newBPP = *lptemp++;
		lptemp =(int*)((char*)pSMemFix+0x20);		//pSMemFix+0x20��ʼ���δ�����¶���...
		if(newXSize != XSize || newYSize != YSize || newBPP != BPP || newFixedPalette != FixedPalette){
			(char*)ret = LCDSIM_Init();
			if(ret !=0){
				MessageBox(hWndMain, (char*)ret, BRANDING_GetAppNameShort(), 0);
			}
			else{
				XSize = *lptemp++;
				YSize = *lptemp++;
				VXSize = *lptemp++;
				VYSize = *lptemp++;
				FixedPalette = *lptemp++;			//�˴��ݿ���...
				BPP = *lptemp++;
				wsprintf(LCDCaption, "LCD %d*%d %dbpp, FixedPalette %d", XSize, YSize, BPP, FixedPalette);
				SetWindowText(hwnd, (LPCSTR)LCDCaption);
			}
		}
		/* 2005-6-5 12:43:24
		*lptemp++ = NumColors;
		BPP = LCD_GetBitsPerPixel();
		FixedPalette = LCD_GetFixedPalette();*/
	}
}

void LCDSIM_Paint(HDC hDC)
{
//	LCDSIM_CheckInit();		//����Ƿ���Ҫ���³�ʼ��,�������ÿ�����ص�λ��,��LCD��ߵ�...
	LCDSIM_PaintAt(hDC, 0, 0);
}

void LCDSIM_PaintAt(HDC hDC, int x, int y)
{
	RECT RectDest, RectSrc;
	RectDest.left = 0;
	RectDest.top = 0;
	RectDest.right = XSize - 1;
	RectDest.bottom = YSize - 1;
	RectSrc = RectDest;
	RectSrc.left = x;
	RectSrc.right += x;
	RectSrc.top = y;
	RectSrc.bottom += y;
	LCDSIM_PaintEx(hDC, &RectDest, &RectSrc);
}


///////////////////////////////////////////////////////////////////////
//
// ������       : LCDSIM_PaintEx
// ��������     : ���ڴ�ӳ��������LCD��ʾ��Ļ�е�����(һ��λͼ)��ʾ����..
//				: ���Զ��ж��Ƿ���Ҫ������ʾ....
// ����         : HDC hDC
// ����         : LPRECT pRectDest
// ����         : LPRECT pRectSrc
// ����ֵ       : void 
//
///////////////////////////////////////////////////////////////////////
void LCDSIM_PaintEx(HDC hDC, LPRECT pRectDest, LPRECT pRectSrc)
{
	int i  = 0;
	int* lptemp = 0;
	int DestWidth = 0, DestHeight = 0;
	int SrcWidth = 0, SrcHeight = 0;

	if(BPP == 0)	return;
	lptemp = (int*)pBitmapInfo;
	if(lptemp == 0)	return;
	for(i = 0; i < 10; i++)
		*lptemp++ = 0;
	lptemp = (int*)pBitmapInfo;
	*lptemp++ = 0x28;
	*lptemp++ = XSize;
	_asm{
		mov eax, YSize;
		neg eax;
		push esi;
		lea esi, lptemp;
	//	mov [esi], eax;
		mov esi, lptemp;
		mov [esi], eax;
		pop esi;
	}
	lptemp++;
	*((short int*)lptemp)++ = 1;
	if(BPP > 8){
		*((short int*)lptemp)++ = 0x20;
	}
	else{
		*((short int*)lptemp)++ = 8;
	}
	*lptemp++ = 0;
	SetStretchBltMode(hDC, STRETCH_DELETESCANS);
	DestWidth = pRectDest->right - pRectDest->left;
	DestHeight = pRectDest->bottom - pRectDest->top;
	SrcWidth = pRectSrc->right - pRectSrc->left;
	SrcHeight = pRectSrc->top - pRectSrc->bottom;
	//��������ʾ��ʾλͼ...
	if((DestWidth != SrcWidth)|| (SrcHeight != SrcHeight)){
		StretchDIBits(hDC, pRectDest->left, pRectDest->top, DestWidth, SrcHeight, pRectSrc->left, pRectSrc->right, SrcWidth, SrcHeight+1, paaPixel, pBitmapInfo,0, 0x0CC0020);
	}
	else{
		SetDIBitsToDevice(hDC, pRectDest->left, pRectDest->top, DestWidth, DestHeight, pRectSrc->left, YSize - pRectSrc->bottom - 1, 0, YSize, paaPixel, pBitmapInfo, 0);
	}
}



/* 2005-5-30 23:56:22
int	SIM_SetMag(int simmag)
{
	int Oldsimmag = Mag;
	Mag = simmag;
	return Oldsimmag;
}

int	SIM_SetMag(int simmag)
{
	return timeGetTime()-timeStartup;
}*/
