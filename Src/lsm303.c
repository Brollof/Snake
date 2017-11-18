#include "main.h"
#include "board.h"
#include "xprintf.h"
#include "snake.h"

#define ACC_I2C_ADDRESS (0x19 << 1) // 0011001b

/************** ACCELEROMTERER REGISTERS **************/
#define CTRL_REG1       0x20
#define CTRL_REG2       0x21
#define CTRL_REG3       0x22
#define CTRL_REG4       0x23
#define CTRL_REG5       0x24
#define CTRL_REG6       0x25

#define REFERENCE       0x26
#define STATUS_REG      0x27

#define OUT_X_L         0x28
#define OUT_X_H         0x29
#define OUT_Y_L         0x2A
#define OUT_Y_H         0x2B
#define OUT_Z_L         0x2C
#define OUT_Z_H         0x2D

#define FIFO_CTRL_REG   0x2E
#define FIFO_SRC_REG    0x2F
#define INT1_CFG        0x30
#define INT1_SOURCE     0x31
#define INT1_THS        0x32
#define INT1_DURATION   0x33
#define INT2_CFG        0x34
#define INT2_SOURCE     0x35
#define INT2_THS        0x36
#define INT2_DURATION   0x37
#define CLICK_CFG       0x38
#define CLICK_SRC       0x39
#define CLICK_THS       0x3A
#define TIME_LIMIT      0x3B
#define TIME_LATENCY    0x3C
#define TIME_WINDOW     0x3D

/****************** CONFIGURATION ******************/
#define CFG_POWER_DOWN 0x00

#define CFG_X_EN (1 << 0)
#define CFG_Y_EN (1 << 1)
#define CFG_Z_EN (1 << 2)

#define CFG_1_HZ (1 << 4)
#define CFG_10_HZ (1 << 5)
#define CFG_25_HZ (1 << 4) | (1 << 5)
#define CFG_50_HZ (1 << 6)
#define CFG_100_HZ (1 << 6) | (1 << 4)
#define CFG_200_HZ (1 << 6) | (1 << 5)
#define CFG_400_HZ (1 << 6) | (1 << 5) | (1 << 4)

extern I2C_HandleTypeDef hi2c3;

typedef struct
{
  int16_t x, y, z;
} acc_t;

acc_t acc;

void accInit(void)
{
  uint8_t cfg = CFG_100_HZ | CFG_X_EN | CFG_Y_EN | CFG_Z_EN;

  HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c3, ACC_I2C_ADDRESS, CTRL_REG1, 1, &cfg, 1, 100);
  if (status != HAL_OK)
    xprintf("ACC init FAIL\n");
  else
    xprintf("ACC init OK\n");

}

static void readRaw(void)
{
  uint8_t buf[6];
  HAL_I2C_Mem_Read(&hi2c3, ACC_I2C_ADDRESS, (OUT_X_L | 0x80), 1, buf, 6, 100);

  acc.x = (buf[1] << 8) | buf[0];
  acc.y = (buf[3] << 8) | buf[2];
  acc.z = (buf[5] << 8) | buf[4];
}

direction_t getDirection(void)
{
  readRaw();

  if (acc.x > 10000)
    return DIR_UP;
  else if (acc.x < -10000)
    return DIR_DOWN;
  else if (acc.y > 10000)
    return DIR_LEFT;
  else if (acc.y < -10000)
    return DIR_RIGHT;

  return DIR_NONE;
}
