/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_assert.h
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : Header file of assert_failed function and HAL_CHECK macro.
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
#ifndef __CH32V00X_HAL_ASSERT_H
#define __CH32V00X_HAL_ASSERT_H

#ifdef HAL_USE_FULL_ASSERT

#define assert_param(expr) ((expr) ? (void)0U : assert_failed((char *)__FILE__, __LINE__))

void assert_failed(char *file, int line);

#else
#define assert_param(expr) ((void)0U)
#endif

#ifdef HAL_USE_FULL_ASSERT
#define HAL_PARAM_CHECK(expr) assert_param(expr)
#else
#define HAL_PARAM_CHECK(expr) do { if (!(expr)) return HAL_ERROR; } while (0)
#endif

#endif /* __CH32V00X_HAL_ASSERT_H */
