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
#ifndef _GLCD_H_
#define _GLCD_H_

void setLCDkey(char val);
void setLCD_refresh(void);

void init_lcd(void);
void LCD_refresh(void);

#endif // !_GLCD_H_


