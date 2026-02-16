/*
 * Copyright (c) 2022, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "shipa.h" /* include of own header file */

/* the bitmap data, copy from generated file */
static const uint8_t pixData[] = {
  /* bitmap data */
    0xf8, 0x00,
    0xe0, 0x00,
    0xf0, 0x00,
    0xfc, 0x00,
    0x7f, 0x80,
    0x7f, 0x80,
    0xfc, 0x00,
    0xf0, 0x00,
    0xe0, 0x00,
    0xf8, 0x00,
};

static const TIMAGE image = {
  .width = 9, /* number from converted file: .header.w */
  .height = 10, /* number from converted file: .header.h */
  .size = sizeof(pixData), /* size of bitmap data */
  .pixmap = pixData, /* pointer to bitmap data above */
  .name = "shipa.bmp", /* optional name of file */
};

const PIMAGE ShipA_GetImage(void) {
  return (PIMAGE)&image;
}

