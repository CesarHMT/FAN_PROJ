/*
 * queue.c
 *
 *  Created on: Jun 28, 2022
 *      Author: chmt
 */
/**
  ******************************************************************************
  * @file           : queue.c
  * @brief          : Main program body
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

#include <Evqueue.h>

/**
  * @brief  Buttons rise edge callback
  * @note   This function manage all inerrupts comming from control inputs
  * HAL_GPIO_EXTI_IRQHandler. It handles the iterrup reques, clearing and
  * passing the control to the callback function for user process.
  * @param  GPIO_Pin : ID from GPIO_pin
  * @retval None
  */
static unsigned char prvisFull(queue_t * const me){
	return (me->size == 0);
}

/**
  * @brief  Buttons rise edge callback
  * @note   This function manage all inerrupts comming from control inputs
  * HAL_GPIO_EXTI_IRQHandler. It handles the iterrup reques, clearing and
  * passing the control to the callback function for user process.
  * @param  GPIO_Pin : ID from GPIO_pin
  * @retval None
  */
static unsigned char vQueueisEmpty(queue_t * const me){
	return (me->size == QUEUE_SIZE);
}

/**
  * @brief  Buttons rise edge callback
  * @note   This function manage all inerrupts comming from control inputs
  * HAL_GPIO_EXTI_IRQHandler. It handles the iterrup reques, clearing and
  * passing the control to the callback function for user process.
  * @param  GPIO_Pin : ID from GPIO_pin
  * @retval None
  */
static int prvNextIndex(queue_t * const me, int index){
	return (index + 1) % QUEUE_SIZE;
}

/**
  * @brief  Buttons rise edge callback
  * @note   This function manage all inerrupts comming from control inputs
  * HAL_GPIO_EXTI_IRQHandler. It handles the iterrup reques, clearing and
  * passing the control to the callback function for user process.
  * @param  GPIO_Pin : ID from GPIO_pin
  * @retval None
  */
queue_t * pQueueCreate(void){
	queue_t * me = (queue_t *) malloc(sizeof(queue_t));
	if(me != NULL){
		vQueueInit(me);
	}
	else{
		//HAL_UART_Transmit(huart, "Can't be allocated the object memory \n\r", MSG_SIZE, TIMEOUT_UART);
	}
	return me;
}

/**
  * @brief  Buttons rise edge callback
  * @note   This function manage all inerrupts comming from control inputs
  * HAL_GPIO_EXTI_IRQHandler. It handles the iterrup reques, clearing and
  * passing the control to the callback function for user process.
  * @param  GPIO_Pin : ID from GPIO_pin
  * @retval None
  */
void vQueueInit(queue_t * const me){
	me->head = 0;
	me->size = 0;
	me->tail = 0;
}

/**
  * @brief  Buttons rise edge callback
  * @note   This function manage all inerrupts comming from control inputs
  * HAL_GPIO_EXTI_IRQHandler. It handles the iterrup reques, clearing and
  * passing the control to the callback function for user process.
  * @param  GPIO_Pin : ID from GPIO_pin
  * @retval None
  */
void vQueueDestroy(queue_t * const me){

	free(me);
}

/**
  * @brief  Buttons rise edge callback
  * @note   This function manage all inerrupts comming from control inputs
  * HAL_GPIO_EXTI_IRQHandler. It handles the iterrup reques, clearing and
  * passing the control to the callback function for user process.
  * @param  GPIO_Pin : ID from GPIO_pin
  * @retval None
  */
int iQueuePost(queue_t * const me, eEvent_t event){
	//IMPLEMENTAR SEMAFOROS PARA PROCESOS ATOMICOS
	if(!prvisFull(me)){
		me->itsqueue[me->head] = event;
		me->head = prvNextIndex(me,me->head);
		me->size += 1;
		//IMPLEMENTAR SEMAFOROS PARA PROCESOS ATOMICOS
		return 1;
	}
	else{
		//IMPLEMENTAR SEMAFOROS PARA PROCESOS ATOMICOS
		return 0;
	}
}

/**
  * @brief  Buttons rise edge callback
  * @note   This function manage all inerrupts comming from control inputs
  * HAL_GPIO_EXTI_IRQHandler. It handles the iterrup reques, clearing and
  * passing the control to the callback function for user process.
  * @param  GPIO_Pin : ID from GPIO_pin
  * @retval None
  */
eEvent_t * vQueuePull(queue_t * const me){
	eEvent_t * pEvent;
	//IMPLEMENTAR SEMAFOROS PARA PROCESOS ATOMICOS
	if(!vQueueisEmpty(me)){
		pEvent = (eEvent_t *) malloc(sizeof(eEvent_t));
		*pEvent = me->itsqueue[me->tail];
		me->tail = prvNextIndex(me,me->tail);
		me->size -= 1;
		return &pEvent;
	}
}
