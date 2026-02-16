/*
 * Copyright (c) 2023-2026, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Getter for the sensor values, from the same measurment
 * \param pTemperature Pointer to where to store temperature value in degree
 * \param pHumidity Pointer where to store the humidity value as % RH
 */
void Sensor_GetValues(float *pTemperature, float *pHumidity);

/*!
 * \brief Sensor module initialization
 */
void Sensor_Deinit(void);

/*!
 * \brief Sensor module initialization
 */
void Sensor_Init(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* SENSOR_H_ */
