///*----------------------------------------------------------------------------
// *      doorlock_KEYPAD.c
// *----------------------------------------------------------------------------
// *      Name:    doorlock_KEYPAD.c
// *---------------------------------------------------------------------------*/
//


#ifndef doorlock_KEYPAD_H
#include "doorLock_KEYPAD.h"
#endif // !doorlock_KEYPAD_H

//keypad libarary
void EXT_IO_init(void) {
	/*EXT_IO on PORT0 as Output*/
	LPC_GPIO0->FIODIR |= (1 << 4) | (1 << 19) | (1 << 20) | (1 << 21);	// Mainboard Chip Select
	LPC_GPIO1->FIODIR |= (1 << 21);
	LPC_GPIO2->FIODIR |= (1 << 11);
}

void KEYPAD_DIR_Output(void) {
	/* PORT0 defined as Output    */
	LPC_GPIO0->FIODIR |= (1 << 5);
	LPC_GPIO0->FIODIR |= (1 << 10);
	/* PORT2 defined as output    */
	LPC_GPIO2->FIODIR |= (1 << 12);
	LPC_GPIO2->FIODIR |= (1 << 13);
}

void KEYPAD_DIR_Input(void) {
	LPC_GPIO0->FIOPIN |= (1 << 4);  // DIR A->B
	/* PORT0 defined as Input    */
	LPC_GPIO0->FIODIR &= ~(1 << 23);
	LPC_GPIO0->FIODIR &= ~(1 << 24);
	/* PORT3 defined as input    */
	LPC_GPIO3->FIODIR &= ~(1 << 25);
	LPC_GPIO3->FIODIR &= ~(1 << 26);
}

void EXT_IO_A_CS(void) {
	/* EXT_IO_A, DIR nOE setting B->A*/
	LPC_GPIO0->FIOPIN &= ~(1 << 19); // EXT_IO_A On
	LPC_GPIO0->FIOPIN |= (1 << 20);  // EXT_IO_B Off
	LPC_GPIO0->FIOPIN |= (1 << 21);  // EXT_IO_C Off
}

void EXT_IO_B_CS(void) {
	/* EXT_IO_B, DIR nOE setting B->A*/
	LPC_GPIO0->FIOPIN |= (1 << 19); 	// EXT_IO_A Off
	LPC_GPIO0->FIOPIN &= ~(1 << 20);  // EXT_IO_B On
	LPC_GPIO0->FIOPIN |= (1 << 21);  	// EXT_IO_C Off
}

void EXT_IO_C_CS(void) {
	/* EXT_IO_C, DIR nOE setting B->A*/
	LPC_GPIO0->FIOPIN |= (1 << 19); 	// EXT_IO_A Off
	LPC_GPIO0->FIOPIN |= (1 << 20); 	// EXT_IO_B Off
	LPC_GPIO0->FIOPIN &= ~(1 << 21);  // EXT_IO_C On
}

void KEYPAD_Output_Row_1(void) {
	KEYPAD_DIR_Output();
	/* PORT0 defined as Output    */
	LPC_GPIO0->FIOPIN &= ~(1 << 5);
	LPC_GPIO0->FIOPIN |= (1 << 10);
	/* PORT2 defined as output    */
	LPC_GPIO2->FIOPIN |= (1 << 12);
	LPC_GPIO2->FIOPIN |= (1 << 13);

	LPC_GPIO2->FIOPIN |= (1 << 11); // CS Clock High Edge
	LPC_GPIO2->FIOPIN &= ~(1 << 11);
}

void KEYPAD_Output_Row_2(void) {
	KEYPAD_DIR_Output();
	/* PORT0 defined as Output    */
	LPC_GPIO0->FIOPIN |= (1 << 5);
	LPC_GPIO0->FIOPIN &= ~(1 << 10);
	/* PORT2 defined as output    */
	LPC_GPIO2->FIOPIN |= (1 << 12);
	LPC_GPIO2->FIOPIN |= (1 << 13);

	LPC_GPIO2->FIOPIN |= (1 << 11); // CS Clock High Edge
	LPC_GPIO2->FIOPIN &= ~(1 << 11);
}

