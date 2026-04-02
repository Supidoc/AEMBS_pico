/************************************************************
 * @file    rtos.c
 * @brief   Filedescription
 * @author  dg
 * @date    19 Mar 2026
 ************************************************************/

/********************
 *     Includes    *
 ********************/
#include <stdint.h>
#include "McuRTOS.h"
#include "rtos.h"
#include "rtosConfig.h"
#include "leds.h"
#include "McuShellCdcDevice.h"
#include "buttons.h"
#include "stdio.h"

/************************************
 *     Private Macros / Defines    *
 ************************************/

/***************************
 *     Private Typedefs     *
 ***************************/

/*****************************************
 *     Private Function Declarations     *
 *****************************************/

static void blinky_task(void *pvParameters);
static void init_blinky_task(void);
static void hello_world_task(void *pvParameters);
static void init_hello_world_task(void);
static void buttons_task(void *pvParameters);
static void init_buttons_task(void);

/****************************
 *     Public Variables     *
 ****************************/

/*****************************
 *     Private Variables     *
 *****************************/

static QueueHandle_t buttonsQueue                          = NULL;

static const char     *buttonNames[BUTTONS_NOF_BUTTONS]    = {"NAV_UP", "NAV_DOWN", "NAV_LEFT", "NAV_RIGHT", "NAV_CENTER"};
static const Buttons_e buttonBitsEnum[BUTTONS_NOF_BUTTONS] = {BUTTONS_BIT_NAV_UP, BUTTONS_BIT_NAV_DOWN, BUTTONS_BIT_NAV_LEFT,
                                                              BUTTONS_BIT_NAV_RIGHT, BUTTONS_BIT_NAV_CENTER};

/*******************************************
 *     Public Function Implementations     *
 *******************************************/

void RTOS_Init(void)
{
#if RTOS_CONFIG_USE_BLINKY_TASK
  init_blinky_task();
#endif

#if RTOS_CONFIG_USE_HELLO_WORLD_TASK
  init_hello_world_task();
#endif

#if RTOS_CONFIG_USE_BUTTONS_TASK
  init_buttons_task();
#endif
}

void RTOS_on_buttons_isr(uint32_t buttonBits)
{

  for (uint8_t i = 0; i < BUTTONS_NOF_BUTTONS; i++)
  {
    if (buttonBits & buttonBitsEnum[i])
    {
      RTOS_ButtonsQueueItem_t queueItem;
      queueItem.button = buttonBitsEnum[i];
      xQueueSendFromISR(buttonsQueue, &queueItem, NULL);
    }
  }
}
/********************************************
 *     Private Function Implementations     *
 ********************************************/

static void blinky_task(void *pvParameters)
{

  TickType_t xPreviousWakeTime = xTaskGetTickCount();
  for (;;)
  {
    Leds_Neg(LEDS_GREEN);

    vTaskDelayUntil(&xPreviousWakeTime, pdMS_TO_TICKS(RTOS_CONFIG_BLINKY_DELAY_MS));
  }
}

static void init_blinky_task(void)
{
  if (xTaskCreate(blinky_task, "BlinkyTask", 600 / sizeof(StackType_t), NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
  {
    for (;;)
      ;
  }
}

static void hello_world_task(void *pvParameters)
{

  TickType_t xPreviousWakeTime = xTaskGetTickCount();
  for (;;)
  {
    McuShellCdcDevice_WriteStr("Hello World!\r\n");
    McuShellCdcDevice_Flush();

    vTaskDelayUntil(&xPreviousWakeTime, pdMS_TO_TICKS(RTOS_CONFIG_HELLO_WORLD_DELAY_MS));
  }
}

static void init_hello_world_task(void)
{
  if (xTaskCreate(hello_world_task, "HelloWorld", 600 / sizeof(StackType_t), NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
  {
    for (;;)
      ;
  }
}

static void buttons_task(void *pvParameters)
{
  for (;;)
  {
    RTOS_ButtonsQueueItem_t queueItem;
    xQueueReceive(buttonsQueue, &queueItem, portMAX_DELAY);

    for (size_t i = 0; i < BUTTONS_NOF_BUTTONS; i++)
    {
      if (queueItem.button & buttonBitsEnum[i])
      {
        char msg[64];
        snprintf(msg, sizeof(msg), "Button %s pressed\r\n", buttonNames[i]);
        McuShellCdcDevice_WriteStr(msg);
      }
    }
    McuShellCdcDevice_Flush();
  }
}

static void init_buttons_task(void)
{
  buttonsQueue = xQueueCreate(RTOS_CONFIG_BUTTONS_QUEUE_LENGTH, sizeof(RTOS_ButtonsQueueItem_t));

  if (buttonsQueue == NULL)
  {
    for (;;)
      ;
  }

  if (xTaskCreate(buttons_task, "Buttons", 600 / sizeof(StackType_t), NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
  {
    for (;;)
      ;
  }
}
