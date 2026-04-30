/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game_config.h"
#include "gameController.h"
#include "gameView.h"
#include "gameModel.h"
#include "sprite.h"
#include "McuUtility.h"
#include "McuGDisplaySSD1306.h"
#include "McuFontDisplay.h"
#include "McuFontHelv10Normal.h"
#include "McuFontHelv08Normal.h"
#include "McuFontHelv12Normal.h"
#include "McuFontHelv18Bold.h"
#include "gameModel.h"
#include "stdio.h"

static void Show1Liner(const unsigned char *text0);
static void Show2Liner(const unsigned char *text0, const unsigned char *text1);
static void PaintGamePlayScreen(void);
static void PaintGameScoreScreen(void);
static void delay_ms(uint32_t ms);

static void delay_ms(uint32_t ms) { vTaskDelay(pdMS_TO_TICKS(ms)); }

/*!
 * \brief Shows one line of text centered on the display, with a border around it
 * \param text0 Line one of text
 */
static void Show1Liner(const unsigned char *text0)
{
  /*! \todo Make sure things are reentrant! */
  PGFONT_Callbacks        font = McuFontHelv12Normal_GetFont();
  McuFontDisplay_PixelDim x, y;
  McuFontDisplay_PixelDim charHeight, totalHeight;

  McuGDisplaySSD1306_Clear();
  McuFontDisplay_GetFontHeight(font, &charHeight, &totalHeight);

  x = McuGDisplaySSD1306_GetWidth() / 2 - McuFontDisplay_GetStringWidth((unsigned char *)text0, font, NULL) / 2;
  y = McuGDisplaySSD1306_GetHeight() / 2 - charHeight / 2; /* 1 line */
  McuFontDisplay_WriteString((unsigned char *)text0, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth() - 1, McuGDisplaySSD1306_GetHeight() - 1, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth() - 1 - 4, McuGDisplaySSD1306_GetHeight() - 1 - 4, 1,
                             McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_UpdateFull();
}

/*!
 * \brief Shows two lines of text centered on the display, with a border around it
 * \param text0 Line one of text
 * \param text1 Line two of text
 */
static void Show2Liner(const unsigned char *text0, const unsigned char *text1)
{
  /*! \todo Make sure things are reentrant! */
  PGFONT_Callbacks        font = McuFontHelv12Normal_GetFont();
  McuFontDisplay_PixelDim x, y;
  McuFontDisplay_PixelDim charHeight, totalHeight;

  McuGDisplaySSD1306_Clear();
  McuFontDisplay_GetFontHeight(font, &charHeight, &totalHeight);

  x = McuGDisplaySSD1306_GetWidth() / 2 - McuFontDisplay_GetStringWidth((unsigned char *)text0, font, NULL) / 2;
  y = McuGDisplaySSD1306_GetHeight() / 2 - (2 * charHeight) / 2; /* 2 lines */
  McuFontDisplay_WriteString((unsigned char *)text0, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  x = McuGDisplaySSD1306_GetWidth() / 2 - McuFontDisplay_GetStringWidth((unsigned char *)text1, font, NULL) / 2;
  y += McuGDisplaySSD1306_GetHeight() / 2 - (2 * charHeight) / 2;
  McuFontDisplay_WriteString((unsigned char *)text1, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font);

  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth() - 1, McuGDisplaySSD1306_GetHeight() - 1, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth() - 1 - 4, McuGDisplaySSD1306_GetHeight() - 1 - 4, 1,
                             McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_UpdateFull();
}

static void PaintGamePlayScreen(void)
{
  /*! \todo Make sure things are reentrant! */
  McuGDisplaySSD1306_Clear();
  int        cntr = 0;
  Sprite_t **sprites;

  do
  {
    sprites = Game_IteratorGetSpriteLists(&cntr);
    if (sprites != NULL)
    {
      Sprite_PaintList(*sprites);
    }
  } while (cntr != 0);
  McuGDisplaySSD1306_UpdateFull();
}

static void PaintGameScoreScreen(void)
{
  int32_t score = Game_GetScore();

  char buf[32];
  snprintf(buf, sizeof(buf), "%d", score);
  Show2Liner((const unsigned char *)"Score:", (const unsigned char *)buf);
}

static void PaintGameHelpScreen(void)
{
  /*! \todo Make sure things are reentrant! */
  McuGDisplaySSD1306_Clear();

  PGFONT_Callbacks        font = McuFontHelv08Normal_GetFont();
  McuFontDisplay_PixelDim x, y;
  McuFontDisplay_PixelDim charHeight, totalHeight;

  char   *currentLineStart;
  char   *currentLineEnd;
  size_t  lineLen;
  bool    lastLine = false;
  int32_t displayLineCnt;
  int32_t linePixelWidth;
  int32_t lineIndex = 0;
  int32_t displayLineIndex;

  char *helpText =
      "Help:\n- Use joystick buttons to move\n- Press center button to fire \n- Avoid asteroids or go kaboom\n- Try to get the "
      "highest score!";

  McuFontDisplay_GetFontHeight(font, &charHeight, &totalHeight);

  int32_t maxLineWidth = McuGDisplaySSD1306_GetWidth() - 12;
  int32_t maxLines     = (McuGDisplaySSD1306_GetHeight() - 12) / charHeight;

  y                    = 6;

  currentLineStart     = helpText;

  while (lastLine != true)
  {
    displayLineIndex = 0;

    currentLineEnd   = strchr(currentLineStart, '\n');
    if (currentLineEnd == NULL)
    {
      currentLineEnd = strchr(currentLineStart, '\0');
      lastLine       = true;
    }
    lineLen = currentLineEnd - currentLineStart;

    char lineBuf[64];
    strncpy(lineBuf, currentLineStart, lineLen);
    x = 6;
    McuFontDisplay_WriteStringWidth(lineBuf, McuGDisplaySSD1306_COLOR_BLUE, &x, &y, font, maxLineWidth);
    y += totalHeight;
    if (y - 6 + totalHeight >= maxLines * charHeight - 6)
    {
      McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth() - 1, McuGDisplaySSD1306_GetHeight() - 1, 1,
                                 McuGDisplaySSD1306_COLOR_BLUE);
      McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth() - 1 - 4, McuGDisplaySSD1306_GetHeight() - 1 - 4, 1,
                                 McuGDisplaySSD1306_COLOR_BLUE);
      McuGDisplaySSD1306_UpdateFull();
      delay_ms(8000);
      McuGDisplaySSD1306_Clear();
      y = 6;
    }
    currentLineStart = currentLineEnd + 1;
  }
  McuGDisplaySSD1306_DrawBox(0, 0, McuGDisplaySSD1306_GetWidth() - 1, McuGDisplaySSD1306_GetHeight() - 1, 1, McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_DrawBox(2, 2, McuGDisplaySSD1306_GetWidth() - 1 - 4, McuGDisplaySSD1306_GetHeight() - 1 - 4, 1,
                             McuGDisplaySSD1306_COLOR_BLUE);
  McuGDisplaySSD1306_UpdateFull();
}

void Game_ShowScreen(Game_Screen_e screen)
{

  switch (screen)
  {
  case GAME_SCREEN_INTRO:
    Show1Liner((const unsigned char *)"AEMBS");
    break;
  case GAME_SCREEN_GAMEPLAY:
    PaintGamePlayScreen();
    break;
  case GAME_SCREEN_GAME_WON:
    Show2Liner((const unsigned char *)"You won!", (const unsigned char *)":-)");
    break;
  case GAME_SCREEN_GAME_LOST:
    Show2Liner((const unsigned char *)"You lost!", (const unsigned char *)":-(");
    break;
  case GAME_SCREEN_SCORE:
    PaintGameScoreScreen();
    break;
  case GAME_SCREEN_HELP:
    PaintGameHelpScreen();
    break;
  default:
    break;
  }
}

void Game_ViewInit(void) { /* nothing needed */ }

void Game_ViewDeinit(void) { /* nothing needed */ }
