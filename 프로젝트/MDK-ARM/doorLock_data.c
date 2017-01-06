///*----------------------------------------------------------------------------
// *      doorLock_data.c
// *----------------------------------------------------------------------------
// *      Name:    doorLock_data.c
// *---------------------------------------------------------------------------*/
//
#include "doorlock_data.h"

// system state data
//������ �ý��� ��带 �����ϴ� ����
int systemMode = SYSTEM_STATE_SELECT_LOCKER_NUMBER;
//������ �ý��� ��尡 ���ߴ��� �����ϴ� �÷��� ����
bool _isChangedSystemMode = false;

//���� �ý��� ��尡 ����Ǿ����� Ȯ���ϴ� �����Լ�
bool DATA_isChangedSystemState(void) {
	//��尡 ��������� ���� ������ �����ϱ�����
	//_isChangedSystemMode�� false�� �����
	//true�� ��ȯ�Ѵ�
	if (_isChangedSystemMode == true) {
		_isChangedSystemMode = false;
		return true;
	}
	return false;
}
//systemModed�� getter�Լ�
int DATA_getSystemState(void) {
	return systemMode;
}
//systemModed�� setter�Լ�
void DATA_setSystemState(int mode) {
	//���� ������ ���� �ٸ��ٸ� ��尡 ����Ǿ����Ƿ�
	//_isChangedSystemMode�� true �����
	if (systemMode != mode) {
		systemMode = mode;
		_isChangedSystemMode = true;
	}
}
// end system mode data

// time data
// ��¥ �� �ð��� �����ϱ����� ������
int year = 1999;
int month = 12;
int day = 31;
int hour  = 23;
int minute = 59;
int second = 40;
//�� ���� ������ �����ϴ� �迭
int daysInMonth[13] = { 1,31,28,31,30,31,30,31,31,30,31,30,31 };

//���� ��¥�� �ð����� 1�ʰ� ���� ���� �ð����� ������Ʈ�Ѵ�
void DATA_timeUpdate(void) {
	//�� ��, ��, ��, ��, ��, �� ������ ������ �������� �ø��� �������� ��
	//���� �����÷ο�� �ø����� �������� �� ������ ������Ʈ �ϸ� 
	//1�� ���� �ð����� ������Ʈ�ȴ�

	//�� ������ �ø��� �̰��� ���� ������ �� ���ش� 1�ʸ� ������Ʈ�ϱ�����
	//�̸� �ø����� 1�� �����Ѵ�
	int carry = 1; 
	second += carry;
	carry = second / 60;
	second %= 60;

	minute += carry;
	carry = minute / 60;
	minute %= 60;

	hour += carry;
	carry = hour / 24;
	hour %= 24;

	//��(��)�� ��� 0���� �������������Ƿ� ó���� -1
	//�������� +1�ϴ°����� ���� ������ �����Ҽ��ִ�
	day--;
	day += carry;
	//��(��)�� ��� �Ŵ޸��� ���� �޶����Ƿ� 
	//daysInMonth[month]�� �̿��Ͽ� ������ �����ϰ� �����Ѵ�
	carry = day / (daysInMonth[month]);
	day %= (daysInMonth[month]);
	day++;

	//���� ��쵵 0���� �������������Ƿ� ó���� -1
	//�������� +1�ϴ°����� ���� ������ �����ϰ� �����Ҽ��ִ�
	month--;
	month += carry;
	carry = month / 12;
	month %= 12;
	month++;

	year += carry;
}
//��¥ �� �ð��� setter �Լ�
void DATA_setTime(int YYYY, int MM, int DD, int hh, int mm, int ss ) {
	year = YYYY;
	month = MM;
	day = DD;
	hour = hh;
	minute = mm;
	second = ss;
}

//��¥ �� �ð��� getter �Լ�
int DATA_getTime(int timeType) {
	switch (timeType){
	case YEAR: return year;
	case MONTH: return month;
	case DAY: return day;
	case HOUR: return hour;
	case MINUTE: return minute;
	case SECOND: return second;
	default:return  -1; 
	}
}
// end time and date data

