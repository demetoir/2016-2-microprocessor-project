///*----------------------------------------------------------------------------
// *      doorlock_UART.c
// *----------------------------------------------------------------------------
// *      Name:    doorlock_UART.c
// *---------------------------------------------------------------------------*/

#include "doorLock_UART.h"

//UART�� ������¸� ��Ÿ���� enum ����
typedef enum {
	UART_CONNECTED, 
	UART_DISCONNECTED
} UART_CONNECTION_STATE;
//UART�� ������¸� �����ϴ� ����
UART_CONNECTION_STATE UART_currentState = UART_DISCONNECTED;

//���۹��� �����͸� �����ϴ� ����
uint8_t receivedBuffer[UART_BUFFER_SIZE];	// ���ڿ� ���� ����
//���ۿ� Write�� ��ġ
uint8_t bufferCount = 0;			// ���� Write ���� ��ġ


//uart�� �ʱ�ȭ�ϴ� �Լ�
//uart �ʱ�ȭ��  ���̺귯���� �⺻������
//Baud_rate = 115200;
//Databits = UART_DATABIT_8;
//Parity = UART_PARITY_NONE;
//Stopbits = UART_STOPBIT_1;
//���� �����Ͽ���
void UART_init(void) {
	// UART0 �ʱ�ȭ
	Uart0_Init();	

	//�ʱ�ȭ �� �׽�Ʈ�� �޼����� �����Ѵ�
	UARTPuts(LPC_UART0, "hello world this is arm processor \n");
	
	//�ʱ⿬����¸� �ʱ�ȭ�Ѵ�
	DATA_setIsServerConnected(false);
}

//���۹��� ���۸� �ʱ�ȭ �Ѵ�
void UART_clearReceivedBuffer(void) {
	memset(receivedBuffer, 0x00, sizeof(receivedBuffer)); 
	bufferCount = 0;
}

//polling ������� uart0�� �޼����� �����Ѵ�
//rs323 to usb ���̺� ���ο��� ���۰� �����Ѵ�
//rs323 -> usb �� ���� ���⿡ ���۰� �����Ͽ� 
//�޼����� �ܼ��� ����������� �����־ �ȴ�
//�̰��� ���������� ���ۿ� �����÷ο찡 �Ͼ��
//�ȵ��� ����������� ���Ź޾ƾ��Ѵ�
void UART_sendMsg(char msg[UART_BUFFER_SIZE]) {
	UARTPuts(LPC_UART0, msg);
}

//���Ź޴°�� ���� ����������� ���Ź��� �ѹ���Ʈ�����Ͽ�
//echo�� �����Ͽ� ���������� ���Ź����� �˷��־ �������� 
//���� ����Ʈ�� �����ϴ°��� �ݺ��Ͽ� msg�� �����Ѵ�. 
//���ſ� �����ϸ� ���� ���ۿ� �޼����� �����ϰ� true�� ��ȯ
//���� �������� ���ų�, ���۰� ������ ��� false�� ��ȯ�Ѵ�.
bool UART_receiveMsg(char buffer[UART_BUFFER_SIZE]) {
	//�ÿ�Ǿ��� ������ �ʱ�ȭ
	bool isReceivedMsg = false;
	int i = 0;
	char received_char;
	bool isReceivedByte = false;
	char tempReceivedMsg[UART_BUFFER_SIZE];

	//���۰� �������� ���Ź����� �����Ƿ� �����Ѵ�
	if (bufferCount == UART_BUFFER_SIZE) return false;

	//���� ���ۿ��� �ִ�� ���Ź������ִ� ��ŭ�� ������ �˻��Ѵ�
	for (i = bufferCount; i < UART_BUFFER_SIZE; i++) {
		//polling ������� byte�� �����Ѵ�
		isReceivedByte = UART_Receive(LPC_UART0, 
			(uint8_t *)tempReceivedMsg, UART_BUFFER_SIZE, NONE_BLOCKING);
	
		//���Ź��� ���� ������� �����Ѵ�
		if (isReceivedByte == false) 
			continue;

		//���Ź��� byte�� �����´�
		received_char = tempReceivedMsg[0];

		//���Ź��� byte�� ���Ͽ� echo�� �����Ѵ�
		UARTPuts(LPC_UART0, &received_char);

		//���Ź��� byte�� ���ۿ� �����Ѵ�
		buffer[bufferCount] = received_char;
		bufferCount++;

		//���Ź��� byte�� ���๮���ϰ�� �޼����� ���̹Ƿ� 
		//������ �����Ͽ���
		if (received_char == '\n') {
			isReceivedMsg = true;
			break;
		}
	}

	//���� ������ �÷��� ������ ��ȯ�Ѵ�
	return isReceivedMsg;
}

