/*****************************************************************************
 * Simple 4 wire touch screen driver
 * Landscape orientation
 *****************************************************************************
 * FileName:        TouchScreen.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") is intended and supplied to you, the Company's
 * customer, for use solely and exclusively with products manufactured
 * by the Company. 
 *
 * The software is owned by the Company and/or its supplier, and is 
 * protected under applicable copyright laws. All rights are reserved. 
 * Any use in violation of the foregoing restrictions may subject the 
 * user to criminal sanctions under applicable laws, as well as to 
 * civil liability for the breach of the terms and conditions of this 
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES, 
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, 
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR 
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok		01/08/07	...
 * Anton Alkhimenok		06/06/07	Basic calibration and GOL messaging are added
 *****************************************************************************/

#include "MainDemo.h"

//////////////////////// LOCAL PROTOTYPES ////////////////////////////
void TouchGetCalPoints(WORD* ax, WORD* ay);

//////////////////////// GLOBAL VARIABLES ////////////////////////////
#define  DEBOUNCE           2
#define  TOUCH_DELAY_PRESS  1800/DEBOUNCE
#define  TOUCH_DELAY_MOVE   100/DEBOUNCE
#define  CAL_DELTA          0x60

// Max/Min ADC values for each derection
WORD _calXMin = XMINCAL;
WORD _calXMax = XMAXCAL;
WORD _calYMin = YMINCAL;
WORD _calYMax = YMAXCAL;

// Current ADC values for X and Y channels
volatile SHORT adcX     = -1;
volatile SHORT adcY     = -1;
volatile SHORT adcPot   = 0;
volatile char  debounce_counter = 0;
volatile char  calibration = 0;

typedef enum{
SET_X_CHANNEL,
MEASURE_X,
SET_Y_CHANNEL,
MEASURE_Y,
SET_POT_CHANNEL,
MEASURE_POT
} ADC_STATES;

volatile ADC_STATES state = SET_X_CHANNEL;

/*********************************************************************
* Function: Timer3 ISR
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Timer3 ISR provides delay between 2 ADC conversions.
*           The delay provides debouncing for touch screen.
*
* Note: none
*
********************************************************************/
#if 0
void __attribute__((interrupt, shadow, auto_psv)) _T3Interrupt(void)
{
    switch(state){
        case SET_X_CHANNEL:
            // Disconnect X pins
            ADPCFG_XPOS = 0;
            ADPCFG_XNEG = 0;
            TRIS_XPOS = 1;
            TRIS_XNEG = 1;

            // Set Y bias
            ADPCFG_YPOS = 1;
            ADPCFG_YNEG = 1;
            LAT_YPOS = 1;    // to VCC
            LAT_YNEG = 0;    // to GND
            TRIS_YPOS = 0;
            TRIS_YNEG = 0;

            // Switch ADC channel
            AD1CHS = ADC_XPOS;
            debounce_counter = 0;    
            state = MEASURE_X;

        case MEASURE_X:
            // Run conversion
            AD1CON1bits.SAMP = 1;
            break;

        case SET_Y_CHANNEL:
            // Disconnect Y pins
            ADPCFG_YPOS = 0;
            ADPCFG_YNEG = 0;
            TRIS_YPOS = 1;
            TRIS_YNEG = 1;

            // Set X bias
            ADPCFG_XPOS = 1;
            ADPCFG_XNEG = 1;
            LAT_XPOS = 1;    // to VCC
            LAT_XNEG = 0;    // to GND
            TRIS_XPOS = 0; 
            TRIS_XNEG = 0;

            // Switch ADC
            AD1CHS = ADC_YPOS;
            debounce_counter = 0;    
            state = MEASURE_Y;

        case MEASURE_Y:          
            // Run conversion
            AD1CON1bits.SAMP = 1;
            break;

        case SET_POT_CHANNEL:
            // Switch ADC
            AD1CHS = ADC_POT;
            state = MEASURE_POT;

        case MEASURE_POT:          
            // Run conversion
            AD1CON1bits.SAMP = 1;
            break;

    }
    // Clear flag
    IFS0bits.T3IF = 0;
}

