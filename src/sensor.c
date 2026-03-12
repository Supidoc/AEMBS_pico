/*
 * Copyright (c) 2023-2026, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sensor.h"
#include "McuLog.h"
#include "McuRTOS.h"
#include "McuSHT31.h"
#include "McuSystemView.h"
#include "McuUtility.h"
#include "platform.h"

static float temperature, humidity; /* note: values need to be from the exact same measurement! */

void Sensor_GetValues(float* pTemperature, float* pHumidity)
{
    *pTemperature = temperature;
    *pHumidity    = humidity;
}

void Sensor_Deinit(void)
{
}

void Sensor_Init(void)
{
}
