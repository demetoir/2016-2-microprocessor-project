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

uint8_t iKeypad_Value = KEYPAD_UP;
char 	cKeypad_Value = CHAR_KEYPAD_UP;



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
	return iKeypad_Value;
}
char getKEYPAD_cvalue(void) {
	return cKeypad_Value;
}

void init_keypad(void) {
	Keypad_DIR_Input();
	set_EXT_IO_DIRECTION('C');
	save_LCP_GPIO_setting_to(keypad_GPIO_SETTING);
	Keypad_test();
}

void Keypad_test(void) {
	int Keypad_Value = 0;
	load_LPC_GPIO_setting_to(keypad_GPIO_SETTING);
	Keypad_Value = Keypad(DEFAULT_KEYPAD_EXIO_NUM);

	if (Keypad_Value == KEYPAD_UP)
		cKeypad_Value = CHAR_KEYPAD_UP;
	else if (Keypad_Value >= 10)
		cKeypad_Value = Keypad_Value + 'A' - 10;
	else if (Keypad_Value < 10)
		cKeypad_Value = Keypad_Value + '0';

	setLCDkey(cKeypad_Value);

	if (cKeypad_Value == '1')
		setCloseDoorLock();
	else if (cKeypad_Value == '3')
		setOpenDoorLock();
}



