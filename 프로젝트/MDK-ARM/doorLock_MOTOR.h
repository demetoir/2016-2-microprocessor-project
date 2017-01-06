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

#define MOTOR_DELAY_TIME		(SEC_1 / 150)

#define move					0
#define stop					1
#define LOCKER_CLOSE			0		
#define LOCKER_OPEN				1

void MOTOR_init(void);
void MOTOR_moveCCWHalfCycle(void);
void MOTOR_setOpenLocker(void);
void MOTOR_setCloseLocker(void);
void MOTOR_mainTask(void);

#endif








