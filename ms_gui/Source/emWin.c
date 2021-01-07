// Date		:	2005-05-28
//
// Author	:	HOUHH
//
// File		:	emWin.c,the main file for the emWin Simulation...
//
// Version	:	ver1.0.0

#include "stdafx.h"
#include "resource.h"
#include "Branding.h"

#include "Common.h"

typedef struct tagkeystruc{
	RECT keyarea;
	int keystate;
	SIM_HARDKEY_CB* keyrpoc;
	int keymode;
}KeyStruc, lpKeyStruc;

typedef struct tagkeydata{
	int VirCode;	//键盘虚拟码
	int key;		//对应在UCGUI当中的键值...
}KeyData, lpKey;

typedef struct tagLCD{
	HWND	lcd_hwnd;
	int		TimerID;
}LCDScreen;


#define DELAYEVERYTIME		10		//每次延时的时间(sleep())...
#define MAX_LOADSTRING		100
#define TOTAL_TASK			4		//支持多任务时的多任务数...
#define MAXKEY				80		//按键的个数...
#define MAXLCDNUM			16		//最大可打开LCD的个数...
#define VKEYNUM				11		//常用功能键数目,如VK_SHIFT等...
#define MAXLOGLEN			1024*12	//常用功能键数目,如VK_SHIFT等...


//以下为用到到部分全局变量...
int		LCDWidth = 0, LCDHeight = 0;
int		LCDWidth2 = 0, LCDHeight2 = 0;
int		wndWidth = 400, wndHeight = 300;
int		LOG_y0 = 0;								//LOG显示窗口纵坐标...
int		xPosLCD = -1, yPosLCD = -1, Mag = 1;
int		MessageBoxOnError = 1;
int		LOG_xsize = 0x1F4, LOG_ysize = 0x3C;
DWORD	timeStartup = 0;
char	LCDCaption[200];
char	LCDCaption2[200];

enum	SimStausEnum {terminated=0, started, suspended, paused, notrunning, continued, notstopped};
char*	SimStatusString[] = {"Application suspended",
					"Application started",
					"Application terminated.",
					"Application paused already...",
					"Application is not running...",
					"Application continued",
					"Application is not stopped..."};

enum	SimClassNameEnum {SimulationMainframe=0, LCDInfo, LCD, LCD1, emWinDevice, emWinHardkey, Log};
char*	SimClassName[]	= {"emWin Simulation Mainframe",
					"emWin LCD Info",
					"emWin LCD",
					"emWin LCD1",
					"emWinDevice",
					"emWinHardkey",
					"Log"};

char*	SimStatus[] = {"(Suspended)", "(Executing)", "(Terminated)"};
char	SimErrorFile[] = {"SimError.log"};
HANDLE	hLogo = 0, ahBmpDevice = 0, ahBmpDevice2 = 0;
HMENU	hMenuPopup = 0;									//右键单出控制开启与暂停菜单...
HWND	hWndMain = 0, hWndLCD1 = 0, LOG_hWnd = 0;
//motion log
HWND	hWnd_MotionLog = 0;
//main task log
HWND	hWnd_MainTaskLog = 0;
//Motion Data Disp
HWND    HWnd_MotionDisp = 0;

DWORD	rgbTransparent = 0;
//HWND	LCD_aWinData
int		CmdKill = 0;
HANDLE	ghThread = 0;
int		SuspendCount = 0;
int		NumTask = 0;
HANDLE	ahThread[TOTAL_TASK];
DWORD	aThreadID[TOTAL_TASK];
int		KeyBuffer = 0;
HANDLE	SimFileMap = 0;

char	LOG_acBuffer[MAXLOGLEN];

//Motion Log Buffer   缓冲
char	MotionLog_Buffer[MAXLOGLEN];

char	MainTaskLog_Buffer[MAXLOGLEN];

KeyStruc	aHardkey[MAXKEY];
int			NumHardkeys = 0;
LCDScreen	LCD_aWinData[MAXLCDNUM];
KeyData		aVKey2Key[] = {{VK_UP, GUI_KEY_UP}, {VK_DOWN, GUI_KEY_DOWN},{VK_LEFT, GUI_KEY_LEFT}, \
							{VK_RIGHT, GUI_KEY_RIGHT}, {VK_HOME, GUI_KEY_HOME}, {VK_END, GUI_KEY_END}, {VK_SHIFT, GUI_KEY_SHIFT},\
							{VK_CONTROL, GUI_KEY_CONTROL},{VK_BACK, GUI_KEY_BACKSPACE},{VK_INSERT, GUI_KEY_INSERT},{VK_DELETE, GUI_KEY_DELETE}};
//VK_SHIFT

HINSTANCE	hInst;											// current instance
TCHAR	szTitle[MAX_LOADSTRING];							// The title bar text
TCHAR	szWindowClass[MAX_LOADSTRING];						// The title bar text

