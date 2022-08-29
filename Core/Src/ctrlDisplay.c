/*
 * ctrlDisplay.c
 *
 *  Created on: Jul 12, 2022
 *      Author: chmt
 */
#include "ctrlDisplay.h"

//----------------------Private Variables------------------------------
void vCtrlDisplay_tempdisp(ctrlDisplay_t * me);
void vCtrlDisplay_unitsdisp(ctrlDisplay_t * me);
void vCtrlDisplay_receiveMessage(ctrlDisplay_t * me);
void vCtrlDisplay_timerdisp(TimerClock_t * me);
//--------------------------------------------------------------------

ctrlDisplay_t * pCtrlDisplay_create(void){
	ctrlDisplay_t * me = (ctrlDisplay_t *) malloc(sizeof(ctrlDisplay_t));
	if(me != NULL){
		vCtrlDisplay_init(me);
	}
	else{
		//myprintf("Failed to create the object \n\r");
	}
	return me;
}

void vCtrlDisplay_init(ctrlDisplay_t * me){
	me->stateID = DISPIDLE;
	me->unit = CELSIUS;
	pcf8574_init();
}

void vCtrlDisplay_destroy(ctrlDisplay_t * me){
	if(me != NULL){
		free(me);
	}
}

void vCtrlDisplay_EventDispatch(ctrlDisplay_t * me){
	vCtrlDisplay_receiveMessage(me);
	if(me->msg.xStatus == pdPASS){
		switch(me->msg.mode){
		case NORMAL:
			switch(me->msg.event){
			case BUTTON_INCREASE:
				me->modeID = DISPUNITS;
				vCtrlDisplay_Display(me);
				break;
			case BUTTON_INCHOLD:
				me->modeID = DISPUNITS;
				vCtrlDisplay_Display(me);
				break;
			default:
				break;
			}
		case SETTEMP:
			me->modeID = DISPTEMP;
			vCtrlDisplay_Display(me);
			break;
		case SETTIME:
			me->modeID = DISPTIME;
			vCtrlDisplay_Display(me);
			break;
		case DEFAULTM:
			break;
		default:
			break;
		}
	}

	//HAL_Delay(100);

	if(me->msg.xStatus != pdPASS){
		me->modeID = DISPIDLE;
	}
}

void vCtrlDisplay_Display(ctrlDisplay_t * me){
	switch(me->modeID){
	case DISPIDLE:
		vCtrlDisplay_unitsdisp(me);
		//HAL_Delay(30);
		break;
	case DISPUNITS:
		vCtrlDisplay_tempdisp(me);
		vCtrlDisplay_unitsdisp(me);
		break;
	case DISPTEMP:
		vCtrlDisplay_tempdisp(me);
		break;
	case DISPTIME:
		vCtrlDisplay_TimeDisp(me);
		//vCtrlDisplay_timerdisp(&me->myclock);
		HAL_Delay(1);
		break;
	case DISPERROR:
		break;
	default:
		break;
	}
}

void vCtrlDisplay_Units(ctrlDisplay_t * me){
	vCtrlDisplay_unitsdisp(me);
	//HAL_Delay(30);
}

void vCtrlDisplay_ErrDisp(ctrlDisplay_t * me){

}

void vCtrlDisplay_TimeDisp(ctrlDisplay_t * me){
	me->myclock.min = (me->msg.data % MINUTETOHOUR);
	me->myclock.hrs = (me->msg.data / MINUTETOHOUR);
	me->myclock.sec = 0;
	vCtrlDisplay_timerdisp(&me->myclock);
}

void vCtrlDisplay_TempDisp(ctrlDisplay_t * me){
	vCtrlDisplay_tempdisp(me);
}

//////////////-------------------Interfaces-------------////////////////////

void vCtrlDisplay_tempdisp(ctrlDisplay_t * me){
	pcf8574_cursor(0, 0);
	pcf8574_send_integer(me->msg.data);
}

void vCtrlDisplay_unitsdisp(ctrlDisplay_t * me){
    me->unit = me->msg.data;
	pcf8574_cursor(0, 8);
	if(me->unit == FAHRENHEIT){
		pcf8574_send_string("F");
	}
	if(me->unit == CELSIUS){
		pcf8574_send_string("C");
	}
}

void vCtrlDisplay_timerdisp(TimerClock_t * me){
	pcf8574_cursor(1,0);
	pcf8574_send_integer(me->hrs);
	pcf8574_cursor(1,3);
	pcf8574_send_string(":");
	pcf8574_cursor(1,4);
	pcf8574_send_integer(me->min);
	pcf8574_cursor(1,7);
	pcf8574_send_string(":");
	pcf8574_cursor(1,8);
	pcf8574_send_integer(me->sec);
}

void vCtrlDisplay_receiveMessage(ctrlDisplay_t * me){
	if(uxQueueMessagesWaiting(DisplayQueue)!=0){
		me->msg.xStatus = xQueueReceive(DisplayQueue,&(me->msg),0);
	}
}
