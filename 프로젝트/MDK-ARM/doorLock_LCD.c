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

 //lcd 화면이 갱신이 필요한지에 대한 flag 변수, get함수
 //lcd의 io속도 가 매우 느리므로 갱신해야 할때만 갱신 요청하여
 //cpu의 효율을 높인다
bool isNeedRefreash = false;
void LCD_setRefresh(void) {
	isNeedRefreash = true;
}

//lcd를 초기화 하는 변수
void LCD_init(void) {
	isNeedRefreash = false;
	//GLCD라이브러리로 lcd를 초기화하고 화면을 지운다
	GLCD_init();
		GLCD_clear(White);
	//화면 갱신을 요청하고 화면을 출력한다
	LCD_setRefresh();
	LCD_mainTask();
}

//LCD_print라는 접두사가 붙은 함수들은 lcd에 화면 출력을 하는 
//함수들이고 clearPage는 화면의 header부분을 제외한 화면을
//지운다. 화면을 출력할떄는 주로 sprintf를 사용하여 출력할
//문자열을 생성하고 GLCD_displayStringLn로 문자열을 한줄에 
//출력하였다
void LCD_clearPage(void) {
	GLCD_clearLn(Line3);
	GLCD_clearLn(Line4);
	GLCD_clearLn(Line5);
	GLCD_clearLn(Line6);
	GLCD_clearLn(Line7);
	GLCD_clearLn(Line8);
	GLCD_clearLn(Line9);
}

//lcd화면 상단에 로고와 날짜 및 시간을 출력한다
void LCD_printHeader(void) {
	char lineBuffer[21];

	//글자의 색과 배경색을 지정한다 
	GLCD_setBackColor(Blue);
	GLCD_setTextColor(White);
	GLCD_displayStringLn(Line0, "   Fridge Locker     ");

	//글자의 색과 배경색을 지정한다 
	GLCD_setBackColor(White);
	GLCD_setTextColor(DarkGrey);
	//DATA를 접두사로 가지는 함수를 통하여 현재시간을 가져오고 출력한다
	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, " DATE: %4d/%2d/%2d",
		DATA_getTime(YEAR), DATA_getTime(MONTH), DATA_getTime(DAY));
	GLCD_displayStringLn(Line1, lineBuffer);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, " TIME: %2d:%2d:%2d",
		DATA_getTime(HOUR), DATA_getTime(MINUTE), DATA_getTime(SECOND));
	GLCD_displayStringLn(Line2, lineBuffer);
}

//초기화 화면을 출력
void LCD_printInitPage(void) {
	char lineBuffer[21];

	GLCD_setBackColor(White);                           /* Set the Text Color */
	GLCD_setTextColor(Blue);                          /* Set the Text Color */

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "  initializing...");
	GLCD_displayStringLn(Line5, lineBuffer);
}

//락커 번호를 선택하는 화면 출력
void LCD_printSelectLockerPage(void) {
	char lineBuffer[21];
	int selLockNum = 0;
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

	selLockNum = DATA_getSelectedLockerNumber();
	if (selLockNum == LOCKER_NUMBER_UNSELECTED) {
		GLCD_clearLn(Line8);
	}
	else {
		GLCD_setTextColor(Purple);
		memset(lineBuffer, 0x00, sizeof(lineBuffer));
		sprintf(lineBuffer, "         %d ", selLockNum);
		GLCD_displayStringLn(Line8, lineBuffer);
	}
}

//비밀번호를 입력하는 화면을 출력 
//입력된 비밀번호를 '*'로 출력하고 
void LCD_printEnterPasswordPage(void) {
	char lineBuffer[21];
	int i = 0;
	int pwdSize = 0;
	GLCD_setBackColor(White);
	GLCD_setTextColor(Blue);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "   Enter password");
	GLCD_displayStringLn(Line4, lineBuffer);

	//비밀번호의 자리를 표시하기위한 언더바 출력
	for (i = 0; i < 4; i++)
		GLCD_displayChar(90 + 40 * i, 140, '_');

	//입력된 비밀번호의 수많큼 왼쪽에서 '*'을 출력하고
	//나머지는 공백을 출
	pwdSize = DATA_getPasswordSize();
	for (i = 0; i < 4; i++) {
		if (i < pwdSize)
			GLCD_displayChar(90 + 40 * i, 130, '*');
		else
			GLCD_displayChar(90 + 40 * i, 130, ' ');
	}
}

//대기중 화면을 출력
void LCD_printWaitingPage(void) {
	char lineBuffer[21];
	GLCD_setBackColor(White);
	GLCD_setTextColor(Blue);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "   Wait for second");
	GLCD_displayStringLn(Line4, lineBuffer);
}

//열렸을때의 화면을 출력
void LCD_printOpenLockerPage(void) {
	char lineBuffer[21];
	GLCD_setBackColor(White);
	GLCD_setTextColor(Blue);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "opened  %d  Locker  ",
		DATA_getSelectedLockerNumber());
	GLCD_displayStringLn(Line3, lineBuffer);
}

// 서버와 연결끊어졌을경우 메세지를 출력
void LCD_printServerDisconnectionMsg(void) {
	char lineBuffer[21];

	GLCD_setBackColor(White);
	GLCD_setTextColor(Red);
	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "Server Disconnceted");
	GLCD_displayStringLn(Line9, lineBuffer);
}

//비밀번호를 잘못 입력했을떄 화면을 출력
void LCD_printWrongPasswordPage(void) {
	char lineBuffer[21];
	GLCD_setBackColor(White);
	GLCD_setTextColor(Red);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "   Wrong Password  ");
	GLCD_displayStringLn(Line4, lineBuffer);
}

//LCD모듈의 mainTask를 실행하는 함수
//lcd의 io속도가 느리기 때문에 main에서 매번 실행되는것이 아닌
//화면의 표시할것이 달라졋을 떄만 출력하도록 하였다.
//화면상단에 header로 로고, 날짜, 시간을 출력하고 나머지 화면에는
//system mode에 해당하는 화면을 출력한다
void LCD_mainTask(void) {
	int mode = DATA_getSystemState();

	//화면이 갱신될필요가 없으면 바로 함수를 종료한다
	if (isNeedRefreash == false)
		return;
	//화면의 갱신이 중복되지 않게 플래그 변수를 false로 만든다
	isNeedRefreash = false;

	//lcd의 헤더와
	LCD_printHeader();

	//서버와 연결이 끊어졌을 경우 메세지를 출력한다 
	if (DATA_isServerConnected() == false) {
		LCD_printServerDisconnectionMsg();
	}
	//서버와 연결이 끊어졌다가 연결됫을 경우 메세지를 지운다
	else {
		GLCD_clearLn(Line9);
	}

	//만약 화면의 모드가 변경된다면 header를 제외한 부분이 
	//다른부분을 출력하기 위해 이전 화며을 지운다
	if (DATA_isChangedSystemState() == true) {
		LCD_clearPage();
	}

	//각 모드에 맞는 lcd 화면을 출력한다
	switch (mode) {
	case SYSTEM_STATE_SELECT_LOCKER_NUMBER: LCD_printSelectLockerPage(); break;
	case SYSTEM_STATE_ENTER_PASSWORD: LCD_printEnterPasswordPage(); break;
	case SYSTEM_STATE_OPEN_SUCCESS: LCD_printOpenLockerPage(); break;
	case SYSTEM_STATE_OPEN_FAIL: LCD_printWrongPasswordPage(); break;
	case SYSTEM_STATE_WAITING_RESPONSE: LCD_printWaitingPage(); break;
	default:break;
	}
}








