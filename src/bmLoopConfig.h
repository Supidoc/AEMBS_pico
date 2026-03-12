/*
 * Copyright (c) 2025-2026, Dominik Grob
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BMLOOP_CONFIG_H__
#define __BMLOOP_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define BML_LED_DELAY_MS                                                       \
  (1000) /*< Delay for the led to toggle and for the message to be sent*/
#define BML_MSG_DELAY_MS (4000) /*< Delay for the message to be sent*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BMLOOP_CONFIG_H__ */