#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "main.h"
#include <stdbool.h>
#include "food.h"

typedef struct food_s food_t;
typedef enum
{
  DIR_NONE,
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT,
} direction_t;

typedef struct snake_s
{
  uint8_t length; // długość snejka
  uint8_t size; // rozmiar w pikselach
  void (*eat)(food_t *food);
  bool (*move)(void);
  void (*show)(void);
  GUI_RECT tail[768]; // max długosc snejka dla rozmiaru rownego 10 pix
  direction_t dir; // aktualny kierunek poruszania się snejka
} snake_t;

snake_t *snakeInit(uint8_t x, uint8_t y, uint8_t size);

#endif