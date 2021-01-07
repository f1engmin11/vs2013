Microchip Graphics Library
官方的link:http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=2608&page=1&param=en532061
代码下载link: http://ww1.microchip.com/downloads/en/DeviceDoc/Microchip%20Graphics%20Library%20v1.4.zip
本代码包为该GUI LIB在PC上的测试移植.
PIC_Graphics 为PIC GUI的源代码目录
Demo         为PIC GUI 测试代码目录
其他的代码和目录均是uCGUI 原来用于模拟一个LCD的代码(来自http://www.ucgui.com/bbs/index.asp)
涉及移植的代码
DisplayL.c中
void PutPixel(SHORT x, SHORT y) 以指定颜色显示一个点
void  DelayMs( WORD time )      延时

Demo中需要一个函数void  timerfunc(void)来位置该Demo的tick
由于LCD模拟的代码中有开定时器于是利用该定时器来实现
emWin.c
line 407 :LRESULT CALLBACK WndProcLCD(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		case WM_TIMER:
            timerfunc();//利用刷新LCD显示的定时器来完成PIC GUI的计时
在main.c中void GUI_TOUCH_GetState(LCD_tMouseState *state)得到鼠标数据 该函数在LCDSIM.C中实现.

GUI_SIM.exe为编译后的文件,可以直观看到这个GUI的效果.有兴趣自己编译VC工程:\Project\PICGUI.dsw

个人感觉这个GUI Lib比较简陋,比uCGUI简单,但代码也简单,比较值得研究一下代码.至于使用许可.你自己
官方得说明吧.

                              --liandao
                              --lycld@163.com
                              2008/08/30


GUI_TOUCH_GetState(&State);获取触摸状态和坐标值，返回按键消息
GOLMsg(pMsg);处理触摸返回的消息（逐个obj控件处理）
SideButtonsMsg(pMsg);获取侧边触摸信息，返回信息
GOLMsg(pMsg);处理触摸返回的消息（逐个obj控件处理）