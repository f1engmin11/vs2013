
/***************************************************************
  REVISION LOG ENTRY
  Revision By: ...
  Revised on 2005-05-30 20:25:27
  Comments: LCDSIM.c LCD模似显示屏...

  1. 修改函数LCDSIM_FillRect在BPP大于八位时的BUG, 并增加加束开关, 提高矩形直填充速度.
  2. 修改函数LCDSIM_SetPixelIndex, 在BPP在于八位时如果填充颜色索引与上一次相同, 则无须则进行索引到
     RGB颜色值的转换, 这样大大提高了在矩形填充及类似大面积填充相同颜色时的速度.
  3. 修正了LCDSIM_SetPixelIndex中记录填充象素的变量的存错误. 调色板的记录变量也如此, 已修正.

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
#define SPACEBEFORELCDM		0x400		//表示映象起始地址与LCD显示内存之间的空间...	

int		LCDSIM_aLCDColorBlack[LCDColorBlackNum];
int		LCDSIM_aLCDColorWhite[LCDColorWhiteNum] = {0xffffff, 0xffffff, 0xffffff, 0xffffff}; 
int		XSize = 0, YSize = 0, VYSize = 0, VXSize = 0;

LPVOID	pFix;				//申请的内存映象开始地址...
LPVOID	pSMemFix;			//申请的内存映象开始地址...
LPVOID	pSMemVar;			//申请的用于LCD显存的起始映象地址pSMemFix+0x1000[为SPACEBEFORELCDM]...
LPVOID	paaPixel;			//申请的用于LCD显存的起始映象地址...		
LPVOID	pBitmapInfo;		//映象中LCD中显示位图之调色板的映象起始地址pSMemFix+0x28...

DWORD	FixedPalette = 0;	//调色板最大可用的色彩数目...
int		BPP = 8;			//一个象素所占位数...
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
// 函数名       : SIM_SetTransColor
// 功能描述     : 设置透明色
// 参数         : int Color
// 返回值       : 
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
// 函数名       : SIM_SetLCDColorBlack
// 功能描述     : 设置背景色.共可设置四种.
// 参数         : unsigned int Index
// 参数         : int Color
// 返回值       : 
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
// 函数名       : SIM_SetLCDColorWhite
// 功能描述     : 设置前景色....
// 参数         : unsigned int Index
// 参数         : int Color
// 返回值       : 
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
// 函数名       : LCDSIM_Init
// 功能描述     : 此函数负责初始化LCD所使用的内存映象...其中一块于用存调色板.
//				: 一块用作显示屏幕..
// 参数         : void
// 返回值       : char*  
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
		lpCount = (XSize * YSize & 0xfffffff0)/4;	//调整为被4整除...
		lptemp = (int*) pSMemVar;
		for(i = 0; i < lpCount; i++)
		{
			*lptemp++ = 0;
		}
		lpret = (char*)lptemp;
		lpCount = XSize * YSize & 3;				//初始化不足球4个字节的内存...
		for(i  = 0; i < lpCount; i++)
		{
			*((char*)lpret)++ = 0;
		//	__asm mov lpret, 0;
		}
		lpret = 0;
		pBitmapInfo = (char*)pSMemFix + 0x100; 
		paaPixel = pSMemVar;
									
		// 2005-6-5 15:18:39
		lptemp =(int*)((char*)pSMemFix+0x20);			//pSMemFix+0x20开始依次存放以下东西...
		*lptemp++ = XSize;
		*lptemp++ = YSize;
		*lptemp++ = VXSize;
		*lptemp++ = VYSize;
		*lptemp++ = FixedPalette;						//此处暂空闲...
		*lptemp++ = BPP;
		*lptemp++ = NumColors;
		
		if(BPP <= 8){
			BytesPerLine = (XSize + 3) & 0xFFFFFFFC;	//一行多少个字节,BPP为8则一个字节表示一个象素
														//否则,用四个字表示一个象素,+3是表示LCD显示屏左边与底边有空...
			for(i = 0; i < NumColors; i++)				//此处初始化调色板...
			{
			//	LCDSIM_SetLUTEntry(i, LCD_L0_Index2Color(i));
			}
		}
		else{
			BytesPerLine = (XSize* 4 + 3)  & 0xFFFFFFFC;	//一行多少个字节,BPP为8则一个字节表示一个象素
		//	BytesPerLine = (XSize + 3) * 4 + 3;
		}
		return 0;
	}while(0);
	lpret = "Could not alloc Server data ...";
	return lpret;
}



///////////////////////////////////////////////////////////////////////
//
// 函数名       : InitSMem
// 功能描述     : 初始化LCD所使用的内存映象, 首先是打开, 必须要注意的是
//				: 一点是, 在UCGUI应用程序已经运行的情况下且模拟器也已经打开了,
//				: 如果些时关掉原来的UCGUI应用程序而再开一个新的, 那么将会进行一次
//				: 重新初始化, InitSMem会被再次调用, 此时无须再次映象...
// 参数         : void
// 返回值       : void  
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

//由上面的注释内容可以看出LCD_BITSPERPIXEL大于8时只有三种情况,444/555/565

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

//-FixedPalette表示RGB中,R与B对换...
//#include "LCD_Protected.h"

///////////////////////////////////////////////////////////////////////
//
// 函数名       : Convert_Index16IntoIndex32
// 功能描述     : 将颜色索引值转为RGB颜色值, 如FixedPalette为444时, 可显示
//				: 4096种颜色, 其索引值则为0-4095, 画象素函数中指定的不是RGB, 而
//				: 是此索引值, 写入到LCD显示内存先, 要转为FixedPalette对应RGB值.
//				: FixedPalette值不同, 转换也有小的差别的...
//				: gui\ConvertColor\下面的几个文件专门用于此的...
//				: 索引转换成RGB的基本原理------
// 参数         : int Index
// 返回值       : LCD_COLOR 
//
///////////////////////////////////////////////////////////////////////

// 索引转换成RGB的基本原理------首先, 对于RGB三值, 由FixedPalette指定其每个位所占的
// 位数, 如为444则各4位. FixedPalette中指定能表示的RGB的范围分别为(R,G,B)1111/1111/1111,
// 即16(R)/16(G)/16(B). 则当索引值Index为444时, 则是RGB各值索引最大的时候, 由此即可分析, 
// R,G,B三值要表示的颜色范围是0x00-0xff, 为了让有限的0-16的索引来表示0x00-0xff这些颜色
// 值, 那么当然只能是跳隔着来表示, 拿R来说, 最好是取值为(17*0, 17*1, 17*2, 17*3,.....17*15=248), 
// 如此的话, 才能差不多在有限的16个索引值下将0xff种颜色最大程度的表示到(比较平均)...

// 同理, 对于FixedPalette为555时, 则R取值最好为(8*0, 8*1, 8*2, 8*3,.....8*31=243)
// 同理, 对于FixedPalette为565时, 则B取值最好为(4*0, 4*1, 4*2, 4*3,.....4*63=252)
// 知道这一点, 则对于以下函数的理解就非常容易...
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
	case -444:	//无此项转换...
		break;
	}
// 2005-8-21 16:32:22
	r = (convertColor & 0xff) << 16;
	g = (convertColor & 0xff00);
	b = (convertColor & 0xff0000)>>16;
	convertColor = r | g | b;
	return convertColor;

/* 2005-6-4 12:33:06
	return INDEX2COLOR(Index);		//此一句可以底上前面的N句...
*/
}


