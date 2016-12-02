/*----------------------------------------------------------------------------
 *      doorLock_LCD.c
 *----------------------------------------------------------------------------
 *      Name:    doorLock_LCD.c
 *---------------------------------------------------------------------------*/


#ifndef _doorLock_h_
#include "doorLock_LCD.h"
#endif // !_doorLock_h


#define bool 							int
#define false							0
#define true 							1

bool isNeedRepaintLCD;
typedef enum {
LCD_MODE_SET_PASSWORD,
LCD_SHOW_PASSWORD,
LCD_OPEN_DOOR,
LCD_SELECT_STORAGE,
LCD_INIT_MODE,
LCD_CONNECTING_SERVER,
LCD_WRONG_PASSWORD,
}LCD_MODE;

LCD_MODE lcdMode;





char cLCD_Keypad_Value;


void setLCDkey(char  val) {
	cLCD_Keypad_Value = val;
}

void setLCD_refresh(void) {
	isNeedRepaintLCD = true;
}

void setLCDMode(LCD_MODE mode) {
	lcdMode = mode;
}

void showLabel(void) {
	char lineBuffer[21];
	
	GLCD_setBackColor(Blue);                           /* Set the Text Color */
	GLCD_setTextColor(White);                          /* Set the Text Color */
	GLCD_displayStringLn(Line0, "   Fridge Locker     ");

	GLCD_setBackColor(White);                           /* Set the Text Color */
	GLCD_setTextColor(Blue);                          /* Set the Text Color */
	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, " data: %4d/%2d/%2d",
		getTime(YEAR), getTime(MONTH), getTime(DAY));
	GLCD_displayStringLn(Line1, lineBuffer);
	
	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, " time: %2d:%2d:%2d",
		getTime(HOUR), getTime(MINUTE), getTime(SECOND));
	GLCD_displayStringLn(Line2, lineBuffer);

}

void showInit(void) {
	char lineBuffer[21];

	GLCD_setBackColor(White);                           /* Set the Text Color */
	GLCD_setTextColor(Blue);                          /* Set the Text Color */

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "  initializing...");
	GLCD_displayStringLn(Line5, lineBuffer);
}

void showServerDisconnection(void) {
	char lineBuffer[21];

	GLCD_setBackColor(White);                           /* Set the Text Color */
	GLCD_setTextColor(Blue);                          /* Set the Text Color */

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "server disconnceted");
	GLCD_displayStringLn(Line4, lineBuffer);
}





void init_lcd() {
	isNeedRepaintLCD = false;

	lcdMode = LCD_INIT_MODE;


	cLCD_Keypad_Value = 'x';
		
	//LPC_PINCON->PINSEL9 &= ~(3 << 24);   back light on
	GLCD_init();                              /* Initialize the GLCD           */
	GLCD_clear(White);						/* Clear the GLCD                */
	setLCD_refresh();
	LCD_refresh();
	showInit();
}



void LCD_refresh(void) {
	if (isNeedRepaintLCD == false) return;
	isNeedRepaintLCD = false;

	showLabel();
	showInit();
	showServerDisconnection();


	//show lcd
	switch (lcdMode) {
	case LCD_MODE_SET_PASSWORD: break;
	case LCD_SHOW_PASSWORD: break;
	case LCD_OPEN_DOOR: break;
	case LCD_SELECT_STORAGE: break;
	case LCD_INIT_MODE: break;
	case LCD_CONNECTING_SERVER: break;
	case LCD_WRONG_PASSWORD: break;
	default:
		break;
	}

}








