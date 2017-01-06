///*----------------------------------------------------------------------------
// *      doorLock_utility.h
// *----------------------------------------------------------------------------
// *      Name:    doorLock_utility.h
// *---------------------------------------------------------------------------*/
//		GPIO setting function

//


#ifndef _doorLock_utility_h	
#include "doorLock_utility.h"
#endif //!"doorlock_utility.h"

// GPIO setting function

// arm processor는 하나의 GPIO를 가지고 있다
// arm processor에서 GPIO는 LPC_GPIO라는 구조체로 표현되고 
//이것은 LPC_GPIO0 ~ LPC_GPIO4까지 총 5개의 값을 가진다.  

//GPIOn의 값을 복사한다
void copy_GPIOn_setting(LPC_GPIO_TypeDef *source, LPC_GPIO_TypeDef *dest) {
	dest->FIODIR = source->FIODIR;
	dest->FIOMASK = source->FIOMASK;
	dest->FIOPIN = source->FIOPIN;
	dest->FIOCLR = source->FIOCLR;
	dest->FIOSET = source->FIOSET;
}
//arm processor에 사용되는 LPC_GPIO에 GPIO_setting를 로드한다
void load_LPC_GPIO_settingFrom(LPC_GPIO_TypeDef GPIO_setting[5]) {
	copy_GPIOn_setting(&(GPIO_setting[0]), LPC_GPIO0);
	copy_GPIOn_setting(&(GPIO_setting[1]), LPC_GPIO1);
	copy_GPIOn_setting(&(GPIO_setting[2]), LPC_GPIO2);
	copy_GPIOn_setting(&(GPIO_setting[3]), LPC_GPIO3);
	copy_GPIOn_setting(&(GPIO_setting[4]), LPC_GPIO4);
}
//arm processor에 사용되는 LPC_GPIO를 GPIO_setting에 저장한다
void save_LCP_GPIO_settingTo(LPC_GPIO_TypeDef GPIO_setting[5]) {
	copy_GPIOn_setting(LPC_GPIO0, &(GPIO_setting[0]));
	copy_GPIOn_setting(LPC_GPIO1, &(GPIO_setting[1]));
	copy_GPIOn_setting(LPC_GPIO2, &(GPIO_setting[2]));
	copy_GPIOn_setting(LPC_GPIO3, &(GPIO_setting[3]));
	copy_GPIOn_setting(LPC_GPIO4, &(GPIO_setting[4]));
}

//LCP_GPIO의 값이 적용이될 EXT IO를 고른다
//이것을 하지않으면 EXT IO A, B, C 에 LCP_GPIO이 적용이되어 
//동작하려는 EXT IO 외의 것들이 동작함을 방지한다
void set_EXT_IO_DIRECTION(char EXT_IO_NUM) {
	if (EXT_IO_NUM == 'A' || EXT_IO_NUM == 'a') {
		LPC_GPIO0->FIOPIN &= ~(1 << 19);	// EXT_IO_A On
		LPC_GPIO0->FIOPIN |= (1 << 20);		// EXT_IO_B Off
		LPC_GPIO0->FIOPIN |= (1 << 21);		// EXT_IO_C Off
	}
	else if (EXT_IO_NUM == 'B' || EXT_IO_NUM == 'b'){
		LPC_GPIO0->FIOPIN |= (1 << 19); 	// EXT_IO_A Off
		LPC_GPIO0->FIOPIN &= ~(1 << 20);	// EXT_IO_B On
		LPC_GPIO0->FIOPIN |= (1 << 21);  	// EXT_IO_C Off
	}
	else if (EXT_IO_NUM == 'C' || EXT_IO_NUM == 'c') {
		LPC_GPIO0->FIOPIN |= (1 << 19); 	// EXT_IO_A Off
		LPC_GPIO0->FIOPIN |= (1 << 20); 	// EXT_IO_B Off
		LPC_GPIO0->FIOPIN &= ~(1 << 21);	// EXT_IO_C On
	}
} 




