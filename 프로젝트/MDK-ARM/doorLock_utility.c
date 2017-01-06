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

// arm processor�� �ϳ��� GPIO�� ������ �ִ�
// arm processor���� GPIO�� LPC_GPIO��� ����ü�� ǥ���ǰ� 
//�̰��� LPC_GPIO0 ~ LPC_GPIO4���� �� 5���� ���� ������.  

//GPIOn�� ���� �����Ѵ�
void copy_GPIOn_setting(LPC_GPIO_TypeDef *source, LPC_GPIO_TypeDef *dest) {
	dest->FIODIR = source->FIODIR;
	dest->FIOMASK = source->FIOMASK;
	dest->FIOPIN = source->FIOPIN;
	dest->FIOCLR = source->FIOCLR;
	dest->FIOSET = source->FIOSET;
}
//arm processor�� ���Ǵ� LPC_GPIO�� GPIO_setting�� �ε��Ѵ�
void load_LPC_GPIO_settingFrom(LPC_GPIO_TypeDef GPIO_setting[5]) {
	copy_GPIOn_setting(&(GPIO_setting[0]), LPC_GPIO0);
	copy_GPIOn_setting(&(GPIO_setting[1]), LPC_GPIO1);
	copy_GPIOn_setting(&(GPIO_setting[2]), LPC_GPIO2);
	copy_GPIOn_setting(&(GPIO_setting[3]), LPC_GPIO3);
	copy_GPIOn_setting(&(GPIO_setting[4]), LPC_GPIO4);
}
//arm processor�� ���Ǵ� LPC_GPIO�� GPIO_setting�� �����Ѵ�
void save_LCP_GPIO_settingTo(LPC_GPIO_TypeDef GPIO_setting[5]) {
	copy_GPIOn_setting(LPC_GPIO0, &(GPIO_setting[0]));
	copy_GPIOn_setting(LPC_GPIO1, &(GPIO_setting[1]));
	copy_GPIOn_setting(LPC_GPIO2, &(GPIO_setting[2]));
	copy_GPIOn_setting(LPC_GPIO3, &(GPIO_setting[3]));
	copy_GPIOn_setting(LPC_GPIO4, &(GPIO_setting[4]));
}

//LCP_GPIO�� ���� �����̵� EXT IO�� ����
//�̰��� ���������� EXT IO A, B, C �� LCP_GPIO�� �����̵Ǿ� 
//�����Ϸ��� EXT IO ���� �͵��� �������� �����Ѵ�
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




