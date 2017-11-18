#include "main.h"
#include "WM.h"
#include "guiLib.h"
#include <stdlib.h>
#include "xprintf.h"

void guiInit(void)
{
  WM_SetCreateFlags(WM_CF_MEMDEV);
  // WM_MULTIBUF_Enable(1);
  GUI_Init();

  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();
}

void endGameScreen(uint8_t score)
{
  int x = LCD_GetXSize() / 2;
  int y = LCD_GetYSize() / 2;

  uint16_t xOffset = 80, yOffset = 50;

  GUI_RECT endR =
  {
    .x0 = x - xOffset,
    .y0 = y - yOffset,
    .x1 = x + xOffset,
    .y1 = y + yOffset
  };

  GUI_SetColor(GUI_LIGHTGRAY);
  GUI_FillRectEx(&endR);


  GUI_SetColor(GUI_BLACK);
  GUI_SetFont(&GUI_Font20_ASCII);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_DispStringHCenterAt("GAME OVER", x, y - 35);
  char buf[20] = {0};
  xsprintf(buf, "SCORE: %d", score);
  GUI_DispStringHCenterAt(buf, x, y + 5);

}