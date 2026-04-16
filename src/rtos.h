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
#include "McuDebounce.h"

/***********************************
 *     Public Macros / Defines     *
 ***********************************/

/***************************
 *     Public Typedefs     *
 ***************************/

typedef struct _RTOS_ButtonsQueueItem
{
  Buttons_e button;
#if PL_CONFIG_USE_DEBOUNCE
  McuDbnc_EventKinds eventKind;
#endif
} RTOS_ButtonsQueueItem_t;

/****************************
 *     Public Variables     *
 ****************************/

/**************************************
 *     Public Function Prototypes    *
 **************************************/

void RTOS_Init(void);

void RTOS_on_buttons_isr(uint32_t buttonBits);
void RTOS_on_debounce_button_event(Buttons_e button, McuDbnc_EventKinds kind);

#endif // RTOS_H_