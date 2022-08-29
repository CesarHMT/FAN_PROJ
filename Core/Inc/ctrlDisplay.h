/*
 * ctrlDisplay.h
 *
 *  Created on: Jul 12, 2022
 *      Author: chmt
 */

#ifndef INC_CTRLDISPLAY_H_
#define INC_CTRLDISPLAY_H_

#include "ctrlManager_pkg.h"
#include "ctrlDisplay_pkg.h"
#include "stdlib.h"
#include "pcf8574.h"

extern QueueHandle_t DisplayQueue;

typedef struct _ctrlDisplay_t{
	message_t msg;
	eUnits_t unit;
	eDispState_t stateID;
	eDispMode_t modeID;
	TimerClock_t myclock;
} ctrlDisplay_t;

ctrlDisplay_t * pCtrlDisplay_create(void);
void vCtrlDisplay_EventDispatch(ctrlDisplay_t * me);

void vCtrlDisplay_init(ctrlDisplay_t * me);
void vCtrlDisplay_destroy(ctrlDisplay_t * me);
void vCtrlDisplay_Units(ctrlDisplay_t * me);
void vCtrlDisplay_ErrDisp(ctrlDisplay_t * me);
void vCtrlDisplay_TimeDisp(ctrlDisplay_t * me);
void vCtrlDisplay_TempDisp(ctrlDisplay_t * me);
void vCtrlDisplay_Error(ctrlDisplay_t * me);
void vCtrlDisplay_Default(ctrlDisplay_t * me);
void vCtrlDisplay_Display(ctrlDisplay_t * me);

#endif /* INC_CTRLDISPLAY_H_ */
