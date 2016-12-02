///*----------------------------------------------------------------------------
// *      doorlock_UART.c
// *----------------------------------------------------------------------------
// *      Name:    doorlock_UART.c
// *---------------------------------------------------------------------------*/
//

#include "doorLock_UART.h"



//		uart state
typedef enum {
	SEND_HANDSHAKE_MSG, RECEIVE_HANDSHAKE_MSG,
	UART_CONNECTED, UART_DISCONNECTED
} UART_CONNECTION_STATE;
//		end uart state


//		uart function
bool is_UART_connected = false;

UART_CONNECTION_STATE UART_current_state = UART_DISCONNECTED;

char char_UART_input = UART_DEFAULT_INPUT;

uint8_t ch;
uint8_t aTxBuffer[UART_BUFFER_SIZE];	// 문자열 저장 버퍼
uint8_t buffer_count = 0;			// 현재 Write 버퍼 위치

bool UART_refresh = false;
int UART_refresh_time_count = 0;
int UART_seq_num = 0;
int UART_handShake_time_count = 0;

int UART_LiveMsgTimer = 0;


void uart_init(void) {
	Uart0_Init();	// UART0 초기화
	UARTPuts(LPC_UART0, "hello world this is arm processor \n");
	UART_refresh = true;
	UART_seq_num = 0;
	UART_handShake_time_count = 0;
	UART_LiveMsgTimer = 0;
}

void uart_clearBuffer(void) {
	memset(aTxBuffer, 0x00, sizeof(aTxBuffer)); // Buffer 클리어					
	buffer_count = 0;
}

void uart_sendMsg(char msg[UART_BUFFER_SIZE]) {
	UARTPuts(LPC_UART0, msg);
}

