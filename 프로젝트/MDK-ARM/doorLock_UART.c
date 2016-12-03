///*----------------------------------------------------------------------------
// *      doorlock_UART.c
// *----------------------------------------------------------------------------
// *      Name:    doorlock_UART.c
// *---------------------------------------------------------------------------*/

#include "doorLock_UART.h"

typedef enum {
	UART_CONNECTED, UART_DISCONNECTED
} UART_CONNECTION_STATE;
UART_CONNECTION_STATE UART_current_state = UART_DISCONNECTED;

uint8_t ch;
uint8_t aTxBuffer[UART_BUFFER_SIZE];	// 문자열 저장 버퍼
uint8_t buffer_count = 0;			// 현재 Write 버퍼 위치

bool UART_refresh = false;
int UART_refresh_time_count = 0;
int UART_handShake_time_count = 0;
int UART_LiveMsgTimer = 0;

void UART_init(void) {
	Uart0_Init();	// UART0 초기화
	UARTPuts(LPC_UART0, "hello world this is arm processor \n");
	UART_handShake_time_count = 0;
	UART_LiveMsgTimer = 0;
	set_isServerConnected(false);
}

void UART_clearBuffer(void) {
	memset(aTxBuffer, 0x00, sizeof(aTxBuffer)); // Buffer 클리어					
	buffer_count = 0;
}

void UART_sendMsg(char msg[UART_BUFFER_SIZE]) {
	UARTPuts(LPC_UART0, msg);
}

bool UART_receiveMsg(char buffer[UART_BUFFER_SIZE]) {
	bool isReceivedMsg = false;
	int i = 0;
	char received_char;
	char received_msg[UART_BUFFER_SIZE];
	bool is_received_char = false;

	if (buffer_count == UART_BUFFER_SIZE) return false;

	for (i = buffer_count; i < UART_BUFFER_SIZE; i++) {
		is_received_char = UART_Receive(LPC_UART0, (uint8_t *)received_msg, UART_BUFFER_SIZE, NONE_BLOCKING);	// Polling으로 Data 읽어오기
		if (is_received_char == 0) continue;

		received_char = received_msg[0];

		//receive echo
		UARTPuts(LPC_UART0, &received_char);

		//buffer count 수정 필요
		buffer[buffer_count] = received_char;
		buffer_count++;

		if (received_char == '\n') {
			isReceivedMsg = true;
			break;
		}
	}

	if (isReceivedMsg)
		return true;
	else
		return false;
}


void UART_makeHandShakeMsg(char msg[UART_BUFFER_SIZE]) {
	memset(msg, 0x00, sizeof(msg));
	sprintf(msg, "%s %s", UART_MSG_MARK_handshake_receive, UART_MSG_MARK_end);
}

void UART_makeCheckConnectionMsg(char msg[UART_BUFFER_SIZE]) {
	memset(msg, 0x00, sizeof(msg));
	sprintf(msg, "%s %s", UART_MSG_MARK_check_connection, UART_MSG_MARK_end);
}

void UART_makeDisconnectionMsg(char msg[UART_BUFFER_SIZE]) {
	memset(msg, 0x00, sizeof(msg));
	sprintf(msg, "%s %s %s", UART_MSG_MARK_disconnect, UART_MSG_MARK_success, UART_MSG_MARK_end);
}

void UART_makeRequestPermssionMsg(char msg[UART_BUFFER_SIZE]) {
	//get password
	char strPassword[10];
	int lockNum = 0;
	memset(msg, 0x00, sizeof(msg));
	memset(strPassword, 0x00, sizeof(strPassword));
	strPassword[0] = getPwd_i(0) + '0';
	strPassword[1] = getPwd_i(1) + '0';
	strPassword[2] = getPwd_i(2) + '0';
	strPassword[3] = getPwd_i(3) + '0';
	lockNum = getSelectedLockerNumber();

	sprintf(msg, "%s %s %d %s", UART_MSG_MARK_request_permission, strPassword, lockNum, UART_MSG_MARK_end);
}


