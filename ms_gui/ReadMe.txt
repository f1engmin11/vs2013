Microchip Graphics Library
�ٷ���link:http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=2608&page=1&param=en532061
��������link: http://ww1.microchip.com/downloads/en/DeviceDoc/Microchip%20Graphics%20Library%20v1.4.zip
�������Ϊ��GUI LIB��PC�ϵĲ�����ֲ.
PIC_Graphics ΪPIC GUI��Դ����Ŀ¼
Demo         ΪPIC GUI ���Դ���Ŀ¼
�����Ĵ����Ŀ¼����uCGUI ԭ������ģ��һ��LCD�Ĵ���(����http://www.ucgui.com/bbs/index.asp)
�漰��ֲ�Ĵ���
DisplayL.c��
void PutPixel(SHORT x, SHORT y) ��ָ����ɫ��ʾһ����
void  DelayMs( WORD time )      ��ʱ

Demo����Ҫһ������void  timerfunc(void)��λ�ø�Demo��tick
����LCDģ��Ĵ������п���ʱ���������øö�ʱ����ʵ��
emWin.c
line 407 :LRESULT CALLBACK WndProcLCD(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		case WM_TIMER:
            timerfunc();//����ˢ��LCD��ʾ�Ķ�ʱ�������PIC GUI�ļ�ʱ
��main.c��void GUI_TOUCH_GetState(LCD_tMouseState *state)�õ�������� �ú�����LCDSIM.C��ʵ��.

GUI_SIM.exeΪ�������ļ�,����ֱ�ۿ������GUI��Ч��.����Ȥ�Լ�����VC����:\Project\PICGUI.dsw

���˸о����GUI Lib�Ƚϼ�ª,��uCGUI��,������Ҳ��,�Ƚ�ֵ���о�һ�´���.����ʹ�����.���Լ�
�ٷ���˵����.

                              --liandao
                              --lycld@163.com
                              2008/08/30


GUI_TOUCH_GetState(&State);��ȡ����״̬������ֵ�����ذ�����Ϣ
GOLMsg(pMsg);���������ص���Ϣ�����obj�ؼ�����
SideButtonsMsg(pMsg);��ȡ��ߴ�����Ϣ��������Ϣ
GOLMsg(pMsg);���������ص���Ϣ�����obj�ؼ�����