/*********************************************************************
* Function: ADC ISR
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: ADC ISR processes result of conversions.
*           If 2 conversions are equal the result is stable and it can be
*           used for position calculation.
*
* Note: none
*
********************************************************************/
void __attribute__((interrupt, shadow, auto_psv)) _ADC1Interrupt(void)
{
static SHORT prevRes = -1;
static SHORT tempX;
SHORT res;

    res = ADC1BUF0;
    switch(state){
        case SET_X_CHANNEL:
            break;

        case MEASURE_X:
            // Check if screen is not touched
            if( (res<(XMINCAL-CAL_DELTA)) ||
                (res>(XMAXCAL+CAL_DELTA)) ){
                 PR3 = TOUCH_DELAY_PRESS;
                 res  = -1;
            }else{
                    PR3 = TOUCH_DELAY_MOVE;
            }

            if(res == prevRes){
                debounce_counter++;
                if(debounce_counter>DEBOUNCE){
                    tempX = res;
                    state = SET_Y_CHANNEL;
                }
                break;
            }
            debounce_counter = 0;
            prevRes = res;
            break;

        case SET_Y_CHANNEL:
            break;

        case MEASURE_Y:          
            // Check if screen is not touched
            if( (res<(YMINCAL-CAL_DELTA)) ||
                (res>(YMAXCAL+CAL_DELTA)) ){
                res = -1;
                PR3 = TOUCH_DELAY_PRESS;
            }else{
                PR3 = TOUCH_DELAY_MOVE;
            }
            if(res == prevRes){
                debounce_counter++;
                if(debounce_counter>DEBOUNCE){
                    adcX = tempX;
                    adcY = res;
                    state = SET_POT_CHANNEL;
                }
                break;
            }
            debounce_counter = 0;
            prevRes = res;
            break;

        case SET_POT_CHANNEL:
            break;

        case MEASURE_POT:
            adcPot = res;
            state = SET_X_CHANNEL;
            break;
    }

    if(calibration)
        PR3 = TOUCH_DELAY_PRESS;
    // Clear flag
    IFS0bits.AD1IF = 0;
}
#endif

/*********************************************************************
* Function: void TouchInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: sets ADC 
*
* Note: none
*
********************************************************************/
void TouchInit(void)
{
#if 0
    // Initialize ADC
	AD1CON1 = 0x80E0;				//Turn on, auto-convert
	AD1CON2 = 0;					//AVdd, AVss, int every conversion, MUXA only
	AD1CON3 = 0x1F85;				//31 Tad auto-sample, Tad = 5*Tcy
    AD1CHS = ADC_TEMP;
	AD1PCFGbits.PCFG4 = 0;          //Disable digital input on AN4 for temperature sensor
	AD1CSSL = 0;					//No scanned inputs

    IFS0bits.AD1IF = 0;             //Clear int flag
    IEC0bits.AD1IE = 1;             //Enable interrupt

    // Initialize Timer3
    TMR3 = 0;
    PR3 = TOUCH_DELAY_PRESS;
    T3CONbits.TCKPS = 0b01;         //Set prescale to 1:8
    IFS0bits.T3IF = 0;              //Clear flag
    IEC0bits.T3IE = 1;              //Enable interrupt
    T3CONbits.TON = 1;              //Run timer
#endif      
}

#if 0
/*********************************************************************
* Function: SHORT TouchGetX()
*
* PreCondition: none
*
* Input: none
*
* Output: x coordinate
*
* Side Effects: none
*
* Overview: returns x coordinate if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
SHORT TouchGetX()
{
	long result;

    result = ADCGetX();    

    if(result>=0)
    {
        result = GetMaxX()-(GetMaxX()*(result - _calXMin))/(_calXMax - _calXMin);
    }
    return result;
}

/*********************************************************************
* Function: SHORT TouchGetY()
*
* PreCondition: none
*
* Input: none
*
* Output: y coordinate
*
* Side Effects: none
*
* Overview: returns y coordinate if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
SHORT TouchGetY()
{
	long result;

    result = ADCGetY();

    if(result>=0)
    {
        result =  (GetMaxY()*(result - _calYMin))/(_calYMax - _calYMin);
    }
    
    return result;
}

/*********************************************************************
* Function: void TouchGetMsg(GOL_MSG* pMsg)
*
* PreCondition: none
*
* Input: pointer to the message structure to be populated
*
* Output: none
*
* Side Effects: none
*
* Overview: populates GOL message structure
*
* Note: none
*
********************************************************************/
void TouchGetMsg(GOL_MSG* pMsg)
{
	static SHORT prevX = -1;
	static SHORT prevY = -1;

	SHORT x,y;
		
    pMsg->type    = TYPE_TOUCHSCREEN;
    pMsg->uiEvent = EVENT_INVALID;

    x = TouchGetX();
    if( x == -1 )
    {
        y = -1;
    }
    else
    {
        y = TouchGetY();
        if( y == -1 )
            x = -1;
    }

    if( (prevX == x) && (prevY == y) )
        return;

	x = pMsg->param1;
	y = pMsg->param2;
	
    if( (prevX != -1) && (prevY != -1) )
    {
        if( (x != -1) && (y != -1) )
        {
            // Move
            pMsg->uiEvent = EVENT_MOVE;
        }
        else
        {
            // Released
            pMsg->uiEvent = EVENT_RELEASE;
            pMsg->param1  = prevX;
            pMsg->param2  = prevY;
            prevX = x;
            prevY = y;
            return;
        }
    }
    else
    {
        if( (x != -1) && (y != -1) )
        {
            // Pressed
            pMsg->uiEvent = EVENT_PRESS;
        }
        else
        {
            // No message
            pMsg->uiEvent = EVENT_INVALID;
        }
    }

    pMsg->param1 = x;
    pMsg->param2 = y;
    prevX = x;
    prevY = y;
}

