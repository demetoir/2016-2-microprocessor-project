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

#define uart_refresh_time				1
#define uart_handShake_timeout			5
#define UART_BUFFER_SIZE				100
#define UART_Live_signal_Time			5

//		uart msg mark 
// uart 통신을 위한 프로토콜에 사용되는 mark
// protocol format : "HEADMARK + BODYMARK or data + FOOTMARK"
#define UART_MSG_HEADMARK_handshake_send		"send_handshake"
#define UART_MSG_HEADMARK_handshake_receive		"receive_handshake"
#define UART_MSG_HEADMARK_connected_confirm		"connected_confirm"
#define UART_MSG_HEADMARK_send					"send"
#define UART_MSG_HEADMARK_receive				"receive"
#define UART_MSG_HEADMARK_disconnect			"disconnect"
#define UART_MSG_HEADMARK_SyncTime				"Syc_time"
#define UART_MSG_HEADMARK_request_permission	"request_permission"
#define UART_MSG_HEADMARK_response_permission	"reseponse_permission"
#define UART_MSG_HEADMARK_check_connection		"check_connection"

#define UART_MSG_BODYMARK_yes					"yes"
#define UART_MSG_BODYMARK_no					"no"
#define UART_MSG_BODYMARK_success				"success"

#define UART_MSG_FOOTMARK_end					"end\n"

//		end uart msg mark 

void UART_init(void);

void UART_clearReceivedBuffer(void);

void UART_sendMsg(char msg[UART_BUFFER_SIZE]);
bool UART_receiveMsg(char buffer[UART_BUFFER_SIZE]);

void UART_makeHandShakeMsg(char msg[UART_BUFFER_SIZE]);
void UART_makeCheckConnectionMsg(char msg[UART_BUFFER_SIZE]);
void UART_makeDisconnectionMsg(char msg[UART_BUFFER_SIZE]);
void UART_makeRequestPermssionMsg(char msg[UART_BUFFER_SIZE]);

bool UART_isMsgMeetEndMark(char msg[UART_BUFFER_SIZE]);
bool UART_ishandShakeMsg(char msg[UART_BUFFER_SIZE]);
bool UART_isDisconnectMsg(char msg[UART_BUFFER_SIZE]);
bool UART_isTimeSyncMsg(char msg[UART_BUFFER_SIZE]);
bool Uart_isCheckConnectionMsg(char msg[UART_BUFFER_SIZE]);
bool UART_isReseponsePermissionMsg(char msg[UART_BUFFER_SIZE]);
bool UART_isResponsePemissionOK(char msg[UART_BUFFER_SIZE]);

void UART_syncTime(char raw_msg[UART_BUFFER_SIZE]);

void UART_mainTask(void);
















