/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_gpio.h
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : Header file of GPIO HAL module
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
#ifndef __CH32V00X_HAL_GPIO_H
#define __CH32V00X_HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Output Maximum frequency selection */
typedef enum
{
    GPIO_Speed_10MHz = 1,
    GPIO_Speed_2MHz,
    GPIO_Speed_30MHz
} GPIOSpeed_TypeDef;

/* Configuration Mode enumeration */
typedef enum
{
    GPIO_Mode_AIN = 0x0,
    GPIO_Mode_IN_FLOATING = 0x04,
    GPIO_Mode_IPD = 0x28,
    GPIO_Mode_IPU = 0x48,
    GPIO_Mode_Out_OD = 0x14,
    GPIO_Mode_Out_PP = 0x10,
    GPIO_Mode_AF_OD = 0x1C,
    GPIO_Mode_AF_PP = 0x18
} GPIOMode_TypeDef;

/* GPIO Bit SET and Bit RESET enumeration  */
typedef enum
{
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
} GPIO_PinState;

/* GPIO Init structure definition */
typedef struct
{
    uint16_t GPIO_Pin; /* Specifies the GPIO pins to be configured.
                          This parameter can be any value of @ref GPIO_pins_define */

    GPIOSpeed_TypeDef GPIO_Speed; /* Specifies the speed for the selected pins.
                                     This parameter can be a value of @ref GPIOSpeed_TypeDef */

    GPIOMode_TypeDef GPIO_Mode; /* Specifies the operating mode for the selected pins.
                                   This parameter can be a value of @ref GPIOMode_TypeDef */
} GPIO_InitTypeDef;
/* Exported constants --------------------------------------------------------*/
/* GPIO_pins_define */
#define GPIO_Pin_0                     ((uint16_t)0x0001) /* Pin 0 selected */
#define GPIO_Pin_1                     ((uint16_t)0x0002) /* Pin 1 selected */
#define GPIO_Pin_2                     ((uint16_t)0x0004) /* Pin 2 selected */
#define GPIO_Pin_3                     ((uint16_t)0x0008) /* Pin 3 selected */
#define GPIO_Pin_4                     ((uint16_t)0x0010) /* Pin 4 selected */
#define GPIO_Pin_5                     ((uint16_t)0x0020) /* Pin 5 selected */
#define GPIO_Pin_6                     ((uint16_t)0x0040) /* Pin 6 selected */
#define GPIO_Pin_7                     ((uint16_t)0x0080) /* Pin 7 selected */
#define GPIO_Pin_All                   ((uint16_t)0xFFFF) /* All pins selected */

/* GPIO_Port_Sources */
#define GPIO_PortSourceGPIOA           ((uint8_t)0x00)
#define GPIO_PortSourceGPIOC           ((uint8_t)0x02)
#define GPIO_PortSourceGPIOD           ((uint8_t)0x03)

/* GPIO_Pin_sources */
#define GPIO_PinSource0                ((uint8_t)0x00)
#define GPIO_PinSource1                ((uint8_t)0x01)
#define GPIO_PinSource2                ((uint8_t)0x02)
#define GPIO_PinSource3                ((uint8_t)0x03)
#define GPIO_PinSource4                ((uint8_t)0x04)
#define GPIO_PinSource5                ((uint8_t)0x05)
#define GPIO_PinSource6                ((uint8_t)0x06)
#define GPIO_PinSource7                ((uint8_t)0x07)
/* Exported macro ------------------------------------------------------------*/
/**
  * @brief  Checks whether the specified EXTI line is asserted or not.
  * @param  __EXTI_LINE__ specifies the EXTI line to check.
  *          This parameter can be GPIO_Pin_x where x can be(0..7)
  * @retval The new state of __EXTI_LINE__ (SET or RESET).
  */
#define __HAL_GPIO_EXTI_GET_IT(__EXTI_LINE__) ((EXTI->INTFR & (__EXTI_LINE__)) && (EXTI->INTENR & (__EXTI_LINE__)))

