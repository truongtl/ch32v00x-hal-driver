/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_assert.h
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : This file contains HAL assert_failed function.
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <ch32v00x_hal.h>

__weak void assert_failed(char *file, int line)
{
    printf("ASSERT %s:%d\r\n", file, line);
    while (1) { /* __WFI(); */ }
}
