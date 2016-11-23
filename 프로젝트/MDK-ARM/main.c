///*----------------------------------------------------------------------------
// *      main.c
// *----------------------------------------------------------------------------
// *      Name:    main.c
// *---------------------------------------------------------------------------*/
//

/* Includes ----------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////
#include <LPC17xx.H>
#include "LPC1768_utility.h"
#include "lpc17xx_timer.h"
#include "LPC1768_motor.h"
#include "LPC1768_fnd.h"
#include "GLCD.h"
#include "LED.H"
#include "Keypad.H"
#include "lpc17xx_gpio.h"
#include "LPC17xx.h"
#include "lpc_types.h"
#include "LPC1768_Huins.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#include <string.h>
////////////////////////////////////////////////////////////////////////////////////
/* Includes --------------------------------------------------------------------- */


/* define ------------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////
#define FND_BLINK_DELAY_TIME 			5000
#define bool 							int
#define false							0
#define true 							1
#define KEYPAD_UP						16
#define CHAR_KEYPAD_UP					'X'
////////////////////////////////////////////////////////////////////////////////////
/* end define --------------------------------------------------------------------*/


/* Private variables -------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////
//		GPIO setting variables
LPC_GPIO_TypeDef    FND_GPIO_SETTING[5];
LPC_GPIO_TypeDef    keypad_GPIO_SETTING[5];
LPC_GPIO_TypeDef    step_motor_GPIO_SETTING[5];
LPC_GPIO_TypeDef    default_GPIO_SETTING[5];
LPC_GPIO_TypeDef    cleared_GPIO_SETTING[5];
//		end GPIO setting variables

//		timer variables
uint8_t time_10m 				= 	0;
uint8_t time_1m 				=	0;
uint8_t time_10s 				= 	0;
uint8_t time_1s 				= 	0;

uint8_t lap_time_10m 			=	0;
uint8_t lap_time_1m 			=	0;
uint8_t lap_time_10s 			= 	0;
uint8_t lap_time_1s				= 	0;

char cTime_1s					=	0;
char cTime_10s					=	0;
char cTime_1m					=	0;
char cTime_10m					=	0;

char clap_time_10m 				=	0;
char clap_time_1m 				=	0;
char clap_time_10s 				= 	0;
char clap_time_1s				= 	0;

TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct;
//		end timer variables

//		lcd variables
bool PAINT_LCD 					= 	false;
//		end lcd variables

//		keypad varaibles
uint8_t Keypad_Value			= 	KEYPAD_UP;
char 	cKeypad_Value 			=	CHAR_KEYPAD_UP;
//		end keypad varaibles

//		stepping motor variables
typedef enum { move, stop, } MOTOR_STATE;
MOTOR_STATE motor_state = stop;
//		end stepping motor variables

//		uart variables
#define UART_DEFAULT_INPUT 'X'
char char_uart_input = UART_DEFAULT_INPUT; 
#define UART_BUFFER_SIZE 100
uint8_t ch;
uint8_t aTxBuffer[UART_BUFFER_SIZE];	// 문자열 저장 버퍼
uint8_t buffer_count = 0;			// 현재 Write 버퍼 위치
//		end uart varaibles

////////////////////////////////////////////////////////////////////////////////////
/* end Private variables ---------------------------------------------------------*/


/*declare function ---------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////
//		GPIO setting function
void copy_GPIOn_setting(LPC_GPIO_TypeDef *source, LPC_GPIO_TypeDef *dest);
void save_LPC_GPIO_setting_to(LPC_GPIO_TypeDef source[5]);
void load_LCP_GPIO_setting_to(LPC_GPIO_TypeDef setting[5]);
//		end GPIO setting function

//		etc function ??
void set_EXT_IO_DIRECTION(char EXT_IO_NUM);
//		end etc function ??

//		module function
void LCD_refresh(void);
void FND_blink(void);
void Keypad_test(void);

void move_steppingMotor(void);
void StepMotor_back_Cycle(uint8_t cycle);
//		end modduel function

//		init function
void init_lcd(void);
void init_keypad(void);
void init_EXTI(void);
void init_FND(void);
void init_timer0(void);
void init(void);
void uart_init(void);
//		end init function 

////////////////////////////////////////////////////////////////////////////////////////
/*end declare function ---------------------------------------------------------------*/

