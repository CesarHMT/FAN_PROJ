/*
 * ctrlDisplayPkg.h
 *
 *  Created on: Jul 12, 2022
 *      Author: chmt
 */

#ifndef INC_CTRLDISPLAY_PKG_H_
#define INC_CTRLDISPLAY_PKG_H_

#include "stdint.h"

#define MINUTETOHOUR 60

enum _eDisp_state{
	STEMPDISP,
	STIMEDISP,
	SUNITSDISP,
	SERRORDISP,
	SDEFAULTDISP
};
typedef enum _eDisp_state eDispState_t;

enum _eDisp_Mode{
	DISPIDLE,
	DISPUNITS,
	DISPTEMP,
	DISPTIME,
	DISPERROR
};
typedef enum _eDisp_Mode eDispMode_t;

typedef struct _TimerClock_t{
	uint8_t sec;
	uint8_t min;
	uint8_t hrs;
} TimerClock_t;

#endif /* INC_CTRLDISPLAY_PKG_H_ */
