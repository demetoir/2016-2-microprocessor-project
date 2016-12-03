///*----------------------------------------------------------------------------
// *      doorlock_KEYPAD.c
// *----------------------------------------------------------------------------
// *      Name:    doorlock_KEYPAD.c
// *---------------------------------------------------------------------------*/
//


#ifndef doorlock_KEYPAD_H
#include "doorLock_KEYPAD.h"
#endif // !doorlock_KEYPAD_H


LPC_GPIO_TypeDef    keypad_GPIO_SETTING[5];

uint8_t iKeypadValue	= KEYPAD_UP;
char 	cKeypad_Value	= CHAR_KEYPAD_UP;
int		prevKeypadValue = KEYPAD_UP;

void EXT_IO_init(void) {
	/*EXT_IO on PORT0 as Output*/
	LPC_GPIO0->FIODIR |= (1 << 4) | (1 << 19) | (1 << 20) | (1 << 21);	// Mainboard Chip Select
	LPC_GPIO1->FIODIR |= (1 << 21);
	LPC_GPIO2->FIODIR |= (1 << 11);
}

void Keypad_DIR_Output(void) {
	/* PORT0 defined as Output    */
	LPC_GPIO0->FIODIR |= (1 << 5);
	LPC_GPIO0->FIODIR |= (1 << 10);
	/* PORT2 defined as output    */
	LPC_GPIO2->FIODIR |= (1 << 12);
	LPC_GPIO2->FIODIR |= (1 << 13);
}

