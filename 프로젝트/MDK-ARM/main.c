///*----------------------------------------------------------------------------
// *      main.c
// *----------------------------------------------------------------------------
// *      Name:    main.c
// *---------------------------------------------------------------------------*/
//

/* Includes ----------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////

#ifndef _LPC17xx_H
#include <LPC17xx.H>
#endif // !<LPC17xx.H>

#ifndef  _LPC1768_utility_h
#include "LPC1768_utility.h"
#endif // !"LPC1768_utility.h"

#ifndef _lpc17xx_timer_h
#include "lpc17xx_timer.h"
#endif // !"lpc17xx_timer.h"

#ifndef _LPC1768_motor_h
#include "LPC1768_motor.h"
#endif // !"LPC1768_motor.h"

#ifndef _LPC1768_fnd_h
#include "LPC1768_fnd.h"
#endif // !"LPC1768_fnd.h"

#ifndef _LED_H
#include "LED.H"
#endif // !"LED.H"

#ifndef _Keypad_H
#include "Keypad.H"
#endif // !"Keypad.H"

#ifndef _lpc17xx_gpio_h
#include "lpc17xx_gpio.h"
#endif // !"lpc17xx_gpio.h"

#ifndef _LPC17xx_h
#include "LPC17xx.h"
#endif // !"LPC17xx.h"

#ifndef _lpc_types_h
#include "lpc_types.h"
#endif // !"lpc_types.h"

#ifndef _LPC1768_Huins_h
#include "LPC1768_Huins.h"
#endif // !"LPC1768_Huins.h"

#ifndef _lpc17xx_pinsel_h
#include "lpc17xx_pinsel.h"
#endif // !"lpc17xx_pinsel.h"

#ifndef _debug_frmwrk_h
#include "debug_frmwrk.h"
#endif // !"debug_frmwrk.h"



#ifndef _doorLock_utility_h
#include "doorLock_utility.h"
#endif // !_doorLock_utility_H

#ifndef  _doorLock_LCD_h
#include "doorLock_LCD.h"
#endif // !"doorLock_LCD.h"

#ifndef _doorLock_MOTOR_h
#include "doorLock_MOTOR.h"
#endif // !_doorLock_MOTOR_H



#include <string.h>
#include <stdlib.h>
#include <stdio.h>
////////////////////////////////////////////////////////////////////////////////////
/* end Includes --------------------------------------------------------------------- */


/* define ------------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////
#define bool 							int
#define false							0
#define true 							1
////////////////////////////////////////////////////////////////////////////////////
/* end define --------------------------------------------------------------------*/


/* Private variables -------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
/* end Private variables ---------------------------------------------------------*/


