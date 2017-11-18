#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "food.h"
#include "xprintf.h"

static food_t food;

static uint16_t width;
static uint16_t height;
static uint16_t cols, rows;

static void show(snake_t *snake)
{
  uint16_t x, y, i;
  bool spotFound;

  do
    {
      spotFound = true;

      x = (rand() % cols) * food.size;
      y = (rand() % rows) * food.size;

      xprintf("x: %d\n", x);
      xprintf("y: %d\n", y);

      // sprawdz czy wylosowane wspolrzedne jedzenia nie pokrywaja sie z cialem snejka
      // jesli tak, to ponow losowanie
      for (i = 0; i <= snake->length; i++)
        {
          if (snake->tail[i].x0 == x && snake->tail[i].y0 == y)
            {
              xprintf("Overlap\n");
              spotFound = false;
            }
        }
    }
  while (!spotFound);

  food.pos.x0 = x;
  food.pos.y0 = y;
  food.pos.x1 = x + food.size;
  food.pos.y1 = y + food.size;

  GUI_SetColor(GUI_GREEN);
  GUI_FillRectEx(&food.pos);
}

food_t *foodInit(uint8_t size)
{
  food.show = show;
  food.size = size;

  width = LCD_GetXSize();
  height = LCD_GetYSize();

  cols = width / food.size;
  rows = height / food.size;

  return &food;
}
