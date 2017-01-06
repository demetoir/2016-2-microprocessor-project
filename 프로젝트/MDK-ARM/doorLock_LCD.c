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

 //lcd ȭ���� ������ �ʿ������� ���� flag ����, get�Լ�
 //lcd�� io�ӵ� �� �ſ� �����Ƿ� �����ؾ� �Ҷ��� ���� ��û�Ͽ�
 //cpu�� ȿ���� ���δ�
bool isNeedRefreash = false;
void LCD_setRefresh(void) {
	isNeedRefreash = true;
}

//lcd�� �ʱ�ȭ �ϴ� ����
void LCD_init(void) {
	isNeedRefreash = false;
	//GLCD���̺귯���� lcd�� �ʱ�ȭ�ϰ� ȭ���� �����
	GLCD_init();
		GLCD_clear(White);
	//ȭ�� ������ ��û�ϰ� ȭ���� ����Ѵ�
	LCD_setRefresh();
	LCD_mainTask();
}

//LCD_print��� ���λ簡 ���� �Լ����� lcd�� ȭ�� ����� �ϴ� 
//�Լ����̰� clearPage�� ȭ���� header�κ��� ������ ȭ����
//�����. ȭ���� ����ҋ��� �ַ� sprintf�� ����Ͽ� �����
//���ڿ��� �����ϰ� GLCD_displayStringLn�� ���ڿ��� ���ٿ� 
//����Ͽ���
void LCD_clearPage(void) {
	GLCD_clearLn(Line3);
	GLCD_clearLn(Line4);
	GLCD_clearLn(Line5);
	GLCD_clearLn(Line6);
	GLCD_clearLn(Line7);
	GLCD_clearLn(Line8);
	GLCD_clearLn(Line9);
}

//lcdȭ�� ��ܿ� �ΰ�� ��¥ �� �ð��� ����Ѵ�
void LCD_printHeader(void) {
	char lineBuffer[21];

	//������ ���� ������ �����Ѵ� 
	GLCD_setBackColor(Blue);
	GLCD_setTextColor(White);
	GLCD_displayStringLn(Line0, "   Fridge Locker     ");

	//������ ���� ������ �����Ѵ� 
	GLCD_setBackColor(White);
	GLCD_setTextColor(DarkGrey);
	//DATA�� ���λ�� ������ �Լ��� ���Ͽ� ����ð��� �������� ����Ѵ�
	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, " DATE: %4d/%2d/%2d",
		DATA_getTime(YEAR), DATA_getTime(MONTH), DATA_getTime(DAY));
	GLCD_displayStringLn(Line1, lineBuffer);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, " TIME: %2d:%2d:%2d",
		DATA_getTime(HOUR), DATA_getTime(MINUTE), DATA_getTime(SECOND));
	GLCD_displayStringLn(Line2, lineBuffer);
}

//�ʱ�ȭ ȭ���� ���
void LCD_printInitPage(void) {
	char lineBuffer[21];

	GLCD_setBackColor(White);                           /* Set the Text Color */
	GLCD_setTextColor(Blue);                          /* Set the Text Color */

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "  initializing...");
	GLCD_displayStringLn(Line5, lineBuffer);
}

//��Ŀ ��ȣ�� �����ϴ� ȭ�� ���
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

//��й�ȣ�� �Է��ϴ� ȭ���� ��� 
//�Էµ� ��й�ȣ�� '*'�� ����ϰ� 
void LCD_printEnterPasswordPage(void) {
	char lineBuffer[21];
	int i = 0;
	int pwdSize = 0;
	GLCD_setBackColor(White);
	GLCD_setTextColor(Blue);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "   Enter password");
	GLCD_displayStringLn(Line4, lineBuffer);

	//��й�ȣ�� �ڸ��� ǥ���ϱ����� ����� ���
	for (i = 0; i < 4; i++)
		GLCD_displayChar(90 + 40 * i, 140, '_');

	//�Էµ� ��й�ȣ�� ����ŭ ���ʿ��� '*'�� ����ϰ�
	//�������� ������ ��
	pwdSize = DATA_getPasswordSize();
	for (i = 0; i < 4; i++) {
		if (i < pwdSize)
			GLCD_displayChar(90 + 40 * i, 130, '*');
		else
			GLCD_displayChar(90 + 40 * i, 130, ' ');
	}
}