///////////////////////////////////////////////////////////////////////
//
// 函数名       : LCDSIM_SetPixelIndex
// 功能描述     : 最基本的画点函数...
// 参数         : int x[点x坐标]
// 参数         : int y[点y坐标]
// 参数         : int Index[颜色索引值]
// 返回值       : void  
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
		if(curLUT != preLUT){	//改变了调色板时, 一定要重新根据索引转颜色...
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
	//映象是否成功申请...
	if(pFix != 0){
		lptemp = (char*)((char*)pFix + 0x3c);
		*((int*)lptemp) = ModifyCnt;
		// 2005-8-27 13:04:52, 以前错写一个字符进去, 应该写一INT进去...
		//*lptemp = ModifyCnt;
	}
}

//处理成只有0xff		0xff		0...0xff(256种)
//			0xff		0...0xff	0xff(256种)
//			0...0xff	0xff		0xff(256种)

///////////////////////////////////////////////////////////////////////
//
// 函数名       : LCDSIM_SetSubPixel
// 功能描述     : 此函数主要是为了处理RGB时, 二者为最强色, 另一着为0...0xff
//				: value取值为256种, 颜色值由x%3 << 3来决定, 即x的3倍的点都显示
//				: 同一种颜色(一条竖直线), 颜色值为Value | ~(0xff << (x%3 << 3))
// 参数         : int x
// 参数         : int y
// 参数         : U8 Value[0--255]
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
void LCDSIM_SetSubPixel(int x, int y, U8 Value)
{
	int pixPos = 0;
	char* lptemp = 0;

	if(BPP <= 0)	return;
	//(0xff << (x%3 << 3))求此反码...
	Value = Value | ~(0xff << (x%3 << 3));
	if(BPP > 8){
		pixPos = y * BytesPerLine + x * 4;
		(char*)paaPixel += pixPos;
		*(int*)paaPixel = Value;
	}
	ModifyCnt++;
	//映象是否成功申请...
	if(pFix != 0){
		lptemp = (char*)((char*)pFix + 0x3c);
		*lptemp = ModifyCnt;
	}
}

