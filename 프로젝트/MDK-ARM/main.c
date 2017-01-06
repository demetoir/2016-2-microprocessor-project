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

#include "doorlock_data.h"
#include "doorLock_utility.h"
#include "doorLock_LCD.h"
#include "doorLock_MOTOR.h"
#include "doorLock_KEYPAD.h"
#include "doorLock_UART.h"
////////////////////////////////////////////////////////////////////////////////////
/* end Includes --------------------------------------------------------------------- */


/*declare function ---------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct;
void init_timer0(void);
void DATA_timeUpdate(void);
void TIMER0_IRQHandler(void);

void init(void);
////////////////////////////////////////////////////////////////////////////////////////
/*end declare function ---------------------------------------------------------------*/

//사용할 기능들을 모두 초기화한다
void init() {
	init_timer0();
	LCD_init();
	KEYPAD_init();
	MOTOR_init();
	UART_init();
}

//main
int main(void) {
	//사용할 모든 기능을 초기화
	init();

	//무한 루프를 돌면서 각 기능이 동작하게만든다
	while (1) {
		Keypad_mainTask();
		LCD_mainTask();
		MOTOR_mainTask();
		UART_mainTask();
	}
}

/* INTERRUPT HANDLER -----------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////
// call every 1 sec
void init_timer0() {
	//timer0 init
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;	//	us(microsecond) 기준으로 Prescale 설정
	TIM_ConfigStruct.PrescaleValue = 100;					//	100us
	TIM_MatchConfigStruct.MatchChannel = 0;					//	0번 채널 사용
	TIM_MatchConfigStruct.IntOnMatch = ENABLE;				//	Timer가 일치할 때  Interrupt 발생 Enable
	TIM_MatchConfigStruct.ResetOnMatch = ENABLE;			//	Timer가 일치할 때  Reset Enable
	TIM_MatchConfigStruct.StopOnMatch = DISABLE;			//	Timer가 일치할 때 Timer Stop Disable
	TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;	//	Timer 일치할때 아무런 외부 출력 하지 않음 (외부로 출력하지 않기 때문에 GPIO를 설정하지 않아도 됨.)
	TIM_MatchConfigStruct.MatchValue = 10000;				// Timer 일치 값 설정 100us * 10000번 = 1초

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIM_ConfigStruct);	//  TIM_ConfigStruct 설정 적용
	TIM_ConfigMatch(LPC_TIM0, &TIM_MatchConfigStruct);		// TIM_MatchConfigStruct 설정 적용
	NVIC_EnableIRQ(TIMER0_IRQn);							//	TIMER0 Interrupt 활성화
	TIM_Cmd(LPC_TIM0, ENABLE);								//	Timer Start
}

//매초마다 인터럽트 핸들러에 진한다 
void TIMER0_IRQHandler(void) {
	//timer0의 인터럽트 비트를 클리어하여 다음 1초 후에 진입하도록 만든다
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT); 

	//현재 시간을 1초가 지난 시간으로 업데이트 한다 
	DATA_timeUpdate();	

	//1초마다 lcd 화면이 갱신이 하도록 설정한다
	LCD_setRefresh();

	//arm 프로세서가 서버와 연결되어있다면 연결확인을 위한 카운터를 갱신한다
	if (DATA_isServerConnected())
		DATA_connectionCountDown();

	//비밀번호를 확인하기위해 전달한 메세지를 위한 카운터를 갱신한다
	if (DATA_isRequestedPermission())
		DATA_sendingMsgCountDown();
}





