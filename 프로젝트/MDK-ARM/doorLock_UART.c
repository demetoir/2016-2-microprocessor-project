///*----------------------------------------------------------------------------
// *      doorlock_UART.c
// *----------------------------------------------------------------------------
// *      Name:    doorlock_UART.c
// *---------------------------------------------------------------------------*/

#include "doorLock_UART.h"

//UART의 연결상태를 나타내는 enum 선언
typedef enum {
	UART_CONNECTED, 
	UART_DISCONNECTED
} UART_CONNECTION_STATE;
//UART의 연결상태를 저장하는 변수
UART_CONNECTION_STATE UART_currentState = UART_DISCONNECTED;

//전송받은 데이터를 저장하는 버퍼
uint8_t receivedBuffer[UART_BUFFER_SIZE];	// 문자열 저장 버퍼
//버퍼에 Write할 위치
uint8_t bufferCount = 0;			// 현재 Write 버퍼 위치


//uart를 초기화하는 함수
//uart 초기화시  라이브러리의 기본설정인
//Baud_rate = 115200;
//Databits = UART_DATABIT_8;
//Parity = UART_PARITY_NONE;
//Stopbits = UART_STOPBIT_1;
//으로 설정하였다
void UART_init(void) {
	// UART0 초기화
	Uart0_Init();	

	//초기화 후 테스트용 메세지를 전송한다
	UARTPuts(LPC_UART0, "hello world this is arm processor \n");
	
	//초기연결상태를 초기화한다
	DATA_setIsServerConnected(false);
}

//전송받은 버퍼를 초기화 한다
void UART_clearReceivedBuffer(void) {
	memset(receivedBuffer, 0x00, sizeof(receivedBuffer)); 
	bufferCount = 0;
}

//polling 방식으로 uart0로 메세지를 전송한다
//rs323 to usb 케이블 내부에는 버퍼가 존재한다
//rs323 -> usb 로 가는 방향에 버퍼가 존재하여 
//메세지를 단순히 폴링방식으로 보내주어도 된다
//이경우는 수신측에서 버퍼에 오버플로우가 일어나지
//안도록 폴링방식으로 수신받아야한다
void UART_sendMsg(char msg[UART_BUFFER_SIZE]) {
	UARTPuts(LPC_UART0, msg);
}

//수신받는경우 또한 폴리방식으로 수신받은 한바이트에대하여
//echo를 전송하여 성공적으로 수신받음을 알려주어서 전송측이 
//다음 바이트를 전송하는것을 반복하여 msg를 수신한다. 
//수신에 성공하면 수신 버퍼에 메세지를 저장하고 true를 반환
//수신 받은것이 없거나, 버퍼가 가득찬 경우 false를 반환한다.
bool UART_receiveMsg(char buffer[UART_BUFFER_SIZE]) {
	//시용되어질 변수들 초기화
	bool isReceivedMsg = false;
	int i = 0;
	char received_char;
	bool isReceivedByte = false;
	char tempReceivedMsg[UART_BUFFER_SIZE];

	//버퍼가 가득차면 수신받을수 없으므로 종료한다
	if (bufferCount == UART_BUFFER_SIZE) return false;

	//현재 버퍼에서 최대로 수신받을수있는 만큼만 수신을 검사한다
	for (i = bufferCount; i < UART_BUFFER_SIZE; i++) {
		//polling 방식으로 byte를 수신한다
		isReceivedByte = UART_Receive(LPC_UART0, 
			(uint8_t *)tempReceivedMsg, UART_BUFFER_SIZE, NONE_BLOCKING);
	
		//수신받은 것이 없을경우 생략한다
		if (isReceivedByte == false) 
			continue;

		//수신받은 byte를 가져온다
		received_char = tempReceivedMsg[0];

		//수신받은 byte에 대하여 echo를 전송한다
		UARTPuts(LPC_UART0, &received_char);

		//수신받은 byte를 버퍼에 저장한다
		buffer[bufferCount] = received_char;
		bufferCount++;

		//수신받은 byte가 개행문자일경우 메세지의 끝이므로 
		//수신을 성공하였다
		if (received_char == '\n') {
			isReceivedMsg = true;
			break;
		}
	}

	//수신 받음의 플래그 변수를 반환한다
	return isReceivedMsg;
}

// make msg
//UART_make를 접두사로 가지는 함수들은 모두 msg mark를 조합하여
//보낼 메세지를 만든다

//handshake msg를 만든다
void UART_makeHandShakeMsg(char msg[UART_BUFFER_SIZE]) {
	memset(msg, 0x00, sizeof(msg));
	sprintf(msg, "%s %s", 
		UART_MSG_HEADMARK_handshake_receive, UART_MSG_FOOTMARK_end);
}

