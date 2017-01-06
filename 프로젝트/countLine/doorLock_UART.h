///*----------------------------------------------------------------------------
// *      doorlock_UART.h
// *----------------------------------------------------------------------------
// *      Name:    doorlock_UART.h
// *---------------------------------------------------------------------------*/
//

#ifndef _LPC17xx_H
#include <LPC17xx.H>
#endif // !<LPC17xx.H>
#ifndef _debug_frmwrk_h
#include "debug_frmwrk.h"
#endif // !"debug_frmwrk.h"

#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "LPC1768_Huins.h"
#include "doorLock_MOTOR.h"
#include "doorLock_LCD.h"
#include "doorLock_data.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



#define bool 							int
#define false							0
#define true 							1
#define NULL							0

#define uart_refresh_time 1
#define uart_handShake_timeout 5
#define UART_BUFFER_SIZE 100
#define UART_Live_signal_Time 5

//		uart msg mark 
#define UART_MSG_MARK_end					"end\n"
#define UART_MSG_MARK_handshake_send		"send_handshake"
#define UART_MSG_MARK_handshake_receive		"receive_handshake"
#define UART_MSG_MARK_connected_confirm		"connected_confirm"
#define UART_MSG_MARK_send					"send"
#define UART_MSG_MARK_receive				"receive"
#define UART_MSG_MARK_disconnect			"disconnect"
#define UART_MSG_MARK_success				"success"
#define UART_MSG_MARK_SycTime				"Syc_time"
#define UART_MSG_MARK_LIVE_SIGNAL			"live_signal"
#define UART_MSG_MARK_request_permission	"request_permission"
#define UART_MSG_MARK_response_permission	"reseponse_permission"
#define UART_MSG_MARK_yes					"yes"
#define UART_MSG_MARK_no					"no"
#define UART_MSG_MARK_check_connection		"check_connection"

void UART_init(void);

void UART_clearBuffer(void);
void UART_sendMsg(char msg[UART_BUFFER_SIZE]);
bool UART_receiveMsg(char buffer[UART_BUFFER_SIZE]);

bool UART_ishandShakeMsg(char msg[UART_BUFFER_SIZE]);
bool UART_isDisconnectMsg(char msg[UART_BUFFER_SIZE]);

void UART_makeHandShakeMsg(char msg[UART_BUFFER_SIZE]);
void UART_makeDisconnectionMsg(char msg[UART_BUFFER_SIZE]);


void UART_communication(void);

void uart_refresh_counter_up(void);
void UART_liveSigal_count_up(void);















