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

bool isNeedRepaintLCD;

char cLCD_Keypad_Value;

void init_lcd(void) {
	isNeedRepaintLCD = false;

	lcdMode = LCD_INIT_MODE;

	cLCD_Keypad_Value = 'x';
		
	//LPC_PINCON->PINSEL9 &= ~(3 << 24);   back light on
	GLCD_init();                              /* Initialize the GLCD           */
	GLCD_clear(White);						/* Clear the GLCD                */
	setLCD_refresh();
	LCD_refresh();
	//showInit();
}

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
	GLCD_setTextColor(DarkGrey);                          /* Set the Text Color */
	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, " DATE: %4d/%2d/%2d",
		getTime(YEAR), getTime(MONTH), getTime(DAY));
	GLCD_displayStringLn(Line1, lineBuffer);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, " TIME: %2d:%2d:%2d",
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
	GLCD_setBackColor(White);
	GLCD_setTextColor(Red);
	if (isServerConnected() == false) {
		memset(lineBuffer, 0x00, sizeof(lineBuffer));
		sprintf(lineBuffer, "Server Disconnceted");
		GLCD_displayStringLn(Line9, lineBuffer);
	}
	else {
		GLCD_clearLn(Line9);
	}
}

void showSelectLockerPage(void) {
	char lineBuffer[21];
	int num = 0;
	GLCD_setBackColor(Black);
	GLCD_setTextColor(White);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "    Select Locker         ");
	GLCD_displayStringLn(Line3, lineBuffer);

	GLCD_setBackColor(White);
	GLCD_setTextColor(Blue);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "  1  2  3  4  5  6 ");
	GLCD_displayStringLn(Line5, lineBuffer);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "  7  8  9 10 11 12 ");
	GLCD_displayStringLn(Line6, lineBuffer);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, " 13 14 15 16 17 18 ");
	GLCD_displayStringLn(Line7, lineBuffer);

	num = getSelectedLockerNumber();
	if (num == 0) {
		GLCD_clearLn(Line8);
	}
	else {
		GLCD_setTextColor(Purple);
		memset(lineBuffer, 0x00, sizeof(lineBuffer));
		sprintf(lineBuffer, "         %d ", num);
		GLCD_displayStringLn(Line8, lineBuffer);
	}

}

void showEnterPasswordPage(void) {
	char lineBuffer[21];
	int i = 0;
	int pwdSize = 0;
	GLCD_setBackColor(White);
	GLCD_setTextColor(Blue);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "   Enter password");
	GLCD_displayStringLn(Line4, lineBuffer);

	for (i = 0; i<4; i++)
		GLCD_displayChar(90 + 40 * i, 140, '_');

	pwdSize = getPwdSize();
	//pwdSize = 4;
	for (i = 0; i < 4; i++) {
		if (i< pwdSize)
			GLCD_displayChar(90 + 40 * i, 130, '*');
		else
			GLCD_displayChar(90 + 40 * i, 130, ' ');
	}

}

void clearPage(void) {
	GLCD_clearLn(Line3);
	GLCD_clearLn(Line4);
	GLCD_clearLn(Line5);
	GLCD_clearLn(Line6);
	GLCD_clearLn(Line7);
	GLCD_clearLn(Line8);
	GLCD_clearLn(Line9);
}

void showWaitingPage(void) {
	char lineBuffer[21];
	GLCD_setBackColor(White);
	GLCD_setTextColor(Blue);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "   Wait for second");
	GLCD_displayStringLn(Line4, lineBuffer);
}

void showOpenLockerPage(void) {
	char lineBuffer[21];
	GLCD_setBackColor(White);
	GLCD_setTextColor(Blue);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "opened  %d  Locker  ",getSelectedLockerNumber());
	GLCD_displayStringLn(Line3, lineBuffer);
}

void showWrongPasswordPage(void) {
	char lineBuffer[21];
	GLCD_setBackColor(White);
	GLCD_setTextColor(Red);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "   Wrong Password  ");
	GLCD_displayStringLn(Line4, lineBuffer);
}

void LCD_refresh(void) {
	int mode = getCurrentSystemMode();
	
	if (isNeedRepaintLCD == false) return;
	isNeedRepaintLCD = false;

	showLabel();
	showServerDisconnection();

	if (isChangedCurrentSystemMode() == true) {
		clearPage();
	}

	switch (mode) {
	case SYSTEM_MODE_SELECT_LOCKER_NUMBER: showSelectLockerPage(); break;
	case SYSTEM_MODE_ENTER_PASSWORD: showEnterPasswordPage(); break;
	case SYSTEM_MODE_OPEN_SUCCESS: showOpenLockerPage(); break;
	case SYSTEM_MODE_OPEN_FAIL: showWrongPasswordPage(); break;
	case SYSTEM_MODE_WAITING: showWaitingPage(); break;
	default:break;
	}

}








