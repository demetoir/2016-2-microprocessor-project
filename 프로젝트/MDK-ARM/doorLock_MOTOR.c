///*----------------------------------------------------------------------------
// *      doorLock_L.MOTOR
// *----------------------------------------------------------------------------
// *      Name:    doorLock_MOTOR.c
// *---------------------------------------------------------------------------*/

#ifndef doorLock_MOTOR_H
#include "doorLock_MOTOR.h"
#endif // !doorLock_MOTOR_H

//모터의 상태를 저장하는 변수 
int LockerState = LOCKER_CLOSE;
//모터의 상태가 변했음 저장하는 플래그 변수
bool isNeedLockerMotorMove = false;

//모터의 GPIO setting을 저장하는 변수
LPC_GPIO_TypeDef    motor_GPIO_SETTING[5];

//모터를 초기화하는 함수 
//모터의 GPIO를 세팅하고 초기화 테스트를 실행을 한다
void MOTOR_init() {
	//모터의 GPIO와 EXT_IO를 설정한다
	set_EXT_IO_DIRECTION('A');
	Motor_Init();

	//초기화 테스트 실행
	//모터를 반시계방향으로 두바퀴 회전시킨다
	MOTOR_moveCCWHalfCycle(); 
	Delay(MOTOR_DELAY_TIME);
	MOTOR_moveCCWHalfCycle();
	Delay(MOTOR_DELAY_TIME);
	MOTOR_moveCCWHalfCycle();
	Delay(MOTOR_DELAY_TIME);
	MOTOR_moveCCWHalfCycle();
	Delay(MOTOR_DELAY_TIME);

	//모터의 GPIO를 저장한다
	save_LCP_GPIO_settingTo(motor_GPIO_SETTING);
}

//motor를 반시계방향으로 반바퀴움직이도록 만든다 
//스텝모터는 내부에 2개의 전자석 a,b를 가진다.
//두개의 전자석의 전류방향을 선택하는것이 하나의 스텝이며
//이러한 스텝의 시퀸스를 거치는것으로 하나의 스텝 사이클을
//구성한다. 또한 이런 12번의 스텝사이클을 반복하는것으로
//모터를 한바퀴 움직이게 된다
//따라서 반바퀴만 움직이려면 12스텝사이클의 반인 6번의 
//스텝사이클을 돌면된다.
//또한 하나의 스텝을 할떄마다 모터가 움직이는 시간을 주기위해
//일정시간만큼의 딜레이가 필요하다
void MOTOR_moveCCWHalfCycle() {
	int count = 0;
	for (count = 0; count < 6; count++) {
		//a, b 방향으로 전류를 흘린다
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_24);
		//모터가 움직일 시간을 주기위한 딜레이
		Delay(MOTOR_DELAY_TIME);
		
		//na, b 방향으로 전류를 흘린다
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_24);
		//모터가 움직일 시간을 주기위한 딜레이
		Delay(MOTOR_DELAY_TIME);

		//na, nb 방향으로 전류를 흘린다
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_24);
		//모터가 움직일 시간을 주기위한 딜레이
		Delay(MOTOR_DELAY_TIME);

		//a, nb 방향으로 전류를 흘린다
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_24);
		//모터가 움직일 시간을 주기위한 딜레이
		Delay(MOTOR_DELAY_TIME);
	}
}

//모터를 움직어 락커를 열도록 모터 상태를 세팅한다
void MOTOR_setOpenLocker() {
	//이전의 락커상태와 다를때만 모터를 움직이도록 한다
	if (LockerState != LOCKER_OPEN)
		isNeedLockerMotorMove = true;
	LockerState = LOCKER_OPEN;
}

//모터를 움직어 락커를 닫도록 모터 상태를 세팅한다
void MOTOR_setCloseLocker() {
	//이전의 락커상태와 다를때만 모터를 움직이도록 한다
	if (LockerState != LOCKER_CLOSE)
		isNeedLockerMotorMove = true;
	LockerState = LOCKER_CLOSE;
}

//모터의 mainTask를 실행하는 함수
void MOTOR_mainTask() {
	//모터가 동작할일이 없다면 바로 종료한다
	if (isNeedLockerMotorMove == false) 
		return;
	//중복으로 모터가 동작을 방지를 위해 플래그 변수를 false로 변경한다
	isNeedLockerMotorMove = false;

	//모터의 GPIO의 값을 로드한다
	load_LPC_GPIO_settingFrom(motor_GPIO_SETTING);

	//Locker의 상태에때라 모터를 움직여 락커를 열거나 닫는다
	if (LockerState == LOCKER_OPEN) {
		MOTOR_moveCCWHalfCycle();
	}
	else if (LockerState == LOCKER_CLOSE) {
		MOTOR_moveCCWHalfCycle();
	}	
}
