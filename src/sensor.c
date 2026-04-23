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
#include "stdbool.h"

static void init_sensor_task(void);
static void sensor_task(void *pvParameters);

static SemaphoreHandle_t sensorMutex;

static float temperature, humidity; /* note: values need to be from the exact same measurement! */
static bool  sensorValuesWritten = false;

bool Sensor_GetValues(float *pTemperature, float *pHumidity)
{
  if (xSemaphoreTake(sensorMutex, portMAX_DELAY) == pdTRUE)
  {
    if (sensorValuesWritten)
    {
      *pTemperature = temperature;
      *pHumidity    = humidity;
      xSemaphoreGive(sensorMutex);
      return true;
    }
    xSemaphoreGive(sensorMutex);
    return false;
  }
  return false;
}

void Sensor_Deinit(void) {}

void Sensor_Init(void) { init_sensor_task(); }

static void init_sensor_task(void)
{
  if (xTaskCreate(sensor_task, "SensorTask", 600 / sizeof(StackType_t), NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
  {
    for (;;)
      ;
  }
}

static void sensor_task(void *pvParameters)
{
  TickType_t xPreviousWakeTime = xTaskGetTickCount();
  sensorMutex                  = xSemaphoreCreateMutex();
  if (sensorMutex == NULL)
  {
    for (;;)
      ;
  }
  for (;;)
  {
    if (xSemaphoreTake(sensorMutex, portMAX_DELAY) == pdTRUE)
    {
      McuSHT31_ReadTempHum(&temperature, &humidity);
      McuLog_info("t: %f C, h: %f %%", temperature, humidity);
      xSemaphoreGive(sensorMutex);
    }
    vTaskDelayUntil(&xPreviousWakeTime, pdMS_TO_TICKS(2000));
  }
}