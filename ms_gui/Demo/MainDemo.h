#ifndef _MAINDEMO_H
#define _MAINDEMO_H

#include "../PIC_Graphics/GenericTypeDefs.h"
#include "../PIC_Graphics/Graphics.h"
#include "../PIC_Graphics/CustomControlDemo.h"

#include "TouchScreen.h"
#include "Beep.h"
#include "SideButtons.h"
#include "rtcc.h"

int mainDemo( GOL_MSG * pMsg );
int init_mainDemo( void );

#endif