// make msg
//UART_make�� ���λ�� ������ �Լ����� ��� msg mark�� �����Ͽ�
//���� �޼����� �����

//handshake msg�� �����
void UART_makeHandShakeMsg(char msg[UART_BUFFER_SIZE]) {
	memset(msg, 0x00, sizeof(msg));
	sprintf(msg, "%s %s", 
		UART_MSG_HEADMARK_handshake_receive, UART_MSG_FOOTMARK_end);
}

//check connection msg�� �����
void UART_makeCheckConnectionMsg(char msg[UART_BUFFER_SIZE]) {
	memset(msg, 0x00, sizeof(msg));
	sprintf(msg, "%s %s", 
		UART_MSG_HEADMARK_check_connection, UART_MSG_FOOTMARK_end);
}

//discconection msg�� �����
void UART_makeDisconnectionMsg(char msg[UART_BUFFER_SIZE]) {
	memset(msg, 0x00, sizeof(msg));
	sprintf(msg, "%s %s %s", 
		UART_MSG_HEADMARK_disconnect, UART_MSG_HEADMARK_success, 
		UART_MSG_FOOTMARK_end);
}

//��Ŀ�� ���⸦ ���� ������ ��û�ϴ� msg
void UART_makeRequestPermssionMsg(char msg[UART_BUFFER_SIZE]) {
	char strPassword[10];
	int lockerNumber = 0;

	memset(msg, 0x00, sizeof(msg));
	memset(strPassword, 0x00, sizeof(strPassword));
	//���� ��й�ȣ�� ��Ŀ ��ȣ�� �����´�
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
//uart_is�� ���λ�� ������ �Լ����� msg�� �˻��ϴ� 
//�Լ����̴�. msg mark�� �̿��Ͽ� �ùٸ� msg�� �����ϰ�
//���ڷ� ���� msg�� �˻��Ͽ� �ش��ϴ� msg���� �˻��Ѵ�

//msg�� end mark�� �������� �˻��Ѵ�. 
//�� �Լ��� msg�� ������ �������� �˻��Ѵ�
//������ �������� ��ū���� ������ ������ ��ū�� msg
//end mark�ΰ��� �˻��Ѵ�
bool UART_isMsgMeetEndMark(char msg[UART_BUFFER_SIZE]) {
	//strtok�� msg �ִ´ٸ� msg�� ������ ����ǹǷ� 
	//�ӽ� ���۷� �����Ͽ��˻��Ѵ�
	char tempBuffer[UART_BUFFER_SIZE];
	//end mark�� ������ ����
	char endMark[10]; 
	//token�� ���� �����͸� �����ϴ� ����
	char *tokenPtr = NULL;
	//���ڿ����� �ʱ�ȭ�Ѵ�
	memset(endMark, 0x00, sizeof(endMark));
	memset(tempBuffer, 0x00, sizeof(tempBuffer));
	sprintf(endMark, "%s", UART_MSG_FOOTMARK_end);
	strcpy(tempBuffer, msg);

	//�ӽ� ���۸� ' '(����)���� ��ū�� ������ token�� �˻��Ѵ�
	tokenPtr = strtok(tempBuffer, " ");
	while (tokenPtr != NULL) {
		if (strcmp(tokenPtr, endMark) == 0) return true;
		tokenPtr = strtok(NULL, " ");
	}
	return false;
}

//hand shake msg ���� �˻��Ѵ�
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

//disconection msg���� �˻��Ѵ�
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

//�ð��� ����ȭ�ϴ� msg���� �˻��Ѵ�
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

	//msg�� ù��° mark�� UART_MSG_MARK_SyncTime ���� �˻�
	memset(msgMark, 0x00, sizeof(msgMark));
	strcpy(msgMark, UART_MSG_HEADMARK_SyncTime);
	for (i = 0; i < strlen(msgMark); i++) {
		if (msgMark[i] != msg[i])
			_isTimeSyncMsg = false;
	}

	//msg mark�� UART_MSG_MARK_end ���� �˻�
	memset(msgMark, 0x00, sizeof(msgMark));
	strcpy(msgMark, UART_MSG_FOOTMARK_end);
	for (i = 0; i < strlen(msgMark); i++) {
		if (msgMark[i] != msg[i + 24])
			_isTimeSyncMsg = false;
	}

	return _isTimeSyncMsg;
}

