/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "Common.h"


/* 2005-6-4 13:24:23
volatile int OS_TimeMS;
*/
static	HANDLE hMutex;
int		EntranceCnt = 0;

/*********************************************************************
*
*      Timing:
*                 GUI_X_GetTime()
*                 GUI_X_Delay(int)

  Some timing dependent routines of emWin require a GetTime
  and delay function. Default time unit (tick), normally is
  1 ms.
*/

int GUI_X_GetTime(void)
{ 
//  return OS_TimeMS; 
  return SIM_GetTime();
}


void GUI_X_Delay(int ms)
{ 
/* 2005-6-4 13:22:39
  int tEnd = OS_TimeMS + ms;
  while ((tEnd - OS_TimeMS) > 0);*/
	SIM_Delay(ms);
}

/*********************************************************************
*
*       GUI_X_Init()
*
* Note:
*     GUI_X_Init() is called from GUI_Init is a possibility to init
*     some hardware which needs to be up and running before the GUI.
*     If not required, leave this routine blank.
*/

void GUI_X_Init(void) {}


/*********************************************************************
*
*       GUI_X_ExecIdle
*
* Note:
*  Called if WM is in idle state
*/

void GUI_X_ExecIdle(void)
{
	SIM_ExecIdle();
}

/*********************************************************************
*
*      Logging: Publics referenced by upper layers
*
Note:
  These routines are needed only in higher debug levels.
*/
void GUI_X_Log(const char *s)
{
	SIM_Log(s);
	OutputDebugString(s);
}

void GUI_X_Warn(const char *s)
{
	SIM_Warn(s);
	OutputDebugString(s);
}

void GUI_X_ErrorOut(const char *s)
{
	SIM_ErrorOut(s);
	OutputDebugString(s);
}


/*********************************************************************
*
* ¦ÌC/GUI - Multitask inteface for Win32
*
**********************************************************************
The folling section consisting of 4 routines is used to make
¦ÌC/GUI thread safe with WIN32
*/

void GUI_X_InitOS(void)
{
	hMutex = CreateMutex(NULL, 0, "¦ÌC/GUISim - Mutex");
}

//unsigned int GUI_X_GetTaskId(void)
unsigned long GUI_X_GetTaskId(void)
{
	return GetCurrentThreadId();
}

void GUI_X_Lock(void)
{
	WaitForSingleObject(hMutex, INFINITE);
	EntranceCnt++;
	if(EntranceCnt <= 1)	return;
	SIM_ErrorOut("Error in GUITASK.c module ...");
}

void GUI_X_Unlock(void)
{
	EntranceCnt--;
	ReleaseMutex(hMutex);
}
