/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_rcc.c
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : RCC HAL module driver.
 *                      This file provides hardware abstract interface to manage all RCC functions
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <ch32v00x_hal.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MCO_GPIO_PORT   GPIOC
#define MCO_GPIO_PIN    GPIO_Pin_4

/* RCC registers bit address in the alias region */
#define RCC_OFFSET                 (RCC_BASE - PERIPH_BASE)

/* BDCTLR Register */
#define BDCTLR_OFFSET              (RCC_OFFSET + 0x20)

/* RCC registers bit mask */

/* CTLR register bit mask */
#define CTLR_HSEBYP_Reset          ((uint32_t)0xFFFBFFFF)
#define CTLR_HSEBYP_Set            ((uint32_t)0x00040000)
#define CTLR_HSEON_Reset           ((uint32_t)0xFFFEFFFF)
#define CTLR_HSEON_Set             ((uint32_t)0x00010000)
#define CTLR_HSITRIM_Mask          ((uint32_t)0xFFFFFF07)

#define CFGR0_PLL_Mask             ((uint32_t)0xFFFEFFFF)
#define CFGR0_PLLMull_Mask         ((uint32_t)0x003C0000)
#define CFGR0_PLLSRC_Mask          ((uint32_t)0x00010000)
#define CFGR0_PLLXTPRE_Mask        ((uint32_t)0x00020000)
#define CFGR0_SWS_Mask             ((uint32_t)0x0000000C)
#define CFGR0_SW_Mask              ((uint32_t)0xFFFFFFFC)
#define CFGR0_HPRE_Reset_Mask      ((uint32_t)0xFFFFFF0F)
#define CFGR0_HPRE_Set_Mask        ((uint32_t)0x000000F0)
#define CFGR0_PPRE1_Reset_Mask     ((uint32_t)0xFFFFF8FF)
#define CFGR0_PPRE1_Set_Mask       ((uint32_t)0x00000700)
#define CFGR0_PPRE2_Reset_Mask     ((uint32_t)0xFFFFC7FF)
#define CFGR0_PPRE2_Set_Mask       ((uint32_t)0x00003800)
#define CFGR0_ADCPRE_Reset_Mask    ((uint32_t)0xFFFF07FF)
#define CFGR0_ADCPRE_Set_Mask      ((uint32_t)0x0000F800)

/* RSTSCKR register bit mask */
#define RSTSCKR_RMVF_Set           ((uint32_t)0x01000000)

/* RCC Flag Mask */
#define FLAG_Mask                  ((uint8_t)0x1F)

/* INTR register byte 2 (Bits[15:8]) base address */
#define INTR_BYTE2_ADDRESS         ((uint32_t)0x40021009)

/* INTR register byte 3 (Bits[23:16]) base address */
#define INTR_BYTE3_ADDRESS         ((uint32_t)0x4002100A)

/* CFGR0 register byte 4 (Bits[31:24]) base address */
#define CFGR0_BYTE4_ADDRESS        ((uint32_t)0x40021007)

/* BDCTLR register base address */
#define BDCTLR_ADDRESS             (PERIPH_BASE + BDCTLR_OFFSET)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __I uint8_t APBAHBPrescTable[16] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
static __I uint8_t ADCPrescTable[20] = {2, 4, 6, 8, 4, 8, 12, 16, 8, 16, 24, 32, 16, 32, 48, 64, 32, 64, 96, 128};
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  The result of this function could be not correct when using
  *        fractional value for HSE crystal.
  * @param   RCC_Clocks pointer to a RCC_ClocksTypeDef structure which will hold
  *        the clocks frequencies.
  * @retval none
  */
