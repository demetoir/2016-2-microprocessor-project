///*----------------------------------------------------------------------------
// *      doorLock_L.MOTOR
// *----------------------------------------------------------------------------
// *      Name:    doorLock_MOTOR.c
// *---------------------------------------------------------------------------*/


#ifndef doorLock_MOTOR_H
#include "doorLock_MOTOR.h"
#endif // !doorLock_MOTOR_H

int motor_state = stop;
bool isNeed_doorLock_motor_move = false;

LPC_GPIO_TypeDef    step_motor_GPIO_SETTING[5];

void init_stepping_motor() {
	set_EXT_IO_DIRECTION('A');
	Motor_Init();
	//move_steppingMotor();
	save_LCP_GPIO_setting_to(step_motor_GPIO_SETTING);
}

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
		Delay(SEC_1 / MOTOR_DELAY_TIME);

		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_24);
		Delay(SEC_1 / MOTOR_DELAY_TIME);

		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_24);
		Delay(SEC_1 / MOTOR_DELAY_TIME);

		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_5);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_10);
		GPIO_ClearValue(GPIO_PORT_0, GPIO_PIN_23);
		GPIO_SetValue(GPIO_PORT_0, GPIO_PIN_24);
		Delay(SEC_1 / MOTOR_DELAY_TIME);
	}
}

void stepMotor_move_cw() {

	uint32_t count = 0;
	//GPIO_SetValue(GPIO_PORT_3, GPIO_PIN_26);
	for (count = 0; count < 6; count++) {
		set_motor_output(1, 1, 0, 0);
		Delay(SEC_1 / BACKWARD_MOTOR_TIME);
		set_motor_output(0, 1, 1, 0);
		Delay(SEC_1 / BACKWARD_MOTOR_TIME);
		set_motor_output(0, 0, 1, 1);
		Delay(SEC_1 / BACKWARD_MOTOR_TIME);
		set_motor_output(1, 0, 0, 1);
		Delay(SEC_1 / BACKWARD_MOTOR_TIME);
	}
}

void stepMotor_move_ccw() {
	StepMotor_Cycle(1);
}

void setOpenDoorLock() {
	if (motor_state == DOORLOCK_CLOSE)
		isNeed_doorLock_motor_move = true;
	motor_state = DOORLOCK_OPEN;
}
void setCloseDoorLock() {
	if (motor_state == DOORLOCK_OPEN)
		isNeed_doorLock_motor_move = true;
	motor_state = DOORLOCK_CLOSE;
}


void move_steppingMotor() {
	if (isNeed_doorLock_motor_move == false) 
		return;

	load_LPC_GPIO_setting_to(step_motor_GPIO_SETTING);
	if (motor_state == DOORLOCK_OPEN) {
		stepMotor_move_cw();
		isNeed_doorLock_motor_move = false;
	}
	else if (motor_state == DOORLOCK_CLOSE) {
		//back ward cycle
		stepMotor_move_ccw();
		isNeed_doorLock_motor_move = false;
	}
}
