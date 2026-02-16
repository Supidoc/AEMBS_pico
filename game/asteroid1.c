/*
 * Copyright (c) 2022, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "asteroid1.h"

static const uint8_t pixData[] = {
    0x0c,
    0x3c,
    0x7e,
    0x7e,
    0xff,
    0x7f,
    0x3f,
    0x0e,
};

static const TIMAGE image = {
  .width = 8, /* .header.w */
  .height = 8, /* .header.h */
  .size = sizeof(pixData),
  .pixmap = pixData,
  .name = "asteroid1.bmp",
};

const PIMAGE Asteroid1_GetImage(void) {
  return (PIMAGE)&image;
}