//����� ȭ���� ���
void LCD_printWaitingPage(void) {
	char lineBuffer[21];
	GLCD_setBackColor(White);
	GLCD_setTextColor(Blue);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "   Wait for second");
	GLCD_displayStringLn(Line4, lineBuffer);
}

//���������� ȭ���� ���
void LCD_printOpenLockerPage(void) {
	char lineBuffer[21];
	GLCD_setBackColor(White);
	GLCD_setTextColor(Blue);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "opened  %d  Locker  ",
		DATA_getSelectedLockerNumber());
	GLCD_displayStringLn(Line3, lineBuffer);
}

// ������ �������������� �޼����� ���
void LCD_printServerDisconnectionMsg(void) {
	char lineBuffer[21];

	GLCD_setBackColor(White);
	GLCD_setTextColor(Red);
	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "Server Disconnceted");
	GLCD_displayStringLn(Line9, lineBuffer);
}

//��й�ȣ�� �߸� �Է������� ȭ���� ���
void LCD_printWrongPasswordPage(void) {
	char lineBuffer[21];
	GLCD_setBackColor(White);
	GLCD_setTextColor(Red);

	memset(lineBuffer, 0x00, sizeof(lineBuffer));
	sprintf(lineBuffer, "   Wrong Password  ");
	GLCD_displayStringLn(Line4, lineBuffer);
}

//LCD����� mainTask�� �����ϴ� �Լ�
//lcd�� io�ӵ��� ������ ������ main���� �Ź� ����Ǵ°��� �ƴ�
//ȭ���� ǥ���Ұ��� �޶��� ���� ����ϵ��� �Ͽ���.
//ȭ���ܿ� header�� �ΰ�, ��¥, �ð��� ����ϰ� ������ ȭ�鿡��
//system mode�� �ش��ϴ� ȭ���� ����Ѵ�
void LCD_mainTask(void) {
	int mode = DATA_getSystemState();

	//ȭ���� ���ŵ��ʿ䰡 ������ �ٷ� �Լ��� �����Ѵ�
	if (isNeedRefreash == false)
		return;
	//ȭ���� ������ �ߺ����� �ʰ� �÷��� ������ false�� �����
	isNeedRefreash = false;

	//lcd�� �����
	LCD_printHeader();

	//������ ������ �������� ��� �޼����� ����Ѵ� 
	if (DATA_isServerConnected() == false) {
		LCD_printServerDisconnectionMsg();
	}
	//������ ������ �������ٰ� ������� ��� �޼����� �����
	else {
		GLCD_clearLn(Line9);
	}

	//���� ȭ���� ��尡 ����ȴٸ� header�� ������ �κ��� 
	//�ٸ��κ��� ����ϱ� ���� ���� ȭ���� �����
	if (DATA_isChangedSystemState() == true) {
		LCD_clearPage();
	}

	//�� ��忡 �´� lcd ȭ���� ����Ѵ�
	switch (mode) {
	case SYSTEM_STATE_SELECT_LOCKER_NUMBER: LCD_printSelectLockerPage(); break;
	case SYSTEM_STATE_ENTER_PASSWORD: LCD_printEnterPasswordPage(); break;
	case SYSTEM_STATE_OPEN_SUCCESS: LCD_printOpenLockerPage(); break;
	case SYSTEM_STATE_OPEN_FAIL: LCD_printWrongPasswordPage(); break;
	case SYSTEM_STATE_WAITING_RESPONSE: LCD_printWaitingPage(); break;
	default:break;
	}
}