void KEYPAD_Output_Row_3(void) {
	KEYPAD_DIR_Output();
	/* PORT0 defined as Output    */
	LPC_GPIO0->FIOPIN |= (1 << 5);
	LPC_GPIO0->FIOPIN |= (1 << 10);
	/* PORT2 defined as output    */
	LPC_GPIO2->FIOPIN &= ~(1 << 12);
	LPC_GPIO2->FIOPIN |= (1 << 13);

	LPC_GPIO2->FIOPIN |= (1 << 11); // CS Clock High Edge
	LPC_GPIO2->FIOPIN &= ~(1 << 11);
}

void KEYPAD_Output_Row_4(void) {
	KEYPAD_DIR_Output();
	/* PORT0 defined as Output    */
	LPC_GPIO0->FIOPIN |= (1 << 5);
	LPC_GPIO0->FIOPIN |= (1 << 10);
	/* PORT2 defined as output    */
	LPC_GPIO2->FIOPIN |= (1 << 12);
	LPC_GPIO2->FIOPIN &= ~(1 << 13);

	LPC_GPIO2->FIOPIN |= (1 << 11); // CS Clock High Edge
	LPC_GPIO2->FIOPIN &= ~(1 << 11);
}

uint8_t Keypad_Input(uint8_t LowNumber) {
	uint32_t KeyValue = 0;
	LPC_GPIO1->FIOPIN &= ~(1 << 21);  // Keypad Input CS On

	if (LowNumber == 1) {
		if ((LPC_GPIO0->FIOPIN >> 23) & 0x01) KeyValue = 0;
		else KeyValue = 1;
	}
	if (LowNumber == 2) {
		if ((LPC_GPIO0->FIOPIN >> 24) & 0x01) KeyValue = 0;
		else KeyValue = 1;
	}
	if (LowNumber == 3) {
		if ((LPC_GPIO3->FIOPIN >> 25) & 0x01) KeyValue = 0;
		else KeyValue = 1;
	}
	if (LowNumber == 4) {
		if ((LPC_GPIO3->FIOPIN >> 26) & 0x01) KeyValue = 0;
		else KeyValue = 1;
	}

	LPC_GPIO1->FIOPIN |= (1 << 21);  // Keypad Input CS Off

	return KeyValue;
}

uint8_t KEYPAD_getPressKeyNumber(int8_t EXT_IO_DIR) {
	uint8_t Key_Temp, Key_Value = 16;

	if (EXT_IO_DIR == 'A')	EXT_IO_A_CS();
	else if (EXT_IO_DIR == 'B') EXT_IO_B_CS();
	else EXT_IO_C_CS();

	KEYPAD_Output_Row_1();
	Key_Temp = Keypad_Input(1);
	if (Key_Temp == 1) Key_Value = 3;
	Key_Temp = Keypad_Input(2);
	if (Key_Temp == 1) Key_Value = 2;
	Key_Temp = Keypad_Input(3);
	if (Key_Temp == 1) Key_Value = 1;
	Key_Temp = Keypad_Input(4);
	if (Key_Temp == 1) Key_Value = 0;

	KEYPAD_Output_Row_2();
	Key_Temp = Keypad_Input(1);
	if (Key_Temp == 1)  Key_Value = 7;
	Key_Temp = Keypad_Input(2);
	if (Key_Temp == 1)  Key_Value = 6;
	Key_Temp = Keypad_Input(3);
	if (Key_Temp == 1)  Key_Value = 5;
	Key_Temp = Keypad_Input(4);
	if (Key_Temp == 1)  Key_Value = 4;

	KEYPAD_Output_Row_3();
	Key_Temp = Keypad_Input(1);
	if (Key_Temp == 1)  Key_Value = 11;
	Key_Temp = Keypad_Input(2);
	if (Key_Temp == 1)  Key_Value = 10;
	Key_Temp = Keypad_Input(3);
	if (Key_Temp == 1)  Key_Value = 9;
	Key_Temp = Keypad_Input(4);
	if (Key_Temp == 1)  Key_Value = 8;

	KEYPAD_Output_Row_4();
	Key_Temp = Keypad_Input(1);
	if (Key_Temp == 1)  Key_Value = 15;
	Key_Temp = Keypad_Input(2);
	if (Key_Temp == 1)  Key_Value = 14;
	Key_Temp = Keypad_Input(3);
	if (Key_Temp == 1)  Key_Value = 13;
	Key_Temp = Keypad_Input(4);
	if (Key_Temp == 1)  Key_Value = 12;

	return Key_Value;
}
//keypad libarary end