ATOM	RegisterClasses(HINSTANCE hInstance);
BOOL	InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcLCD(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void	MainWnd_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//void	MainWnd_Paint(HWND hwnd);
void	MainWnd_Paint(HWND hwnd, HDC hdc);
void	MainWnd_OnTimer(HWND hWnd);
void	CreateWndLCD();
int		FindFreeLCDIndex();
void	InitMenu(HMENU hMenu);
void	HandleKeyEvents(UINT message, WPARAM wParam);
//void	LOG_AddPlain(const char* log);
void LOG_AddPlain(const char* log,char * LogBuffer, HWND DispHWin);
void	LogTime();
void	LOG_Add(const char* log);
void	LOG_DelFirstLine(LOG_acBuffer);
void UpDataHALDisp(void);


// 2005-5-31 23:23:02
int		SIM_GetTime();
int		SIM_SetMag(int simmag);
void	SIM_Delay(int delaytime);
void	SIM_ExecIdle();

void	THREAD_Sleep(int delaytime);
void	THREAD_KillAll();
void	THREAD_StartApplication();
void	THREAD_ContinueApplication();
void	THREAD_StopApplication();
void	Thread();
void	CreateTask(void * pFunc);


//初始化之前并做清除...
int APIENTRY	WinMain_NoClean(HINSTANCE hInstance,
									HINSTANCE hPrevInstance,
									LPSTR     lpCmdLine,
									int       nCmdShow);

int APIENTRY	WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	int ret = 0, i = 0;
//	char curPath[255];
//SIM初始化...
	SIM_X_Init();

//初始化汉字库2005-6-12 23:53:15
/* 2005-06-17 9:07:46
	GetCurrentDirectory(255, curPath);
	for(i = strlen(curPath); i > 0; i--){
		if(curPath[i] == '\\'){
			curPath[i+1] = 0;
			break;
		}
	}
	strcat(curPath, "GUI\\GUISource\\Chinese\\hzk16");
	OpenHZ(curPath);*/
/*
	i = OpenHZ("..\\..\\GUI\\GUISource\\Chinese\\hzk16");
	if(i == -1){
		do{
			i = OpenHZ(curPath);
			if(i != -1)	break;
			sprintf(curPath, "%s\n加载汉字库hzk16出错!", curPath);;
			MessageBox(0, curPath, "error", 0);
		}while(0);
	}
*/
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EMWIN, szWindowClass, MAX_LOADSTRING);

	ret = WinMain_NoClean(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	if(hLogo != 0) DeleteObject(hLogo);	//加载所有用的位图与图标...o
	if(ahBmpDevice != 0) DeleteObject(ahBmpDevice);
	if(ahBmpDevice2 != 0)  DeleteObject(ahBmpDevice2);
	if(SimFileMap != 0)  CloseHandle(SimFileMap);
//	if(i != -1)	CloseHZ();
	return ret;
}




///////////////////////////////////////////////////////////////////////
//
// 函数名       : FUNCTION: MyRegisterClass
// 功能描述     :
// 返回值       : ATOM
//
//
///////////////////////////////////////////////////////////////////////

ATOM RegisterClasses(HINSTANCE hInstance)
{
	ATOM ret = 1;
	WNDCLASSEX wcex;

//模拟器主窗口类...
	//IDC_ARROW=7f00;BLACK_BRUSH
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProcMain;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_EMWIN;
	wcex.lpszClassName	= SimClassName[SimulationMainframe];
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);
	wcex.hIconSm		= 0;

	ret = ret | RegisterClassEx(&wcex);

//模拟器硬件设备模拟窗口类...
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)WndProcMain;
	wcex.lpszMenuName	= (LPCSTR)0;
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName	= SimClassName[emWinDevice];

	ret = ret | RegisterClassEx(&wcex);

//模拟器硬件按键模拟窗口类...
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)WndProcMain;
	wcex.lpszMenuName	= (LPCSTR)0;
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName	= SimClassName[emWinHardkey];
	ret = ret | RegisterClassEx(&wcex);

//模拟器显示屏模拟窗口类(LCD)...
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)WndProcLCD;
	wcex.lpszMenuName	= (LPCSTR)0;
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	ret = LCD;
	wcex.lpszClassName	= SimClassName[LCD];
	ret = ret | RegisterClassEx(&wcex);

//模拟器调色板信息显示窗口类(LCD Info)...
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)WndProcMain;
	wcex.lpszMenuName	= (LPCSTR)0;
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName	= SimClassName[LCDInfo];
	ret = ret | RegisterClassEx(&wcex);

