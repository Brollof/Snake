#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if (huart->Instance == USART1)
    {
      __USART1_CLK_ENABLE();

      /**USART1 GPIO Configuration
      PA9     ------> USART1_TX
      PA10     ------> USART1_RX
      */
      GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if (huart->Instance == UART4)
    {
      __UART4_CLK_DISABLE();

      /**UART4 GPIO Configuration
      PA0/WKUP     ------> UART4_TX
      PA1     ------> UART4_RX
      */
      HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

      /* Peripheral DMA DeInit*/
      HAL_DMA_DeInit(huart->hdmatx);

      /* Peripheral interrupt DeInit*/
      HAL_NVIC_DisableIRQ(UART4_IRQn);
    }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if (hi2c->Instance == I2C3)
    {
      /* USER CODE BEGIN I2C3_MspInit 0 */

      /* USER CODE END I2C3_MspInit 0 */

      /**I2C3 GPIO Configuration
      PC9     ------> I2C3_SDA
      PA8     ------> I2C3_SCL
      */
      GPIO_InitStruct.Pin = GPIO_PIN_9;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_8;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      /* Peripheral clock enable */
      __HAL_RCC_I2C3_CLK_ENABLE();
      /* USER CODE BEGIN I2C3_MspInit 1 */

      /* USER CODE END I2C3_MspInit 1 */
    }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{

  if (hi2c->Instance == I2C3)
    {
      /* USER CODE BEGIN I2C3_MspDeInit 0 */

      /* USER CODE END I2C3_MspDeInit 0 */
      /* Peripheral clock disable */
      __HAL_RCC_I2C3_CLK_DISABLE();

      /**I2C3 GPIO Configuration
      PC9     ------> I2C3_SDA
      PA8     ------> I2C3_SCL
      */
      HAL_GPIO_DeInit(GPIOC, GPIO_PIN_9);

      HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);

    }
  /* USER CODE BEGIN I2C3_MspDeInit 1 */

  /* USER CODE END I2C3_MspDeInit 1 */
}