/**
  * @brief  Clears the EXTI's line pending bits.
  * @param  __EXTI_LINE__ specifies the EXTI lines to clear.
  *          This parameter can be any combination of GPIO_Pin_x where x can be (0..7)
  * @retval None
  */
#define __HAL_GPIO_EXTI_CLEAR_IT(__EXTI_LINE__) (EXTI->INTFR = (__EXTI_LINE__))
/* Exported functions --------------------------------------------------------*/
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);
GPIO_PinState HAL_GPIO_ReadInput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
GPIO_PinState HAL_GPIO_ReadOutput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_PinLockConfig(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
/* Private macros ------------------------------------------------------------*/
/* GPIO check port */
#define IS_GPIO_INSTANCE(INSTANCE) ( \
    ((INSTANCE) == GPIOA)     || \
    ((INSTANCE) == GPIOC)     || \
    ((INSTANCE) == GPIOD) )

/* GPIO check pin */
#define IS_GPIO_PIN(PIN) ( \
    ((PIN) == GPIO_Pin_0)    || \
    ((PIN) == GPIO_Pin_1)    || \
    ((PIN) == GPIO_Pin_2)    || \
    ((PIN) == GPIO_Pin_3)    || \
    ((PIN) == GPIO_Pin_4)    || \
    ((PIN) == GPIO_Pin_5)    || \
    ((PIN) == GPIO_Pin_6)    || \
    ((PIN) == GPIO_Pin_7) )

/* GPIO check speed */
#define IS_GPIO_SPEED(SPD)    ( \
    (SPD) == GPIO_Speed_10MHz     || \
    (SPD) == GPIO_Speed_2MHz     || \
    (SPD) == GPIO_Speed_30MHz  )

/* GPIO check mode */
#define IS_GPIO_MODE(MODE) ( \
    ((MODE) == GPIO_Mode_AIN)    || \
    ((MODE) == GPIO_Mode_IN_FLOATING)     || \
    ((MODE) == GPIO_Mode_IPD)  || \
    ((MODE) == GPIO_Mode_IPU)  || \
    ((MODE) == GPIO_Mode_Out_OD) || \
    ((MODE) == GPIO_Mode_Out_PP) || \
    ((MODE) == GPIO_Mode_AF_OD)     || \
    ((MODE) == GPIO_Mode_AF_PP) )

/* GPIO check inout mode */
#define IS_GPIO_INPUT(MODE) ( \
    ((MODE) == GPIO_Mode_AIN)    || \
    ((MODE) == GPIO_Mode_IN_FLOATING)     || \
    ((MODE) == GPIO_Mode_IPD)  || \
    ((MODE) == GPIO_Mode_IPU)  )

/* GPIO check HIGH/LOW pin state */
#define IS_GPIO_PIN_STATE(LVL)   ( (LVL) == GPIO_PIN_RESET || (LVL) == GPIO_PIN_SET )

/* GPIO check port source */
#define IS_GPIO_PORT_SOURCE(PORT) ( \
    ((PORT) == GPIO_PortSourceGPIOA) || \
    ((PORT) == GPIO_PortSourceGPIOC) || \
    ((PORT) == GPIO_PortSourceGPIOD) )

/* GPIO check pin source */
#define IS_GPIO_PIN_SOURCE(PIN) ( \
    ((PIN) == GPIO_PinSource0) || \
    ((PIN) == GPIO_PinSource1) || \
    ((PIN) == GPIO_PinSource2) || \
    ((PIN) == GPIO_PinSource3) || \
    ((PIN) == GPIO_PinSource4) || \
    ((PIN) == GPIO_PinSource5) || \
    ((PIN) == GPIO_PinSource6) || \
    ((PIN) == GPIO_PinSource7) )

#ifdef __cplusplus
}
#endif

#endif /* __CH32V00X_HAL_GPIO_H */