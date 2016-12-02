///*----------------------------------------------------------------------------
// *      doorLock_data.c
// *----------------------------------------------------------------------------
// *      Name:    doorLock_data.c
// *---------------------------------------------------------------------------*/
//
#include "doorlock_data.h"


// time data
int year = 1999;
int month = 12;
int day = 31;
int hour  = 23;
int minute = 59;
int second = 40  ;

int dayInmonth[13] = { 1,31,28,31,30,31,30,31,31,30,31,30,31 };
void time_update(void) {
	int carry;
	second++;
	carry = second / 60;
	second %= 60;

	minute += carry;
	carry = minute / 60;
	minute %= 60;

	hour += carry;
	carry = hour / 24;
	hour %= 24;

	day--;
	day += carry;
	carry = day / (dayInmonth[month]);
	day %= (dayInmonth[month]);
	day++;

	month--;
	month += carry;
	carry = month / 12;
	month %= 12;
	month++;

	year += carry;
}
void setTimer(int YYYY, int MM, int DD, int hh, int mm, int ss  ) {
	year = YYYY;
	month = MM;
	day = DD;
	hour = hh;
	minute = mm;
	second = ss;
}
int getTime(int timeType) {
	switch (timeType){
	case YEAR: return year;
	case MONTH: return month;
	case DAY: return day;
	case HOUR: return hour;
	case MINUTE: return minute;
	case SECOND: return second;
	default:return  -1; 
		break;
	}
}


//request flag
bool isRequestPermission = false;
bool isRequestedPermission(void) {
	return isRequestPermission;
}
void setRequsetPermission(bool value) {
	isRequestPermission = value;
}

// connection info ..?
bool isconnected = false;

//password data
int pwd[PASSWORD_SIZE];
int pwdSize = 0;
void clearPwd(void) {
	pwd[0] = 0;
	pwd[1] = 0;
	pwd[2] = 0;
	pwd[3] = 0;
	pwdSize = 0;
}
void setPwd_i(int index, int pwdValue) {
	if (pwdSize == index) {
		pwd[index] = pwdValue;
		pwdSize++;
	}
}
int getPwd_i(int index) {
	return pwd[index];
}
int getPwdSize(void) {
	return pwdSize;
}


// log
//50개 정도 보관함.