bool UART_isEnd(char msg[UART_BUFFER_SIZE]) {
	//end\n
	char cTempBuffer[UART_BUFFER_SIZE];
	char mark[10];
	char *cPtr = NULL;
	memset(mark, 0x00, sizeof(mark));
	memset(cTempBuffer, 0x00, sizeof(cTempBuffer));
	sprintf(mark, "%s", UART_MSG_MARK_end);
	strcpy(cTempBuffer, msg);

	cPtr = strtok(cTempBuffer, " ");
	while (cPtr != NULL) {
		if (strcmp(cPtr, mark) == 0) return true;
		cPtr = strtok(NULL, " ");
	}
	return false;
}

bool UART_ishandShakeMsg(char msg[UART_BUFFER_SIZE]) {
	char handShakeMsg[UART_BUFFER_SIZE];
	memset(handShakeMsg, 0x00, sizeof(handShakeMsg));

	sprintf(handShakeMsg, "%s %s", UART_MSG_MARK_handshake_send, UART_MSG_MARK_end);
	if (strcmp(msg, handShakeMsg) == 0)
		return true;
	else
		return false;
}

bool UART_isDisconnectMsg(char msg[UART_BUFFER_SIZE]) {
	char disconnectionMsg[UART_BUFFER_SIZE];
	memset(disconnectionMsg, 0x00, sizeof(disconnectionMsg));

	sprintf(disconnectionMsg, "%s %s", UART_MSG_MARK_disconnect, UART_MSG_MARK_end);
	if (strcmp(msg, disconnectionMsg) == 0)
		return true;
	return
		false;
}

bool UART_isTimeSycMsg(char msg[UART_BUFFER_SIZE]) {
	char msgMark[10];
	bool ret = true;
	int i;
	memset(msgMark, 0x00, sizeof(msgMark));

	// SycTime Format : "Syc_Time yyyymmddhhmmss end\n"
	//							year month day hour minute second
	strcpy(msgMark, UART_MSG_MARK_SycTime);
	for (i = 0; i < 7; i++) {
		if (msgMark[i] != msg[i])
			ret = false;
	}

	memset(msgMark, 0x00, sizeof(msgMark));
	strcpy(msgMark, UART_MSG_MARK_end);

	for (i = 0; i < 4; i++) {
		if (msgMark[i] != msg[i + 24])
			ret = false;
	}

	return ret;
}

bool Uart_isCheckConnectionMsg(char msg[UART_BUFFER_SIZE]) {
	char checkConnectionMsg[UART_BUFFER_SIZE];
	memset(checkConnectionMsg, 0x00, sizeof(checkConnectionMsg));
	sprintf(checkConnectionMsg, "%s %s", UART_MSG_MARK_check_connection, UART_MSG_MARK_end);
	if (strcmp(msg, checkConnectionMsg) == 0)
		return true;
	else
		return false;
}

bool UART_isReseponsePermissionMsg(char msg[UART_BUFFER_SIZE]) {
	bool ret = true;
	char cmp_str[UART_BUFFER_SIZE];
	int i = 0;
	int str_size = strlen(UART_MSG_MARK_response_permission);
	memset(cmp_str, 0x00, sizeof(cmp_str));
	sprintf(cmp_str, "%s", UART_MSG_MARK_response_permission);

	for (i = 0; i < str_size; i++) {
		if (cmp_str[i] != msg[i])
			ret = false;
	}

	return ret;
}

bool UART_isResponsePemissionOK(char msg[UART_BUFFER_SIZE]) {
	bool ret = false;
	char cmp_str[UART_BUFFER_SIZE];
	memset(cmp_str, 0x00, sizeof(cmp_str));
	sprintf(cmp_str, "%s %s %s", UART_MSG_MARK_response_permission, UART_MSG_MARK_yes, UART_MSG_MARK_end);

	if (strcmp(cmp_str, msg) == 0)
		ret = true;
	return ret;
}


void UART_sycTime(char raw_msg[UART_BUFFER_SIZE]) {
	// SycTime Format : "SycTime yyyymmddhhmmss end\n"
	//							year month day hour minute second
	int strTime[20];
	int i;
	int year = 1999;
	int month = 12;
	int day = 31;
	int hour = 23;
	int minute = 59;
	int second = 40;
	memset(strTime, 0x00, sizeof(strTime));

	for (i = 0; i < 14; i++) {
		strTime[i] = raw_msg[9 + i] - '0';
	}
	year = strTime[0] * 1000 + strTime[1] * 100 + strTime[2] * 10 + strTime[3];
	month = strTime[4] * 10 + strTime[5];
	day = strTime[6] * 10 + strTime[7];
	hour = strTime[8] * 10 + strTime[9];
	minute = strTime[10] * 10 + strTime[11];
	second = strTime[12] * 10 + strTime[13];

	setTimer(year, month, day, hour, minute, second);
}


