/************************************************************
 * @file    rtos.h
 * @brief   Filedescription
 * @author  dg
 * @date    19 Mar 2026
 ************************************************************/

#ifndef RTOS_H_
#define RTOS_H_

/********************
 *     Includes    *
 ********************/

#include "buttons_config.h"

/***********************************
 *     Public Macros / Defines     *
 ***********************************/

/***************************
 *     Public Typedefs     *
 ***************************/

typedef struct _RTOS_ButtonsQueueItem
{
  Buttons_e button;
} RTOS_ButtonsQueueItem_t;

/****************************
 *     Public Variables     *
 ****************************/

/**************************************
 *     Public Function Prototypes    *
 **************************************/

void RTOS_Init(void);

void RTOS_On_Buttons_ISR(uint32_t buttonBits);

#endif // RTOS_H_