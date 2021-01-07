// Date		:	2005-05-28
//
// Author	:	HOUHH
//
// File		:	Branding.c...
//
// Version	:	ver1.0.0


/***********************************************************************
  REVISION LOG ENTRY
  Revision By: ...
  Revised on 2005-5-29 1:07:35
  Comments: ...
 ***********************************************************************/



#include "Branding.h"

 //2005-6-27 1:20:10
const char acCopyright[] = {" (c) 2002  Micrium, Inc.\n \
www.micrium.com \n\n(c) \
1998-2002  Segger,\n\
 Microcontroller Systeme GmbH,\n\
 www.segger.com"};
	
/*
const char acAppNameLong[] = {"�C/GUI-Simulatio"};
const char acAppNameShort[] = {"�C/GUI-Simulation"};
*/
//const char acCopyright[] = {" (c) 2005  UCGUIרҵ��̳ \n \
//www.ucgui.com \n\
//�����Լ���GUI��ϵ!\n\
//2005-2006  CopyRight,\n"};
	

const char acAppNameLong[] = {"uC/GUI-Sim"};
const char acAppNameShort[] = {"uC/GUI-Sim"};


const char* BRANDING_GetAppNameLong()
{
	return acAppNameLong;
}

const char* BRANDING_GetAppNameShort()
{
	return acAppNameLong;
}

const char* BRANDING_GetCopyright()
{
	return acCopyright;
}

