/*****************************************************************************
 *
 * Beeper.
 *
 *****************************************************************************
 * FileName:        Beep.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30 V3.00
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright  2007 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok		07/10/07	...
 *****************************************************************************/

#include "MainDemo.h"

volatile WORD pulseCounter;

/*********************************************************************
* Function: Timer2 ISR
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: counts beeper pulses and turns off it after BEEP_TIME
*
* Note: none
*
********************************************************************/
#if 0
void __attribute__((interrupt, shadow, auto_psv)) _T2Interrupt(void)
{
    if(BEEP_TIME < pulseCounter++){
        // clear pulse counter
        pulseCounter = 0;
    	// stop counter
        T2CONbits.TON = 0;
        // clear timer
        TMR2 = 0;
    }

    //clear flag
    IFS0bits.T2IF = 0;
}
#endif
/*********************************************************************
* Function:  void BeepInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: initializes beeper
*
* Note: none
*
********************************************************************/
void BeepInit()
{

    // clear pulse counter
    pulseCounter = 0;

#if 0
    // init OC1
    // PWM mode
    OC1CONbits.OCM = 0b110;
    OC1RS = TIMER_BASE/BEEP_DUTY;
    OC1R =  TIMER_BASE/BEEP_DUTY;
	OC_TRIS_BIT = 0;

    // init Timer2
    TMR2 = 0;
    PR2 =   TIMER_BASE;
    //set prescale to 1:8
    T2CONbits.TCKPS = 0b01;
	//clear flag
    IFS0bits.T2IF = 0;
	//enable interrupt
    IEC0bits.T2IE = 1;
#endif
}