//check connection msg를 만든다
void UART_makeCheckConnectionMsg(char msg[UART_BUFFER_SIZE]) {
	memset(msg, 0x00, sizeof(msg));
	sprintf(msg, "%s %s", 
		UART_MSG_HEADMARK_check_connection, UART_MSG_FOOTMARK_end);
}

//discconection msg를 만든다
void UART_makeDisconnectionMsg(char msg[UART_BUFFER_SIZE]) {
	memset(msg, 0x00, sizeof(msg));
	sprintf(msg, "%s %s %s", 
		UART_MSG_HEADMARK_disconnect, UART_MSG_HEADMARK_success, 
		UART_MSG_FOOTMARK_end);
}

//락커를 열기를 위한 권한을 요청하는 msg
void UART_makeRequestPermssionMsg(char msg[UART_BUFFER_SIZE]) {
	char strPassword[10];
	int lockerNumber = 0;

	memset(msg, 0x00, sizeof(msg));
	memset(strPassword, 0x00, sizeof(strPassword));
	//보낼 비밀번호와 락커 번호를 가져온다
	strPassword[0] = DATA_getSinglePassword(0) + '0';
	strPassword[1] = DATA_getSinglePassword(1) + '0';
	strPassword[2] = DATA_getSinglePassword(2) + '0';
	strPassword[3] = DATA_getSinglePassword(3) + '0';
	lockerNumber = DATA_getSelectedLockerNumber();

	sprintf(msg, "%s %s %d %s", UART_MSG_HEADMARK_request_permission, 
		strPassword, lockerNumber, UART_MSG_FOOTMARK_end);
}
// end make msg

// check msg
//uart_is를 접두사로 가지는 함수들은 msg를 검사하는 
//함수들이다. msg mark를 이용하여 올바른 msg를 생성하고
//인자로 들어온 msg를 검사하여 해당하는 msg인지 검사한다

//msg가 end mark로 끝나는지 검사한다. 
//이 함수로 msg가 완전히 끝나는지 검사한다
//공백을 구분으로 토큰으로 나누어 마지막 토큰이 msg
//end mark인것을 검사한다
bool UART_isMsgMeetEndMark(char msg[UART_BUFFER_SIZE]) {
	//strtok로 msg 넣는다면 msg의 내용이 변경되므로 
	//임시 버퍼로 복사하여검사한다
	char tempBuffer[UART_BUFFER_SIZE];
	//end mark를 저장할 변수
	char endMark[10]; 
	//token의 시작 포이터를 저장하는 변수
	char *tokenPtr = NULL;
	//문자열들을 초기화한다
	memset(endMark, 0x00, sizeof(endMark));
	memset(tempBuffer, 0x00, sizeof(tempBuffer));
	sprintf(endMark, "%s", UART_MSG_FOOTMARK_end);
	strcpy(tempBuffer, msg);

	//임시 버퍼를 ' '(공백)으로 토큰을 나누고 token을 검사한다
	tokenPtr = strtok(tempBuffer, " ");
	while (tokenPtr != NULL) {
		if (strcmp(tokenPtr, endMark) == 0) return true;
		tokenPtr = strtok(NULL, " ");
	}
	return false;
}

//hand shake msg 인지 검사한다
//handShake msg format :
//"UART_MSG_MARK_disconnect UART_MSG_MARK_end"
bool UART_ishandShakeMsg(char msg[UART_BUFFER_SIZE]) {
	char handShakeMsg[UART_BUFFER_SIZE];
	memset(handShakeMsg, 0x00, sizeof(handShakeMsg));

	sprintf(handShakeMsg, "%s %s", 
		UART_MSG_HEADMARK_handshake_send, UART_MSG_FOOTMARK_end);
	if (strcmp(msg, handShakeMsg) == 0)
		return true;
	else
		return false;
}

//disconection msg인지 검사한다
//disconection msg format :
//"UART_MSG_MARK_disconnect UART_MSG_MARK_end"
bool UART_isDisconnectMsg(char msg[UART_BUFFER_SIZE]) {
	char disconnectionMsg[UART_BUFFER_SIZE];
	memset(disconnectionMsg, 0x00, sizeof(disconnectionMsg));

	sprintf(disconnectionMsg, "%s %s", 
		UART_MSG_HEADMARK_disconnect, UART_MSG_FOOTMARK_end);
	if (strcmp(msg, disconnectionMsg) == 0)
		return true;
	return
		false;
}

