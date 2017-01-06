///*----------------------------------------------------------------------------
// *      doorLock_utility.h
// *----------------------------------------------------------------------------
// *      Name:    doorLock_utility.h
// *---------------------------------------------------------------------------*/

#ifndef _LPC17xx_H
#include <LPC17xx.H>
#endif // !<LPC17xx.H>


#ifndef _DOORLOCK_UTILITY_H_
#define _DOORLOCK_UTILITY_H_
#define bool 							int
#define false							0
#define true 							1

void copy_GPIOn_setting(LPC_GPIO_TypeDef *source, LPC_GPIO_TypeDef *dest);
void load_LPC_GPIO_settingFrom(LPC_GPIO_TypeDef source[5]);
void save_LCP_GPIO_settingTo(LPC_GPIO_TypeDef dest[5]);

void set_EXT_IO_DIRECTION(char EXT_IO_NUM);

#endif 