/*declare function ---------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////

/// utility function ??
//		GPIO setting function
LPC_GPIO_TypeDef    FND_GPIO_SETTING[5];
LPC_GPIO_TypeDef    keypad_GPIO_SETTING[5];
LPC_GPIO_TypeDef    default_GPIO_SETTING[5];

//		end GPIO setting function

/// end utility function ??


//		fnd fucntion
#define FND_BLINK_DELAY_TIME 			5000
void init_FND(void);
void FND_blink(void);
//		end fnd function

//		keypad function
#define KEYPAD_UP						16
#define CHAR_KEYPAD_UP					'X'
uint8_t Keypad_Value = KEYPAD_UP;
char 	cKeypad_Value = CHAR_KEYPAD_UP;
void init_keypad(void);
void Keypad_test(void);
//		end keypad function


//		uart function
bool is_uart_connected = false;

//통신의 끝은 eof???
//		uart state
typedef enum {
	SEND_HANDSHAKE_MSG, RECEIVE_HANDSHAKE_MSG,
	UART_CONNECTED, UART_DISCONNECTED
} UART_CONNECTION_STATE;
UART_CONNECTION_STATE UART_current_state = UART_DISCONNECTED;
//		end uart state

#define UART_DEFAULT_INPUT 'X'
char char_uart_input = UART_DEFAULT_INPUT;

#define UART_BUFFER_SIZE 100
uint8_t ch;
uint8_t aTxBuffer[UART_BUFFER_SIZE];	// 문자열 저장 버퍼
uint8_t buffer_count = 0;			// 현재 Write 버퍼 위치

									//		uart msg mark 
#define UART_MSG_MARK_end					"end\r\n"
#define UART_MSG_MARK_handshake_send		"send_handshake"
#define UART_MSG_MARK_handshake_receive		"receive_handshake"
#define UART_MSG_MARK_connected_confirm		"connected_confirm"
#define UART_MSG_MARK_send					"send"
#define UART_MSG_MARK_receive				"receive"
#define UART_MSG_MAKR_disconnect			"disconnect"
									//		end uart msg mark

									//		uart refresh
#define uart_refresh_time 5
bool UART_refresh = false;
int uart_refresh_time_count = 0;
//		end uart refresh

//		uart seq num
#define uart_seq_num_size  100
int uart_seq_num = 0;
//		end uart seq num

//		uart hand shake time
#define uart_handShake_timeout 5
int uart_handShake_time_count = 0;
//		end uart hand shake time

void uart_init(void);
void uart_clearBuffer(void);
void uart_sendMsg(char msg[UART_BUFFER_SIZE]);
bool uart_receiveMsg(char buffer[UART_BUFFER_SIZE]);
bool ishandShakeMsg(char msg[UART_BUFFER_SIZE]);
void uart_makeHandShakeMsg(char msg[UART_BUFFER_SIZE], int seqnum);
void UART_communication(void);

//		end uart function

//		interrupt key
void init_EXTI(void);

//		timer0
uint8_t time_10h = 0;
uint8_t time_1h = 0;
uint8_t time_10m = 0;
uint8_t time_1m = 0;
uint8_t time_10s = 0;
uint8_t time_1s = 0;

uint8_t lap_time_10h = 0;
uint8_t lap_time_1h = 0;
uint8_t lap_time_10m = 0;
uint8_t lap_time_1m = 0;
uint8_t lap_time_10s = 0;
uint8_t lap_time_1s = 0;


TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct;

void init_timer0(void);
//		end timer0


// system total init leave in main.c ??
void init(void);

//		end modduel function


void time_update(void);

void TIMER0_IRQHandler(void);

// INT button pressed
void EINT0_IRQHandler(void);




////////////////////////////////////////////////////////////////////////////////////////
/*end declare function ---------------------------------------------------------------*/

/* Private function ------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////

//		init function

void init_EXTI() {
	EXTI_Init();	// EXTI 초기화
	AF_ConfigPin(GPIO_PORT_2, PINSEL_PIN_10, PINSEL_FUNC_1);	// 단순 GPIO 기능이 아닌 Alternate Function을 사용하기 위해 설정
	EXTI_ConfigPin(EXTI_EINT0); // EXTI0 설정
	NVIC_EnableIRQ(EINT0_IRQn); // EXTI0 활성화
}


void init_timer0() {
	//timer0 init
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;	//	us(microsecond) 占쏙옙占쏙옙占쏙옙占쏙옙 Prescale 占쏙옙占쏙옙
	TIM_ConfigStruct.PrescaleValue = 100;	//	100us
	TIM_MatchConfigStruct.MatchChannel = 0;	//	0占쏙옙 채占쏙옙 占쏙옙占쏙옙
	TIM_MatchConfigStruct.IntOnMatch = ENABLE;	//	Timer占쏙옙 占쏙옙치占쏙옙 占쏙옙  Interrupt 占쌩삼옙 Enable
	TIM_MatchConfigStruct.ResetOnMatch = ENABLE;	//	Timer占쏙옙 占쏙옙치占쏙옙 占쏙옙  Reset Enable
	TIM_MatchConfigStruct.StopOnMatch = DISABLE;	//	Timer占쏙옙 占쏙옙치占쏙옙 占쏙옙 Timer Stop Disable
	TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;	//	Timer 占쏙옙치占쌀띰옙 占싣뱄옙占쏙옙 占쌤븝옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙 (占쌤부뤄옙 占쏙옙占쏙옙占쏙옙占쏙옙 占십깍옙 占쏙옙占쏙옙占쏙옙 GPIO占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占십아듸옙 占쏙옙.)
	TIM_MatchConfigStruct.MatchValue = 10000;	// Timer 占쏙옙치 占쏙옙 占쏙옙占쏙옙 100us * 10000占쏙옙 = 1占쏙옙

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIM_ConfigStruct);	//  TIM_ConfigStruct 설정 적용
	TIM_ConfigMatch(LPC_TIM0, &TIM_MatchConfigStruct);		// TIM_MatchConfigStruct 설정 적용
	NVIC_EnableIRQ(TIMER0_IRQn);	//	TIMER0 Interrupt 활성화
	TIM_Cmd(LPC_TIM0, ENABLE);	//	Timer Start
}


void init() {
	save_LCP_GPIO_setting_to(default_GPIO_SETTING);
	init_FND();
	init_timer0();
	init_lcd();
	init_keypad();
	init_EXTI();
	init_stepping_motor();
	uart_init();
}
//		init function



//		modduel function
//			FND function
void init_FND() {
	FND_Init();	// FND 사용 PIN 초기화
	set_EXT_IO_DIRECTION('B');
	FND_blink();
	save_LCP_GPIO_setting_to(FND_GPIO_SETTING);
}
void FND_blink() {

	/* time hhmmss */
	FND_COM_DATA_Select(8, time_1s);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(7, time_10s);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(6, time_1m);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(5, time_10m);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(4, time_1h);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(3, time_10h);
	Delay(FND_BLINK_DELAY_TIME);


	//forcheck
	FND_COM_DATA_Select(2, 0);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(1, 0);
	Delay(FND_BLINK_DELAY_TIME);

	//clear FND
	FND_Data_Reset();
	FND_Data_CS();
	FND_COM_Init();
	FND_COM_CS();
	FND_Init();
}
//			end sFND function

