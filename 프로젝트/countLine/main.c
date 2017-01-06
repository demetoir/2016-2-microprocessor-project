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

//#ifndef _debug_frmwrk_h
//#include "debug_frmwrk.h"
//#endif // !"debug_frmwrk.h"

#include "doorlock_data.h"
#include "doorLock_utility.h"
#include "doorLock_LCD.h"
#include "doorLock_MOTOR.h"
#include "doorLock_FND.h"
#include "doorLock_KEYPAD.h"
#include "doorLock_UART.h"
////////////////////////////////////////////////////////////////////////////////////
/* end Includes --------------------------------------------------------------------- */


/*declare function ---------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////

////		timer0

TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct;
void init_timer0(void);
void time_update(void);
void TIMER0_IRQHandler(void);
//		end timer0

// INT button pressed
void init_EXTI(void);
void EINT0_IRQHandler(void);

// system total init leave in main.c ??
void init(void);

////////////////////////////////////////////////////////////////////////////////////////
/*end declare function ---------------------------------------------------------------*/

/* Private function ------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////
//		init function
void init() {
	init_FND();
	init_timer0();
	init_lcd();
	init_keypad();
	init_EXTI();
	init_stepping_motor();
	UART_init();
}
//		init function
/* end Private function --------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////


int main(void) {
	init();
	while (1) {
		FND_blink();
		Keypad_test();
		LCD_refresh();
		move_steppingMotor();
		UART_communication();
	}
}

/* INTERRUPT HANDLER -----------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////

// call every 1 sec
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


void TIMER0_IRQHandler(void) {
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT); //TIM0 interrupt clear

	// timer for 1sec,10sec,1minute,10minute
	time_update();	

	//repaint
	setLCD_refresh();

	//uart_refresh count 
	uart_refresh_counter_up();
	UART_liveSigal_count_up();

	if (isServerConnected())
		connectionCountDown();

	if (isRequestedPermission())
		sendingMsgCountDown();
}

// INT button pressed
void init_EXTI() {
	EXTI_Init();	// EXTI 초기화
	AF_ConfigPin(GPIO_PORT_2, PINSEL_PIN_10, PINSEL_FUNC_1);	// 단순 GPIO 기능이 아닌 Alternate Function을 사용하기 위해 설정
	EXTI_ConfigPin(EXTI_EINT0); // EXTI0 설정
	NVIC_EnableIRQ(EINT0_IRQn); // EXTI0 활성화
}
void EINT0_IRQHandler(void) {
	//clear INTERRUPT bit
	EXTI_ClearEXTIFlag(EXTI_EINT0);

	//capture current time.
	setOpenDoorLock();
}



////////////////////////////////////////////////////////////////////////////////////////
/* end INTERRUPT HANDLER -------------------------------------------------------------*/









