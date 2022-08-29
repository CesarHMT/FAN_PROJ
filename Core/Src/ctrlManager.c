/*
 * ctrlManager.c
 *
 *  Created on: Jun 28, 2022
 *      Author: chmt
 */
#include "ctrlManager.h"

/*--------------------------- private functions ------------------------------------------*/
void vCtrlManager_sendMessage(ctrlManager_t * const me);
/*----------------------------------------------------------------------------------------*/

ctrlManager_t * vCtrlManager_create(void){
	ctrlManager_t * me = (ctrlManager_t *) malloc(sizeof(ctrlManager_t));
	if(me != NULL){
		vCtrlManager_init(me);
	}
	else{
		//HAL_UART_Transmit(huart, "Object can't be created \n\r", MSG_LENGHT, TIMEOUT);
	}
	return me;
}

void vCtrlManager_init(ctrlManager_t * const me){
	me->eventID = NO_INPUT;
	me->modeID = NORMAL;
	me->stateID = SWAIT4BUTTON;
	me->nextStateID = SWAIT4BUTTON;
	ctrl_temp = 100;   ///////////////////////////////////
	ctrl_timer = 250;  ///////////////////////////////////
}

void vCtrlManager_destroy(ctrlManager_t * const me){
	free(me);
}

void vCtrlManager_EventDispatch(ctrlManager_t * const me, eEvent_t e){

	switch(me->nextStateID){
	case SWAIT4BUTTON:
		vCtrlManager_enter_Wait4Button(me);
		break;
	case SDEBOUNCER:
		vCtrlManager_enter_Debouncer(me, e);
		break;
	case SDECODE:
		vCtrlManager_enter_Decode(me);
		break;
	case SALARMOFF:
		vCtrlManager_enter_AlarmOff(me);
		break;
	case STOGGLEUNITS:
		vCtrlManager_enter_ToggleUnits(me);
		vCtrlManager_sendMessage(me);
		break;
	case STEMPINC1:
		me->data = 100; /// SOLO PARA PROBAR EL CIRCUITO
		vCtrlManager_enter_TempInc1(me);
		vCtrlManager_sendMessage(me);
		if(evFlag == Start){
			vCtrlManager_enter_RepeatDelay(me);
		}
		else{
			vCtrlManager_enter_Wait4Button(me);
		}
		break;
	case STEMPINC10:
		vCtrlManager_enter_TempInc10(me);
		vCtrlManager_sendMessage(me);
		if(evFlag == Start){
			vCtrlManager_enter_RepeatDelay(me);
		}
		else{
			vCtrlManager_enter_Wait4Button(me);
		}
		break;
	case STIMEINC1:
		me->data = 120; /// solo para probar el circuito
		vCtrlManager_enter_TimerInc1(me);
		vCtrlManager_sendMessage(me);
		if(evFlag == Start){
			vCtrlManager_enter_RepeatDelay(me);
		}
		else{
			vCtrlManager_enter_Wait4Button(me);
		}
		break;
	case STIMEINC10:
		vCtrlManager_enter_TimerInc10(me);
		vCtrlManager_sendMessage(me);
		if(evFlag == Start){
			vCtrlManager_enter_RepeatDelay(me);
		}
		else{
			vCtrlManager_enter_Wait4Button(me);
		}
		break;
	case SREPEATDELAY:
		vCtrlManager_enter_RepeatDelay(me);
		if(1){
			vCtrlManager_enter_Decode(me);
		}
	default:
		break;
	}
}

void vCtrlManager_enter_Wait4Button(ctrlManager_t * const me){

	myprintf("Waiting Button press: \r\n");
	xSemaphoreTake(ButtonsSemaphore,portMAX_DELAY);

		if(evFlag == Start){
			myprintf("Button press: \r\n");
			__HAL_TIM_SET_COUNTER(&htim10,0);
			counter = __HAL_TIM_GET_COUNTER(&htim10);
			me->nextStateID = SDEBOUNCER;
		}

		if(evFlag == Finish){
			myprintf("Release Button: \r\n");
			counter = 0;
			me->nextStateID = SWAIT4BUTTON;
		}

		me->stateID = SWAIT4BUTTON;
}