//模拟器LOG信息显示窗口类(LOG)...
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)WndProcMain;
	wcex.lpszMenuName	= (LPCSTR)0;
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName	= SimClassName[Log];
	ret = ret | RegisterClassEx(&wcex);

	return ret;
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(SimClassName[SimulationMainframe], szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void InitMenu(HMENU hMenu)
{
	if(ghThread != 0)
	{
		EnableMenuItem(hMenu, IDM_NEWLCD, MF_ENABLED);
		if(SuspendCount != 0){
			EnableMenuItem(hMenu, IDM_PAUSEAPP, MF_DISABLED | MF_GRAYED);
			EnableMenuItem(hMenu, IDM_RESUMEAPP, MF_ENABLED);
		}else{
			EnableMenuItem(hMenu, IDM_PAUSEAPP, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_RESUMEAPP, MF_DISABLED | MF_GRAYED);
		}
	}else{
		EnableMenuItem(hMenu, IDM_RESUMEAPP, MF_DISABLED | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_PAUSEAPP, MF_DISABLED | MF_GRAYED);
	//	EnableMenuItem(hMenu, IDM_NEWLCD, MF_DISABLED | MF_GRAYED);
	}
}


LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	RECT rt;

	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	switch (message)
	{
		case WM_CREATE:
			SetTimer(hWnd, 0, 20, 0);
			break;
		case WM_TIMER:
		//	MainWnd_OnTimer(hWnd);
			break;
		case WM_INITMENU:
			InitMenu((HMENU)wParam);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
			HandleKeyEvents(message, wParam);
			break;
		case WM_COMMAND:
			MainWnd_Command(hWnd, message, wParam, lParam);
			break;
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case IDM_NEWLCD:
				CreateWndLCD();
				break;

			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			GetClientRect(hWnd, &rt);
			rt.top+= 70;
			rt.bottom+= 100;

			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			MainWnd_Paint(hWnd, hdc);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			KillTimer(hWnd, 0);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


///////////////////////////////////////////////////////////////////////
//
// 函数名       : WndProcLCD
// 功能描述     : LCD模拟屏幕消息处理函数, 重要一点是处理重画, 以及关于窗口
//				: 画面的更新,即隔多少秒要重画一次, 另外还有MOUSE消息处理...
// 参数         : HWND hWnd
// 参数         : UINT message
// 参数         : WPARAM wParam
// 参数         : LPARAM lParam
// 返回值       : LRESULT CALLBACK
//
///////////////////////////////////////////////////////////////////////
extern void  timerfunc(void);
LRESULT CALLBACK WndProcLCD(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int xPos = -1, yPos = -1;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	RECT rt;
	POINT TrackMenuPoint = {0,0};
	int i = 0;
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	switch (message)
	{
		case WM_CREATE:
			SetTimer(hWnd, FindFreeLCDIndex(), 20, 0);
			break;
		case WM_TIMER:
            timerfunc();//利用刷新LCD显示的定时器来完成PIC GUI的计时
			if(wParam <= 16){
				do{
					if(LCD_aWinData[wParam].TimerID == LCDSIM_GetModifyCnt())	break;
					InvalidateRect(hWnd, 0, 0);
					LCD_aWinData[wParam].TimerID = LCDSIM_GetModifyCnt();
					break;
				}while(0);
			}
			else{
			}
			LCDSIM_CheckMouseState();
			//if(ghThread == 0) PostQuitMessage(0);
			break;
		/* 2005-6-8 23:32:58
			#define WM_MOUSEFIRST   0x0200
			#define WM_MOUSEMOVE    0x0200
			#define WM_LBUTTONDOWN  0x0201
			#define WM_LBUTTONUP    0x0202
			cmp     eax, 1FFh		//此处处理所有三种MOUSE消息, 在反编译中容易看漏的, 如下所示...
			jbe     loc_55D4		//小于等于0x1ff跳往DefWindowProc处理...
			cmp     eax, 202h		//msg小于0x202则转往处理MOUSE消息...
			jbe     short loc_557E	*/
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			//此处判断MOUSE坐标时,一定注意所取的MOUSE坐标范围要翻倍...
			SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			if(xPos >= 0 &&  yPos >=0 && xPos <= LCD_GetDevCap(LCD_DEVCAP_XSIZE)*LCD_GetXMag() && yPos <= LCD_GetDevCap(LCD_DEVCAP_YSIZE)*LCD_GetYMag()){
				LCDSIM_SetMouseState(xPos, yPos, wParam);
			}else{
				LCDSIM_SetMouseState(-1, -1, 0);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			if(ps.fErase != 0){
				rt.left = 0;
				rt.top = 0;
				rt.right = 0xfff;
				rt.bottom = 0xfff;
				FillRect(hdc, &rt, GetStockObject(BLACK_BRUSH));
			}
			//InvalidateRect(hWnd,NULL, 0); //此句不可丢,否则无法显示出要画的位图, 改在定时器中刷新窗口....
		//	LCDSIM_CheckInit(hWnd);
			LCDSIM_Paint(hdc);
			EndPaint(hWnd, &ps);
			break;
		case WM_RBUTTONDOWN:;
			TrackMenuPoint.x = LOWORD(lParam);
			TrackMenuPoint.y = HIWORD(lParam);
			ClientToScreen(hWnd, &TrackMenuPoint);
			TrackPopupMenu(hMenuPopup, TPM_RIGHTBUTTON, TrackMenuPoint.x, TrackMenuPoint.y, 0, hWndMain, 0);
			break;
		case WM_DESTROY:
			for(i = 0; i < MAXLCDNUM; i++){
				if(hWnd == LCD_aWinData[i].lcd_hwnd){
					LCD_aWinData[i].lcd_hwnd = 0;
					LCD_aWinData[i].TimerID = 0;
					break;
				}
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

LRESULT CALLBACK LogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
//	char buf[500];
//	HWND log_edit = NULL;
	RECT rt;

	switch (message)
	{
		case WM_INITDIALOG:
			{
			//	SetWindowPos(hDlg, 0, 0, LOG_y0, 50, wndWidth, 0);
				GetWindowRect(hWndMain, &rt);
				//MoveWindow(hDlg, rt.left, rt.top+LOG_y0, wndWidth, 150, 1);
                MoveWindow(hDlg, rt.left, rt.bottom-10, wndWidth, 150, 1);
				LOG_hWnd = GetDlgItem(hDlg, IDC_EDITLOG);
				GetWindowRect(hDlg, &rt);
				MoveWindow(LOG_hWnd, 0, 0, rt.right - rt.left-5, rt.bottom - rt.top-30, 1);
				if(LOG_hWnd)	SetWindowText(LOG_hWnd, (LPCTSTR)LOG_acBuffer);
				return TRUE;
			}

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
		case WM_SIZE:
			GetWindowRect(hDlg, &rt);
			MoveWindow(LOG_hWnd, 0, 0, rt.right - rt.left-5, rt.bottom - rt.top-30, 1);
			break;
		case WM_DESTROY:
			LOG_hWnd  = 0;
	}
    return FALSE;
}
#define MotionLogWin_YSIZE 150
//显示Motion 的Debug 内容
LRESULT CALLBACK MotionLogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    RECT rt;

    switch (message)
    {
    case WM_INITDIALOG:
        {
            //ShowWindow(hDlg,SW_SHOW);
            //	SetWindowPos(hDlg, 0, 0, LOG_y0, 50, wndWidth, 0);
            GetWindowRect(hWndMain, &rt);
            //MoveWindow(hDlg, rt.left, rt.top+LOG_y0, wndWidth, 150, 1);
            MoveWindow(hDlg, rt.left, rt.bottom, wndWidth, MotionLogWin_YSIZE, 1);//窗口位置
            hWnd_MotionLog = GetDlgItem(hDlg, IDC_MotionLog);
            GetWindowRect(hDlg, &rt);
            MoveWindow(hWnd_MotionLog, 0, 0, rt.right - rt.left-5, rt.bottom - rt.top-30, 1); //内部打印窗口位置
            if(hWnd_MotionLog)	SetWindowText(hWnd_MotionLog, (LPCTSTR)"Motion Debug Log");
            return TRUE;
        }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    case WM_SIZE:
        GetWindowRect(hDlg, &rt);
        MoveWindow(hWnd_MotionLog, 0, 0, rt.right - rt.left-5, rt.bottom - rt.top-30, 1);
        break;
    case WM_DESTROY:
        hWnd_MotionLog  = 0;
    }
    return FALSE;
}
//显示MainTask 的Debug 内容
LRESULT CALLBACK MainTaskLogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    RECT rt;

    switch (message)
    {
    case WM_INITDIALOG:
        {
            //ShowWindow(hDlg,SW_SHOW);
            //	SetWindowPos(hDlg, 0, 0, LOG_y0, 50, wndWidth, 0);
            GetWindowRect(hWnd_MotionLog, &rt);
            //MoveWindow(hDlg, rt.left, rt.top+LOG_y0, wndWidth, 150, 1);
            MoveWindow(hDlg, rt.left, rt.bottom, wndWidth, MotionLogWin_YSIZE, 1);//窗口位置偏置于MotionLogProc
            hWnd_MainTaskLog = GetDlgItem(hDlg, IDC_MainTaskLog);
            GetWindowRect(hDlg, &rt);
            MoveWindow(hWnd_MainTaskLog, 0, 0, rt.right - rt.left-5, rt.bottom - rt.top-30, 1); //内部打印窗口位置
            if(hWnd_MainTaskLog)
            {
                //SendMessage(hWnd_MainTaskLog,LB_ADDSTRING,0,(LONG)"MainTask  Debug Log");//SetWindowText(DispHWin, (LPCTSTR)LogBuffer);
                SetWindowText(hWnd_MainTaskLog, (LPCTSTR)"MainTask  Debug Log");
            }
            return TRUE;
        }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    case WM_SIZE:
        GetWindowRect(hDlg, &rt);
        MoveWindow(hWnd_MainTaskLog, 0, 0, rt.right - rt.left-5, rt.bottom - rt.top-30, 1);
        break;
    case WM_DESTROY:
        hWnd_MainTaskLog  = 0;
    }
    return FALSE;
}
#define HALSHMDispWin_XSize  200

#define HALSHMDispWin_YSize  300

LRESULT CALLBACK HALSHMDispProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    RECT rt;

    switch (message)
    {
    case WM_INITDIALOG:
        {
            //ShowWindow(hDlg,SW_SHOW);
            //	SetWindowPos(hDlg, 0, 0, LOG_y0, 50, wndWidth, 0);
            GetWindowRect(hWndMain, &rt);
            //MoveWindow(hDlg, rt.left, rt.top+LOG_y0, wndWidth, 150, 1);
            MoveWindow(hDlg, rt.right, rt.top, HALSHMDispWin_XSize, rt.bottom-rt.top, 1);
            HWnd_MotionDisp = GetDlgItem(hDlg, IDC_MotionDisp);
            GetWindowRect(hDlg, &rt);
            MoveWindow(HWnd_MotionDisp, 0, 0, rt.right - rt.left-5, rt.bottom - rt.top-30, 1);
            if(HWnd_MotionDisp)
            {
                //SendMessage(hWnd_MainTaskLog,LB_ADDSTRING,0,(LONG)"MainTask  Debug Log");
                SetWindowText(HWnd_MotionDisp, (LPCTSTR)"Moton HAL Data Disp");
            }
            //SetTimer(hDlg, 1, 2000, NULL);
            return TRUE;
        }
//    case WM_TIMER:
        //        UpDataHALDisp();
        //        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    case WM_SIZE:
        GetWindowRect(hDlg, &rt);
        //MoveWindow(hWnd_MainTaskLog, 0, 0, rt.right - rt.left-5, rt.bottom - rt.top-30, 1);
        break;
    case WM_DESTROY:
        HWnd_MotionDisp  = 0;
        break;
    }
    return FALSE;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char buf[500];
	switch (message)
	{
		case WM_INITDIALOG:
			{
				sprintf(buf, "About %s", BRANDING_GetAppNameLong());
				SetWindowText(hDlg, buf);
				SetDlgItemText(hDlg, ID_ABOUT_APPC, BRANDING_GetAppNameShort());
				//SetDlgItemText(hDlg, ID_ABOUT_VERSION, GUI_GetVersionString());
				SetDlgItemText(hDlg, ID_ABOUT_COPYRIGHT, BRANDING_GetCopyright());
				return TRUE;
			}

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


int VirtKey2Key(int nVirtKey)
{
	int key = 0, i  = 0;
	for(i  = 0; i < VKEYNUM; i++)
	{
		if(aVKey2Key[i].VirCode == nVirtKey){
			return aVKey2Key[i].key;
		}
	}
	return key;
}

int Keydown2ASCII(int nVirtKey)
{
	int key = 0, ret = -1;
	char allKeyState[258];
	GetKeyboardState(allKeyState);
	ret = ToAscii(nVirtKey, MapVirtualKey(nVirtKey, 0), allKeyState, (LPWORD)&key, 0);
	if(ret == 1) return key;
	return key | nVirtKey;
}
int _KEY_;
int _keyCount_ = 0;
//VK_LEFT VK_SHIFT
void HandleKeyEvents(UINT message, WPARAM wParam)
{
	int key = 0, keyCount = 0;
	switch(message){
	case WM_KEYDOWN:
		key = VirtKey2Key(wParam);
		if(key == 0) key = Keydown2ASCII(wParam);
		keyCount = 1;
		break;
	case WM_KEYUP:
		key = VirtKey2Key(wParam);
		keyCount = 0;
		break;
	}
	if(key != 0){
        //GUI_StoreKeyMsg(key, keyCount);
        _KEY_ = key;
        _keyCount_ = keyCount;
    }

}
int GUI_GetKey(void)
{
    if(0 != _keyCount_){
        _keyCount_ = 0;
        return _KEY_;        
    }else{
        return 0;
    }
}

void MainWnd_OnTimer(HWND hWnd)
{
	char MainWndTitle[200] = "(Executing)";
	static char newMainWndTitle[200];
	static int hasgetCaption = 0;
	if(hasgetCaption++ == 0){
		GetWindowText(hWnd, newMainWndTitle, 100);
	}
	sprintf(MainWndTitle, "%s %s", newMainWndTitle, "(Executing)");
	SetWindowText(hWnd, MainWndTitle);
}

//WM_PAINT wParam在菜单消息当中即为菜单标志ID..
void MainWnd_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = 0, wmEvent = 0;
    HWND hdlg;
    long erro=0;
	switch(message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_NEWLCD:
			CreateWndLCD();
			break;
		case IDM_LOG:
			CreateDialogParam(hInst, (LPCTSTR)IDD_LOG, hWnd, (DLGPROC)LogProc, 0);
			break;
        case IDM_MotionLog:
            hdlg = CreateDialog(hInst, (LPCTSTR)IDD_MotionLog, hWnd, (DLGPROC)MotionLogProc);
            erro = GetLastError();
            break;
        case IDM_MainTaskLog:
            hdlg = CreateDialog(hInst, (LPCTSTR)IDD_MainTaskLog, hWnd, (DLGPROC)MainTaskLogProc);
            erro = GetLastError();
            break;
		case IDM_PAUSEAPP:
			THREAD_StopApplication();
			InitMenu(hMenuPopup);
			break;
		case IDM_RESUMEAPP:
			THREAD_ContinueApplication();
			InitMenu(hMenuPopup);
			break;
		case IDM_COPYCLIP:
			break;
		case 32774:
			THREAD_StartApplication();
			break;
		default:
			break;
		}
	}
}


///////////////////////////////////////////////////////////////////////
//
// 函数名       : MainWnd_Paint
// 功能描述     : 在模拟器主窗口当中画MICRIUM的标志[画于窗口中心处]...
// 参数         : HWND hwnd
// 返回值       : void
//
///////////////////////////////////////////////////////////////////////
void MainWnd_Paint(HWND hwnd, HDC hdc)
{
/* 2005-6-7 1:45:24
	HDC			hdc, hMemdc;
	PAINTSTRUCT	ps;*/
	HDC			hMemdc;
	RECT		rt;

	int			logo_x = 0, logo_y = 0;
	BITMAP		logobmpinfo;
	int			blt = 0;
	HBITMAP		logoBmp = 0;

/* 2005-6-7 1:45:29
	InvalidateRect(hwnd,NULL,FALSE);		//此句不可丢,否则无法显示出要画的位图...
	hdc = BeginPaint(hwnd, &ps);*/
	hMemdc = CreateCompatibleDC(hdc);
	GetClientRect(hwnd, &rt);
	logoBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LOGO));
	GetObject(logoBmp, 0x18, &logobmpinfo);
	logo_x = (rt.right - logobmpinfo.bmWidth - rt.left)/2;
	logo_y = (rt.bottom - logobmpinfo.bmHeight - rt.top)/2;
	SelectObject(hMemdc, hLogo);
	blt = BitBlt(hdc, logo_x, logo_y, logobmpinfo.bmWidth, logobmpinfo.bmHeight, hMemdc, 0, 0, SRCCOPY);
	DeleteObject(logoBmp);
	DeleteDC(hMemdc);
//	EndPaint(hwnd, &ps);
}

