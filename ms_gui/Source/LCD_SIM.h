
/***********************************************************************
  REVISION LOG ENTRY
  Revision By: ...
  Revised on 2005-6-5 23:03:23
  Comments: LCD_SIM.h
 ***********************************************************************/

#ifndef __LCD_SIM_H__
#define __LCD_SIM_H__

void	InitSMem(void);
void	NotifyMouseState(LCD_tMouseState mouseState);
int		COLORREF2Index(LCD_COLOR color);
void	LCDSIM_CheckInit(HWND hwnd);




#endif //__LCD_SIM_H__