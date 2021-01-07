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
File        : Main.c
Purpose     : Call of MainTask
--------------------END-OF-HEADER-------------------------------------
*/
#include "..\Demo\MainDemo.h"
#include "LCD.H"
void GUI_TOUCH_GetState(LCD_tMouseState *state);
void SIM_Delay(int delaytime);

void main(void)
{
   	GOL_MSG uiMsg = { -1, -1, 0, 0 };
    LCD_tMouseState State;
   	int buttonPressed = 0;
    //initDevice( );
    init_mainDemo();
	
    while(1){
        GUI_TOUCH_GetState(&State);
        if(State.KeyStat){
            if( buttonPressed == 0){
                uiMsg.type    = TYPE_TOUCHSCREEN;
                uiMsg.uiEvent = EVENT_PRESS;
                uiMsg.param1  = State.x;
                uiMsg.param2  = State.y;
                buttonPressed = 1;
            }else{
                uiMsg.type    = TYPE_TOUCHSCREEN;
                uiMsg.uiEvent = EVENT_MOVE;
                uiMsg.param1  = State.x;
                uiMsg.param2  = State.y;
            }
        }else{
            if(buttonPressed == 1){//release
                uiMsg.type    = TYPE_TOUCHSCREEN;
                uiMsg.uiEvent = EVENT_RELEASE;
                uiMsg.param1  = State.x;
                uiMsg.param2  = State.y;
                buttonPressed = 0;
            }
        }
        mainDemo( &uiMsg );
        SIM_Delay(1);
    }
    
}