/* Private function ------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////
//		GPIO setting function
void copy_GPIOn_setting(LPC_GPIO_TypeDef *source, LPC_GPIO_TypeDef *dest){
	dest->FIODIR = source->FIODIR;
	dest->FIOMASK = source->FIOMASK;
	dest->FIOPIN = source->FIOPIN;
	dest->FIOCLR = source->FIOCLR;
	dest->FIOSET = source->FIOSET;
}

void save_LPC_GPIO_setting_to(LPC_GPIO_TypeDef source[5]){
	copy_GPIOn_setting(&(source[0]), LPC_GPIO0);
	copy_GPIOn_setting(&(source[1]), LPC_GPIO1);
	copy_GPIOn_setting(&(source[2]), LPC_GPIO2);
	copy_GPIOn_setting(&(source[3]), LPC_GPIO3);
	copy_GPIOn_setting(&(source[4]), LPC_GPIO4);
}

void load_LCP_GPIO_setting_to(LPC_GPIO_TypeDef setting[5]){
	copy_GPIOn_setting(LPC_GPIO0, &(setting[0]));
	copy_GPIOn_setting(LPC_GPIO1, &(setting[1]));
	copy_GPIOn_setting(LPC_GPIO2, &(setting[2]));
	copy_GPIOn_setting(LPC_GPIO3, &(setting[3]));
	copy_GPIOn_setting(LPC_GPIO4, &(setting[4]));
}
//		end GPIO setting function

//		init function
void init_lcd() {
	GLCD_init();                              /* Initialize the GLCD           */
	GLCD_clear(White);                        /* Clear the GLCD                */
	PAINT_LCD = true;
	LCD_refresh();
}

void init_keypad() {
	Keypad_DIR_Input();
	set_EXT_IO_DIRECTION('C');
	Keypad_test();
	load_LCP_GPIO_setting_to(keypad_GPIO_SETTING);
}

void init_EXTI() {
	EXTI_Init();	// EXTI 초기화
	AF_ConfigPin(GPIO_PORT_2, PINSEL_PIN_10, PINSEL_FUNC_1);	// 단순 GPIO 기능이 아닌 Alternate Function을 사용하기 위해 설정
	EXTI_ConfigPin(EXTI_EINT0); // EXTI0 설정
	NVIC_EnableIRQ(EINT0_IRQn); // EXTI0 활성화
}

void init_FND() {
	FND_Init();	// FND 사용 PIN 초기화
	set_EXT_IO_DIRECTION('B');
	FND_blink();
	load_LCP_GPIO_setting_to(FND_GPIO_SETTING);
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

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);	//  TIM_ConfigStruct 설정 적용
	TIM_ConfigMatch(LPC_TIM0,&TIM_MatchConfigStruct);		// TIM_MatchConfigStruct 설정 적용
	NVIC_EnableIRQ(TIMER0_IRQn);	//	TIMER0 Interrupt 활성화
	TIM_Cmd(LPC_TIM0, ENABLE);	//	Timer Start
}

void init_stepping_motor() {
	set_EXT_IO_DIRECTION('A');
	Motor_Init();
	move_steppingMotor();
	load_LCP_GPIO_setting_to(step_motor_GPIO_SETTING);
}

void init(){
	load_LCP_GPIO_setting_to(default_GPIO_SETTING);
	init_FND();
	init_timer0();
	init_lcd();
	init_keypad();
	init_EXTI();
	init_stepping_motor();
	uart_init();
}
//		init function

//		etc function
void set_EXT_IO_DIRECTION(char EXT_IO_NUM) {
	if(EXT_IO_NUM == 'A' || EXT_IO_NUM == 'a')
		EXT_IO_A_CS();
	else if(EXT_IO_NUM == 'B' || EXT_IO_NUM == 'b')
		EXT_IO_B_CS();
	else if (EXT_IO_NUM == 'C' || EXT_IO_NUM == 'c')
		EXT_IO_C_CS();
}
//		etc function

