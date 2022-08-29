/*
 * ctrlManager_pkg.h
 *
 *  Created on: Jun 28, 2022
 *      Author: chmt
 */

#ifndef INC_CTRLMANAGER_PKG_H_
#define INC_CTRLMANAGER_PKG_H_

#include "stdint.h"
#include "main.h"

enum _eEvent_t {
	BUTTON_INCREASE,
	BUTTON_DECREASE,
	BUTTON_SET,
	BUTTON_INCHOLD,
	BUTTON_DECHOLD,
	BUTTON_SETHOLD,
	BUTTON_RELEASED,
	NO_INPUT
};

typedef enum _eEvent_t eEvent_t;

enum _eTstate_t {
		SWAIT4BUTTON,
		SDEBOUNCER,
		SDECODE,
		STIMEINC1,
		STIMEINC10,
		STEMPINC1,
		STEMPINC10,
		STOGGLEUNITS,
		SALARMOFF,
		SREPEATDELAY,
		SERROR,
		SDEFAULT
};

typedef enum _eTstate_t eTstate_t;

enum _eMode_t {
	NORMAL,
	SETTEMP,
	SETTIME,
	DEFAULTM,
	ERRORM
};
typedef enum _eMode_t eMode_t;

enum _eUnits_t {
	CELSIUS,
	FAHRENHEIT
};
typedef enum _eUnits_t eUnits_t;

enum _eActive_t {
	ON_MODE,
	OFF_MODE
};
typedef enum _eActive_t eActive_t;

typedef enum _eventFlag{
	Start,
	Finish
} eventFlag;

typedef struct _message_t{
	eMode_t mode;
	eTstate_t state;
	eEvent_t event;
	uint8_t data;
	portBASE_TYPE xStatus;
}message_t;

#endif /* INC_CTRLMANAGER_PKG_H_ */
