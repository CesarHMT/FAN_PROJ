/*
 * ctrlManager.h
 *
 *  Created on: Jun 28, 2022
 *      Author: chmt
 */

#ifndef INC_CTRLMANAGER_H_
#define INC_CTRLMANAGER_H_

#include "ctrlManager_pkg.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h> //for va_list var arg functions
#include "main.h"

extern QueueHandle_t DisplayQueue;
/*----------------------------- Mocking function includes ----------------------------------------*/
//------------------------------------------------------------------------------------------------//

#define MSG_LENGHT 15
#define TIMEOUT 100
#define DEBOUNCE_TIME  120        // Expressed in milliseconds
#define DEBOUNCE_LONG_TIME  1000  // Expressed in milliseconds

//---------------------Original Variables -------------------------------

extern volatile int counter;
extern TIM_HandleTypeDef htim10;
extern SemaphoreHandle_t ButtonsSemaphore;
extern eventFlag evFlag;
uint8_t ctrl_temp;
uint8_t ctrl_timer;
eUnits_t ctrl_units;
eActive_t timer_state;

//---------------------ceedling test variables-----------------------------
/*int counter;
eventFlag evFlag;
ctrlManager_t ctrl;
u_int8_t ctrl_temp;
u_int8_t ctrl_timer;
eEvent_t eventx; */
//---------------------------------------------------------------------------

typedef struct _ctrlManager_t{
	eTstate_t stateID;
	eTstate_t nextStateID;
	eMode_t modeID;
	eEvent_t eventID;
	uint8_t data;
} ctrlManager_t;

ctrlManager_t * vCtrlManager_create(void);
void vCtrlManager_init(ctrlManager_t * const me);
void vCtrlManager_destroy(ctrlManager_t * const me);
void vCtrlManager_EventDispatch(ctrlManager_t * const me, eEvent_t e);

void vCtrlManager_enter_Wait4Button(ctrlManager_t * const me);
void vCtrlManager_enter_Debouncer(ctrlManager_t * const me, eEvent_t e);
void vCtrlManager_enter_Decode(ctrlManager_t * const me);
void vCtrlManager_enter_TimerInc1(ctrlManager_t * const me);
void vCtrlManager_enter_TimerInc10(ctrlManager_t * const me);
void vCtrlManager_enter_TempInc1(ctrlManager_t * const me);
void vCtrlManager_enter_TempInc10(ctrlManager_t * const me);
void vCtrlManager_enter_ToggleUnits(ctrlManager_t * const me);
void vCtrlManager_enter_AlarmOff(ctrlManager_t * const me);
void vCtrlManager_enter_RepeatDelay(ctrlManager_t * const me);
void vCtrlManager_enter_Default(ctrlManager_t * const me);
void vCtrlManager_enter_Error(ctrlManager_t * const me);

#endif /* INC_CTRLMANAGER_H_ */