#else

void TouchGetMsg(GOL_MSG* pMsg)
{
	
}    

SHORT TouchGetX()
{
	return 0;
}

SHORT TouchGetY()
{
	return 0;
}     
#endif



/*********************************************************************
* Function: void TouchStoreCalibration(void)
*
* PreCondition: EEPROMInit() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: stores calibration parameters into EEPROM
*
* Note: none
*
********************************************************************/
void TouchStoreCalibration(void)
{
//    EEPROMWriteWord(_calXMin, EEPROM_XMIN);
//    EEPROMWriteWord(_calXMax, EEPROM_XMAX);
//    EEPROMWriteWord(_calYMin, EEPROM_YMIN);
//    EEPROMWriteWord(_calYMax, EEPROM_YMAX);
}

/*********************************************************************
* Function: void TouchLoadCalibration(void)
*
* PreCondition: EEPROMInit() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: loads calibration parameters from EEPROM
*
* Note: none
*
********************************************************************/
void TouchLoadCalibration(void)
{
//    _calXMin = EEPROMReadWord(EEPROM_XMIN);
//    _calXMax = EEPROMReadWord(EEPROM_XMAX);
//    _calYMin = EEPROMReadWord(EEPROM_YMIN);
//    _calYMax = EEPROMReadWord(EEPROM_YMAX);
}

