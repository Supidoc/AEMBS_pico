/*
 * Copyright (c) 2022, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "asteroid0.h"

static const uint8_t pixData[] = {
    0x04, 0x00,
    0x0c, 0x00,
    0x3e, 0x00,
    0x7f, 0x00,
    0xff, 0x80,
    0x7f, 0xc0,
    0x3f, 0xc0,
    0x1f, 0x80,
    0x1e, 0x00,
    0x0c, 0x00,
};

static const TIMAGE image = {
  .width = 10, /* .header.w */
  .height = 10, /* .header.h */
  .size = sizeof(pixData),
  .pixmap = pixData,
  .name = "asteroid0.bmp",
};

const PIMAGE Asteroid0_GetImage(void) {
  return (PIMAGE)&image;
}