#include "mmsystem.h"
void main(void);

int APIENTRY	WinMain_NoClean(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	int ret = 0;
	HWND hWnd;
	MSG msg;
	HACCEL hAccelTable;

	MessageBoxOnError = 0;			//错误消息不弹出对话框,只记入LOG文档...
//	timeStartup = timeGetTime();
	RegisterClasses(hInstance);		//注册所有窗口要用到的窗口类...

	hLogo = LoadImage(hInstance, (LPCTSTR )IDB_LOGO, 0, 0, 0, 0);	//加载所有用的位图与图标...
	ahBmpDevice = LoadImage(hInstance, (LPCTSTR )IDB_DEVICE, 0, 0, 0, 0);
	if(ahBmpDevice == 0) ahBmpDevice = LoadImage(hInstance, (LPCTSTR )"Device.bmp", 0, 0, 0, 0);
	ahBmpDevice2 = LoadImage(hInstance, (LPCTSTR )IDB_DEVICE2, 0, 0, 0, 0);
	if(ahBmpDevice2 == 0) ahBmpDevice2 = LoadImage(hInstance, (LPCTSTR )"Device1.bmp", 0, 0, 0, 0);
	hMenuPopup = LoadMenu(hInstance, (LPCSTR)IDC_POPEMWIN);
	hMenuPopup = GetSubMenu(hMenuPopup, 0);

	LOG_y0 = LCD_GetYSize() * LCD_GetYMag() + 0x1E;
	wndHeight = LOG_y0 - 0x1e + 0x5C;
	wndWidth =  LCD_GetXSize() * LCD_GetXMag() + 0x13;

	hInst = hInstance; // Store instance handle in our global variable
	hWnd = CreateWindowEx(0, SimClassName[SimulationMainframe], szTitle, 0x2CF0000,
		CW_USEDEFAULT, 0, wndWidth, wndHeight, NULL, NULL, hInstance, NULL);

/* 2005-5-29 1:20:56
   hWnd = CreateWindow(SimClassName[SimulationMainframe], szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
*/
	if (!hWnd)	return ret;
	hWndMain = hWnd;

	(char*)ret = LCDSIM_Init();
	if(ret !=0){
		MessageBox(hWnd, (char*)ret, BRANDING_GetAppNameShort(), 0);
		return 1;
	}
	if(LCD_GetNumDisplays() > 1){
	//	LCDSIM_1_Init();
	}

	if(!ret){
		CreateWndLCD();
	//	CreateDialogParam(hInst, (LPCTSTR)IDD_LOG, hWnd, (DLGPROC)LogProc, 0);
    	//CreateDialog(hInst, (LPCTSTR)IDD_MotionLog, hWnd, (DLGPROC)MotionLogProc);
        //CreateDialog(hInst, (LPCTSTR)IDD_MainTaskLog, hWnd, (DLGPROC)MainTaskLogProc);//启动就开始显示log 窗口
//        CreateDialog(hInst, (LPCTSTR)IDD_HALSHMDisp, hWnd, (DLGPROC)HALSHMDispProc);

	}

	THREAD_StartApplication();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_EMWIN);
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//TranslateAccelerator FCONTROL
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	THREAD_KillAll();
	return 0;
}

