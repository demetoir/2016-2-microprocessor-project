///*----------------------------------------------------------------------------
// *      doorlock_FND.h
// *----------------------------------------------------------------------------
// *      Name:    doorlock_FND.h
// *---------------------------------------------------------------------------*/
//

#ifndef _LPC1768_fnd_h
#include "LPC1768_fnd.h"
#endif // !"LPC1768_fnd.h"

#ifndef  _LPC1768_utility_h
#include "LPC1768_utility.h"
#endif // !"LPC1768_utility.h"

#ifndef _DOORLOCK_UTILITY_H_
#include "doorLock_utility.h"
#endif // !_doorLock_utility_H

#include "doorLock_data.h"

#ifndef _DOORLOCK_FND_H_
#define _DOORLOCK_FND_H_

#define bool 							int
#define false							0
#define true 							1


#define FND_BLINK_DELAY_TIME 			5000

void setFNDTime(void);
void init_FND(void);
void FND_blink(void);



#endif // !_doorlock_FND_H_


