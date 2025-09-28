/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_rcc.h
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : Header file of RCC HAL module
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
#ifndef __CH32V00X_HAL_RCC_H
#define __CH32V00X_HAL_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* RCC Init Structure definition */
typedef struct
{
    uint32_t SYSCLK_Frequency; /* returns SYSCLK clock frequency expressed in Hz */
    uint32_t HCLK_Frequency;   /* returns HCLK clock frequency expressed in Hz */
    uint32_t PCLK1_Frequency;  /* returns PCLK1 clock frequency expressed in Hz */
    uint32_t PCLK2_Frequency;  /* returns PCLK2 clock frequency expressed in Hz */
    uint32_t ADCCLK_Frequency; /* returns ADCCLK clock frequency expressed in Hz */
} RCC_ClocksTypeDef;
/* Exported constants --------------------------------------------------------*/
/* MCO Index */
#define RCC_MCO1                         ((uint32_t)0x00000000)

/* HSE_configuration */
#define RCC_HSE_OFF                      ((uint32_t)0x00000000)
#define RCC_HSE_ON                       ((uint32_t)0x00010000)
#define RCC_HSE_Bypass                   ((uint32_t)0x00040000)

/* PLL_entry_clock_source */
#define RCC_PLLSource_HSI_MUL2           ((uint32_t)0x00000000)
#define RCC_PLLSource_HSE_MUL2           ((uint32_t)0x00010000)

/* System_clock_source */
#define RCC_SYSCLKSource_HSI             ((uint32_t)0x00000000)
#define RCC_SYSCLKSource_HSE             ((uint32_t)0x00000001)
#define RCC_SYSCLKSource_PLLCLK          ((uint32_t)0x00000002)

/* AHB_clock_source */
#define RCC_SYSCLK_Div1                  ((uint32_t)0x00000000)
#define RCC_SYSCLK_Div2                  ((uint32_t)0x00000010)
#define RCC_SYSCLK_Div3                  ((uint32_t)0x00000020)
#define RCC_SYSCLK_Div4                  ((uint32_t)0x00000030)
#define RCC_SYSCLK_Div5                  ((uint32_t)0x00000040)
#define RCC_SYSCLK_Div6                  ((uint32_t)0x00000050)
#define RCC_SYSCLK_Div7                  ((uint32_t)0x00000060)
#define RCC_SYSCLK_Div8                  ((uint32_t)0x00000070)
#define RCC_SYSCLK_Div16                 ((uint32_t)0x000000B0)
#define RCC_SYSCLK_Div32                 ((uint32_t)0x000000C0)
#define RCC_SYSCLK_Div64                 ((uint32_t)0x000000D0)
#define RCC_SYSCLK_Div128                ((uint32_t)0x000000E0)
#define RCC_SYSCLK_Div256                ((uint32_t)0x000000F0)

/* RCC_Interrupt_source */
#define RCC_IT_LSIRDY                    ((uint8_t)0x01)
#define RCC_IT_HSIRDY                    ((uint8_t)0x04)
#define RCC_IT_HSERDY                    ((uint8_t)0x08)
#define RCC_IT_PLLRDY                    ((uint8_t)0x10)
#define RCC_IT_CSS                       ((uint8_t)0x80)

/* ADC_clock_source */
#define RCC_PCLK2_Div2                   ((uint32_t)0x00000000)
#define RCC_PCLK2_Div4                   ((uint32_t)0x00004000)
#define RCC_PCLK2_Div6                   ((uint32_t)0x00008000)
#define RCC_PCLK2_Div8                   ((uint32_t)0x0000C000)
#define RCC_PCLK2_Div12                  ((uint32_t)0x0000A000)
#define RCC_PCLK2_Div16                  ((uint32_t)0x0000E000)
#define RCC_PCLK2_Div24                  ((uint32_t)0x0000A800)
#define RCC_PCLK2_Div32                  ((uint32_t)0x0000E800)
#define RCC_PCLK2_Div48                  ((uint32_t)0x0000B000)
#define RCC_PCLK2_Div64                  ((uint32_t)0x0000F000)
#define RCC_PCLK2_Div96                  ((uint32_t)0x0000B800)
#define RCC_PCLK2_Div128                 ((uint32_t)0x0000F800)

