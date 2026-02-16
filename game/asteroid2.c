/*
 * Copyright (c) 2022, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "asteroid2.h"

static const uint8_t pixData[] = {
    0x30,
    0x78,
    0xdc,
    0xfe,
    0xf4,
    0x78,
    0x30,
};

static const TIMAGE image = {
  .width = 7, /* .header.w */
  .height = 7, /* .header.h */
  .size = sizeof(pixData),
  .pixmap = pixData,
  .name = "asteroid2.bmp",
};

const PIMAGE Asteroid2_GetImage(void) {
  return (PIMAGE)&image;
}
