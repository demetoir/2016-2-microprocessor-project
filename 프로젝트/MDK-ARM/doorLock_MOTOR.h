///*----------------------------------------------------------------------------
// *      doorLock_MOTOR.h
// *----------------------------------------------------------------------------
// *      Name:    doorLock_MOTOR.h
// *---------------------------------------------------------------------------*/

#ifndef  _LPC1768_UTILITY_H_
#include "LPC1768_utility.h"
#endif // !"LPC1768_utility.h"

#ifndef _LPC1768_MOTOR_H_
#include "LPC1768_motor.h"
#endif // !"LPC1768_motor.h"

#ifndef _DOORLOCK_UTILITY_H_
#include "doorLock_utility.h"
#endif // !_doorLock_utility_H

#ifndef _DOORLOCK_MOTOR_H_
#define _DOORLOCK_MOTOR_H_

#define bool 							int
#define false							0
#define true 							1

#define CCW_MOVE_MOTOR_TIME 100
#define CW_MOVE_MOTOR_TIME 100
#define MOTOR_DELAY_TIME 100

#define move 0
#define stop 1
#define DOORLOCK_CLOSE 0
#define DOORLOCK_OPEN 1

void init_stepping_motor(void);
void set_motor_output(int a, int na, int b, int nb);
void StepMotor_Cycle(uint8_t cycle);
void stepMotor_move_cw(void);
void stepMotor_move_ccw(void);

void setOpenDoorLock(void);
void setCloseDoorLock(void);
void move_steppingMotor(void);

#endif








