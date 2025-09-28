/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_tick.c
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : SysTick manager.
 *                      This file provides interface to manage SysTick functions
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <ch32v00x_hal.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef HAL_TICK_DEFAULT_HZ
#define HAL_TICK_DEFAULT_HZ   1000u   /* 1 kHz = 1 ms/tick */
#endif

#define SYSTICK_STE_BIT     (1u << 0)   /* enable counter */
#define SYSTICK_STIE_BIT    (1u << 1)   /* interrupt enable */
#define SYSTICK_STCLK_BIT   (1u << 2)   /* counter clock source HCLK */
#define SYSTICK_STRE_BIT    (1u << 3)   /* auto-reload */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint32_t uwTick = 0;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  Initialize SysTick at 1000Hz, period 1ms
  * @retval none
  */
void HAL_TickInit(void)
{
    uint32_t cmp = SystemCoreClock / HAL_TICK_DEFAULT_HZ;

    if (cmp == 0u) cmp = 1u;

    CLEAR_REG(SysTick->CTLR);     /* stop */
    CLEAR_REG(SysTick->CNT);     /* clear counter */
    CLEAR_REG(SysTick->SR);     /* clear flag */
    WRITE_REG(SysTick->CMP, cmp);   /* period */
    WRITE_REG(SysTick->CTLR, SYSTICK_STE_BIT | SYSTICK_STIE_BIT | SYSTICK_STCLK_BIT | SYSTICK_STRE_BIT);
    NVIC_EnableIRQ(SysTicK_IRQn);
}

/**
  * @brief Provides a tick value in millisecond.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval tick value
  */
uint32_t HAL_GetTick(void)
{
    return uwTick;
}

/**
  * @brief This function provides minimum delay (in milliseconds) based
  *        on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals where uwTick
  *       is incremented.
  * @param Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
void HAL_Delay(uint32_t Delay)
{
    uint32_t start = HAL_GetTick();

    while ((uint32_t)(HAL_GetTick() - start) < Delay) { __asm volatile("nop"); }
}

/**
  * @brief This function provides minimum delay (in microseconds) based
  *        on variable incremented.
  * @param Delay specifies the delay time length, in microseconds.
  * @retval None
  */
void HAL_DelayUs(uint32_t Delay)
{
    if (Delay == 0u) return;

    uint32_t need = ((uint32_t)Delay * (uint32_t)SystemCoreClock) / 1000000ull;

    if (need == 0ull) need = 1ull;

    uint32_t cmp  = READ_REG(SysTick->CMP); if (cmp == 0u) cmp = 1u;
    uint32_t last = READ_REG(SysTick->CNT);
    uint32_t acc  = 0ull;

    while (1)
    {
        uint32_t now = READ_REG(SysTick->CNT);
        uint32_t delta = (now >= last) ? (now - last) : ((cmp - last) + now);
        acc += (uint32_t)delta;
        last = now;

        if (acc >= need) break;
    }
}

/**
  * @brief  This function checks whether a timeout has expired
  *         based on the system tick.
  * @param  start_ms   Tick count value at the start of the timeout period.
  * @param  timeout_ms Timeout duration in milliseconds.
  * @retval TRUE  Timeout has expired.
  * @retval FALSE Timeout is still ongoing.
  */
uint8_t HAL_TickExpired(uint32_t start_ms, uint32_t timeout_ms)
{
    return ((uint32_t)(HAL_GetTick() - start_ms) > timeout_ms) ? TRUE : FALSE;
}

/**
  * @brief Tick count will increase by 1 every 1ms.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval none
  */
__attribute__((weak)) void HAL_IncTick(void) { uwTick++; }

/**
  * @brief ISR of SysTick, which will trigger tick count to be increased by 1 every 1ms.
  * @retval none
  */
__attribute__((interrupt("WCH-Interrupt-fast")))
void SysTick_Handler(void)
{
    CLEAR_REG(SysTick->SR);
    HAL_IncTick();
}
