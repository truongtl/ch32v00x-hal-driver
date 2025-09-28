/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v00x_hal_nvic.c
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : RCC HAL module driver.
 *                      This file provides hardware abstract interface to manage all NVIC functions
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <ch32v00x_hal.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t NVIC_Priority_Group = 0;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
/*********************************************************************
 * @fn      HAL_NVIC_PriorityGroupConfig
 *
 * @brief   Configures the priority grouping - pre-emption priority and subpriority.
 *
 * @param   NVIC_PriorityGroup - specifies the priority grouping bits length.
 *            NVIC_PriorityGroup_0 - 0 bits for pre-emption priority
 *                                   2 bits for subpriority
 *            NVIC_PriorityGroup_1 - 1 bits for pre-emption priority
 *                                   1 bits for subpriority
 *
 * @return  none
 */
void HAL_NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
    HAL_PARAM_CHECK(IS_NVIC_PRI_GROUP(NVIC_PriorityGroup));
    NVIC_Priority_Group = NVIC_PriorityGroup;
}

/*********************************************************************
 * @fn      HAL_NVIC_Init
 *
 * @brief   Initializes the NVIC peripheral according to the specified parameters in
 *        the NVIC_InitStruct.
 *
 * @param   NVIC_InitStruct - pointer to a NVIC_InitTypeDef structure that contains the
 *        configuration information for the specified NVIC peripheral.
 *              interrupt nesting enable(CSR-0x804 bit1 = 1)
 *            NVIC_IRQChannelPreemptionPriority - range from 0 to 1.
 *            NVIC_IRQChannelSubPriority - range from 0 to 1.
 *
 *              interrupt nesting disable(CSR-0x804 bit1 = 0)
 *            NVIC_IRQChannelPreemptionPriority - range is 0.
 *            NVIC_IRQChannelSubPriority - range from 0 to 3.
 *
 * @return  none
 */
void HAL_NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct)
{
    HAL_PARAM_CHECK(IS_NVIC_IRQ_CHANNEL(NVIC_InitStruct->NVIC_IRQChannel));
    HAL_PARAM_CHECK(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));
    HAL_PARAM_CHECK(IS_NVIC_SUB_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelSubPriority));
    HAL_PARAM_CHECK(IS_NVIC_CHANNEL_CMD(NVIC_InitStruct->NVIC_IRQChannelCmd));

#if (INTSYSCR_INEST == INTSYSCR_INEST_NoEN)
    if (NVIC_Priority_Group == NVIC_PriorityGroup_0)
    {
        NVIC_SetPriority(NVIC_InitStruct->NVIC_IRQChannel, NVIC_InitStruct->NVIC_IRQChannelSubPriority << 6);
    }
#else
    if (NVIC_Priority_Group == NVIC_PriorityGroup_1)
    {
        if (NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority == 1)
        {
            NVIC_SetPriority(NVIC_InitStruct->NVIC_IRQChannel, (1 << 7) | (NVIC_InitStruct->NVIC_IRQChannelSubPriority << 6));
        }
        else if (NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority == 0)
        {
            NVIC_SetPriority(NVIC_InitStruct->NVIC_IRQChannel, (0 << 7) | (NVIC_InitStruct->NVIC_IRQChannelSubPriority << 6));
        }
    }
#endif

    if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
    {
        NVIC_EnableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    }
    else
    {
        NVIC_DisableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    }
}

/**
  * @brief  Enables a device specific interrupt in the NVIC interrupt controller.
  * @note   To configure interrupts priority correctly, the HAL_NVIC_PriorityGroupConfig()
  *         function should be called before.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  * @retval None
  */
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    HAL_PARAM_CHECK(IS_NVIC_IRQ_CHANNEL(IRQn));

    /* Enable interrupt */
    NVIC_EnableIRQ(IRQn);
}

/**
  * @brief  Disables a device specific interrupt in the NVIC interrupt controller.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  * @retval None
  */
void HAL_NVIC_DisableIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    HAL_PARAM_CHECK(IS_NVIC_IRQ_CHANNEL(IRQn));

    /* Disable interrupt */
    NVIC_DisableIRQ(IRQn);
}

/**
  * @brief  Initiates a system reset request to reset the MCU.
  * @retval None
  */
void HAL_NVIC_SystemReset(void)
{
    /* System Reset */
    NVIC_SystemReset();
}