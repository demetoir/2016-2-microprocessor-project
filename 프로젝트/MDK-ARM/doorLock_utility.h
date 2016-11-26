///*----------------------------------------------------------------------------
// *      doorLock_utility.h
// *----------------------------------------------------------------------------
// *      Name:    doorLock_utility.h
// *---------------------------------------------------------------------------*/

#define bool 							int
#define false							0
#define true 							1

void copy_GPIOn_setting(LPC_GPIO_TypeDef *source, LPC_GPIO_TypeDef *dest);
void load_LPC_GPIO_setting_to(LPC_GPIO_TypeDef source[5]);
void save_LCP_GPIO_setting_to(LPC_GPIO_TypeDef dest[5]);

void set_EXT_IO_DIRECTION(char EXT_IO_NUM);





