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


//keypad의 입력값을 프로그램에 필요한 값으로 매핑해주는 벡터
char keypadMappingVector[16];
//keypadMappingVector를 초기화한다
//관리의 편의성을 위하여 함수로 초기화한다
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

//keypad의 GPIO의 세팅을 저장하는 변수
LPC_GPIO_TypeDef    keypad_GPIO_SETTING[5];
//keypad의 초기화하는 함수 
void KEYPAD_init(void) {
	KEYPAD_initKeypadMappingVector();	//키패트 매핑 벡터 초기화
	KEYPAD_DIR_Input();					//입력방향지정
	set_EXT_IO_DIRECTION('C');			//EXT IO 지정
	save_LCP_GPIO_settingTo(keypad_GPIO_SETTING);//위의 설정을 저장한다
	Keypad_mainTask(); // 초기화 테스트를 위해 실행한다
}


//이전의 눌린 키패드의 값
//키패드의 값이 변할때마다 갱신되어야한다
int	prevKeypadValue = KEYPAD_UP;
//keypad의 mainTask함수
//system mode에 따라 서로 다른 동작을 한다 
//락커 번호와 비밀번호를 입력 및 정정,
//뒤로가기, 다음 화면, 닫기 버튼등을 처리한다
void Keypad_mainTask(void) {
	int rawKeypadValue	= 0;
	char mappedValue	= '0';																																					// To Revise Keypad #
	int lockerNumber	= 0;
	int mode = DATA_getSystemState();
	
	//keypad를 작동시키위해 GPIO 세팅을 로드한다
	load_LPC_GPIO_settingFrom(keypad_GPIO_SETTING);
	//keypad가 눌린값을 얻어고 매핑된 키패드의 값을 구한다
	rawKeypadValue = KEYPAD_getPressKeyNumber(DEFAULT_KEYPAD_EXIO_NUM);
	mappedValue = keypadMappingVector[rawKeypadValue];

	//눌린것이 없다면 이전값을 갱신하고 반환한다
	if (rawKeypadValue == KEYPAD_UP) {
		prevKeypadValue = KEYPAD_UP;
		return;
	}
	//키패드가 down이 되는 상태를 검출하는 조건문
	//키패드가 눌린 이후 중복되게 입력 
	if (rawKeypadValue != KEYPAD_UP && prevKeypadValue == KEYPAD_UP) {
		prevKeypadValue = rawKeypadValue;

		//서버와 연결이 끊어졌을경우 입력을 처리하지 않는다
		if (DATA_isServerConnected() == false) {
			return ;
		}

		//각 모드에 따른 keypad 동작을 처리한다
		//각 모드에 대해 올바른 입력을 했을떄 LCD_setRefresh를
		//호출하는것으로 lcd 화면을 갱신한다
		switch (mode){		
		//락커의 번호를 선택하는 모드 
		case SYSTEM_STATE_SELECT_LOCKER_NUMBER: {
			//번호를 눌렀을때의 처리
			//락커의 번호는 1~18 까지 있으므로 범위를 넘어가는 값은
			//처리하지않다. 범위를 안에 있다면 선택한 락커번호를 갱신한다.
			if (mappedValue >= '0' && mappedValue <= '9') {
				lockerNumber = DATA_getSelectedLockerNumber();
				lockerNumber = lockerNumber * 10 + mappedValue - '0';
				//선택한 락커번호를 갱신한다
				if (lockerNumber <= 18) {
					LCD_setRefresh();
					DATA_setSelectedLockerNumber(lockerNumber);
				}
			}
			//입력된 마지막번호를 지우는 동작을 처리한다
			else if (mappedValue == KEY_CLEAR) {
				LCD_setRefresh();
				lockerNumber = DATA_getSelectedLockerNumber();
				lockerNumber /= 10;
				DATA_setSelectedLockerNumber(lockerNumber);
			}
			//다음페이지를 선택 동작에 대한 처리
			else if (mappedValue == KEY_GO_NEXT) {
				lockerNumber = DATA_getSelectedLockerNumber();
				//선택한 락커의 번호가 유효할때만 다음화면으로 넘어간다
				if (lockerNumber <= 18 && lockerNumber >= 1) {
					LCD_setRefresh();
					DATA_setSystemState(SYSTEM_STATE_ENTER_PASSWORD);
				}
			}
			break;
		}
		//선택한 락커에 대한 비밀번호를 입력하는 모드 
		case SYSTEM_STATE_ENTER_PASSWORD: {
			//번호 입력시 비밀번호 입력 처리
			if (mappedValue >= '0' && mappedValue <= '9') {
				LCD_setRefresh();
				DATA_pushbackPassword(mappedValue - '0');
			}
			//입력된 마지막 번호 삭제 처리
			else if (mappedValue == KEY_CLEAR) {
				LCD_setRefresh();
				DATA_popbackPassword();
			}			
			//다음페이지를 처리한다
			else if (mappedValue == KEY_GO_NEXT) {
				LCD_setRefresh();
				DATA_setRequsetPermission(true);
				DATA_setSystemState(SYSTEM_STATE_WAITING_RESPONSE);
			}
			//비밀번호 입력중 뒤로 돌아가기 처리
			else if (mappedValue == KEY_GO_BACK) {
				//입력된 비밀번호와 락커번호를 삭제한다
				DATA_clearPassword();
				DATA_setSelectedLockerNumber(LOCKER_NUMBER_UNSELECTED);
				LCD_setRefresh();
				DATA_setSystemState(SYSTEM_STATE_SELECT_LOCKER_NUMBER);
			}
			break;
		}
		//서버와 통신을 하는 모드 통신중에는 입력 받지않는다
		case SYSTEM_STATE_WAITING_RESPONSE: {
			break;
		}
		//락커가 열렸을 때
		case SYSTEM_STATE_OPEN_SUCCESS: {
			//락커 닫기를 눌렀을떄 락커를 닫고 비밀번호와 선택한 
			//락커 번호를 초기화하고 처음 화면으로 돌아간다
			if (mappedValue == KEY_CLOSE) {
				MOTOR_setCloseLocker();
				LCD_setRefresh();
				DATA_setSystemState(SYSTEM_STATE_SELECT_LOCKER_NUMBER);
				DATA_clearPassword();
				DATA_setSelectedLockerNumber(LOCKER_NUMBER_UNSELECTED);
			}
			break;
		}
		//비밀번호에 틀려 열기를 실패 했을때
		case SYSTEM_STATE_OPEN_FAIL: {			
			//돌아기기 또는 다음화면 버튼을 누르면 비밀번호와 선택한 
			//락커 번호를 초기화하고 처음 화면으로 돌아간다
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