int	FindFreeLCDIndex()
{
	int i = 0;
	for(i  = 0; i < MAXLCDNUM; i++)
	{
		if(LCD_aWinData[i].lcd_hwnd == 0)	return i;
	}
	return -1;
}


///////////////////////////////////////////////////////////////////////
//
// 函数名       : CreateWndLCD
// 功能描述     : 创建一个模拟LCD窗口, 最大创建16个....
// 返回值       : void
//
///////////////////////////////////////////////////////////////////////
void CreateWndLCD()
{
	int		LCD_xPos = 0, LCD_yPos = 0;
	int		LCDIndex = 0;
	HWND	LCDHwnd = 0;
	memset(LCDCaption, 0, 200);
	memset(LCDCaption2, 0, 200);

	LCDIndex = FindFreeLCDIndex();
	if(LCDIndex >= 0){
		LCDWidth =  LCD_GetXSize() * LCD_GetXMag();
		LCDHeight = LCD_GetYSize() * LCD_GetYMag();
		wsprintf(LCDCaption, "LCD %d*%d %dbpp, FixedPalette %d", LCDWidth, LCDHeight, LCD_GetBitsPerPixel(), LCD_GetFixedPalette());
		LCDWidth += GetSystemMetrics(SM_CXFRAME) * 2;
		LCDHeight += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2;
		LCD_xPos = LCDIndex * 10;	//每一个新开的窗口X坐标间距为10象素...
		LCD_yPos = 0;

		LCDHwnd = CreateWindowEx(0, SimClassName[LCD], LCDCaption, 0x44CF0000,
			LCD_xPos, LCD_yPos, LCDWidth, LCDHeight, hWndMain, NULL, hInst, NULL);
		LCD_aWinData[LCDIndex].lcd_hwnd = LCDHwnd;
		LCD_aWinData[LCDIndex].TimerID = 0;
		ShowWindow(LCDHwnd, SW_SHOW);
	}
	else{
/*
		//创建Create lcd1...
		if(LCD_GetNumDisplays() <= 1)	return;
		LCDWidth2 =  LCD_GetXSize_1();
		LCDHeight2 = LCD_GetYSize_1();
		wsprintf(LCDCaption, "LCD %d*%d %dbpp, FixedPalette %d", LCDWidth, LCDHeight, LCD_GetBitsPerPixel_1(), LCD_GetFixedPalette_1());
		LCDWidth2 += GetSystemMetrics(SM_CXFRAME) * 2;
		LCDHeight2 += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2;
		LCD_xPos = LCDIndex * 10;	//每一个新开的窗口X坐标间距为10象素...
		if(LCD_xPos < 0)	LCD_xPos = 0;
		LCD_yPos = 0;

		LCDHwnd = CreateWindowEx(0, SimClassName[LCD1], LCDCaption2, 0x54CF0000,
			LCD_xPos, LCD_yPos, LCDWidth, LCDHeight, hWndMain, NULL, hInst, NULL);
*/
	}

//SM_ARRANGE LCD_GetNumDisplays

}

