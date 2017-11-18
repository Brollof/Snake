#include "main.h"
#include <board.h>
#include <stdbool.h>
#include <xprintf.h>
#include <libUART.h>
#include <string.h>
#include "guiLib.h"
#include "snake.h"
#include "lsm303.h"
#include "food.h"

typedef enum
{
  LED_EVENT,
  GAME_EVENT,
  BUTTON_EVENT,

  eventsNum,
} events_t;

typedef struct
{
  void (*callback)(void);
  uint32_t period;
  uint32_t lastExeTime;
} periodEvents_t;

static void gameUpdate(void);
static void changeLedState(void);
static void checkInput(void);
static void doPeriodically(void);

periodEvents_t periodEvents[eventsNum] =
{
  [LED_EVENT] = {changeLedState, 1000},
  [GAME_EVENT] = {gameUpdate, 700}, // predkosc gry
  [BUTTON_EVENT] = {checkInput, 20},
};

static snake_t *snake = NULL;
static food_t *food = NULL;

// rozmiar siatki po ktorej porusza sie snejk
#define GRID_SIZE 40

int main(void)
{
  boardInit();
  guiInit();
  xprintf("Init done.\n");

  snake = snakeInit(LCD_GetXSize() / 2, LCD_GetYSize() / 2, GRID_SIZE);
  food = foodInit(GRID_SIZE);
  food->show(snake);

  periodEvents[GAME_EVENT].callback = gameUpdate;

  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

  while (1)
    {

      doPeriodically();
    }
}

static void doPeriodically(void)
{
  static uint32_t tick = 0;
  static uint8_t i;

  tick = HAL_GetTick();
  for (i = 0; i < eventsNum; i++)
    {
      if (tick - periodEvents[i].lastExeTime >= periodEvents[i].period)
        {
          if (periodEvents[i].callback)
            {
              periodEvents[i].callback();
              periodEvents[i].lastExeTime = tick;
            }
        }
    }
}

static void gameUpdate(void)
{
  if (!snake->move()) // warunek konca gry
    {
      periodEvents[GAME_EVENT].callback = NULL;
      endGameScreen(snake->length);
      return;
    }

  snake->eat(food);
  snake->show();
}

static void changeLedState(void)
{
  GREEN_LED_TOGGLE();
}

static void gameRestart(void)
{
  GUI_Clear();
  snake = snakeInit(LCD_GetXSize() / 2, LCD_GetYSize() / 2, GRID_SIZE);
  food->show(snake);
  periodEvents[GAME_EVENT].callback = gameUpdate;
}

static void checkInput(void) // called every 20 ms
{
  static uint8_t state = 0;
  static bool pressed = false;

  if (BSP_PB_GetState(BUTTON_KEY) == 1)
    {
      state++;
      if (state > 3 && pressed == false)
        {
          gameRestart();
          pressed = true;
        }
    }
  else
    {
      pressed = false;
      state = 0;
    }
}
