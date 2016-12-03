///*----------------------------------------------------------------------------
// *      doorLock_data.h
// *----------------------------------------------------------------------------
// *      Name:    doorLock_data.h
// *---------------------------------------------------------------------------*/
//

#include <LPC17xx.H>
#define bool int 
#define false 0
#define true 1

#define YEAR 0
#define MONTH 1 
#define DAY 2
#define HOUR 3
#define MINUTE 4
#define SECOND 5

bool isRequestedPermission(void);

void time_update(void);
void setTimer(int YYYY, int MM, int DD, int hh, int mm, int ss);
int getTime(int timeType);
//request flag
bool isRequestedPermission(void);
void setRequsetPermission(bool value);

// connection info ..?

//password data
#define PASSWORD_SIZE 4
#define	MAX_PASSWORD_FAIL 3
void clearPwd(void);
void setPwd(int pwdValue);
int getPwd_i(int index);
int getPwdSize(void);
void popbackPwd(void);

bool isServerConnected(void);
void set_isServerConnected(bool flag);

#define TOP_CONNECTION_COUNTER 100
void connectionCountDown(void);
bool isConnectionCountZero(void);
void resetConnectionCounter(void);


int getSelectedLockerNumber(void);
void setSelectedLockerNumber(int num);



#define SYSTEM_MODE_SELECT_LOCKER_NUMBER		0
#define SYSTEM_MODE_ENTER_PASSWORD				1
#define SYSTEM_MODE_OPEN_FAIL					2
#define SYSTEM_MODE_OPEN_SUCCESS				3
#define SYSTEM_MODE_UNUSINGLOCKER				4
#define SYSTEM_MODE_WAITING						5
int getCurrentSystemMode(void);
void setCurrentSustemMode(int mode);
bool isChangedCurrentSystemMode(void);

#define SENDING_MSG_COUNT_TOP			1
void sendingMsgCountDown(void);
void resetSendingMsgCounter(void);
bool isSendingMsgCounterZero(void);





