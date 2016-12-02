///*----------------------------------------------------------------------------
// *      doorlock_KEYPAD.h
// *----------------------------------------------------------------------------
// *      Name:    doorlock_KEYPAD.h
// *---------------------------------------------------------------------------*/
//

#ifndef _lpc17xx_gpio_h
#include "lpc17xx_gpio.h"
#endif // !"lpc17xx_gpio.h"


//#include "Keypad.H"

#ifndef  _DOORLOCK_LCD_H_
#include "doorLock_LCD.h"
#endif // !"doorLock_LCD.h"

#ifndef _DOORLOCK_MOTOR_H_
#include "doorLock_MOTOR.h"
#endif // !_doorLock_MOTOR_H

#ifndef _DOORLOCK_FND_H_
#include "doorLock_FND.h"
#endif // !_doorLock_FND_H

#ifndef _DOORLOCK_UTILITY_H_
#include "doorLock_utility.h"
#endif // !_doorLock_utility_H

#include "doorLock_data.h"


#ifndef _DOORLOCK_KEYPAD_H_
#define _DOORLOCK_KEYPAD_H_


#define KEYPAD_UP						16
#define CHAR_KEYPAD_UP					'X'
#define	DEFAULT_KEYPAD_EXIO_NUM			'C'

//////////////////// Mapping //////////////////////////////
//
// cKeypad_Value       Rev_Value
// --------------      ----------
// 0						1
// 1						2
// 2    					3
// 3						C		(Reservated 1)
// 4						4
// 5						5
// 6						6
// 7						D		(Reservated 2)
// 8						7
// 9						8
// A						9
// B						E		(Reservated 3)
// C						L		(LOCK)
// D						0
// E						O       (OPEN)
// F						F		(Reservated 4)
//
// * --- * --- * --- * --- *
// |   1  |   2   |   3   |  C  |
// * --- * --- * --- * --- *
// |   4  |   5   |   6   |  R  |
// * --- * --- * --- * --- *
// |   7  |   8   |   9   |  E  |
// * --- * --- * --- * --- *
// |   L  |   0   |   O   |  F  |
// * --- * --- * --- * --- *
//
////////////////////////////////////////////////////////////

void EXT_IO_init(void);
void Keypad_DIR_Output(void);
void Keypad_DIR_Input(void);
void EXT_IO_A_CS(void);
void EXT_IO_B_CS(void);
void EXT_IO_C_CS(void);
void Keypad_Output_Row_1(void);
void Keypad_Output_Row_2(void);
void Keypad_Output_Row_3(void);
void Keypad_Output_Row_4(void);
uint8_t Keypad_Input(uint8_t Low_Num);
uint8_t Keypad(int8_t EXT_IO_DIR);

int getKEYPAD_ivalue(void);
char getKEYPAD_cvalue(void);

void init_keypad(void);
void Keypad_test(void);
void init_keypad_vactor(void);
#endif // !_doorlock_keypad_H_