//시간을 동기화하는 msg인지 검사한다
// SycTime Format msg :
//"UART_MSG_MARK_SyncTime yyyyMMddhhmmss UART_MSG_MARK_end"
//yyyy	: year 
//MM	: month 
//dd	: day 
//hh	: hour
//mm	: minute 
//ss	: second
bool UART_isTimeSyncMsg(char msg[UART_BUFFER_SIZE]) {
	int i;
	char msgMark[10];
	bool _isTimeSyncMsg = true;

	//msg의 첫번째 mark가 UART_MSG_MARK_SyncTime 인지 검사
	memset(msgMark, 0x00, sizeof(msgMark));
	strcpy(msgMark, UART_MSG_HEADMARK_SyncTime);
	for (i = 0; i < strlen(msgMark); i++) {
		if (msgMark[i] != msg[i])
			_isTimeSyncMsg = false;
	}

	//msg mark가 UART_MSG_MARK_end 인지 검사
	memset(msgMark, 0x00, sizeof(msgMark));
	strcpy(msgMark, UART_MSG_FOOTMARK_end);
	for (i = 0; i < strlen(msgMark); i++) {
		if (msgMark[i] != msg[i + 24])
			_isTimeSyncMsg = false;
	}

	return _isTimeSyncMsg;
}

//check connection msg인지 검사
//check connection msg format :
//"UART_MSG_MARK_check_connection UART_MSG_MARK_end"
bool Uart_isCheckConnectionMsg(char msg[UART_BUFFER_SIZE]) {
	char checkConnectionMsg[UART_BUFFER_SIZE];

	memset(checkConnectionMsg, 0x00, sizeof(checkConnectionMsg));
	sprintf(checkConnectionMsg, "%s %s", 
		UART_MSG_HEADMARK_check_connection, UART_MSG_FOOTMARK_end);

	if (strcmp(msg, checkConnectionMsg) == 0)
		return true;
	else
		return false;
}

//check connection msg인지 검사
//check connection msg format :
//"UART_MSG_MARK_check_connection UART_MSG_MARK_end"
bool UART_isReseponsePermissionMsg(char msg[UART_BUFFER_SIZE]) {
	bool _isReseponsePermissionMsg = true;
	char cmp_str[UART_BUFFER_SIZE];
	int i = 0;
	int str_size = strlen(UART_MSG_HEADMARK_response_permission);

	memset(cmp_str, 0x00, sizeof(cmp_str));
	sprintf(cmp_str, "%s", UART_MSG_HEADMARK_response_permission);

	for (i = 0; i < str_size; i++) {
		if (cmp_str[i] != msg[i])
			_isReseponsePermissionMsg = false;
	}

	return _isReseponsePermissionMsg;
}
//ResponsePemissionOK msg인지 검사
//ResponsePemissionOK msg  format :
//"UART_MSG_MARK_check_connection UART_MSG_MARK_end"
bool UART_isResponsePemissionOK(char msg[UART_BUFFER_SIZE]) {
	bool ret = false;
	char cmp_str[UART_BUFFER_SIZE];
	memset(cmp_str, 0x00, sizeof(cmp_str));
	sprintf(cmp_str, "%s %s %s", 
		UART_MSG_HEADMARK_response_permission, UART_MSG_BODYMARK_yes, 
		UART_MSG_FOOTMARK_end);

	if (strcmp(cmp_str, msg) == 0)
		ret = true;
	return ret;
}
// end check msg

//uart로 수신받은 시간으로 동기화한다
void UART_syncTime(char raw_msg[UART_BUFFER_SIZE]) {
	int i;
	//추출한 시간및 날짜 정보를 저장할 문자열
	int strTime[20];
	//정수로 변환된 시간및 날짜를 저장할 변수들
	int year = 1999;
	int month = 12;
	int day = 31;
	int hour = 23;
	int minute = 59;
	int second = 40;

	//받은 메세지중 시간부분의 문자열을 추출한다
	memset(strTime, 0x00, sizeof(strTime));
	for (i = 0; i < 14; i++) {
		strTime[i] = raw_msg[9 + i] - '0';
	}

	//추출된 문자열을 정수로 변환한다
	year = strTime[0] * 1000 + strTime[1] * 100
			+ strTime[2] * 10 + strTime[3];
	month = strTime[4] * 10 + strTime[5];
	day	= strTime[6] * 10 + strTime[7];
	hour = strTime[8] * 10 + strTime[9];
	minute = strTime[10] * 10 + strTime[11];
	second = strTime[12] * 10 + strTime[13];

	//정수로 변환된 시간및 날짜를 설정한다
	DATA_setTime(year, month, day, hour, minute, second);
}


