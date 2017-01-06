///*----------------------------------------------------------------------------
// *      doorLock_data.h
// *----------------------------------------------------------------------------
// *      Name:    doorLock_data.h
// *---------------------------------------------------------------------------*/
//

#include <LPC17xx.H>
#define bool	int 
#define false	0
#define true	1

//system state�� ���� ���� 
#define SYSTEM_STATE_SELECT_LOCKER_NUMBER	0 //��Ŀ ��ȣ�� �����ϴ� ����
#define SYSTEM_STATE_ENTER_PASSWORD			1 //��й�ȣ�� �Է¹޴� ����
#define SYSTEM_STATE_OPEN_FAIL				2 //��Ŀ ���� ���� ����
#define SYSTEM_STATE_OPEN_SUCCESS			3 //��Ŀ ���� ���� ����
#define SYSTEM_STATE_WAITING_RESPONSE		4 //��Ŀ ���� ��û�� ���� ��� ����
int DATA_getSystemState(void);
void DATA_setSystemState(int mode);
bool DATA_isChangedSystemState(void);

//timer data
#define YEAR	0
#define MONTH	1 
#define DAY		2
#define HOUR	3
#define MINUTE	4
#define SECOND	5
void DATA_timeUpdate(void);
void DATA_setTime(int YYYY, int MM, int DD, int hh, int mm, int ss);
int DATA_getTime(int timeType);
//end timer data

//selected locker number data
#define LOCKER_NUMBER_UNSELECTED	0
int DATA_getSelectedLockerNumber(void);
void DATA_setSelectedLockerNumber(int num);
//end selected locker number data

//password data
#define PASSWORD_SIZE	4
void DATA_clearPassword(void);
void DATA_pushbackPassword(int pwdValue);
int DATA_getSinglePassword(int index);
int DATA_getPasswordSize(void);
void DATA_popbackPassword(void);
//end password data

//uart data
bool DATA_isServerConnected(void);
void DATA_setIsServerConnected(bool flag);

bool DATA_isRequestedPermission(void);
void DATA_setRequsetPermission(bool value);

#define SENDING_MSG_COUNT_TOP	1
void DATA_sendingMsgCountDown(void);
void DATA_resetSendingMsgCounter(void);
bool DATA_isSendingMsgCounterZero(void);

#define TOP_CONNECTION_COUNTER	100
void DATA_connectionCountDown(void);
bool DATA_isConnectionCountZero(void);
void DATA_resetConnectionCounter(void);
//end uart data






