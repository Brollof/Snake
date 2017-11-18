#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "snake.h"
#include "lsm303.h"
#include "xprintf.h"

/* Utworz snejka */
static snake_t snake;

static GUI_RECT tempRect;

/*
 * Sprawdza kolizje ze scianami oraz z cialem snejka, zwraca:
 * false jesli nie wystapila kolizja
 * true jesli kolizja wystapila
 */
static bool checkCollision(void)
{
  // krawedzie ekranu
  if (snake.tail[0].x1 > LCD_GetXSize() || snake.tail[0].x0 < 0)
    return true;

  if (snake.tail[0].y1 > LCD_GetYSize() || snake.tail[0].y0 < 0)
    return true;

  // cialo snejka
  uint16_t i;
  for (i = 1; i <= snake.length; i++)
    {
      if (snake.tail[0].x0 == snake.tail[i].x0 && snake.tail[0].y0 == snake.tail[i].y0)
        return true;
    }

  return false;
}

static direction_t filterDirection(direction_t newDir)
{
  static direction_t prevDir = DIR_NONE;

  if (snake.length == 0)
    {
      prevDir = newDir;
      return newDir;
    }

  switch (newDir)
    {
    case DIR_UP:
      if (prevDir == DIR_DOWN)
        return prevDir;
      break;

    case DIR_DOWN:
      if (prevDir == DIR_UP)
        return prevDir;
      break;

    case DIR_LEFT:
      if (prevDir == DIR_RIGHT)
        return prevDir;
      break;

    case DIR_RIGHT:
      if (prevDir == DIR_LEFT)
        return prevDir;
      break;

    default:
      break;
    }

  prevDir = newDir;
  return newDir;
}

/*
 * Aktualizuje pozycje snejka - usuwa ostatni jego element,
 * dodaje nowy na początek. Funkcja zwraca:
 * false jesli nie udalo się poruszyc (wystąpila kolizja)
 * true jesli wykonano ruch na prawidlowe pole
 */
static bool move(void)
{
  if (getDirection() != DIR_NONE)
    snake.dir = filterDirection(getDirection());

  tempRect = snake.tail[0];

  switch (snake.dir)
    {
    case DIR_UP:
      GUI_MoveRect(&tempRect, 0, -snake.size);
      break;

    case DIR_DOWN:
      GUI_MoveRect(&tempRect, 0, snake.size);
      break;

    case DIR_RIGHT:
      GUI_MoveRect(&tempRect, snake.size, 0);
      break;

    case DIR_LEFT:
      GUI_MoveRect(&tempRect, -snake.size, 0);
      break;

    case DIR_NONE:
      break;

    }

  if (checkCollision())
    return false;

  return true;
}

/*
 * Sprawdza czy glowa snejka znajduje sie na jedzeniu,
 * jesli tak, to usuwa jedzenie z ekranu i wydluza snejka
 */
static void eat(food_t *food)
{
  uint16_t i;

  if (tempRect.x0 == food->pos.x0 && tempRect.y0 == food->pos.y0)
    {
      snake.length++;
      food->show(&snake);
    }
  else
    {
      GUI_ClearRectEx(&snake.tail[snake.length]);
    }

  for (i = snake.length; i > 0; i--)
    snake.tail[i] = snake.tail[i - 1];

  snake.tail[0] = tempRect;
  // xprintf("x: %d, y: %d\n", snake.tail[0].x0, snake.tail[0].y0);
}

/* Wyswietla snejka na ekranie */
static void show(void)
{
  uint16_t i;

  GUI_SetColor(GUI_BLACK);
  for (i = 0; i <= snake.length; i++)
    GUI_FillRectEx(&snake.tail[i]);
}

/*
 * Konstruktor
 * x, y - pozycja snejka
 * size - rozmiar snejka
 */
snake_t *snakeInit(uint8_t x, uint8_t y, uint8_t size)
{
  snake.eat = eat;
  snake.move = move;
  snake.show = show;
  snake.size = size;
  snake.length = 0;

  snake.tail[0].x0 = x;
  snake.tail[0].y0 = y;
  snake.tail[0].x1 = x + size;
  snake.tail[0].y1 = y + size;

  snake.dir = DIR_NONE;

  // Wyswietl snejka przy startupie
  GUI_SetColor(GUI_BLACK);
  GUI_FillRectEx(&snake.tail[0]);

  xprintf("snake.x: %d\n", snake.tail[0].x0);
  xprintf("snake.y: %d\n", snake.tail[0].y0);

  return &snake;
}
