#include "stm32f4xx_hal.h"
#include <xprintf.h>
#include "board.h"
#include "libUART.h"
#include <string.h>
#include <stdbool.h>

volatile bool semaphore = false;
UART_HandleTypeDef huart1;
uint8_t rxChar = 0;

void sendConsole(char data)
{
  HAL_UART_Transmit_IT(&huart1, (uint8_t *)&data, sizeof(uint8_t));
  semaphore = true;
  while (semaphore);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
    {
      HAL_UART_Receive_IT(&huart1, &rxChar, 1);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
    {
      semaphore = false;
    }
}

void uartInit(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200; // Prędkość transmisji równa 115200 bit/s
  huart1.Init.WordLength = UART_WORDLENGTH_8B; // Osiem bitów danych
  huart1.Init.StopBits = UART_STOPBITS_1; // Jeden bit stopu
  huart1.Init.Parity = UART_PARITY_NONE; // Brak bitu parzystości
  huart1.Init.Mode = UART_MODE_TX_RX; // Komunikacja w obie strony (nadawanie i odbieranie)
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE; // Brak sprzętowej kontroli przepływu
  huart1.Init.OverSampling = UART_OVERSAMPLING_16; // Nadpróbkowanie sygnału
  HAL_UART_Init(&huart1); // Inicjalizacja peryferium

  HAL_NVIC_SetPriority(USART1_IRQn, 2, 0); // Ustawienie priorytetu przerwań
  HAL_NVIC_EnableIRQ(USART1_IRQn); // Włączenie przerwań peryferium USART1

  HAL_UART_Receive_IT(&huart1, &rxChar, 1); // Rozpoczęcie nasłuchiwania w trybie przerwań
}