/************************************************************
 * @file    rtosConfig.h
 * @brief   Filedescription
 * @author  dg
 * @date    19 Mar 2026
 ************************************************************/

#ifndef RTOSCONFIG_H_
#define RTOSCONFIG_H_

#define RTOS_CONFIG_USE_BLINKY_TASK      (1)
#define RTOS_CONFIG_USE_HELLO_WORLD_TASK (1)
#define RTOS_CONFIG_USE_BUTTONS_TASK     (1)

#define RTOS_CONFIG_BLINKY_DELAY_MS      (500)
#define RTOS_CONFIG_HELLO_WORLD_DELAY_MS (1000)

#define RTOS_CONFIG_BUTTONS_QUEUE_LENGTH (10)

#endif // RTOSCONFIG_H_