//keypad�� �Է°��� ���α׷��� �ʿ��� ������ �������ִ� ����
char keypadMappingVector[16];
//keypadMappingVector�� �ʱ�ȭ�Ѵ�
//������ ���Ǽ��� ���Ͽ� �Լ��� �ʱ�ȭ�Ѵ�
void KEYPAD_initKeypadMappingVector(void) {
	//number key
	keypadMappingVector[0]	= '1';
	keypadMappingVector[1]	= '2';
	keypadMappingVector[2]	= '3';
	keypadMappingVector[4]	= '4';
	keypadMappingVector[5]	= '5';
	keypadMappingVector[6]	= '6';
	keypadMappingVector[8]	= '7';
	keypadMappingVector[9]	= '8';
	keypadMappingVector[10] = '9';
	keypadMappingVector[13] = '0';
	
	//function key
	keypadMappingVector[3]	= 'C';
	keypadMappingVector[7]	= 'R';
	keypadMappingVector[11] = 'E';
	keypadMappingVector[15] = 'F';

	//not using key
	keypadMappingVector[12] = 'O';
	keypadMappingVector[14] = 'L';
}

//keypad�� GPIO�� ������ �����ϴ� ����
LPC_GPIO_TypeDef    keypad_GPIO_SETTING[5];
//keypad�� �ʱ�ȭ�ϴ� �Լ� 
void KEYPAD_init(void) {
	KEYPAD_initKeypadMappingVector();	//Ű��Ʈ ���� ���� �ʱ�ȭ
	KEYPAD_DIR_Input();					//�Է¹�������
	set_EXT_IO_DIRECTION('C');			//EXT IO ����
	save_LCP_GPIO_settingTo(keypad_GPIO_SETTING);//���� ������ �����Ѵ�
	Keypad_mainTask(); // �ʱ�ȭ �׽�Ʈ�� ���� �����Ѵ�
}