//计算一种颜色介于黑白之间的的对比度...

///////////////////////////////////////////////////////////////////////
//
// 函数名       : FilterColor
// 功能描述     : 这个函数是在设置调色板(LCDSIM_SetLUTEntry)时用到的, 主要用于
//				: 用8位及8位以下来表示一个象素点时. 要求它对应颜色值, 只须求
//				: 其于黑白之间一个比例值, 如果白为0xff, 黑为0x0,那么color本身就为颜色值,
//				: 如果白为0x80, 黑为0x10, 那么[color]_R=(0x80-0x10)*color/255, 对于B,G原理相同
//				: 但是, 在调用LCDSIM_SetLUTEntry(i, LCD_L0_Index2Color(i))这个来看, color其
//				: 实是一个RGB颜色, 对FixedPalette为111时, 调色板中只须要八种颜色, 不须要256个
//				: 选项, 八种为(0x000000[0], 0x0000ff[1], 0x00ff00[2], 0x00ffff[3], 0xff0000[4], 0xff00ff[5], 0xffff00[6], 0xffffff[7] )
//				: 即调色板索引(0-7, 8-15, 16-24, ....248-255)对于FixedPalette为111来说是重复的...
//				: 所以初始化调色板时, 要先调用LCD_L0_Index2Color(i)将Index转为颜色, 再与黑白成比
//				: 例综合. 这样了就可对负所有实际使用调色板为2项,4项,8项,16项,256项的一个字节表示点的所有
//				: 用到调色析的情况.
// 参数         : LCD_COLOR color[RGB颜色值]
// 参数         : LCD_COLOR colorBlack
// 参数         : LCD_COLOR colorWhite
// 返回值       : LCD_COLOR 
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
// 函数名       : LCDSIM_SetLUTEntry
// 功能描述     : 初始化象素点为8位及以下时, 要用到调色板来显示位图的情况.调色板为256项.
//				: 只有真正用到8位表示一个点时才用满调色板.其余情况下,调色析值有重复出现.
// 参数         : U8 Pos
// 参数         : LCD_COLOR color
// 返回值       : void  
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
// 函数名       : LCDSIM_Index2Color
// 功能描述     : 索引转化成RGB颜色, 分象素点占8位(及8位以下)和大于8位的情况
//				: 在UCGUI官方提供的转换函数中有BUG(大于8位时), 但由于此函数一般
//				: 情况下只被8位的情况下调用, 所以不会导致出错.因为首先调用的是LCDSIM_GetPixelColor.
//				: 对于8的情况, 根据索引直接在调色板去取, 大于8位的则要进行一个转换.
// 参数         : int Index
// 返回值       : int 
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
// 函数名       : LCDSIM_GetPixelColor
// 功能描述     : 取指定象素点颜色, 
// 参数         : int x
// 参数         : int y
// 返回值       : int 
//
///////////////////////////////////////////////////////////////////////
int LCDSIM_GetPixelColor(int x, int y)
{
	int Color = 0;
	if(paaPixel == 0)	return Color;
	if(BPP > 8){
		Color = *((int*)paaPixel + y * BytesPerLine + x * 4);		//直接在显示内存中取, 其值即为该点RGB值.
	}
	else if(BPP <= 8){
		Color = *((char*)paaPixel + BytesPerLine * y + x * 4);
		Color = LCDSIM_Index2Color(LCDSIM_GetPixelIndex(x, y));
	}
	return Color;
}

