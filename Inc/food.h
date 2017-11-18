#ifndef _FOOD_H_
#define _FOOD_H_

#include "snake.h"

typedef struct snake_s snake_t;

typedef struct food_s
{
  uint8_t size;
  GUI_RECT pos;
  void (*show)(snake_t *snake);
} food_t;

food_t *foodInit(uint8_t size);

#endif