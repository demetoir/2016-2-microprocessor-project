///*----------------------------------------------------------------------------
// *      doorLock_LCD.h
// *----------------------------------------------------------------------------
// *      Name:    doorLock_LCD.h
// *---------------------------------------------------------------------------*/
//

#include "GLCD.h"
#include <stdio.h>
#include <string.h>
#include "doorLock_data.h"


void init_lcd(void);
void setLCDkey(char  val);
void setLCD_refresh(void);
//void setLCDMode(LCD_MODE mode);

void showLabel(void);
void showInit(void);
void showServerDisconnection(void);
void showSelectLockerPage(void);
void LCD_refresh(void);
void clearPage(void);







