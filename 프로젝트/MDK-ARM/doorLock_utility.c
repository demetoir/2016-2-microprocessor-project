///*----------------------------------------------------------------------------
// *      doorLock_utility.h
// *----------------------------------------------------------------------------
// *      Name:    doorLock_utility.h
// *---------------------------------------------------------------------------*/
//		GPIO setting function

//
#ifndef _LPC17xx_H
#include <LPC17xx.H>
#endif // !<LPC17xx.H>


#ifndef _doorLock_utility_h	
#include "doorLock_utility.h"
#endif //!"doorlock_utility.h"

void copy_GPIOn_setting(LPC_GPIO_TypeDef *source, LPC_GPIO_TypeDef *dest) {
	dest->FIODIR = source->FIODIR;
	dest->FIOMASK = source->FIOMASK;
	dest->FIOPIN = source->FIOPIN;
	dest->FIOCLR = source->FIOCLR;
	dest->FIOSET = source->FIOSET;
}

void load_LPC_GPIO_setting_to(LPC_GPIO_TypeDef source[5]) {
	copy_GPIOn_setting(&(source[0]), LPC_GPIO0);
	copy_GPIOn_setting(&(source[1]), LPC_GPIO1);
	copy_GPIOn_setting(&(source[2]), LPC_GPIO2);
	copy_GPIOn_setting(&(source[3]), LPC_GPIO3);
	copy_GPIOn_setting(&(source[4]), LPC_GPIO4);
}

void save_LCP_GPIO_setting_to(LPC_GPIO_TypeDef dest[5]) {
	copy_GPIOn_setting(LPC_GPIO0, &(dest[0]));
	copy_GPIOn_setting(LPC_GPIO1, &(dest[1]));
	copy_GPIOn_setting(LPC_GPIO2, &(dest[2]));
	copy_GPIOn_setting(LPC_GPIO3, &(dest[3]));
	copy_GPIOn_setting(LPC_GPIO4, &(dest[4]));
}
//		end GPIO setting function

//		etc function
void set_EXT_IO_DIRECTION(char EXT_IO_NUM) {
	if (EXT_IO_NUM == 'A' || EXT_IO_NUM == 'a') {
		LPC_GPIO0->FIOPIN &= ~(1 << 19); // EXT_IO_A On
		LPC_GPIO0->FIOPIN |= (1 << 20);  // EXT_IO_B Off
		LPC_GPIO0->FIOPIN |= (1 << 21);  // EXT_IO_C Off
	}
	else if (EXT_IO_NUM == 'B' || EXT_IO_NUM == 'b'){
		LPC_GPIO0->FIOPIN |= (1 << 19); 	// EXT_IO_A Off
		LPC_GPIO0->FIOPIN &= ~(1 << 20);  // EXT_IO_B On
		LPC_GPIO0->FIOPIN |= (1 << 21);  	// EXT_IO_C Off
	}
	else if (EXT_IO_NUM == 'C' || EXT_IO_NUM == 'c') {
		LPC_GPIO0->FIOPIN |= (1 << 19); 	// EXT_IO_A Off
		LPC_GPIO0->FIOPIN |= (1 << 20); 	// EXT_IO_B Off
		LPC_GPIO0->FIOPIN &= ~(1 << 21);  // EXT_IO_C On
	}
} 
//		etc function