int	SIM_SetMag(int simmag)
{
	int Oldsimmag = Mag;
	Mag = simmag;
	return Oldsimmag;
}

int	SIM_GetTime()
{
//	return timeGetTime()-timeStartup;
}


void SIM_Delay(int delaytime)
{
	THREAD_Sleep(delaytime);
}


void SIM_ExecIdle()
{
	THREAD_Sleep(1);
}

void SIM_StoreKey(int key)
{
	if(KeyBuffer !=	0)	return;
	KeyBuffer = key;
}

int SIM_GetKey()
{
	char temp = KeyBuffer;
	if(KeyBuffer ==	0)	return 0;
	KeyBuffer = 0;
	return temp;
}

//返回已经开启的任务数...
void SIM_Start()
{
	int i = 0;
	if(NumTask > 0){
		while(i < NumTask){
			SetThreadPriority(ahThread[i++], 0xfffffffe);
			ResumeThread(ahThread[i-1]);
		}
	}
	for(;;)	Sleep(DELAYEVERYTIME);	//此处为开启多任务支持, 其所在线程即为MainTask函数所在线程,
									//为gui图形应用程序主线程...
}

void THREAD_Sleep(int delaytime)
{
	if(delaytime <= 0)	return;
CmdKill:
	if(CmdKill == 0){
		Sleep(DELAYEVERYTIME);
		if(delaytime--)	goto CmdKill;
		else return;
	}
	SetThreadPriority(ghThread, THREAD_PRIORITY_HIGHEST);	//THREAD_PRIORITY_HIGHEST
	ghThread = 0;
	ExitThread(0);
}