void vCtrlManager_enter_Debouncer(ctrlManager_t * const me, eEvent_t e){

	me->nextStateID = SWAIT4BUTTON;

	if(evFlag==Start){

		HAL_Delay(DEBOUNCE_TIME);                       //evFlag==Finish &&
		if(evFlag==Finish){
			myprintf("Button Short Press: \r\n");
			//if(counter>=1000 && counter<10000){
			me->eventID = e;
			//}
		}

		if(evFlag==Start){
			HAL_Delay(DEBOUNCE_LONG_TIME);                  //evFlag==Finish &&
			myprintf("Button held down: \r\n");

			switch(e){
			case BUTTON_INCREASE:
				me->eventID = BUTTON_INCHOLD;
				break;
			case BUTTON_DECREASE:
				me->eventID = BUTTON_DECHOLD;
				break;
			case BUTTON_SET:
				me->eventID = BUTTON_SETHOLD;
				break;
				//Other cases doesn't make sense, doesnt apply
			}
		}
		me->stateID = SDEBOUNCER;
		me->nextStateID = SDECODE;
	}
		myprintf("Getting out of debouncer: \r\n");

}

void vCtrlManager_enter_Decode(ctrlManager_t * const me){

	myprintf("Decode function start \n\r");

	switch(me->modeID){
	case NORMAL:
		switch(me->stateID){
		case SDEBOUNCER:
			switch(me->eventID){
			case BUTTON_DECREASE:
				me->modeID = NORMAL;
				me->nextStateID = SALARMOFF;
				break;
			case BUTTON_DECHOLD:
				me->modeID = NORMAL;
				me->nextStateID = SALARMOFF;
				break;
			case BUTTON_INCREASE:
				me->modeID = NORMAL;
				me->nextStateID = STOGGLEUNITS;
				break;
			case BUTTON_INCHOLD:
				me->modeID = NORMAL;
				me->nextStateID = STOGGLEUNITS;
				break;
			case BUTTON_SET:
				me->modeID = NORMAL;
				me->nextStateID = SWAIT4BUTTON;
				//DISPLAY INFORMATION
				break;
			case BUTTON_SETHOLD:
				me->modeID = SETTEMP;
				me->nextStateID = SWAIT4BUTTON;
				me->data = ctrl_temp;
				vCtrlManager_sendMessage(me);
				//SEND INFORMATION BY MEANS OF A SCREEN "CONFIGURATION MODE"
				break;
			default:
				me->stateID = DEFAULTM;
				//Display error of configuration
				break;
			}
			break;

		default:
			me->stateID = DEFAULTM;
			break;
		}
		break;

	case SETTEMP:
		switch(me->stateID){
		case SDEBOUNCER:
			switch(me->eventID){
			case BUTTON_INCREASE:
				me->modeID = SETTEMP;
				me->nextStateID = STEMPINC1;
				break;
			case BUTTON_INCHOLD:
				me->modeID = SETTEMP;
				me->nextStateID = STEMPINC10;
				break;
			case BUTTON_SET:
				me->modeID = SETTIME;
				me->nextStateID = SWAIT4BUTTON;
				me->data = ctrl_timer;
				vCtrlManager_sendMessage(me);
				//DISPLAY INFO
				break;
			case BUTTON_SETHOLD:
				me->modeID = SETTIME;
				me->nextStateID = SWAIT4BUTTON;
				me->data = ctrl_timer;
				vCtrlManager_sendMessage(me);
				//DISPLAY INFO
				break;
			default:
				//DISPLAY ERROR OF CONFIGURATION
				break;
			}
			break;

		default:
			me->modeID = DEFAULTM;
			break;
		}
		break;

		case SETTIME:
			switch(me->stateID){
			case SDEBOUNCER:
				switch(me->eventID){
				case BUTTON_DECREASE:
					me->modeID = SETTIME;
					me->nextStateID = STIMEINC1;
					break;
				case BUTTON_DECHOLD:
					me->modeID = SETTIME;
					me->nextStateID = STIMEINC10;
					break;
				case BUTTON_SETHOLD:
					me->modeID = NORMAL;
					me->nextStateID = SWAIT4BUTTON;
					me->data = ctrl_temp;
					vCtrlManager_sendMessage(me);
					//SEND INFORMATION BY MEANS OF A SCREEN "CONFIGURATION MODE"
					break;
				case BUTTON_SET:
					me->modeID = NORMAL;
					me->nextStateID = SWAIT4BUTTON;
					me->data = 0;
					vCtrlManager_sendMessage(me);
					//DISPLAY INFORMATION
					break;

				default:
					//Display error of configuration
					break;
				}
				break;
			}
			break;

	case DEFAULTM:
		switch(me->stateID){
		case SDEBOUNCER:
			break;
		case SREPEATDELAY:
			break;
		default:
			me->modeID = DEFAULTM;
			break;
		}
		break;
	}

	me->stateID = SDECODE;
	myprintf("Decode function Finish \n\r");
}