//check connection msg���� �˻�
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

//check connection msg���� �˻�
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
//ResponsePemissionOK msg���� �˻�
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

//uart�� ���Ź��� �ð����� ����ȭ�Ѵ�
void UART_syncTime(char raw_msg[UART_BUFFER_SIZE]) {
	int i;
	//������ �ð��� ��¥ ������ ������ ���ڿ�
	int strTime[20];
	//������ ��ȯ�� �ð��� ��¥�� ������ ������
	int year = 1999;
	int month = 12;
	int day = 31;
	int hour = 23;
	int minute = 59;
	int second = 40;

	//���� �޼����� �ð��κ��� ���ڿ��� �����Ѵ�
	memset(strTime, 0x00, sizeof(strTime));
	for (i = 0; i < 14; i++) {
		strTime[i] = raw_msg[9 + i] - '0';
	}

	//����� ���ڿ��� ������ ��ȯ�Ѵ�
	year = strTime[0] * 1000 + strTime[1] * 100
			+ strTime[2] * 10 + strTime[3];
	month = strTime[4] * 10 + strTime[5];
	day	= strTime[6] * 10 + strTime[7];
	hour = strTime[8] * 10 + strTime[9];
	minute = strTime[10] * 10 + strTime[11];
	second = strTime[12] * 10 + strTime[13];

	//������ ��ȯ�� �ð��� ��¥�� �����Ѵ�
	DATA_setTime(year, month, day, hour, minute, second);
}