bool uart_receiveMsg(char buffer[UART_BUFFER_SIZE]) {
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

void uart_makeHandShakeMsg(char msg[UART_BUFFER_SIZE], int seqnum) {
	// hand shake mark + seq + end
	char num[10];
	memset(num, 0x00, sizeof(num));

	//i to a
	num[0] = UART_seq_num / 10 + '0';
	num[1] = UART_seq_num % 10 + '0';

	sprintf(msg, "%s %s %s", UART_MSG_MARK_handshake_send, num, UART_MSG_MARK_end);
}

void uart_makeDisconnectionMsg(char msg[UART_BUFFER_SIZE]) {
	sprintf(msg, "%s %s %s", UART_MSG_MARK_disconnect, UART_MSG_MARK_success, UART_MSG_MARK_end);
}

void makeRequestPermssionMsg(char msg[UART_BUFFER_SIZE]) {
	//get password
	char strPassword[10];
	int pwd1 = 0;
	int pwd2 = 0;
	int pwd3 = 0;
	int pwd4 = 0;
	
	memset(strPassword, 0x00, sizeof(strPassword));
	strPassword[0] = pwd1;
	strPassword[1] = pwd2;
	strPassword[2] = pwd3;
	strPassword[3] = pwd4;

	sprintf(msg, "%s %s %s", UART_MSG_MARK_request_permission, strPassword, UART_MSG_MARK_end);
}

bool ishandShakeMsg(char msg[UART_BUFFER_SIZE]) {
	char temp_str[UART_BUFFER_SIZE];
	char num[10];
	memset(num, 0x00, sizeof(num));
	memset(temp_str, 0x00, sizeof(temp_str));

	//i to a
	num[0] = UART_seq_num / 10 + '0';
	num[1] = UART_seq_num % 10 + '0';

	sprintf(temp_str, "%s %s %s", UART_MSG_MARK_handshake_receive, num, UART_MSG_MARK_end);
	if (strcmp(msg, temp_str) == 0)
		return true;
	else
		return false;
}

bool isDisconnectMsg(char msg[UART_BUFFER_SIZE]){
	char temp_str[UART_BUFFER_SIZE];
	memset(temp_str, 0x00, sizeof(temp_str));
	
	sprintf(temp_str, "%s %s", UART_MSG_MARK_disconnect, UART_MSG_MARK_end);
	if (strcmp(msg, temp_str) == 0 )	
		return true;
	return 
		false;
}

bool isTimeSycMsg(char msg[UART_BUFFER_SIZE]) {
	char temp[10];
	bool ret = true;
	int i ;
	memset(temp, 0x00, sizeof(temp));

	// SycTime Format : "SycTime yyyymmddhhmmss end\n"
	//							year month day hour minute second
	strcpy(temp, UART_MSG_MARK_SycTime);
	for (i = 0; i < 6; i++) {
		if (temp[i] != msg[i]) 
			ret = false;
	}
	return ret;
}

bool isReseponsePermissionMsg(char msg[UART_BUFFER_SIZE]) {
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

bool isResponsePemissionOK(char msg[UART_BUFFER_SIZE]) {
	bool ret = false;
	char cmp_str[UART_BUFFER_SIZE];
	memset(cmp_str, 0x00, sizeof(cmp_str));
	sprintf(cmp_str, "%s %s %s", UART_MSG_MARK_response_permission, UART_MSG_MARK_yes, UART_MSG_MARK_end);

	if (strcmp(cmp_str, msg) == 0)
		ret = true;
	return ret;
}

void sycTime(char raw_msg[UART_BUFFER_SIZE]) {
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

	for ( i = 0; i < 14; i++) {
		strTime[i] = raw_msg[8 + i]- '0';
	}
	year = strTime[0] * 1000 + strTime[1] * 100 + strTime[2] * 10 + strTime[3];
	month = strTime[4] * 10 + strTime[5];
	day = strTime[6] * 10 + strTime[7];
	hour = strTime[8] * 10 + strTime[9];
	minute = strTime[10] * 10 + strTime[11];
	second = strTime[12] * 10 + strTime[13];

	setTimer( year, month, day, hour, minute, second);
}

void UART_communication(void) {
	char tempSendMsg[UART_BUFFER_SIZE];
	memset(tempSendMsg, 0, sizeof(tempSendMsg));

	//if (UART_refresh == false) return;
	UART_refresh = false;

	if (UART_current_state == UART_DISCONNECTED
		|| UART_current_state == SEND_HANDSHAKE_MSG) {
		//send hand shake msg
		uart_clearBuffer();
		uart_makeHandShakeMsg(tempSendMsg, UART_seq_num);
		uart_sendMsg(tempSendMsg);
		UART_current_state = RECEIVE_HANDSHAKE_MSG;
	}
	else if (UART_current_state == RECEIVE_HANDSHAKE_MSG) {
		UART_refresh = true;

		//receive nothing 
		if (uart_receiveMsg((char *)aTxBuffer) == false) 
			return;

		// hand shake success
		if (ishandShakeMsg((char *)aTxBuffer)) {
			uart_clearBuffer();
			UART_current_state = UART_CONNECTED;
			UART_handShake_time_count = 0;
			uart_sendMsg("arm : success hand shake \n");
		}
		else if (isDisconnectMsg((char *)aTxBuffer)) {
			uart_clearBuffer();
			uart_makeDisconnectionMsg(tempSendMsg);
			uart_sendMsg(tempSendMsg);
			UART_current_state = UART_DISCONNECTED;
			uart_sendMsg("arm : success disconnect\n");
			UART_refresh = true;
		}

		//handshake time out 
		else if (UART_handShake_time_count > uart_handShake_timeout) {
			UART_seq_num = (UART_seq_num + 1) % uart_seq_num_size;
			UART_current_state = SEND_HANDSHAKE_MSG;
			uart_sendMsg("arm : time out hand shake \n");
		}
		uart_clearBuffer();

	}
	else if (UART_current_state == UART_CONNECTED) {

		// send live signal msg
		if (UART_LiveMsgTimer >= UART_Live_signal_Time) {
			memset(tempSendMsg, 0, sizeof(tempSendMsg));
			sprintf(tempSendMsg,"%s %s", UART_MSG_MARK_LIVE_SIGNAL, UART_MSG_MARK_end);
			uart_sendMsg(tempSendMsg);
			UART_LiveMsgTimer = 0;
		}

		//receive part 
		uart_receiveMsg((char *)aTxBuffer);
		if (isDisconnectMsg((char *)aTxBuffer)) {
			uart_makeDisconnectionMsg(tempSendMsg);
			uart_sendMsg(tempSendMsg);
			UART_current_state = UART_DISCONNECTED;
			uart_sendMsg("arm : success disconnect\n");
			UART_refresh = true;
		}
		
		//need test
		if (isTimeSycMsg((char*)aTxBuffer) == true) {
			sycTime((char*)aTxBuffer);
			return;
		}

		//need test
		if (isReseponsePermissionMsg((char *)aTxBuffer) == true) {
			//
			if (isResponsePemissionOK((char *)aTxBuffer) == true) {
				setOpenDoorLock();
				//update_state();
			}
			else {
				//update_state();
			}
			setLCD_refresh();
			return;
		}

		
		//send part
		//인증 요청 
		// need test
		if (isRequestedPermission() == true) {
			memset(tempSendMsg, 0, sizeof(tempSendMsg));
			makeRequestPermssionMsg((char*)tempSendMsg);
			uart_sendMsg((char * )tempSendMsg);
			setLCD_refresh();
			//setLCD_Mod(connecting server );
			return ;
		}

		//상태 업데이트 
		// 필요함???
		//if (isThereStatusMsg() == true) {
		//	memset(tempSendMsg, 0, sizeof(tempSendMsg));
		//	getStatusMsg((char*)tempSendMsg);
		//	uart_sendMsg(tempSendMsg);
		//}		
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