void vCtrlManager_enter_TimerInc1(ctrlManager_t * const me){

	if((me->eventID == BUTTON_INCREASE) && (me->modeID == SETTIME)){
		ctrl_timer += 1;
		me->stateID = STIMEINC1;
	}
	if((me->eventID == BUTTON_DECREASE) && (me->modeID == SETTIME)){
		ctrl_timer -= 1;
		me->stateID = STIMEINC1;
	}
	else{
		//ERROR
	}
	me->eventID = STIMEINC1;
}

void vCtrlManager_enter_TimerInc10(ctrlManager_t * const me){

	if((me->eventID == BUTTON_INCHOLD) && (me->modeID == SETTIME)){
		ctrl_timer += 5;
	}
	if((me->eventID == BUTTON_DECHOLD) && (me->modeID == SETTIME)){
		ctrl_timer -= 5;
	}
	else{
		//ERROR
	}
	me->stateID = STIMEINC10;
}

void vCtrlManager_enter_TempInc1(ctrlManager_t * const me){

	if((me->eventID == BUTTON_INCREASE) && (me->modeID == SETTEMP)){
		ctrl_temp += 1;
	}
	if((me->eventID == BUTTON_DECREASE) && (me->modeID == SETTEMP)){
		ctrl_temp -= 1;
	}
	else{
		//ERROR
	}
	me->stateID = STEMPINC1;
}

void vCtrlManager_enter_TempInc10(ctrlManager_t * const me){

	if((me->eventID == BUTTON_INCHOLD) && (me->modeID == SETTEMP)){
		ctrl_temp += 5;
		me->stateID = STEMPINC10;
	}
	if((me->eventID == BUTTON_DECHOLD) && (me->modeID == SETTEMP)){
		ctrl_temp -= 5;
		me->stateID = STEMPINC10;
	}
	else{
		//ERROR
	}
	me->stateID = STEMPINC10;
}

void vCtrlManager_enter_ToggleUnits(ctrlManager_t * const me){

	myprintf("Entering ToggleUnits state\n\r");

	if((me->eventID==BUTTON_INCHOLD) && (me->modeID==NORMAL)){

		if(ctrl_units == CELSIUS){
			myprintf("Units in FAHRENHEIT\n\r");
			ctrl_units = FAHRENHEIT;
		}
		else{
			myprintf("Units in CELSIUS\n\r");
			ctrl_units = CELSIUS;
		}
	}

	if((me->eventID == BUTTON_INCREASE) && (me->modeID == NORMAL)){
		if(ctrl_units == CELSIUS){
			myprintf("Units in FAHRENHEIT\n\r");
			ctrl_units = FAHRENHEIT;
		}
		else{
			myprintf("Units in CELSIUS\n\r");
			ctrl_units = CELSIUS;
		}
	}
	me->data = ctrl_units;
	me->stateID = STOGGLEUNITS;
	me->nextStateID = SWAIT4BUTTON;
}

void vCtrlManager_enter_AlarmOff(ctrlManager_t * const me){

	if((me->eventID==BUTTON_DECHOLD) && (me->modeID==NORMAL)){

		if(timer_state == ON_MODE){
			timer_state = OFF_MODE;
		}
		else{
			//TIMER IS ALREADY OFF
		}
	}

	if((me->eventID==BUTTON_DECREASE) && (me->modeID==NORMAL)){

		if(timer_state == ON_MODE){
			timer_state = OFF_MODE;
		}
		else{
			//TIMER IS ALREADY OFF
		}
	}
	me->stateID = SALARMOFF;
}

void vCtrlManager_enter_RepeatDelay(ctrlManager_t * const me){
	HAL_Delay(100);
	me->stateID = SREPEATDELAY;
}

void vCtrlManager_enter_Default(ctrlManager_t * const me){

}

void vCtrlManager_enter_Error(ctrlManager_t * const me){

}

void vCtrlManager_sendMessage(ctrlManager_t * const me){
	message_t msg;
	msg.mode = me->modeID;
	msg.state = me->stateID;
	msg.event = me->eventID;
	msg.data = me->data;
	xQueueSendToFront(DisplayQueue,&msg,0);
}