//�Էµ� ��й�ȣ�� ���̸� �����ϴ� ����
int password[PASSWORD_SIZE];
int passwordSize = 0;

//�Էµ� ��й�ȣ�� �����Ѵ�
void DATA_clearPassword(void) {
	password[0] = 0;
	password[1] = 0;
	password[2] = 0;
	password[3] = 0;
	passwordSize = 0;
}
//��й�ȣ�� ������ ���ڸ� �� �߰� �ϵ��� �����
//4�ڸ��� �Ѿ�°�� �߰������ʴ´�
void DATA_pushbackPassword(int pwdValue) {
	if (passwordSize < 4) {
		password[passwordSize] = pwdValue;
		passwordSize++;
	}
}
//��й�ȣ�� �������ڸ��� ���ڸ��� �����Ѵ�  
//0�� �����϶��� �������� �ʴ´�
void DATA_popbackPassword(void) {
	if (password > 0) {
		passwordSize--;
		password[passwordSize] = 0;
	}
}
//��й�ȣ�� ��й�ȣ ������ get�Լ�
int DATA_getSinglePassword(int index) {
	return password[index];
}
int DATA_getPasswordSize(void) {
	return passwordSize;
}

// locker number data
// ���� ���õǾ��� ��Ŀ�� ��ȣ�� �����ϴ� ����
int SelectedLockerNumber = LOCKER_NUMBER_UNSELECTED;

//SelectedLockerNumber�� get,set �Լ�
int DATA_getSelectedLockerNumber(void) {
	return SelectedLockerNumber;
}
void DATA_setSelectedLockerNumber(int num) {
	SelectedLockerNumber = num;
}
// end locker number data

// uart data
// ������ ������¿� ���� �÷��� ������ get,set�Լ�
bool _isServerConnected = false;
bool DATA_isServerConnected(void) {
	return  _isServerConnected;
}
void DATA_setIsServerConnected(bool flag) {
	_isServerConnected = flag;
}

//��Ŀ�� �������� ��û�� �������� �����ϴ� ����
//��Ŀ ��ȣ�� ��й�ȣ�� �Է��̵Ǹ� true�� �ǰ�
//��û������ ������ ���� false�� ����ȴ�
bool _isRequestedPermission = false;
bool DATA_isRequestedPermission(void) {
	return _isRequestedPermission;
}
void DATA_setRequsetPermission(bool value) {
	_isRequestedPermission = value;
}

//������ ������ Ȯ���� ���� ī���� ���� �� �Լ�
//�޼����� �´ٸ� �����̵ǰ� ī��Ʈ�� 0�̵Ǹ�
//������ ������������ ó���Ѵ� 
int connectionCounter = TOP_CONNECTION_COUNTER;
void DATA_connectionCountDown(void) {
	connectionCounter--;
}
bool DATA_isConnectionCountZero(void)  {
	if (connectionCounter <= 0)
		return true;
	else		
		return false;
}
void DATA_resetConnectionCounter(void) {
	connectionCounter = TOP_CONNECTION_COUNTER;
}

//��û �޼����� �������� �ϱ����� ī��Ʈ���� �� �Լ�
//ī��Ʈ ���� �پ��� 0�̵Ǹ� �������� �� �� �ְ� �����
int _requestSendingMsgCounter = 0;
void DATA_sendingMsgCountDown(void) {
	_requestSendingMsgCounter--;
	if (_requestSendingMsgCounter < 0)
		_requestSendingMsgCounter = 0;
}
void DATA_resetSendingMsgCounter(void) {
	_requestSendingMsgCounter = SENDING_MSG_COUNT_TOP;
}
bool DATA_isSendingMsgCounterZero(void){
	if (_requestSendingMsgCounter == 0)
		return true;
	return false;
}
// end uart data