/*********************************************************************
* Function:  void TouchCalibration()
*
* PreCondition: InitGraph() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: calibrates touch screen
*
* Note: none
*
********************************************************************/
void TouchCalibration()
{
SHORT counter;
SHORT x,y;
WORD  ax[3],ay[3];

SHORT textHeight;

    // Set touch screen timing for calibration
    calibration = 1;

    SetFont((void*)&GOLFontDefault);
    textHeight = GetTextHeight((void*)&GOLFontDefault);

    SetColor(WHITE);
    ClearDevice();

    SetColor(BRIGHTRED);
    OutTextXY(0,0*textHeight,"IMPORTANT.");
    SetColor(BLACK);
    OutTextXY(0,1*textHeight,"Now touch screen calibration");
    OutTextXY(0,2*textHeight,"will be perfomed.Touch points");
    OutTextXY(0,3*textHeight,"EXACTLY at the positions");
    OutTextXY(0,4*textHeight,"shown by arrows.");
    SetColor(BRIGHTRED);
    OutTextXY(0,6*textHeight,"Touch screen to continue.");

    // Wait for touch
    do{
        x=ADCGetX(); y=ADCGetY();
    }while((y==-1)||(x==-1));

    Beep();

    DelayMs(500);

    SetColor(WHITE);
    ClearDevice();

    SetColor(BRIGHTRED);

    Line(5,5,5,15);
    Line(4,5,4,15);
    Line(6,5,6,15);

    Line(5,5,15,5);
    Line(5,4,15,4);
    Line(5,6,15,6);

    Line(5,6,15,16);
    Line(5,4,15,14);
    Line(5,5,15,15);

    TouchGetCalPoints(ax, ay);

    // Choose max X and min Y
    _calXMax = 0;
    _calYMin = 0xFFFF;
    for(counter=0; counter<3; counter++){
        if(_calXMax < ax[counter])
            _calXMax = ax[counter];

        if(_calYMin > ay[counter])
            _calYMin = ay[counter];
    }

    SetColor(WHITE);
    ClearDevice();

    SetColor(BRIGHTRED);

    Line(5,GetMaxY()-5,5,GetMaxY()-15);
    Line(4,GetMaxY()-5,4,GetMaxY()-15);
    Line(6,GetMaxY()-5,6,GetMaxY()-15);

    Line(5,GetMaxY()-5,15,GetMaxY()-5);
    Line(5,GetMaxY()-4,15,GetMaxY()-4);
    Line(5,GetMaxY()-6,15,GetMaxY()-6);

    Line(5,GetMaxY()-6,15,GetMaxY()-16);
    Line(5,GetMaxY()-4,15,GetMaxY()-14);
    Line(5,GetMaxY()-5,15,GetMaxY()-15);

    TouchGetCalPoints(ax, ay);

    // Choose max Y
    _calYMax = 0;
    for(counter=0; counter<3; counter++){
        if(_calYMax < ay[counter])
            _calYMax = ay[counter];
    }

    SetColor(WHITE);
    ClearDevice();

    SetColor(BRIGHTRED);    

    Line(GetMaxX()-5,GetMaxY()/2-5,GetMaxX()-5,GetMaxY()/2-15);
    Line(GetMaxX()-4,GetMaxY()/2-5,GetMaxX()-4,GetMaxY()/2-15);
    Line(GetMaxX()-6,GetMaxY()/2-5,GetMaxX()-6,GetMaxY()/2-15);

    Line(GetMaxX()-5,GetMaxY()/2-5,GetMaxX()-15,GetMaxY()/2-5);
    Line(GetMaxX()-5,GetMaxY()/2-4,GetMaxX()-15,GetMaxY()/2-4);
    Line(GetMaxX()-5,GetMaxY()/2-6,GetMaxX()-15,GetMaxY()/2-6);

    Line(GetMaxX()-5,GetMaxY()/2-6,GetMaxX()-15,GetMaxY()/2-16);
    Line(GetMaxX()-5,GetMaxY()/2-4,GetMaxX()-15,GetMaxY()/2-14);
    Line(GetMaxX()-5,GetMaxY()/2-5,GetMaxX()-15,GetMaxY()/2-15);

    TouchGetCalPoints(ax, ay);

    // Choose min X
    _calXMin = 0xFFFF;
    for(counter=0; counter<3; counter++){
        if(_calXMin > ax[counter])
            _calXMin = ax[counter];
    }


    SetColor(WHITE);
    ClearDevice();

    SetColor(BLACK);
    OutTextXY(10,1*textHeight,"Hold S3 button and");
    OutTextXY(10,2*textHeight,"press MCLR reset(S1)");
    OutTextXY(10,3*textHeight,"to REPEAT the calibration");
    OutTextXY(10,4*textHeight,"procedure.");
    SetColor(BRIGHTRED);
    OutTextXY(10,6*textHeight,"Touch screen to continue.");

    // Wait for touch
    do{
        x=ADCGetX(); y=ADCGetY();
    }while((y==-1)||(x==-1));

    Beep();

    DelayMs(500);

    SetColor(BLACK);
    ClearDevice();

    // Set touch screen timing for work mode
    calibration = 0;
}

/*********************************************************************
* Function: void TouchGetCalPoints(WORD* ax, WORD* ay)
*
* PreCondition: InitGraph() must be called before
*
* Input: ax - pointer to array receiving 3 X touch positions
*        ay - pointer to array receiving 3 Y touch positions
*
* Output: none
*
* Side Effects: none
*
* Overview: gets values for 3 touches
*
* Note: none
*
********************************************************************/
void TouchGetCalPoints(WORD* ax, WORD* ay){
static const char calStr[] = "CALIBRATION";
char calTouchLeft[] = "3 touches left";
SHORT counter;
SHORT x,y;

    SetFont((void*)&GOLFontDefault);

    SetColor(BRIGHTRED);

    OutTextXY((GetMaxX()-GetTextWidth((char*)calStr,(void*)&GOLFontDefault))>>1,
              (GetMaxY()-GetTextHeight((void*)&GOLFontDefault))>>1,
              (char*)calStr);

    for(counter=0; counter<3; counter++){

        SetColor(BRIGHTRED);

        calTouchLeft[0] = '3' - counter;

        OutTextXY((GetMaxX()-GetTextWidth(calTouchLeft,(void*)&GOLFontDefault))>>1,
                  (GetMaxY()+GetTextHeight((void*)&GOLFontDefault))>>1,
                   calTouchLeft);

        // Wait for press
        do{
            x=ADCGetX(); y=ADCGetY();
        }while((y==-1)||(x==-1));

        Beep();

        *(ax+counter) = x; *(ay+counter) = y;
       
        // Wait for release
        do{
            x=ADCGetX(); y=ADCGetY();
        }while((y!=-1)&&(x!=-1));

        SetColor(WHITE);

        OutTextXY((GetMaxX()-GetTextWidth(calTouchLeft,(void*)&GOLFontDefault))>>1,
                  (GetMaxY()+GetTextHeight((void*)&GOLFontDefault))>>1,
                   calTouchLeft);

        DelayMs(500);
    }
}