void UART_communication(void) {
	char sendingMsgBuffer[UART_BUFFER_SIZE];
	memset(sendingMsgBuffer, 0, sizeof(sendingMsgBuffer));

	if (UART_current_state == UART_DISCONNECTED) {
		set_isServerConnected(false);
		UART_receiveMsg((char *)aTxBuffer);
		if (UART_isEnd((char *)aTxBuffer) == true) {
			if (UART_ishandShakeMsg((char *)aTxBuffer)) {
				UART_clearBuffer();
				UART_makeHandShakeMsg((char*) sendingMsgBuffer);
				UART_sendMsg(sendingMsgBuffer);
				UART_current_state = UART_CONNECTED;
				UART_handShake_time_count = 0;
				set_isServerConnected(true);
			}
			else if (UART_isDisconnectMsg((char *)aTxBuffer)) {
				UART_clearBuffer();
				UART_makeDisconnectionMsg(sendingMsgBuffer);
				UART_sendMsg(sendingMsgBuffer);
				UART_current_state = UART_DISCONNECTED;
				//UART_sendMsg("arm : success disconnect\n");
				set_isServerConnected(false);
				resetConnectionCounter();
			}

			UART_clearBuffer();
		}
	}
	else if (UART_current_state == UART_CONNECTED) {
		//receive part 
		UART_receiveMsg((char *)aTxBuffer);
		if (UART_isEnd((char*)aTxBuffer) == true) {
			if (UART_isDisconnectMsg((char *)aTxBuffer)) {
				UART_clearBuffer();
				UART_makeDisconnectionMsg(sendingMsgBuffer);
				UART_sendMsg(sendingMsgBuffer);
				UART_current_state = UART_DISCONNECTED;
				//UART_sendMsg("arm : success disconnect\n");
				set_isServerConnected(false);
				resetConnectionCounter();
			}

			if (UART_isTimeSycMsg((char*)aTxBuffer) == true) {
				UART_sycTime((char*)aTxBuffer);
				UART_clearBuffer();
				resetConnectionCounter();
			}

			//need test
			if (UART_isReseponsePermissionMsg((char *)aTxBuffer) == true) {
				
				if (UART_isResponsePemissionOK((char *)aTxBuffer) == true) {
					setCurrentSustemMode(SYSTEM_MODE_OPEN_SUCCESS);
					setOpenDoorLock();
				}
				else {
					setCurrentSustemMode(SYSTEM_MODE_OPEN_FAIL);
				}
				setLCD_refresh();
				UART_clearBuffer();
				resetConnectionCounter();
				setRequsetPermission(false);
			}

			if (Uart_isCheckConnectionMsg((char *)aTxBuffer) == true) {
				UART_clearBuffer();
				UART_makeCheckConnectionMsg((char *)sendingMsgBuffer);
				UART_sendMsg(sendingMsgBuffer);
				set_isServerConnected(true);
				resetConnectionCounter();
			}
			UART_clearBuffer();

		}


		//send part
		//인증 요청 
		// need test
		if (isRequestedPermission() == true) {
			if (isSendingMsgCounterZero() == true) {
				UART_makeRequestPermssionMsg((char*)sendingMsgBuffer);
				UART_sendMsg((char *)sendingMsgBuffer);
				setLCD_refresh();
				resetSendingMsgCounter();
			}
		}
		
		if (isConnectionCountZero()) {
			resetConnectionCounter();
			UART_current_state = UART_DISCONNECTED;
		}
	}

	return;
}

void uart_refresh_counter_up(void) {
	UART_refresh_time_count = (UART_refresh_time_count + 1) % uart_refresh_time;
	if (UART_refresh_time_count == 0) {
		UART_refresh = true;
	}
	UART_refresh_time_count++;
}

void UART_liveSigal_count_up() {
	UART_LiveMsgTimer++;
}