void HAL_RCC_GetClocksFreq(RCC_ClocksTypeDef *RCC_Clocks)
{
    uint32_t cfgr    = READ_REG(RCC->CFGR0);
    uint32_t sysclk  = HSI_VALUE;
    uint32_t hclk, pclk;
    uint32_t sws     = (cfgr & CFGR0_SWS_Mask);                /* 0x00=HSI, 0x04=HSE, 0x08=PLL */
    uint32_t hpre_cd = (cfgr & CFGR0_HPRE_Set_Mask) >> 4;      /* 0..15 */
    uint32_t adc_cd  = (cfgr & CFGR0_ADCPRE_Set_Mask) >> 11;   /* 0..19 (5-bit) */
    uint32_t presc;

    /* ---- SYSCLK source ---- */
    switch (sws)
    {
        case 0x00: /* HSI */
            sysclk = HSI_VALUE;
            break;
        case 0x04: /* HSE */
            sysclk = HSE_VALUE;
            break;
        case 0x08: /* PLL = (PLLSRC ? HSE : HSI) * 2 */
        {
            uint32_t pllsrc = (cfgr & CFGR0_PLLSRC_Mask);
            sysclk = (pllsrc ? HSE_VALUE : HSI_VALUE) * 2U;
            break;
        }
        default:
            sysclk = HSI_VALUE;
            break;
    }

    /* ---- HCLK = SYSCLK / AHB prescaler ---- */
    presc = APBAHBPrescTable[hpre_cd & 0x0F];
    if (hpre_cd < 8U)
        hclk = sysclk / presc;
    else
        hclk = sysclk >> presc;

    pclk = hclk;

    /* ADCPRE 5-bit (mask 0xF800 >> 11) */
    if (adc_cd >= 20U) adc_cd = 0U;
    RCC_Clocks->ADCCLK_Frequency = pclk / ADCPrescTable[adc_cd];

    RCC_Clocks->SYSCLK_Frequency = sysclk;
    RCC_Clocks->HCLK_Frequency   = hclk;
    RCC_Clocks->PCLK1_Frequency  = pclk;
    RCC_Clocks->PCLK2_Frequency  = pclk;
}

/**
  * @brief  Selects the clock source to output on MCO pin.
  * @param  RCC_MCOx specifies the MCO instance.
  * @param  RCC_MCOSource specifies the clock source to output.
  *          This parameter can be one of the values below:
  *            @arg RCC_MCO_NoClock: No clock selected.
  *            @arg RCC_MCO_SYSCLK: System clock selected.
  *            @arg RCC_MCO_HSI: HSI oscillator clock selected.
  *            @arg RCC_MCO_HSE: HSE oscillator clock selected.
  *            @arg RCC_MCO_PLLCLK: PLL clock selected.
  * @retval none
  */
void HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Validate input */
    HAL_PARAM_CHECK(IS_RCC_MCO(RCC_MCOx));
    HAL_PARAM_CHECK(IS_RCC_MCOSOURCE(RCC_MCOSource));

  if (RCC_MCOx == RCC_MCO1)
  {
        if (RCC_MCOSource != RCC_MCO_NoClock)
        {
            /* Enable clocks for GPIOC, AFIO */
            __HAL_RCC_GPIOC_CLK_ENABLE();
            __HAL_RCC_AFIO_CLK_ENABLE();

            /* Configure PC4 as MCO output: Alternate Function Push-Pull, High speed */
            GPIO_InitStruct.GPIO_Pin   = MCO_GPIO_PIN;
            GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
            GPIO_InitStruct.GPIO_Speed = GPIO_Speed_30MHz;
            HAL_GPIO_Init(MCO_GPIO_PORT, &GPIO_InitStruct);

            /* Select MCO source: write MCO[2:0] in RCC->CFGR0 */
            MODIFY_REG(RCC->CFGR0, RCC_CFGR0_MCO, RCC_MCOSource);

            /* Read-back to flush posted write */
            (void)RCC->CFGR0;
        }
    }
}

/**
  * @brief  Adjusts the Internal High Speed oscillator (HSI) calibration value.
  * @param  HSICalibrationValue specifies the calibration trimming value.
  *                    This parameter must be a number between 0 and 0x1F.
  * @retval none
  */
void HAL_RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue)
{
    uint32_t tmpreg = 0;

    tmpreg = READ_REG(RCC->CTLR);
    tmpreg &= CTLR_HSITRIM_Mask;
    tmpreg |= (uint32_t)HSICalibrationValue << 3;
    WRITE_REG(RCC->CTLR, tmpreg);
}