/* AHB_peripheral */
#define RCC_AHBPeriph_DMA1               ((uint32_t)0x00000001)
#define RCC_AHBPeriph_SRAM               ((uint32_t)0x00000004)

/* APB2_peripheral */
#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOC             ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD             ((uint32_t)0x00000020)
#define RCC_APB2Periph_ADC1              ((uint32_t)0x00000200)
#define RCC_APB2Periph_TIM1              ((uint32_t)0x00000800)
#define RCC_APB2Periph_SPI1              ((uint32_t)0x00001000)
#define RCC_APB2Periph_USART1            ((uint32_t)0x00004000)

/* APB1_peripheral */
#define RCC_APB1Periph_TIM2              ((uint32_t)0x00000001)
#define RCC_APB1Periph_WWDG              ((uint32_t)0x00000800)
#define RCC_APB1Periph_I2C1              ((uint32_t)0x00200000)
#define RCC_APB1Periph_PWR               ((uint32_t)0x10000000)

/* Clock_source_to_output_on_MCO_pin */
#define RCC_MCO_NoClock                  ((uint8_t)0x00)
#define RCC_MCO_SYSCLK                   ((uint8_t)0x04)
#define RCC_MCO_HSI                      ((uint8_t)0x05)
#define RCC_MCO_HSE                      ((uint8_t)0x06)
#define RCC_MCO_PLLCLK                   ((uint8_t)0x07)

/* RCC_Flag */
#define RCC_FLAG_HSIRDY                  ((uint8_t)0x21)
#define RCC_FLAG_HSERDY                  ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY                  ((uint8_t)0x39)
#define RCC_FLAG_LSIRDY                  ((uint8_t)0x61)
#define RCC_FLAG_PINRST                  ((uint8_t)0x7A)
#define RCC_FLAG_PORRST                  ((uint8_t)0x7B)
#define RCC_FLAG_SFTRST                  ((uint8_t)0x7C)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x7D)
#define RCC_FLAG_WWDGRST                 ((uint8_t)0x7E)
#define RCC_FLAG_LPWRRST                 ((uint8_t)0x7F)

/* SysTick_clock_source */
#define SysTick_CLKSource_HCLK_Div8      ((uint32_t)0xFFFFFFFB)
#define SysTick_CLKSource_HCLK           ((uint32_t)0x00000004)
/* Exported macro ------------------------------------------------------------*/
/* ===========================================================
 * RCC clock macros for CH32V003 (APB2/APB1)
 * =========================================================== */

/* ---------- APB2: ENABLE ---------- */
#define __HAL_RCC_AFIO_CLK_ENABLE()    do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_AFIO);                   \
    tmpreg = READ_BIT(RCC->APB2PCENR, RCC_APB2Periph_AFIO);         \
    UNUSED(tmpreg);                                                 \
} while (0U)

#define __HAL_RCC_GPIOA_CLK_ENABLE()   do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOA);                  \
    tmpreg = READ_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOA);        \
    UNUSED(tmpreg);                                                 \
} while (0U)

#define __HAL_RCC_GPIOC_CLK_ENABLE()   do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOC);                  \
    tmpreg = READ_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOC);        \
    UNUSED(tmpreg);                                                 \
} while (0U)

#define __HAL_RCC_GPIOD_CLK_ENABLE()   do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOD);                  \
    tmpreg = READ_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOD);        \
    UNUSED(tmpreg);                                                 \
} while (0U)

#define __HAL_RCC_ADC1_CLK_ENABLE()    do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_ADC1);                   \
    tmpreg = READ_BIT(RCC->APB2PCENR, RCC_APB2Periph_ADC1);         \
    UNUSED(tmpreg);                                                 \
} while (0U)

#define __HAL_RCC_TIM1_CLK_ENABLE()    do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_TIM1);                   \
    tmpreg = READ_BIT(RCC->APB2PCENR, RCC_APB2Periph_TIM1);         \
    UNUSED(tmpreg);                                                 \
} while (0U)

#define __HAL_RCC_SPI1_CLK_ENABLE()    do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_SPI1);                   \
    tmpreg = READ_BIT(RCC->APB2PCENR, RCC_APB2Periph_SPI1);         \
    UNUSED(tmpreg);                                                 \
} while (0U)

#define __HAL_RCC_USART1_CLK_ENABLE()  do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_USART1);                 \
    tmpreg = READ_BIT(RCC->APB2PCENR, RCC_APB2Periph_USART1);       \
    UNUSED(tmpreg);                                                 \
} while (0U)

