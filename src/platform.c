/*
 * Copyright (c) 2023-2026, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#include "McuArmTools.h"
#include "McuButton.h"
#include "McuDebounce.h"
#include "McuGPIO.h"
#include "McuGenericI2C.h"
#include "McuI2cLib.h"
#include "McuLED.h"
#include "McuLib.h"
#include "McuLog.h"
#include "McuRB.h"
#include "McuRTT.h"
#include "McuSHT31.h"
#include "McuShellCdcDevice.h"
#include "McuSystemView.h"
#include "McuUtility.h"
#include "McuWait.h"
#include "bmLoop.h"
#include "leds.h"
#include "buttons.h"
#include "debounce.h"
#include "shell.h"
#include "cdcLock.h"

void PL_Init(void)
{
  McuLib_Init();
  McuRTOS_Init();
  McuArmTools_Init();
  McuLog_Init();
  McuRTT_Init();
  McuSystemView_Init();
  McuWait_Init();
  McuGPIO_Init();
  McuLED_Init();
  McuBtn_Init();
  McuDbnc_Init();
  McuRB_Init();
  McuGenericI2C_Init();
  McuI2cLib_Init();
  McuSHT31_Init();
  McuShellCdcDevice_Init();
  /* setting up module configurations */
  McuShellCdcDevice_SetBufferRxCharCallback(McuShellCdcDevice_QueueChar);
  McuLog_set_console(McuRTT_GetStdio(), 0);
  McuLog_set_console(McuShellCdcDevice_GetStdio(), 1);
  Leds_Init();
  BML_init();
  Buttons_Init();
  Debounce_Init();
  Shell_Init();
  CdcLock_Init();
}

void PL_Deinit(void)
{
  McuShellCdcDevice_Deinit();
  McuSHT31_Deinit();
  McuI2cLib_Deinit();
  McuGenericI2C_Deinit();
  McuRB_Deinit();
  McuDbnc_Deinit();
  McuBtn_Deinit();
  McuLED_Deinit();
  McuGPIO_Deinit();
  McuWait_Deinit();
  McuSystemView_Deinit();
  McuRTT_Deinit();
  McuLog_Deinit();
  McuArmTools_Deinit();
  McuRTOS_Deinit();
  McuLib_Deinit();
  Leds_Deinit();
  BML_deinit();
  Buttons_Deinit();
  Debounce_Deinit();
  Shell_Deinit();
  CdcLock_Deinit();
}
