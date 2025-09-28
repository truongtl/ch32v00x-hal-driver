/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_tick.c
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : Header file of tick manager
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
#ifndef __CH32V00X_HAL_TICK_H
#define __CH32V00X_HAL_TICK_H

/* Exported functions --------------------------------------------------------*/
void HAL_TickInit(void);
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);
void HAL_DelayUs(uint32_t Delay);
uint8_t HAL_TickExpired(uint32_t start_ms, uint32_t timeout_ms);

#endif /* __CH32V00X_HAL_TICK_H */
