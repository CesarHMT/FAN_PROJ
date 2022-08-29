/*
 * queue.h
 *
 *  Created on: Jun 28, 2022
 *      Author: chmt
 */
/**
  ******************************************************************************
  * @file           : queue.h
  * @brief          : Header for queue.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#include "ctrlManager_pkg.h"
#include <stdlib.h>

#define MSG_SIZE 40
#define TIMEOUT_UART 100
#define QUEUE_SIZE 100

typedef struct _queue_t {
	eEvent_t itsqueue[QUEUE_SIZE];
	int head;
	int size;
	int tail;
} queue_t;

queue_t * pQueueCreate(void);
void vQueueInit(queue_t * const me);
void vQueueDestroy(queue_t * const me);
eEvent_t * vQueuePull(queue_t * const me);
int iQueuePost(queue_t * const me, eEvent_t event);

/* "Private Functions of the class"
	static unsigned char NextIndex(queue_t * const me, int index);
	static unsigned char prvisFull(queue_t * const me);
    static unsigned char prvisEmpty(queue_t * const me);
*/
#endif /* INC_QUEUE_H_ */
