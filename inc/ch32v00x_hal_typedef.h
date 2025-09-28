/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_typedef.h
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : This file contains HAL common defines, enumeration and macros.
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
#ifndef _CH32V00X_HAL_TYPEDEF
#define _CH32V00X_HAL_TYPEDEF

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  HAL Status structures definition
  */
typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

/**
  * @brief  HAL Lock structures definition
  */
typedef enum
{
  HAL_UNLOCKED = 0x00U,
  HAL_LOCKED   = 0x01U
} HAL_LockTypeDef;

/* Exported macro ------------------------------------------------------------*/
#define TRUE  1
#define FALSE 0

#if !defined(UNUSED)
#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#endif /* UNUSED */

#define HAL_MAX_DELAY      0xFFFFFFFFU

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define IS_BIT_SET(REG, BIT)  ((READ_BIT(REG, BIT)) == (BIT))

#define IS_BIT_CLR(REG, BIT)  ((READ_BIT(REG, BIT)) == 0U)

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define __weak   __attribute__((weak))

/* Atomic 32-bit register access macro to set one or several bits */
#define ATOMIC32_SET_BIT(REG, BIT)                               \
    do {                                                         \
        uint32_t __ms = _irq_lock();                             \
        __IO uint32_t *__r = (__IO uint32_t *)&(REG);            \
        *__r = (uint32_t)(*__r |  (uint32_t)(BIT));              \
        _irq_unlock(__ms);                                       \
    } while (0)

/* Atomic 32-bit register access macro to clear one or several bits */
#define ATOMIC32_CLEAR_BIT(REG, BIT)                             \
    do {                                                         \
        uint32_t __ms = _irq_lock();                             \
        __IO uint32_t *__r = (__IO uint32_t *)&(REG);            \
        *__r = (uint32_t)(*__r & ~(uint32_t)(BIT));              \
        _irq_unlock(__ms);                                       \
    } while (0)

/* Atomic 32-bit register access macro to clear and set one or several bits */
#define ATOMIC32_MODIFY(REG, CLEARMSK, SETMASK)                                  \
    do {                                                                         \
        uint32_t __ms = _irq_lock();                                             \
        __IO uint32_t *__r = (__IO uint32_t *)&(REG);                            \
        *__r = (uint32_t)(((*__r & ~(uint32_t)(CLEARMSK)) | (uint32_t)(SETMASK))); \
        _irq_unlock(__ms);                                                       \
    } while (0)

/* Atomic 16-bit register access macro to set one or several bits */
#define ATOMIC16_SET_BIT(REG, BIT)                               \
    do {                                                         \
        uint32_t __ms = _irq_lock();                             \
        __IO uint16_t *__r = (__IO uint16_t *)&(REG);            \
        *__r = (uint16_t)(*__r |  (uint16_t)(BIT));              \
        _irq_unlock(__ms);                                       \
    } while (0)

/* Atomic 16-bit register access macro to clear one or several bits */
#define ATOMIC16_CLEAR_BIT(REG, BIT)                             \
    do {                                                         \
        uint32_t __ms = _irq_lock();                             \
        __IO uint16_t *__r = (__IO uint16_t *)&(REG);            \
        *__r = (uint16_t)(*__r & ~(uint16_t)(BIT));              \
        _irq_unlock(__ms);                                       \
    } while (0)

/* Atomic 16-bit register access macro to clear and set one or several bits */
#define ATOMIC16_MODIFY(REG, CLEARMSK, SETMASK)                                  \
    do {                                                                         \
        uint32_t __ms = _irq_lock();                                             \
        __IO uint16_t *__r = (__IO uint16_t *)&(REG);                            \
        *__r = (uint16_t)(((*__r & ~(uint16_t)(CLEARMSK)) | (uint16_t)(SETMASK))); \
        _irq_unlock(__ms);                                                       \
    } while (0)

/* Atomic 8-bit register access macro to set one or several bits */
#define ATOMIC8_SET_BIT(REG, BIT)                                \
    do {                                                         \
        uint32_t __ms = _irq_lock();                             \
        __IO uint8_t *__r = (__IO uint8_t *)&(REG);              \
        *__r = (uint8_t)(*__r |  (uint8_t)(BIT));                \
        _irq_unlock(__ms);                                       \
    } while (0)

/* Atomic 8-bit register access macro to clear one or several bits */
#define ATOMIC8_CLEAR_BIT(REG, BIT)                              \
    do {                                                         \
        uint32_t __ms = _irq_lock();                             \
        __IO uint8_t *__r = (__IO uint8_t *)&(REG);              \
        *__r = (uint8_t)(*__r & ~(uint8_t)(BIT));                \
        _irq_unlock(__ms);                                       \
    } while (0)

/* Atomic 8-bit register access macro to clear and set one or several bits */
#define ATOMIC8_MODIFY(REG, CLEARMSK, SETMASK)                                   \
    do {                                                                         \
        uint32_t __ms = _irq_lock();                                             \
        __IO uint8_t *__r = (__IO uint8_t *)&(REG);                              \
        *__r = (uint8_t)(((*__r & ~(uint8_t)(CLEARMSK)) | (uint8_t)(SETMASK)));  \
        _irq_unlock(__ms);                                                       \
    } while (0)

/* Private inline functions --------------------------------------------------*/
static inline uint32_t _irq_lock(void)
{
    uint32_t ms;
    /* csrrci: read mstatus -> ms, and clear bit 3 (MIE) */
    __asm volatile ("csrrci %0, mstatus, 8" : "=r"(ms) :: "memory");
    /* compiler barrier to order MMIO */
    __asm volatile ("" ::: "memory");
    return ms;
}

static inline void _irq_unlock(uint32_t ms)
{
    /* restore MIE only if it was set before */
    if (ms & 0x8u) {
        __asm volatile ("csrs mstatus, 8" ::: "memory");
    }
    __asm volatile ("" ::: "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* _CH32V00X_HAL_TYPEDEF */
