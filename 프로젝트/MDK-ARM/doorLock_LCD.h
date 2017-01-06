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



void LCD_init(void);

void LCD_setRefresh(void);

void LCD_clearPage(void);

// print lcd
void LCD_printHeader(void);
void LCD_printInitPage(void);
void LCD_printServerDisconnectionMsg(void);
void LCD_printSelectLockerPage(void);
void LCD_printEnterPasswordPage(void);
void LCD_printWaitingPage(void);
void LCD_printOpenLockerPage(void);
void LCD_printWrongPasswordPage(void);
// end print lcd

void LCD_mainTask(void);