/* ---------- APB1: ENABLE ---------- */
#define __HAL_RCC_TIM2_CLK_ENABLE()    do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB1PCENR, RCC_APB1Periph_TIM2);                   \
    tmpreg = READ_BIT(RCC->APB1PCENR, RCC_APB1Periph_TIM2);         \
    UNUSED(tmpreg);                                                 \
} while (0U)

#define __HAL_RCC_WWDG_CLK_ENABLE()    do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB1PCENR, RCC_APB1Periph_WWDG);                   \
    tmpreg = READ_BIT(RCC->APB1PCENR, RCC_APB1Periph_WWDG);         \
    UNUSED(tmpreg);                                                 \
} while (0U)

#define __HAL_RCC_I2C1_CLK_ENABLE()    do {                         \
    __IO uint32_t tmpreg = 0x00U;                                   \
    SET_BIT(RCC->APB1PCENR, RCC_APB1Periph_I2C1);                   \
    tmpreg = READ_BIT(RCC->APB1PCENR, RCC_APB1Periph_I2C1);         \
    UNUSED(tmpreg);                                                 \
} while (0U)

/* ---------- APB2: DISABLE ---------- */
#define __HAL_RCC_AFIO_CLK_DISABLE()   do {                         \
    CLEAR_BIT(RCC->APB2PCENR, RCC_APB2Periph_AFIO);                 \
} while (0U)

#define __HAL_RCC_GPIOA_CLK_DISABLE()  do {                         \
    CLEAR_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOA);                \
} while (0U)

#define __HAL_RCC_GPIOC_CLK_DISABLE()  do {                         \
    CLEAR_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOC);                \
} while (0U)

#define __HAL_RCC_GPIOD_CLK_DISABLE()  do {                         \
    CLEAR_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOD);                \
} while (0U)

#define __HAL_RCC_ADC1_CLK_DISABLE()   do {                         \
    CLEAR_BIT(RCC->APB2PCENR, RCC_APB2Periph_ADC1);                 \
} while (0U)

#define __HAL_RCC_TIM1_CLK_DISABLE()   do {                         \
    CLEAR_BIT(RCC->APB2PCENR, RCC_APB2Periph_TIM1);                 \
} while (0U)

#define __HAL_RCC_SPI1_CLK_DISABLE()   do {                         \
    CLEAR_BIT(RCC->APB2PCENR, RCC_APB2Periph_SPI1);                 \
} while (0U)

#define __HAL_RCC_USART1_CLK_DISABLE() do {                         \
    CLEAR_BIT(RCC->APB2PCENR, RCC_APB2Periph_USART1);               \
} while (0U)

/* ---------- APB1: DISABLE ---------- */
#define __HAL_RCC_TIM2_CLK_DISABLE()   do {                         \
    CLEAR_BIT(RCC->APB1PCENR, RCC_APB1Periph_TIM2);                 \
} while (0U)

#define __HAL_RCC_WWDG_CLK_DISABLE()   do {                         \
    CLEAR_BIT(RCC->APB1PCENR, RCC_APB1Periph_WWDG);                 \
} while (0U)

#define __HAL_RCC_I2C1_CLK_DISABLE()   do {                         \
    CLEAR_BIT(RCC->APB1PCENR, RCC_APB1Periph_I2C1);                 \
} while (0U)
/* Exported functions --------------------------------------------------------*/
void HAL_RCC_GetClocksFreq(RCC_ClocksTypeDef *RCC_Clocks);
void HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource);
void HAL_RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue);
/* Private macros ------------------------------------------------------------*/
/* RCC check MCO instance */
#define IS_RCC_MCO(MCO) ((MCO) == RCC_MCO1)

/* RCC check MCO source */
#define IS_RCC_MCOSOURCE(SOURCE) ( \
    ((SOURCE) == RCC_MCO_NoClock)    || \
    ((SOURCE) == RCC_MCO_SYSCLK)     || \
    ((SOURCE) == RCC_MCO_HSI)     || \
    ((SOURCE) == RCC_MCO_HSE)     || \
    ((SOURCE) == RCC_MCO_PLLCLK) )

#ifdef __cplusplus
}
#endif

#endif /* __CH32V00X_HAL_RCC_H */
