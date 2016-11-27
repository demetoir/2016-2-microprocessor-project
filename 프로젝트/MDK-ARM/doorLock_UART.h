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

#define bool 							int
#define false							0
#define true 							1

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//		uart state
typedef enum {
	SEND_HANDSHAKE_MSG, RECEIVE_HANDSHAKE_MSG,
	UART_CONNECTED, UART_DISCONNECTED
} UART_CONNECTION_STATE;
//		end uart state

#define UART_DEFAULT_INPUT 'X'

#define UART_BUFFER_SIZE 100
									//		uart msg mark 
#define UART_MSG_MARK_end					"end\r\n"
#define UART_MSG_MARK_handshake_send		"send_handshake"
#define UART_MSG_MARK_handshake_receive		"receive_handshake"
#define UART_MSG_MARK_connected_confirm		"connected_confirm"
#define UART_MSG_MARK_send					"send"
#define UART_MSG_MARK_receive				"receive"
#define UART_MSG_MARK_disconnect			"disconnect"
#define UART_MSG_MARK_success				"success"
#define uart_refresh_time 1

#define uart_seq_num_size  100

#define uart_handShake_timeout 5

void uart_init(void);
void uart_clearBuffer(void);
void uart_sendMsg(char msg[UART_BUFFER_SIZE]);
bool uart_receiveMsg(char buffer[UART_BUFFER_SIZE]);
bool ishandShakeMsg(char msg[UART_BUFFER_SIZE]);
void uart_makeHandShakeMsg(char msg[UART_BUFFER_SIZE], int seqnum);
void UART_communication(void);


void uart_refresh_counter_up(void);


bool isDisconnectMsg(char msg[UART_BUFFER_SIZE]);
void uart_makeDisconnectionMsg(char msg[UART_BUFFER_SIZE]);


