//		modduel function
//			lcd function
void LCD_refresh (void){
	if(PAINT_LCD == false) return;
	PAINT_LCD = false;

	GLCD_setBackColor(Blue);                           /* Set the Text Color */
	GLCD_setTextColor(White);                          /* Set the Text Color */
	GLCD_displayStringLn(Line0, "   CortexM3_NXP     ");
	GLCD_displayStringLn(Line1, "    RTX Blinky      ");
	GLCD_displayStringLn(Line2, "   www.huins.com    ");

	cTime_1s = time_1s + '0';
	cTime_10s = time_10s + '0';
	cTime_1m = time_1m + '0';
	cTime_10m = time_10m + '0';
	GLCD_displayChar(20,80,cTime_10m );
	GLCD_displayChar(40,80,cTime_1m);
	GLCD_displayChar(60,80,cTime_10s);
	GLCD_displayChar(80,80,cTime_1s);

	GLCD_displayChar(160,80,cKeypad_Value);

	GLCD_displayChar(200, 80,char_uart_input);

}

//			FND function
void FND_blink(){
	/* time mmss */
	FND_COM_DATA_Select(8,time_1s);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(7,time_10s);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(6,time_1m);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(5,time_10m);
	Delay(FND_BLINK_DELAY_TIME);

	/* Lap Time  */
	FND_COM_DATA_Select(4,lap_time_1s);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(3,lap_time_10s);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(2,lap_time_1m);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(1,lap_time_10m);
	Delay(FND_BLINK_DELAY_TIME);

	//clear FND
	FND_Data_Reset();
	FND_Data_CS();
	FND_COM_Init();
	FND_COM_CS();
	FND_Init();
}

//			keypad function
void Keypad_test(){
	int Keypad_Value = Keypad('C');
	if (Keypad_Value == KEYPAD_UP)
		cKeypad_Value = CHAR_KEYPAD_UP;
	else if (Keypad_Value >= 10 )
		cKeypad_Value = Keypad_Value + 'A' - 10;
	else if (Keypad_Value < 10 )
		cKeypad_Value = Keypad_Value + '0';

}

//			uart function 
bool is_uart_connected = false;
#define uart_msg_mark_connection	"connnect_uart"



//통신의 끝은 eof???
typedef enum {SEND_CONNECTION_MSG, RECEIVE_CONNECTION_MSG, 
			UART_CONNECTED, UART_DISCONNECTED
} UART_CONNECTION_STATE;
UART_CONNECTION_STATE UART_current_state = UART_DISCONNECTED;
bool UART_is_refresh = false;
int uart_refresh_count = 0;

void uart_init(void) {
	Uart0_Init();	// UART0 초기화
	UARTPuts(LPC_UART0, "\r\n hello world \r\n");
	UART_is_refresh = true;
}

// void uart_clear_buffer() {
	// memset(aTxBuffer, 0x00, sizeof(aTxBuffer)); // Buffer 클리어					
	// buffer_count = 0;
// }

// void uart_send_msg(char msg[UART_BUFFER_SIZE]) {
	// UARTPuts(LPC_UART0, msg);
// }

// int uart_receive_msg(char msg[UART_BUFFER_SIZE]) {
	// int bufSize = 0;

	// ch = UART_ReceiveByte(LPC_UART0);	// Polling으로 Data 읽어오기
	// //receive connection msg
	// while (ch) {
		// msg[bufSize] = ch;
		// if (bufSize == UART_BUFFER_SIZE) break;
		// bufSize++;
	// }

	// return bufSize;
// }

// void UART_communication(void) {
	// if (UART_is_refresh == false) return;
	// UART_is_refresh = false;

	// if (UART_current_state == UART_DISCONNECTED
		// || UART_current_state == SEND_CONNECTION_MSG) {
		// //send connection msg 5초에 한번씩 새로만듦

		// int seqnum;
		// char cSeqnum[5];
		// //get seqnum from  cSeqnum

		// uart_send_msg(msg);
	// }
	// else if (UART_current_state == RECEIVE_CONNECTION_MSG){		
		// //receive connection msg
		// buffer_count = uart_receive_msg(aTxBuffer);

		// if (successconnection) {


		// }
		// else {



		// }

		// //clear buffer
		// //UARTPuts(LPC_UART0, "\r\n");	// CR(Carriage Return), LF(Line Feed) 개행
		// uart_clear_buffer();
	// }
	// else if (UART_current_state == UART_CONNECTED){
		// //send msg
		// //send some msg
		

		// //receive msg
		// //uart_receive_msg(msg);

		// // msg 처리

	// }
	// //this error case
	// else {

	// }
	
	// return;