//uart의 mainTask함수
//서버와 연결 및 메세지 전송 및 수신을처리한다
void UART_mainTask(void) {
	//보낼 메세지를 위한 버퍼 및 초기화
	char sendingBuffer[UART_BUFFER_SIZE];
	memset(sendingBuffer, 0, sizeof(sendingBuffer));

	//uart의 상태가 UART_DISCONNECTED일때
	if (UART_currentState == UART_DISCONNECTED) {
		DATA_setIsServerConnected(false);
		UART_receiveMsg((char *)receivedBuffer);

		//수신받은 메세지가 완전한 format을 이룰때
		if (UART_isMsgMeetEndMark((char *)receivedBuffer) == true) {
			//hand shake msg 수신시 msg에 대한 응답을하고 
			//uart 상태를 변경한다
			if (UART_ishandShakeMsg((char *)receivedBuffer)) {
				UART_makeHandShakeMsg((char*) sendingBuffer);
				UART_sendMsg(sendingBuffer);

				UART_currentState = UART_CONNECTED;
				DATA_setIsServerConnected(true);
			}
			//disconection msg 수신시 msg에 대한 응답을하고 
			//uart 상태를 변경한다
			else if (UART_isDisconnectMsg((char *)receivedBuffer)) {
				UART_makeDisconnectionMsg(sendingBuffer);
				UART_sendMsg(sendingBuffer);

				UART_currentState = UART_DISCONNECTED;
				DATA_setIsServerConnected(false);
				DATA_resetConnectionCounter();
			}

			//버프를 비워서 다음 msg를 받을수있도록만든다
			UART_clearReceivedBuffer();
		}
	}
	//uart 연결중일때
	else if (UART_currentState == UART_CONNECTED) {
		//먼저 msg를 수신 받는다 
		UART_receiveMsg((char *)receivedBuffer);

		//수신받은 메세지가 완전한 format을 이룰때
		//msg를 처리한후 receivedBuffer비워 다음 메세지를 
		//수신받을 수있도록 만든다
		if (UART_isMsgMeetEndMark((char*)receivedBuffer) == true) {

			//disconection msg 수신시 msg에 대한 응답을하고 
			//uart 상태를 변경한다
			if (UART_isDisconnectMsg((char *)receivedBuffer)) {
				UART_makeDisconnectionMsg(sendingBuffer);
				UART_sendMsg(sendingBuffer);

				UART_currentState = UART_DISCONNECTED;
				DATA_setIsServerConnected(false);
				DATA_resetConnectionCounter();
			}

			//time sync msg 수신시 msg에 대한 응답을하고 
			//시간을 동기화한다
			if (UART_isTimeSyncMsg((char*)receivedBuffer) == true) {
				UART_syncTime((char*)receivedBuffer);
				DATA_resetConnectionCounter();
			}

			if (UART_isReseponsePermissionMsg((char *)receivedBuffer) == true) {
				//비밀번호가 맞으면 시스템 모드를 변경하고 락커 열도록 설정한다
				if (UART_isResponsePemissionOK((char *)receivedBuffer) == true) {
					DATA_setSystemState(SYSTEM_STATE_OPEN_SUCCESS);
					MOTOR_setOpenLocker();
				}
				//틀린경우 시스템모드를 변경한다
				else {
					DATA_setSystemState(SYSTEM_STATE_OPEN_FAIL);
				}

				//lcd 화면을 갱신한다
				LCD_setRefresh();
				//요청에 대한 응답이 왔으므로 더이상 메시지를 보내지않기위해 
				//요청메세지가 없음으로 변경한다
				DATA_setRequsetPermission(false);
				//성공적인 메세지 교환이므로 연결위한 카운터를 초기화한다
				DATA_resetConnectionCounter();
			}

			//받은 메세지가 서버와 연결을 확인하는 메세지일 경우
			if (Uart_isCheckConnectionMsg((char *)receivedBuffer) == true) {
				//메세지를 조합하여 전송한다
				UART_makeCheckConnectionMsg((char *)sendingBuffer);
				UART_sendMsg(sendingBuffer);

				//서버와 연결상태를 갱신한다
				DATA_setIsServerConnected(true);
				DATA_resetConnectionCounter();
			}

			//버프를 비워서 다음 msg를 받을수있도록만든다
			UART_clearReceivedBuffer();
		}

		//전송할 락커를 열기위해 보낼 메세지가 있을경우 
		if (DATA_isRequestedPermission() == true) {
			//메세지 전송하고 일정시간동안 응답이 없으면
			//응답이 올때까지 재전송을 한다.
			//처음
			if (DATA_isSendingMsgCounterZero() == true) {
				//메세지를 조합하여 전송한다
				UART_makeRequestPermssionMsg((char*)sendingBuffer);
				UART_sendMsg((char *)sendingBuffer);

				//화면을 갱신하도록 설정한다
				LCD_setRefresh();
				//메세지의 재전송 카운터를 초기화한다
				DATA_resetSendingMsgCounter();
			}
		}	

		//일정시간동안 메세지를 받지 못한다면 연결이 끊어진것으로 처리한다
		if (DATA_isConnectionCountZero()) {
			DATA_resetConnectionCounter();
			UART_currentState = UART_DISCONNECTED;
		}
	}

	return;
}