void Keypad_DIR_Input(void) {
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

void Keypad_Output_Row_1(void) {
	Keypad_DIR_Output();
	/* PORT0 defined as Output    */
	LPC_GPIO0->FIOPIN &= ~(1 << 5);
	LPC_GPIO0->FIOPIN |= (1 << 10);
	/* PORT2 defined as output    */
	LPC_GPIO2->FIOPIN |= (1 << 12);
	LPC_GPIO2->FIOPIN |= (1 << 13);

	LPC_GPIO2->FIOPIN |= (1 << 11); // CS Clock High Edge
	LPC_GPIO2->FIOPIN &= ~(1 << 11);
}

void Keypad_Output_Row_2(void) {
	Keypad_DIR_Output();
	/* PORT0 defined as Output    */
	LPC_GPIO0->FIOPIN |= (1 << 5);
	LPC_GPIO0->FIOPIN &= ~(1 << 10);
	/* PORT2 defined as output    */
	LPC_GPIO2->FIOPIN |= (1 << 12);
	LPC_GPIO2->FIOPIN |= (1 << 13);

	LPC_GPIO2->FIOPIN |= (1 << 11); // CS Clock High Edge
	LPC_GPIO2->FIOPIN &= ~(1 << 11);
}

void Keypad_Output_Row_3(void) {
	Keypad_DIR_Output();
	/* PORT0 defined as Output    */
	LPC_GPIO0->FIOPIN |= (1 << 5);
	LPC_GPIO0->FIOPIN |= (1 << 10);
	/* PORT2 defined as output    */
	LPC_GPIO2->FIOPIN &= ~(1 << 12);
	LPC_GPIO2->FIOPIN |= (1 << 13);

	LPC_GPIO2->FIOPIN |= (1 << 11); // CS Clock High Edge
	LPC_GPIO2->FIOPIN &= ~(1 << 11);
}

void Keypad_Output_Row_4(void) {
	Keypad_DIR_Output();
	/* PORT0 defined as Output    */
	LPC_GPIO0->FIOPIN |= (1 << 5);
	LPC_GPIO0->FIOPIN |= (1 << 10);
	/* PORT2 defined as output    */
	LPC_GPIO2->FIOPIN |= (1 << 12);
	LPC_GPIO2->FIOPIN &= ~(1 << 13);

	LPC_GPIO2->FIOPIN |= (1 << 11); // CS Clock High Edge
	LPC_GPIO2->FIOPIN &= ~(1 << 11);
}

uint8_t Keypad_Input(uint8_t Low_Num) {
	uint32_t Key_val = 0;
	LPC_GPIO1->FIOPIN &= ~(1 << 21);  // Keypad Input CS On

	if (Low_Num == 1) {
		if ((LPC_GPIO0->FIOPIN >> 23) & 0x01) Key_val = 0;
		else Key_val = 1;
	}
	if (Low_Num == 2) {
		if ((LPC_GPIO0->FIOPIN >> 24) & 0x01) Key_val = 0;
		else Key_val = 1;
	}
	if (Low_Num == 3) {
		if ((LPC_GPIO3->FIOPIN >> 25) & 0x01) Key_val = 0;
		else Key_val = 1;
	}
	if (Low_Num == 4) {
		if ((LPC_GPIO3->FIOPIN >> 26) & 0x01) Key_val = 0;
		else Key_val = 1;
	}

	LPC_GPIO1->FIOPIN |= (1 << 21);  // Keypad Input CS Off

	return Key_val;
}

uint8_t Keypad(int8_t EXT_IO_DIR) {
	uint8_t Key_Temp, Key_Value = 16;

	if (EXT_IO_DIR == 'A')	EXT_IO_A_CS();
	else if (EXT_IO_DIR == 'B') EXT_IO_B_CS();
	else EXT_IO_C_CS();

	Keypad_Output_Row_1();
	Key_Temp = Keypad_Input(1);
	if (Key_Temp == 1) Key_Value = 3;
	Key_Temp = Keypad_Input(2);
	if (Key_Temp == 1) Key_Value = 2;
	Key_Temp = Keypad_Input(3);
	if (Key_Temp == 1) Key_Value = 1;
	Key_Temp = Keypad_Input(4);
	if (Key_Temp == 1) Key_Value = 0;

	Keypad_Output_Row_2();
	Key_Temp = Keypad_Input(1);
	if (Key_Temp == 1)  Key_Value = 7;
	Key_Temp = Keypad_Input(2);
	if (Key_Temp == 1)  Key_Value = 6;
	Key_Temp = Keypad_Input(3);
	if (Key_Temp == 1)  Key_Value = 5;
	Key_Temp = Keypad_Input(4);
	if (Key_Temp == 1)  Key_Value = 4;

	Keypad_Output_Row_3();
	Key_Temp = Keypad_Input(1);
	if (Key_Temp == 1)  Key_Value = 11;
	Key_Temp = Keypad_Input(2);
	if (Key_Temp == 1)  Key_Value = 10;
	Key_Temp = Keypad_Input(3);
	if (Key_Temp == 1)  Key_Value = 9;
	Key_Temp = Keypad_Input(4);
	if (Key_Temp == 1)  Key_Value = 8;

	Keypad_Output_Row_4();
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


int getKEYPAD_ivalue(void) {
	return iKeypadValue;
}
char getKEYPAD_cvalue(void) {
	return cKeypad_Value;
}

void init_keypad(void) {
	init_keypad_vactor();
	Keypad_DIR_Input();
	set_EXT_IO_DIRECTION('C');
	save_LCP_GPIO_setting_to(keypad_GPIO_SETTING);
	Keypad_test();
}

///////////////////////////////////////////////////////////////////////// 추가부분
char keypad_vector[16];
void init_keypad_vactor(void) {

	//number pad
	keypad_vector[0] = '1';
	keypad_vector[1] = '2';
	keypad_vector[2] = '3';
	keypad_vector[4] = '4';
	keypad_vector[5] = '5';
	keypad_vector[6] = '6';
	keypad_vector[8] = '7';
	keypad_vector[9] = '8';
	keypad_vector[10] = '9';
	keypad_vector[13] = '0';
	
	//function key
	keypad_vector[3] = 'C';
	keypad_vector[7] = 'R';
	keypad_vector[11] = 'E';
	keypad_vector[15] = 'F';

	//open lock
	keypad_vector[12] = 'O';
	keypad_vector[14] = 'L';

}

void Keypad_test(void) {
	int iKeypadValue = 0;
	char Rev_Value = '0';																																					// To Revise Keypad #
	int locknumber = 0;
	int mode = getCurrentSystemMode();
	load_LPC_GPIO_setting_to(keypad_GPIO_SETTING);
	iKeypadValue = Keypad(DEFAULT_KEYPAD_EXIO_NUM);
	Rev_Value = keypad_vector[iKeypadValue];

	//if (iKeypadValue == prevKeypadValue) 
	//	return;

	if (iKeypadValue == KEYPAD_UP) {
		prevKeypadValue = KEYPAD_UP;
	}
	if (iKeypadValue != KEYPAD_UP && prevKeypadValue == KEYPAD_UP) {
		prevKeypadValue = iKeypadValue;

		if (isServerConnected() == false) {
			return ;
		}

		switch (mode){
		
		case SYSTEM_MODE_ENTER_PASSWORD: {
			if (Rev_Value >= '0' && Rev_Value <= '9') {
				setLCD_refresh();
				setPwd(Rev_Value - '0');
			}
			else if (Rev_Value == 'L') {
				setCloseDoorLock();
			}
			else if (Rev_Value == 'O') {
				setLCD_refresh();
				setOpenDoorLock();
			}
			else if (Rev_Value == 'C') {
				setLCD_refresh();
				popbackPwd();
			}
			//
			else if (Rev_Value == 'R') {
				//check password
				setLCD_refresh();
				setRequsetPermission(true);
				setCurrentSustemMode(SYSTEM_MODE_WAITING);

			}
			break;
		}
		case SYSTEM_MODE_SELECT_LOCKER_NUMBER: {
			if (Rev_Value >= '0' && Rev_Value <= '9') {
				locknumber = getSelectedLockerNumber();
				locknumber = locknumber * 10 + Rev_Value - '0';
				if (locknumber <= 100) {
					setLCD_refresh();
					setSelectedLockerNumber(locknumber);
				}
			}
			else if (Rev_Value == 'C') {
				setLCD_refresh();
				locknumber = getSelectedLockerNumber();
				locknumber /= 10;
				setSelectedLockerNumber(locknumber);
			}
			else if (Rev_Value == 'R') {
				setLCD_refresh();
				setCurrentSustemMode(SYSTEM_MODE_ENTER_PASSWORD);
			}
			break;
		}
		case SYSTEM_MODE_OPEN_SUCCESS: {
			if (Rev_Value == 'L') {
				setLCD_refresh();
				setCurrentSustemMode(SYSTEM_MODE_SELECT_LOCKER_NUMBER);
				clearPwd();
				setSelectedLockerNumber(0);
				setCloseDoorLock();
			}
			break;
		}
		case SYSTEM_MODE_OPEN_FAIL: {
			if (Rev_Value == 'R') {
				setLCD_refresh();
				setCurrentSustemMode(SYSTEM_MODE_SELECT_LOCKER_NUMBER);
				clearPwd();
				setSelectedLockerNumber(0);
			}
			break;
		}
		case SYSTEM_MODE_WAITING: {
			break;
		}
		default:
			break;
		}
	}

}




