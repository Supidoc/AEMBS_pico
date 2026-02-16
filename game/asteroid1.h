/*
 * Copyright (c) 2022, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IMAGES_ASTEROID1_H_
#define IMAGES_ASTEROID1_H_

#include "McuGDisplaySSD1306.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Return the image
 * \return Pointer to the (const) image data
 */
const PIMAGE Asteroid1_GetImage(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* IMAGES_ASTEROID1_H_ */