//			keypad function
void init_keypad() {
	Keypad_DIR_Input();
	set_EXT_IO_DIRECTION('C');
	Keypad_test();
	save_LCP_GPIO_setting_to(keypad_GPIO_SETTING);
}

void Keypad_test() {
	int Keypad_Value = Keypad('C');
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
//			end keypad function


//			uart function 

//  ????

void uart_init(void) {
	Uart0_Init();	// UART0 초기화
	UARTPuts(LPC_UART0, "hello world this is arm processor \r\n");
	UART_refresh = true;
	uart_seq_num = 0;
	uart_handShake_time_count = 0;
}

void uart_clearBuffer(void) {
	memset(aTxBuffer, 0x00, sizeof(aTxBuffer)); // Buffer 클리어					
	buffer_count = 0;
}

void uart_sendMsg(char msg[UART_BUFFER_SIZE]) {
	UARTPuts(LPC_UART0, msg);
}

bool uart_receiveMsg(char buffer[UART_BUFFER_SIZE]) {
	bool isReceivedMsg;
	int i = 0;
	char received_char;
	char received_msg[UART_BUFFER_SIZE];
	bool is_received_char = false;
	memset(received_msg, 0, sizeof(received_msg));

	if (buffer_count == UART_BUFFER_SIZE) return false;

	for (i = 0; i < UART_BUFFER_SIZE; i++) {
		is_received_char = UART_Receive(LPC_UART0, (uint8_t *)received_msg, UART_BUFFER_SIZE, NONE_BLOCKING);	// Polling으로 Data 읽어오기
		if (is_received_char == 0) break;

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

	if (isReceivedMsg) return true;
	else return false;
}

void uart_makeHandShakeMsg(char msg[UART_BUFFER_SIZE], int seqnum) {
	// hand shake mark + seq + end
	char num[10];
	memset(num, 0x00, sizeof(num));

	//i to a
	num[0] = uart_seq_num / 10+'0';
	num[1] = uart_seq_num % 10 + '0';

	sprintf(msg, "%s %s %s", UART_MSG_MARK_handshake_send, num, UART_MSG_MARK_end);
}

bool ishandShakeMsg(char msg[UART_BUFFER_SIZE]) {
	char temp_str[UART_BUFFER_SIZE];
	char num[10];
	memset(num, 0x00, sizeof(num));
	memset(temp_str, 0x00, sizeof(temp_str));

	//i to a
	num[0] = uart_seq_num / 10 + '0';
	num[1] = uart_seq_num % 10 + '0';
	
	sprintf(temp_str, "%s %s %s", UART_MSG_MARK_handshake_receive, num, UART_MSG_MARK_end);
	if (strcmp(msg, temp_str) == 0)
		return true;
	else 
		return false;
}

void UART_communication(void) {
	char temp_msg[UART_BUFFER_SIZE];
	bool isHandShakeSuccess = false;

	memset(temp_msg, 0, sizeof(temp_msg));

	if (UART_refresh == false) return;
	UART_refresh = false;

	if (UART_current_state == UART_DISCONNECTED
		|| UART_current_state == SEND_HANDSHAKE_MSG) {
		//send hand shake msg
		memset(temp_msg, 0x00, sizeof(temp_msg));
		uart_makeHandShakeMsg(temp_msg, uart_seq_num);
		uart_sendMsg(temp_msg);
		UART_current_state = RECEIVE_HANDSHAKE_MSG;

		memset(aTxBuffer, 0x00, sizeof(aTxBuffer));
	}
	else if (UART_current_state == RECEIVE_HANDSHAKE_MSG) {
		UART_refresh = true;

		//receive nothing 
		if (uart_receiveMsg((char *)aTxBuffer) == false ) return;
			
		isHandShakeSuccess = ishandShakeMsg((char *)aTxBuffer);

		
		// hand shake success
		if (isHandShakeSuccess) {
			UART_current_state = UART_CONNECTED;
			uart_handShake_time_count = 0;
			uart_sendMsg("arm : success hand shake \r\n");
		}
		//handshake time out 
		else if (uart_handShake_time_count > uart_handShake_timeout) {
			uart_seq_num = (uart_seq_num + 1) % uart_seq_num_size;
			UART_current_state = SEND_HANDSHAKE_MSG;
			uart_sendMsg("arm : time out hand shake \r\n");
		}	

	}
	else if (UART_current_state == UART_CONNECTED) {

		uart_sendMsg("arm : CONNECTED \n");
		//check sending msg
	
		//send msg
		//send some msg
		

		//receive msg
		//uart_receive_msg(msg);

	}
	
	return;
}
//			end uart function 


/* end Private function --------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////


int main(void) {
	init();

	while (1) {
		load_LPC_GPIO_setting_to(FND_GPIO_SETTING);
		FND_blink();

		load_LPC_GPIO_setting_to(keypad_GPIO_SETTING);
		Keypad_test();

		LCD_refresh();

		//load_LCP_GPIO_setting_to(step_motor_GPIO_SETTING);
		move_steppingMotor();

		UART_communication();
	}
}

/* INTERRUPT HANDLER -----------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////

// call every 1 sec

void time_update(void) {
	time_1s++;
	if (time_1s == 10) {
		time_10s++;
		time_1s = 0;
	}
	if (time_10s == 6) {
		time_1m++;
		time_10s = 0;
	}
	if (time_1m == 10) {
		time_10m++;
		time_1m = 0;
	}
	if (time_10m == 6) {
		time_10m = 0;
		time_1m = 0;
		time_10s = 0;
		time_1s = 0;
	}

	
}


void TIMER0_IRQHandler(void) {
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT); //TIM0 interrupt clear

	// timer for 1sec,10sec,1minute,10minute
	time_update();
		

	//repaint
	setLCD_refresh();
	setLCDTime(time_10h, time_1h, time_10m, time_1m, time_10s, time_1s);

	//uart_refresh count 
	uart_refresh_time_count = (uart_refresh_time_count+1)%uart_refresh_time;
	if (uart_refresh_time_count == 0) {
		UART_refresh = true;
	}

	uart_refresh_time_count++;
	
}

// INT button pressed
void EINT0_IRQHandler(void) {
	//clear INTERRUPT bit
	EXTI_ClearEXTIFlag(EXTI_EINT0);

	//capture current time.
	lap_time_10m = time_10m;
	lap_time_1m = time_1m;
	lap_time_1s = time_1s;
	lap_time_10s = time_10s;
	setOpenDoorLock();
}

////////////////////////////////////////////////////////////////////////////////////////
/* end INTERRUPT HANDLER -------------------------------------------------------------*/






