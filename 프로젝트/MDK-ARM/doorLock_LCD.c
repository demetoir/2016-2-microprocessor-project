///*----------------------------------------------------------------------------
// *      doorLock_LCD.c
// *----------------------------------------------------------------------------
// *      Name:    doorLock_LCD.c
// *---------------------------------------------------------------------------*/
//

#ifndef _doorLock_h_
#include "doorLock_LCD.h"
#endif // !_doorLock_h


#define bool 							int
#define false							0
#define true 							1

bool isNeedRepaintLCD;

char cLCD_time_10h ;
char cLCD_time_1h ;
char cLCD_time_10m ;
char cLCD_time_1m;
char cLCD_time_10s;
char cLCD_time_1s;

char cLCD_lap_time_10h;
char cLCD_lap_time_1h;
char cLCD_lap_time_10m;
char cLCD_lap_time_1m;
char cLCD_lap_time_10s;
char cLCD_lap_time_1s;

char cLCD_Keypad_Value;

void setLCDTime(int time_10h, int time_1h, int time_10m, int time_1m, int time_10s, int time_1s) {
	cLCD_time_10h = time_10h + '0';
	cLCD_time_1h = time_1h + '0';
	cLCD_time_10m = time_10m + '0';
	cLCD_time_1m = time_1m + '0';
	cLCD_time_10s = time_10s + '0';
	cLCD_time_1s = time_1s + '0';
}

void setLCDkey(char  val) {
	cLCD_Keypad_Value = val;
}

void setLCD_refresh() {
	isNeedRepaintLCD = true;
}

void init_lcd() {
	isNeedRepaintLCD = false;

	cLCD_time_10h = '0';
	cLCD_time_1h = '0';
	cLCD_time_10m = '0';
	cLCD_time_1m = '0';
	cLCD_time_10s = '0';
	cLCD_time_1s = '0';

	cLCD_lap_time_10h = '0';
	cLCD_lap_time_1h = '0';
	cLCD_lap_time_10m = '0';
	cLCD_lap_time_1m = '0';
	cLCD_lap_time_10s = '0';
	cLCD_lap_time_1s = '0';

	cLCD_Keypad_Value = 'x';
		
	///	LPC_PINCON->PINSEL9 &= ~(3 << 24);   back light on
	GLCD_init();                              /* Initialize the GLCD           */
	GLCD_clear(White);						/* Clear the GLCD                */
	setLCD_refresh();

	LCD_refresh();
}

void LCD_refresh(void) {
	if (isNeedRepaintLCD == false) return;
	isNeedRepaintLCD = false;

	GLCD_setBackColor(Blue);                           /* Set the Text Color */
	GLCD_setTextColor(White);                          /* Set the Text Color */
	GLCD_displayStringLn(Line0, "   CortexM3_NXP     ");
	GLCD_displayStringLn(Line1, "    RTX Blinky      ");
	GLCD_displayStringLn(Line2, "   www.huins.com    ");
	

	GLCD_displayChar(20, 80, cLCD_lap_time_10h);
	GLCD_displayChar(40, 80, cLCD_lap_time_1h);
	GLCD_displayChar(60, 80, cLCD_lap_time_10m);
	GLCD_displayChar(80, 80, cLCD_lap_time_1m);
	GLCD_displayChar(100, 80, cLCD_lap_time_10s);
	GLCD_displayChar(120, 80, cLCD_lap_time_1s);

	GLCD_displayChar(180, 80, cLCD_Keypad_Value);
	GLCD_displayChar(200, 80, 'x');

	GLCD_displayChar(20, 140, cLCD_time_10h);
	GLCD_displayChar(40, 140, cLCD_time_1h);
	GLCD_displayChar(60, 140, cLCD_time_10m);
	GLCD_displayChar(80, 140, cLCD_time_1m);
	GLCD_displayChar(100, 140, cLCD_time_10s);
	GLCD_displayChar(120, 140, cLCD_time_1s);




}





