/*
 * Copyright (c) 2023-2026, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#include "McuLib.h"
#include "McuWait.h"
#include "McuUtility.h"
#include "McuGPIO.h"
#include "McuLED.h"
#include "McuRTT.h"
#include "McuArmTools.h"
#include "McuSystemView.h"
#include "McuButton.h"
#include "McuDebounce.h"
#include "McuGenericI2C.h"
#include "McuI2cLib.h"
#include "McuSHT31.h"
#include "McuRB.h"
#include "McuLog.h"
#include "McuShellCdcDevice.h"

void PL_Init(void) {
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
}

void PL_Deinit(void) {
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
}