//uart�� mainTask�Լ�
//������ ���� �� �޼��� ���� �� ������ó���Ѵ�
void UART_mainTask(void) {
	//���� �޼����� ���� ���� �� �ʱ�ȭ
	char sendingBuffer[UART_BUFFER_SIZE];
	memset(sendingBuffer, 0, sizeof(sendingBuffer));

	//uart�� ���°� UART_DISCONNECTED�϶�
	if (UART_currentState == UART_DISCONNECTED) {
		DATA_setIsServerConnected(false);
		UART_receiveMsg((char *)receivedBuffer);

		//���Ź��� �޼����� ������ format�� �̷궧
		if (UART_isMsgMeetEndMark((char *)receivedBuffer) == true) {
			//hand shake msg ���Ž� msg�� ���� �������ϰ� 
			//uart ���¸� �����Ѵ�
			if (UART_ishandShakeMsg((char *)receivedBuffer)) {
				UART_makeHandShakeMsg((char*) sendingBuffer);
				UART_sendMsg(sendingBuffer);

				UART_currentState = UART_CONNECTED;
				DATA_setIsServerConnected(true);
			}
			//disconection msg ���Ž� msg�� ���� �������ϰ� 
			//uart ���¸� �����Ѵ�
			else if (UART_isDisconnectMsg((char *)receivedBuffer)) {
				UART_makeDisconnectionMsg(sendingBuffer);
				UART_sendMsg(sendingBuffer);

				UART_currentState = UART_DISCONNECTED;
				DATA_setIsServerConnected(false);
				DATA_resetConnectionCounter();
			}

			//������ ����� ���� msg�� �������ֵ��ϸ����
			UART_clearReceivedBuffer();
		}
	}
	//uart �������϶�
	else if (UART_currentState == UART_CONNECTED) {
		//���� msg�� ���� �޴´� 
		UART_receiveMsg((char *)receivedBuffer);

		//���Ź��� �޼����� ������ format�� �̷궧
		//msg�� ó������ receivedBuffer��� ���� �޼����� 
		//���Ź��� ���ֵ��� �����
		if (UART_isMsgMeetEndMark((char*)receivedBuffer) == true) {

			//disconection msg ���Ž� msg�� ���� �������ϰ� 
			//uart ���¸� �����Ѵ�
			if (UART_isDisconnectMsg((char *)receivedBuffer)) {
				UART_makeDisconnectionMsg(sendingBuffer);
				UART_sendMsg(sendingBuffer);

				UART_currentState = UART_DISCONNECTED;
				DATA_setIsServerConnected(false);
				DATA_resetConnectionCounter();
			}

			//time sync msg ���Ž� msg�� ���� �������ϰ� 
			//�ð��� ����ȭ�Ѵ�
			if (UART_isTimeSyncMsg((char*)receivedBuffer) == true) {
				UART_syncTime((char*)receivedBuffer);
				DATA_resetConnectionCounter();
			}

			if (UART_isReseponsePermissionMsg((char *)receivedBuffer) == true) {
				//��й�ȣ�� ������ �ý��� ��带 �����ϰ� ��Ŀ ������ �����Ѵ�
				if (UART_isResponsePemissionOK((char *)receivedBuffer) == true) {
					DATA_setSystemState(SYSTEM_STATE_OPEN_SUCCESS);
					MOTOR_setOpenLocker();
				}
				//Ʋ����� �ý��۸�带 �����Ѵ�
				else {
					DATA_setSystemState(SYSTEM_STATE_OPEN_FAIL);
				}

				//lcd ȭ���� �����Ѵ�
				LCD_setRefresh();
				//��û�� ���� ������ �����Ƿ� ���̻� �޽����� �������ʱ����� 
				//��û�޼����� �������� �����Ѵ�
				DATA_setRequsetPermission(false);
				//�������� �޼��� ��ȯ�̹Ƿ� �������� ī���͸� �ʱ�ȭ�Ѵ�
				DATA_resetConnectionCounter();
			}

			//���� �޼����� ������ ������ Ȯ���ϴ� �޼����� ���
			if (Uart_isCheckConnectionMsg((char *)receivedBuffer) == true) {
				//�޼����� �����Ͽ� �����Ѵ�
				UART_makeCheckConnectionMsg((char *)sendingBuffer);
				UART_sendMsg(sendingBuffer);

				//������ ������¸� �����Ѵ�
				DATA_setIsServerConnected(true);
				DATA_resetConnectionCounter();
			}

			//������ ����� ���� msg�� �������ֵ��ϸ����
			UART_clearReceivedBuffer();
		}

		//������ ��Ŀ�� �������� ���� �޼����� ������� 
		if (DATA_isRequestedPermission() == true) {
			//�޼��� �����ϰ� �����ð����� ������ ������
			//������ �ö����� �������� �Ѵ�.
			//ó��
			if (DATA_isSendingMsgCounterZero() == true) {
				//�޼����� �����Ͽ� �����Ѵ�
				UART_makeRequestPermssionMsg((char*)sendingBuffer);
				UART_sendMsg((char *)sendingBuffer);

				//ȭ���� �����ϵ��� �����Ѵ�
				LCD_setRefresh();
				//�޼����� ������ ī���͸� �ʱ�ȭ�Ѵ�
				DATA_resetSendingMsgCounter();
			}
		}	

		//�����ð����� �޼����� ���� ���Ѵٸ� ������ ������������ ó���Ѵ�
		if (DATA_isConnectionCountZero()) {
			DATA_resetConnectionCounter();
			UART_currentState = UART_DISCONNECTED;
		}
	}

	return;
}