void THREAD_KillAll()
{
	int delaycount = 0;
	do{
		if(ghThread == 0)	break;
		CmdKill = 1;
		while(delaycount++ <= 14){
			Sleep(DELAYEVERYTIME);
			if(ghThread == 0)	break;
		}
		if(ghThread == 0)	break;
		TerminateThread(ghThread, 0);
	}while(0);
	ghThread = 0;
	CmdKill= 0;
}

void THREAD_StartApplication()
{
	OutputDebugString(SimStatusString[started]);
	if(ghThread != 0)	THREAD_KillAll();
	ghThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread, 0, 0, 0);
	SuspendCount = 0;
}

void Thread()
{
	main();
//	ghThread = 0;
	OutputDebugString(SimStatusString[terminated]);
//LogTime();
//LOG_Add();
}

void THREAD_StopApplication()
{
	int i  = 0;
	OutputDebugString(SimStatusString[suspended]);
	if(SuspendCount !=0){
		MessageBox(hWndMain, SimStatusString[paused], BRANDING_GetAppNameShort(), 0);
		return;
	}
	if(ghThread == 0){
		MessageBox(hWndMain, SimStatusString[notrunning], BRANDING_GetAppNameShort(), 0);
		return;
	}
	SuspendCount = SuspendThread(ghThread)+1;
	if(NumTask <= 0)	return;
	while(i < NumTask)	SuspendThread(ahThread[i++]);
//	while(NumTask)	SuspendThread(ahThread[NumTask--]);
}

void THREAD_ContinueApplication()
{
	int i = 0;
	OutputDebugString(SimStatusString[continued]);
	if(SuspendCount ==0){
		MessageBox(hWndMain, SimStatusString[notstopped], BRANDING_GetAppNameShort(), 0);
		return;
	}
	if(ghThread == 0){
		MessageBox(hWndMain, SimStatusString[notrunning], BRANDING_GetAppNameShort(), 0);
		return;
	}
	SuspendCount = ResumeThread(ghThread) - 1;
	if(NumTask <= 0)	return;
	while(i < NumTask)	ResumeThread(ahThread[i++]);
//	while(NumTask)	ResumeThread(ahThread[NumTask--]);
}

void	SIM_CreateTask(char * pName, void * pFunc, unsigned char Priority)
{
	//CREATE_SUSPENDED
//	void* lpfun = CreateTask;
	if(NumTask <= TOTAL_TASK-1){
		ahThread[NumTask-1] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CreateTask, (LPVOID)pFunc, CREATE_SUSPENDED, (LPDWORD)&aThreadID[NumTask++]);
	}
