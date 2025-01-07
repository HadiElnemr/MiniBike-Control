/***********************************************
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: 1.0
Update: 2021-12-09

All rights reserved
***********************************************/
#include "stm32f1xx_hal.h"

/**************************************************************************
Function: External Interrupt Initialization
Parameters: None
Return Value: None
**************************************************************************/

void MiniBalance_EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable clocks for GPIOB and AFIO
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    // Configure PB12 as input with pull-up
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Map EXTI Line to GPIO Pin
    __HAL_AFIO_REMAP_EXTI_LINE(GPIO_PIN_12);

    // Configure NVIC for EXTI Line
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**************************************************************************
Interrupt Handler for EXTI Line [15:10]
**************************************************************************/

//void EXTI15_10_IRQHandler(void)
//{
//    // Check if EXTI Line 12 triggered the interrupt
//    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET)
//    {
//        // Clear the EXTI interrupt flag
//        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
//
//        // Your custom interrupt handling code here
//        // Example: Toggle an LED or log the event
//    }
//}
