///*----------------------------------------------------------------------------
// *      doorlock_FND.c
// *----------------------------------------------------------------------------
// *      Name:    doorlock_FND.c
// *---------------------------------------------------------------------------*/
//

#ifndef doorLock_FND_h
#include "doorLock_FND.h"
#endif // !doorLock_FND_h


LPC_GPIO_TypeDef    FND_GPIO_SETTING[5];

int FNDtime_10h;
int FNDtime_1h;
int FNDtime_10m;
int FNDtime_1m;
int FNDtime_10s;
int FNDtime_1s;

void setFNDTime() {
	int hour, minute, second;
	hour = getTime(HOUR);
	minute = getTime(MINUTE);
	second = getTime(SECOND);

	FNDtime_10h		= hour / 10;
	FNDtime_1h		= hour % 10;
	FNDtime_10m	= minute / 10;
	FNDtime_1m		= minute % 10;
	FNDtime_10s		= second / 10;
	FNDtime_1s		= second % 10;
}

void init_FND() {
	FNDtime_10h = 0;
	FNDtime_1h = 0;
	FNDtime_10m = 0;
	FNDtime_1m = 0;
	FNDtime_10s = 0;
	FNDtime_1s = 0;

	FND_Init();	// FND 사용 PIN 초기화
	set_EXT_IO_DIRECTION('B');
	save_LCP_GPIO_setting_to(FND_GPIO_SETTING);
	FND_blink();
}

void FND_blink() {
	load_LPC_GPIO_setting_to(FND_GPIO_SETTING);

	setFNDTime();

	/* time hhmmss */
	FND_COM_DATA_Select(8, FNDtime_1s);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(7, FNDtime_10s);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(6, FNDtime_1m);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(5, FNDtime_10m);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(4, FNDtime_1h);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(3, FNDtime_10h);
	Delay(FND_BLINK_DELAY_TIME);


	//forcheck
	FND_COM_DATA_Select(2, 0);
	Delay(FND_BLINK_DELAY_TIME);
	FND_COM_DATA_Select(1, 0);
	Delay(FND_BLINK_DELAY_TIME);

	//clear FND
	FND_Data_Reset();
	FND_Data_CS();
	FND_COM_Init();
	FND_COM_CS();
	FND_Init();
}