/* 2005-07-11 21:38:17
CreateTask:
	__asm jmp  [esp+4];*/
}
// 2005-07-11 21:35:03
void	CreateTask(void * pFunc)
{
	__asm add esp, 80;	//2005-07-11 21:35:03
						//由于编译器默认为每个函数分配0x40的栈空间
						//以及参数,EBP等,所以此处调用之后线程结束返回时返会地址不定,会出现异常.
						//帮调整其堆栈, 让其直接返回到调用CreateTask入口之前的栈状态..
						//即用手动堆栈以达到调用CreateTask后返回的效果.
	__asm jmp  pFunc;
}


int	SIM_HARDKEY_GetNum(void)
{
	return NumHardkeys;
}

int	SIM_HARDKEY_GetState(unsigned int i)
{
	if(i >= MAXKEY)	return 0;
	return aHardkey[i].keystate;
}

SIM_HARDKEY_CB*	SIM_HARDKEY_SetCallback(unsigned int KeyIndex, SIM_HARDKEY_CB* pfCallback)
{
	SIM_HARDKEY_CB* oldpfCallback = 0;
	if(KeyIndex >= MAXKEY)	return 0;
	oldpfCallback = aHardkey[KeyIndex].keyrpoc;
	aHardkey[KeyIndex].keyrpoc = pfCallback;
	return oldpfCallback;
}

int	SIM_HARDKEY_SetMode (unsigned int KeyIndex, int Mode)
{
	int oldekeymode = 0;
	if(KeyIndex >= MAXKEY)	return 0;
	oldekeymode = aHardkey[KeyIndex].keymode;
	aHardkey[KeyIndex].keymode = Mode;
	return oldekeymode;
}
int	SIM_HARDKEY_SetState(unsigned int KeyIndex, int State)
{
	int oldkeystate = 0;
	if(KeyIndex >= MAXKEY)	return 0;
	oldkeystate = aHardkey[KeyIndex].keystate;
	aHardkey[KeyIndex].keystate = State;
	return oldkeystate;
}


//日志LOG记录
void SIM_Log(const char *s)
{
	//OutputDebugString(s);
//	LogTime(s);
//	LOG_Add(s);
    LOG_AddPlain(s,LOG_acBuffer,LOG_hWnd);
}
/* 2005-6-4 13:33:32
void SIM_Log1(const char *s, int p0);
void SIM_Log2(const char *s, int p0, int p1);
void SIM_Log3(const char *s, int p0, int p1, int p2);
void SIM_Log4(const char *s, int p0, int p1, int p2,int p3);
*/
void SIM_Warn(const char *s)
{
	OutputDebugString(s);
	LogTime(s);
	LOG_Add(s);
}
/* 2005-6-4 13:33:42
void SIM_Warn1(const char *s, int p0);
void SIM_Warn2(const char *s, int p0, int p1);
void SIM_Warn3(const char *s, int p0, int p1, int p2);
void SIM_Warn4(const char *s, int p0, int p1, int p2, int p3);
*/
void SIM_ErrorOut(const char *s)
{
	OutputDebugString(s);
}
//打印到motion log 窗口
void MotionLogDisp(const char *s)
{
    LOG_AddPlain(s,MotionLog_Buffer,hWnd_MotionLog);
}

//打印到main task log 窗口
void MainTaskLogDisp(const char *s)
{
    LOG_AddPlain(s,MainTaskLog_Buffer,hWnd_MainTaskLog);
}
void LogTime()
{
	char buf[100];
//	sprintf(buf, "\n\r%d:   ", timeGetTime()-timeStartup);
//	OutputDebugString(buf);
	LOG_Add(buf);
}

void LOG_Add(const char* log)
{
	char buf[200];
	memset(buf, 0, 200);
	sprintf(buf, "%s \r\n", log);
//	LOG_AddPlain(buf,LOG_acBuffer,LOG_hWnd);
}

//缓冲区显示到指定的窗口上
//log 需要打印的字符串
//logBuffer 显示缓冲
//DispHWin 显示窗口的句柄
void LOG_AddPlain(const char* log,char * LogBuffer, HWND DispHWin)
{
	while( strlen(log) > (MAXLOGLEN - strlen(LogBuffer))){
			LOG_DelFirstLine(LogBuffer);
		}
	strcat(LogBuffer, log);

	if(DispHWin)
    {
        //SendMessage(DispHWin,LB_ADDSTRING,0,(LONG)log);//
        SetWindowText(DispHWin, (LPCTSTR)LogBuffer);
        //SendMessage(DispHWin,WM_VSCROLL,SB_BOTTOM,0);
    }
}
//删除缓冲区中的一行数据
void LOG_DelFirstLine(char* log)
{
	unsigned int i  = 0;
	if(log != NULL){
		for(i = 0; i < strlen(log); i++){
			if(log[i] == 0xa){
				if(i == strlen(log)-1){
					memset(log, 0, strlen(log));
				}
				else{
					memcpy(log, &log[i+1], strlen(log)-i-1);
					memset(&log[i+1], 0, i);
				}
				break;
			}
		}
	}
}
/*
void UpDataHALDisp(void)
{
    SetWindowText(HWnd_MotionDisp, (LPCTSTR)"Moton HAL Data Disp");
}*/