//此函数有两种实现方法


///////////////////////////////////////////////////////////////////////
//
// 函数名       : COLORREF2Index
// 功能描述     : Convert_Index16IntoIndex32的相反功能函数, 用于象素点大于8位的情况
//				: 下转换指定RGB值到Index...
// 参数         : LCD_COLOR color
// 返回值       : int 
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
	case -444:	//无此项转换...
		break;
	}
	return Index;*/
/* 2005-6-4 12:33:06
	return COLOR2INDEX(color);		//此一句可以底上前面的N句...
*/
}


///////////////////////////////////////////////////////////////////////
//
// 函数名       : LCDSIM_GetPixelIndex
// 功能描述     : 获取指定象素点颜色索引.
// 参数         : int x
// 参数         : int y
// 返回值       : int  
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
//MOSE消息结构...
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
// 函数名       : NotifyMouseState
// 功能描述     : 传递消息进GUI内部, 驱动GUI内部的事件机制...
// 参数         : LCD_tMouseState mouseState
// 返回值       : void 
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
// 函数名       : LCDSIM_CheckMouseState
// 功能描述     : 在lcd窗口函数中的定时器中调用, 用于随时响应外部MOUSE事件,
//				: 如用MOUSE按下左键拖住对话框标题移动...
// 参数         : void
// 返回值       : void 
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

//填充一个矩形...
//#define FASTTING	1	//定义是否加速, 使用串传送, 注意移值性问题...
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

	//矩形宽度是否为整行...
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

// 2005-8-27 14:35:38 使用串传送指令提高速度...
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
	return *((char*)pFix + 0x3c);	//在ucgui-view的程序当中, 要从映象中来取数据,一定要注意这一点,
									//与模拟器是有区别的...
}

int   LCDSIM_GetModifyCntInfo(void)
{
//	return LUT_ModifyCnt;
	return *((char*)pFix + 0x40);	//在ucgui-view的程序当中, 要从映象中来取数据,一定要注意这一点,

}


///////////////////////////////////////////////////////////////////////
//
// 函数名       : LCDSIM_CheckInit
// 功能描述     : 检测是否须要重新初始化映象, 及有关LCD显示当中要用到的一些信息XSize/bpp等...
//				: 对于关掉一个GUI应用后. 重新开一个GUI应用时, 则UCGUIVIEW会检测是否要重新初始化...
// 参数         : HWND hwnd
// 返回值       : void 
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
		lptemp =(int*)((char*)pSMemFix+0x20);		//pSMemFix+0x20开始依次存放以下东西...
		newXSize = *lptemp++;
		newYSize = *lptemp++;
		newVXSize = *lptemp++;
		newVYSize = *lptemp++;
		newFixedPalette = *lptemp++;				//此处暂空闲...
		newBPP = *lptemp++;
		lptemp =(int*)((char*)pSMemFix+0x20);		//pSMemFix+0x20开始依次存放以下东西...
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
				FixedPalette = *lptemp++;			//此处暂空闲...
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
//	LCDSIM_CheckInit();		//检查是否需要重新初始化,如更改了每个象素的位数,或LCD宽高等...
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
// 函数名       : LCDSIM_PaintEx
// 功能描述     : 将内存映象中用作LCD显示屏幕中的数据(一幅位图)显示出来..
//				: 会自动判断是否须要拉伸显示....
// 参数         : HDC hDC
// 参数         : LPRECT pRectDest
// 参数         : LPRECT pRectSrc
// 返回值       : void 
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
	//按比例显示显示位图...
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
