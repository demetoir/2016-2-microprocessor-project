///*----------------------------------------------------------------------------
// *      doorLock_data.c
// *----------------------------------------------------------------------------
// *      Name:    doorLock_data.c
// *---------------------------------------------------------------------------*/
//
#include "doorlock_data.h"

// system state data
//현재의 시스템 모드를 저장하는 변수
int systemMode = SYSTEM_STATE_SELECT_LOCKER_NUMBER;
//현재의 시스템 모드가 변했는지 저장하는 플래그 변수
bool _isChangedSystemMode = false;

//현재 시스템 모드가 변경되었는지 확인하는 질의함수
bool DATA_isChangedSystemState(void) {
	//모드가 변경된이후 다음 변경을 감지하기위해
	//_isChangedSystemMode를 false로 만들고
	//true를 반환한다
	if (_isChangedSystemMode == true) {
		_isChangedSystemMode = false;
		return true;
	}
	return false;
}
//systemModed의 getter함수
int DATA_getSystemState(void) {
	return systemMode;
}
//systemModed의 setter함수
void DATA_setSystemState(int mode) {
	//만약 이전의 모드와 다르다면 모드가 변경되었으므로
	//_isChangedSystemMode를 true 만든다
	if (systemMode != mode) {
		systemMode = mode;
		_isChangedSystemMode = true;
	}
}
// end system mode data

// time data
// 날짜 및 시간을 저장하기위한 변수들
int year = 1999;
int month = 12;
int day = 31;
int hour  = 23;
int minute = 59;
int second = 40;
//각 달의 날수를 저장하는 배열
int daysInMonth[13] = { 1,31,28,31,30,31,30,31,31,30,31,30,31 };

//현재 날짜및 시간에서 1초가 지난 후의 시간으로 업데이트한다
void DATA_timeUpdate(void) {
	//각 초, 분, 시, 일, 월, 연 순서로 각각의 진법으로 올림수 나눗셈을 후
	//몫은 오버플로우된 올림수로 나머지를 각 단위로 업데이트 하면 
	//1초 후의 시간으로 업데이트된다

	//각 단위의 올림수 이것은 다음 단위에 더 해준다 1초를 업데이트하기위해
	//미리 올림수를 1로 시작한다
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

	//날(일)의 경우 0일은 존재하지않으므로 처음이 -1
	//마지막에 +1하는것으로 위의 연산을 적용할수있다
	day--;
	day += carry;
	//날(일)의 경우 매달마다 수가 달라지므로 
	//daysInMonth[month]를 이용하여 연산을 동일하게 적용한다
	carry = day / (daysInMonth[month]);
	day %= (daysInMonth[month]);
	day++;

	//월의 경우도 0월은 존재하지않으므로 처음이 -1
	//마지막에 +1하는것으로 위의 연산을 동일하게 적용할수있다
	month--;
	month += carry;
	carry = month / 12;
	month %= 12;
	month++;

	year += carry;
}
//날짜 및 시간의 setter 함수
void DATA_setTime(int YYYY, int MM, int DD, int hh, int mm, int ss ) {
	year = YYYY;
	month = MM;
	day = DD;
	hour = hh;
	minute = mm;
	second = ss;
}

//날짜 및 시간의 getter 함수
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

//입력된 비밀번호와 길이를 저장하는 변수
int password[PASSWORD_SIZE];
int passwordSize = 0;

//입력된 비밀번호를 삭제한다
void DATA_clearPassword(void) {
	password[0] = 0;
	password[1] = 0;
	password[2] = 0;
	password[3] = 0;
	passwordSize = 0;
}
//비밀번호에 마지막 한자리 더 추가 하도록 만든다
//4자리가 넘어가는경우 추가하지않는다
void DATA_pushbackPassword(int pwdValue) {
	if (passwordSize < 4) {
		password[passwordSize] = pwdValue;
		passwordSize++;
	}
}
//비밀번호의 마지막자리를 한자리를 제거한다  
//0개 이하일때는 제거하지 않는다
void DATA_popbackPassword(void) {
	if (password > 0) {
		passwordSize--;
		password[passwordSize] = 0;
	}
}
//비밀번호와 비밀번호 길이의 get함수
int DATA_getSinglePassword(int index) {
	return password[index];
}
int DATA_getPasswordSize(void) {
	return passwordSize;
}

// locker number data
// 현재 선택되어진 락커의 번호를 저장하는 변수
int SelectedLockerNumber = LOCKER_NUMBER_UNSELECTED;

//SelectedLockerNumber의 get,set 함수
int DATA_getSelectedLockerNumber(void) {
	return SelectedLockerNumber;
}
void DATA_setSelectedLockerNumber(int num) {
	SelectedLockerNumber = num;
}
// end locker number data

// uart data
// 서버와 연결상태에 대한 플래그 변수와 get,set함수
bool _isServerConnected = false;
bool DATA_isServerConnected(void) {
	return  _isServerConnected;
}
void DATA_setIsServerConnected(bool flag) {
	_isServerConnected = flag;
}

//락커를 열기위한 요청이 존재함을 설정하는 변수
//락커 번호와 비밀번호가 입력이되면 true가 되고
//요청에대한 응답이 오면 false로 변경된다
bool _isRequestedPermission = false;
bool DATA_isRequestedPermission(void) {
	return _isRequestedPermission;
}
void DATA_setRequsetPermission(bool value) {
	_isRequestedPermission = value;
}

//서버와 연결을 확인을 위한 카운터 변수 및 함수
//메세지가 온다면 갱신이되고 카운트가 0이되면
//연결이 끊어진것으로 처리한다 
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

//요청 메세지의 재전송을 하기위한 카운트변수 및 함수
//카운트 값이 줄어들어 0이되면 재전송을 할 수 있게 만든다
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