//������ ���� Ű�е��� ��
//Ű�е��� ���� ���Ҷ����� ���ŵǾ���Ѵ�
int	prevKeypadValue = KEYPAD_UP;
//keypad�� mainTask�Լ�
//system mode�� ���� ���� �ٸ� ������ �Ѵ� 
//��Ŀ ��ȣ�� ��й�ȣ�� �Է� �� ����,
//�ڷΰ���, ���� ȭ��, �ݱ� ��ư���� ó���Ѵ�
void Keypad_mainTask(void) {
	int rawKeypadValue	= 0;
	char mappedValue	= '0';																																					// To Revise Keypad #
	int lockerNumber	= 0;
	int mode = DATA_getSystemState();
	
	//keypad�� �۵���Ű���� GPIO ������ �ε��Ѵ�
	load_LPC_GPIO_settingFrom(keypad_GPIO_SETTING);
	//keypad�� �������� ���� ���ε� Ű�е��� ���� ���Ѵ�
	rawKeypadValue = KEYPAD_getPressKeyNumber(DEFAULT_KEYPAD_EXIO_NUM);
	mappedValue = keypadMappingVector[rawKeypadValue];

	//�������� ���ٸ� �������� �����ϰ� ��ȯ�Ѵ�
	if (rawKeypadValue == KEYPAD_UP) {
		prevKeypadValue = KEYPAD_UP;
		return;
	}
	//Ű�е尡 down�� �Ǵ� ���¸� �����ϴ� ���ǹ�
	//Ű�е尡 ���� ���� �ߺ��ǰ� �Է� 
	if (rawKeypadValue != KEYPAD_UP && prevKeypadValue == KEYPAD_UP) {
		prevKeypadValue = rawKeypadValue;

		//������ ������ ����������� �Է��� ó������ �ʴ´�
		if (DATA_isServerConnected() == false) {
			return ;
		}

		//�� ��忡 ���� keypad ������ ó���Ѵ�
		//�� ��忡 ���� �ùٸ� �Է��� ������ LCD_setRefresh��
		//ȣ���ϴ°����� lcd ȭ���� �����Ѵ�
		switch (mode){		
		//��Ŀ�� ��ȣ�� �����ϴ� ��� 
		case SYSTEM_STATE_SELECT_LOCKER_NUMBER: {
			//��ȣ�� ���������� ó��
			//��Ŀ�� ��ȣ�� 1~18 ���� �����Ƿ� ������ �Ѿ�� ����
			//ó�������ʴ�. ������ �ȿ� �ִٸ� ������ ��Ŀ��ȣ�� �����Ѵ�.
			if (mappedValue >= '0' && mappedValue <= '9') {
				lockerNumber = DATA_getSelectedLockerNumber();
				lockerNumber = lockerNumber * 10 + mappedValue - '0';
				//������ ��Ŀ��ȣ�� �����Ѵ�
				if (lockerNumber <= 18) {
					LCD_setRefresh();
					DATA_setSelectedLockerNumber(lockerNumber);
				}
			}
			//�Էµ� ��������ȣ�� ����� ������ ó���Ѵ�
			else if (mappedValue == KEY_CLEAR) {
				LCD_setRefresh();
				lockerNumber = DATA_getSelectedLockerNumber();
				lockerNumber /= 10;
				DATA_setSelectedLockerNumber(lockerNumber);
			}
			//������������ ���� ���ۿ� ���� ó��
			else if (mappedValue == KEY_GO_NEXT) {
				lockerNumber = DATA_getSelectedLockerNumber();
				//������ ��Ŀ�� ��ȣ�� ��ȿ�Ҷ��� ����ȭ������ �Ѿ��
				if (lockerNumber <= 18 && lockerNumber >= 1) {
					LCD_setRefresh();
					DATA_setSystemState(SYSTEM_STATE_ENTER_PASSWORD);
				}
			}
			break;
		}
		//������ ��Ŀ�� ���� ��й�ȣ�� �Է��ϴ� ��� 
		case SYSTEM_STATE_ENTER_PASSWORD: {
			//��ȣ �Է½� ��й�ȣ �Է� ó��
			if (mappedValue >= '0' && mappedValue <= '9') {
				LCD_setRefresh();
				DATA_pushbackPassword(mappedValue - '0');
			}
			//�Էµ� ������ ��ȣ ���� ó��
			else if (mappedValue == KEY_CLEAR) {
				LCD_setRefresh();
				DATA_popbackPassword();
			}			
			//������������ ó���Ѵ�
			else if (mappedValue == KEY_GO_NEXT) {
				LCD_setRefresh();
				DATA_setRequsetPermission(true);
				DATA_setSystemState(SYSTEM_STATE_WAITING_RESPONSE);
			}
			//��й�ȣ �Է��� �ڷ� ���ư��� ó��
			else if (mappedValue == KEY_GO_BACK) {
				//�Էµ� ��й�ȣ�� ��Ŀ��ȣ�� �����Ѵ�
				DATA_clearPassword();
				DATA_setSelectedLockerNumber(LOCKER_NUMBER_UNSELECTED);
				LCD_setRefresh();
				DATA_setSystemState(SYSTEM_STATE_SELECT_LOCKER_NUMBER);
			}
			break;
		}
		//������ ����� �ϴ� ��� ����߿��� �Է� �����ʴ´�
		case SYSTEM_STATE_WAITING_RESPONSE: {
			break;
		}
		//��Ŀ�� ������ ��
		case SYSTEM_STATE_OPEN_SUCCESS: {
			//��Ŀ �ݱ⸦ �������� ��Ŀ�� �ݰ� ��й�ȣ�� ������ 
			//��Ŀ ��ȣ�� �ʱ�ȭ�ϰ� ó�� ȭ������ ���ư���
			if (mappedValue == KEY_CLOSE) {
				MOTOR_setCloseLocker();
				LCD_setRefresh();
				DATA_setSystemState(SYSTEM_STATE_SELECT_LOCKER_NUMBER);
				DATA_clearPassword();
				DATA_setSelectedLockerNumber(LOCKER_NUMBER_UNSELECTED);
			}
			break;
		}
		//��й�ȣ�� Ʋ�� ���⸦ ���� ������
		case SYSTEM_STATE_OPEN_FAIL: {			
			//���Ʊ�� �Ǵ� ����ȭ�� ��ư�� ������ ��й�ȣ�� ������ 
			//��Ŀ ��ȣ�� �ʱ�ȭ�ϰ� ó�� ȭ������ ���ư���
			if (mappedValue == KEY_GO_BACK || mappedValue == KEY_GO_NEXT) {
				LCD_setRefresh();
				DATA_setSystemState(SYSTEM_STATE_SELECT_LOCKER_NUMBER);
				DATA_clearPassword();
				DATA_setSelectedLockerNumber(LOCKER_NUMBER_UNSELECTED);
			}
			break;
		}
		default:
			break;
		}
	}

}