// }
//			end uart function 


//			stepMotor function
#define forward_motor_time 150
#define backward_motor_time 150
#define motor_time 200

void set_motor_output(int a, int na, int b, int nb) {
	if (a)	GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_5);
	else GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_5);

	if (b)	GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_23);
	else GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_23);

	if (na)	GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_10);
	else GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_10);

	if (nb)	GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_24);
	else GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_24);
}

void StepMotor_Cycle(uint8_t cycle) {
	uint32_t count = 0;
	for (count = 0; count < cycle * 6; count++) {
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_24);
		Delay(SEC_1 / motor_time);

		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_24);
		Delay(SEC_1 / motor_time);

		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_24);
		Delay(SEC_1 / motor_time);

		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_24);
		Delay(SEC_1 / motor_time);
	}
}

void StepMotor_move_CWhalf(uint8_t cycle) {
	uint32_t count = 0;
	//GPIO_SetValue(GPIO_PORT_3, GPIO_PIN_26);
	for (count = 0; count < cycle * 6; count++) {
		set_motor_output(1, 1, 0, 0);
		Delay(SEC_1 / backward_motor_time);
		set_motor_output(0, 1, 1, 0);
		Delay(SEC_1 / backward_motor_time);
		set_motor_output(0, 0, 1, 1);
		Delay(SEC_1 / backward_motor_time);
		set_motor_output(1, 0, 0, 1);
		Delay(SEC_1 / backward_motor_time);
	}
}

void stepMotor_move_cw() {
	uint32_t count = 0;
	//GPIO_SetValue(GPIO_PORT_3, GPIO_PIN_26);
	for (count = 0; count <6; count++) {
		set_motor_output(1, 1, 0, 0);
		Delay(SEC_1 / backward_motor_time);
		set_motor_output(0, 1, 1, 0);
		Delay(SEC_1 / backward_motor_time);
		set_motor_output(0, 0, 1, 1);
		Delay(SEC_1 / backward_motor_time);
		set_motor_output(1, 0, 0, 1);
		Delay(SEC_1 / backward_motor_time);
	}
}

void stepMotor_move_ccw() {
	StepMotor_Cycle(1);
}

void move_steppingMotor() {
	if (cKeypad_Value == '1') {
		stepMotor_move_cw();
	}
	else if (cKeypad_Value == '3') {
		//back ward cycle
		stepMotor_move_ccw();
	}
}
//			endstepMotor function

//		end module function


/* end Private function --------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////


int main(void){		
	init();

	while (1){
		save_LPC_GPIO_setting_to(FND_GPIO_SETTING);
		FND_blink();

		save_LPC_GPIO_setting_to(keypad_GPIO_SETTING);
		Keypad_test();

		LCD_refresh();
		
		save_LPC_GPIO_setting_to(step_motor_GPIO_SETTING);
		move_steppingMotor();

		//UART_communication();
	}
}

/* INTERRUPT HANDLER -----------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////

// call every 1 sec
void TIMER0_IRQHandler(void){
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT); //TIM0 interrupt clear

	// timer for 1sec,10sec,1minute,10minute
	time_1s++;
	if(time_1s == 10){
		time_10s++;
		time_1s=0;
	}
	if(time_10s == 6){
		time_1m++;
		time_10s = 0;
	}
	if(time_1m == 10){
		time_10m++;
		time_1m = 0;
	}
	if(time_10m == 6){
		time_10m =0 ;
		time_1m =0 ;
		time_10s = 0;
		time_1s = 0;
	}

	//repaint
	PAINT_LCD = true;


	//uart_refresh count 
	uart_refresh_count++;
	if (uart_refresh_count == 5) {
		UART_is_refresh = true;
		uart_refresh_count = 0;
	}
}

// INT button pressed
void EINT0_IRQHandler(void){
	//clear INTERRUPT bit
	EXTI_ClearEXTIFlag(EXTI_EINT0);

	//capture current time.
	lap_time_10m = time_10m;
	lap_time_1m = time_1m;
	lap_time_1s = time_1s;
	lap_time_10s = time_10s;
	motor_state = move;
}

////////////////////////////////////////////////////////////////////////////////////////
/* end INTERRUPT HANDLER -------------------------------------------------------------*/






