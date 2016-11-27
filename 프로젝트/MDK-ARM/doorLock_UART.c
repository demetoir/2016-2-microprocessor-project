///*----------------------------------------------------------------------------
// *      doorlock_UART.c
// *----------------------------------------------------------------------------
// *      Name:    doorlock_UART.c
// *---------------------------------------------------------------------------*/
//

#include "doorLock_UART.h"

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

void uart_init(void) {
	Uart0_Init();	// UART0 초기화
	UARTPuts(LPC_UART0, "hello world this is arm processor \r\n");
	UART_refresh = true;
	UART_seq_num = 0;
	UART_handShake_time_count = 0;
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

void UART_communication(void) {
	char temp_msg[UART_BUFFER_SIZE];
	bool isHandShakeSuccess = false;
	memset(temp_msg, 0, sizeof(temp_msg));

	//if (UART_refresh == false) return;
	UART_refresh = false;

	if (UART_current_state == UART_DISCONNECTED
		|| UART_current_state == SEND_HANDSHAKE_MSG) {
		//send hand shake msg
		uart_clearBuffer();
		uart_makeHandShakeMsg(temp_msg, UART_seq_num);
		uart_sendMsg(temp_msg);
		UART_current_state = RECEIVE_HANDSHAKE_MSG;


	}
	else if (UART_current_state == RECEIVE_HANDSHAKE_MSG) {
		UART_refresh = true;

		//receive nothing 
		if (uart_receiveMsg((char *)aTxBuffer) == false) 
			return;

		isHandShakeSuccess = ishandShakeMsg((char *)aTxBuffer);
		// hand shake success
		if (isHandShakeSuccess) {
			UART_current_state = UART_CONNECTED;
			UART_handShake_time_count = 0;
			uart_sendMsg("arm : success hand shake \r\n");
			uart_clearBuffer();
		}
		else if (isDisconnectMsg((char *)aTxBuffer)) {
			uart_makeDisconnectionMsg(temp_msg);
			uart_sendMsg(temp_msg);
			UART_current_state = UART_DISCONNECTED;
			uart_sendMsg("arm : success disconnect\r\n");
			UART_refresh = true;
		}

		//handshake time out 
		else if (UART_handShake_time_count > uart_handShake_timeout) {
			UART_seq_num = (UART_seq_num + 1) % uart_seq_num_size;
			UART_current_state = SEND_HANDSHAKE_MSG;
			uart_sendMsg("arm : time out hand shake \r\n");
		}
		uart_clearBuffer();

	}
	else if (UART_current_state == UART_CONNECTED) {
		//memset(aTxBuffer, 0, sizeof(aTxBuffer));
		uart_receiveMsg((char *)aTxBuffer);
		//uart_sendMsg((char *)aTxBuffer);
		if (isDisconnectMsg((char *)aTxBuffer)) {
			uart_makeDisconnectionMsg(temp_msg);
			uart_sendMsg(temp_msg);
			UART_current_state = UART_DISCONNECTED;
			uart_sendMsg("arm : success disconnect\r\n");
			UART_refresh = true;
		}



		//check sending msg

		//send msg
		//send some msg


		//receive msg
		//uart_receive_msg(msg);

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

