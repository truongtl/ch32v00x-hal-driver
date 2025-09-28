/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_uart.c
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : UART HAL module driver.
 *                      This file provides hardware abstract interface to manage all UART functions
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <ch32v00x_hal.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USART_CTLR1_CLEAR_Mask          ((uint16_t)0xE9F3)      /* USART CTLR1 Mask */
#define USART_CTLR3_CLEAR_Mask          ((uint16_t)0xFCFF)      /* USART CTLR3 Mask */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static HAL_StatusTypeDef UART_WaitOnFlagUntilTimeout(UART_HandleTypeDef *huart, uint32_t Flag, FlagStatus Status,
                                                     uint32_t Tickstart, uint32_t Timeout);
static HAL_StatusTypeDef UART_Start_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
static void UART_EndRxTransfer(UART_HandleTypeDef *huart);
static HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart);
static HAL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef *huart);
static HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart);
/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the UART mode according to the specified parameters in
  *         the UART_InitTypeDef and create the associated handle.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart)
{
    /* Check the UART handle allocation */
    if (huart == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameters */
    HAL_PARAM_CHECK(IS_UART_INSTANCE(huart->Instance));
    HAL_PARAM_CHECK(IS_UART_WORD_LENGTH(huart->Init.UART_WordLength));
    HAL_PARAM_CHECK(IS_UART_STOPBITS(huart->Init.UART_StopBits));
    HAL_PARAM_CHECK(IS_UART_PARITY(huart->Init.UART_Parity));
    HAL_PARAM_CHECK(IS_UART_HWCONTROL(huart->Init.UART_HardwareFlowControl));
    HAL_PARAM_CHECK(IS_UART_MODE(huart->Init.UART_Mode));

    uint32_t          tmpreg = 0x00, apbclock = 0x00;
    uint32_t          integerdivider = 0x00;
    uint32_t          fractionaldivider = 0x00;
    uint32_t          usartxbase = 0;
    RCC_ClocksTypeDef RCC_ClocksStatus;

    if (huart->gState == HAL_UART_STATE_RESET)
    {
        /* Init the low level hardware : GPIO, CLOCK */
        HAL_UART_MspInit(huart);
    }

    huart->gState = HAL_UART_STATE_BUSY;

    /* Disable the peripheral */
    __HAL_UART_DISABLE(huart);

    if (huart->Init.UART_HardwareFlowControl != UART_HWCONTROL_NONE)
    {
    }

    usartxbase = (uint32_t)huart->Instance;
    tmpreg = READ_REG(huart->Instance->CTLR2);
    tmpreg &= ~USART_CTLR2_STOP;
    tmpreg |= (uint32_t)huart->Init.UART_StopBits;

    WRITE_REG(huart->Instance->CTLR2, (uint16_t)tmpreg);
    tmpreg = READ_REG(huart->Instance->CTLR1);
    tmpreg &= USART_CTLR1_CLEAR_Mask;
    tmpreg |= (uint32_t)huart->Init.UART_WordLength | huart->Init.UART_Parity |
              huart->Init.UART_Mode;
    WRITE_REG(huart->Instance->CTLR1, (uint16_t)tmpreg);

    tmpreg = READ_REG(huart->Instance->CTLR3);
    tmpreg &= USART_CTLR3_CLEAR_Mask;
    tmpreg |= huart->Init.UART_HardwareFlowControl;
    WRITE_REG(huart->Instance->CTLR3, (uint16_t)tmpreg);

    HAL_RCC_GetClocksFreq(&RCC_ClocksStatus);

    if (usartxbase == USART1_BASE)
    {
        apbclock = RCC_ClocksStatus.PCLK2_Frequency;
    }
    else
    {
        apbclock = RCC_ClocksStatus.PCLK1_Frequency;
    }

    integerdivider = ((25 * apbclock) / (4 * (huart->Init.UART_BaudRate)));
    tmpreg = (integerdivider / 100) << 4;
    fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);

    WRITE_REG(huart->Instance->BRR, (uint16_t)tmpreg);

    /* Enable the peripheral */
    __HAL_UART_ENABLE(huart);

    /* Initialize the UART state */
    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->gState = HAL_UART_STATE_READY;
    huart->RxState = HAL_UART_STATE_READY;

    return HAL_OK;
}

/**
  * @brief  UART MSP Init.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed,
            the HAL_UART_MspInit could be implemented in the user file
    */
}

/**
  * @brief  Sends an amount of data in blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the sent data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 provided through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    const uint8_t  *pdata8bits;
    const uint16_t *pdata16bits;
    uint32_t tickstart = 0U;

    /* Check that a Tx process is not already ongoing */
    if (huart->gState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return  HAL_ERROR;
        }

        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->gState = HAL_UART_STATE_BUSY_TX;

        /* Init tickstart for timeout management */
        tickstart = HAL_GetTick();

        huart->TxXferSize = Size;
        huart->TxXferCount = Size;

        /* In case of 9bits/No Parity transfer, pData needs to be handled as a uint16_t pointer */
        if ((huart->Init.UART_WordLength == UART_WORDLENGTH_9B) && (huart->Init.UART_Parity == UART_PARITY_NONE))
        {
            pdata8bits  = NULL;
            pdata16bits = (const uint16_t *) pData;
        }
        else
        {
            pdata8bits  = pData;
            pdata16bits = NULL;
        }

        while (huart->TxXferCount > 0U)
        {
            if (UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_TXE, RESET, tickstart, Timeout) != HAL_OK)
            {
                huart->gState = HAL_UART_STATE_READY;

                return HAL_TIMEOUT;
            }

            if (pdata8bits == NULL)
            {
                WRITE_REG(huart->Instance->DATAR, (uint16_t)(*pdata16bits & 0x01FFU));
                pdata16bits++;
            }
            else
            {
                WRITE_REG(huart->Instance->DATAR, (uint8_t)(*pdata8bits & 0xFFU));
                pdata8bits++;
            }

            huart->TxXferCount--;
        }

        if (UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_TC, RESET, tickstart, Timeout) != HAL_OK)
        {
            huart->gState = HAL_UART_STATE_READY;

            return HAL_TIMEOUT;
        }

        /* At end of Tx process, restore huart->gState to Ready */
        huart->gState = HAL_UART_STATE_READY;

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Receives an amount of data in blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the received data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 available through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be received.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint8_t  *pdata8bits;
    uint16_t *pdata16bits;
    uint32_t tickstart = 0U;

    /* Check that a Rx process is not already ongoing */
    if (huart->RxState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }

        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->RxState = HAL_UART_STATE_BUSY_RX;

        /* Init tickstart for timeout management */
        tickstart = HAL_GetTick();

        huart->RxXferSize = Size;
        huart->RxXferCount = Size;

        /* In case of 9bits/No Parity transfer, pRxData needs to be handled as a uint16_t pointer */
        if ((huart->Init.UART_WordLength == UART_WORDLENGTH_9B) && (huart->Init.UART_Parity == UART_PARITY_NONE))
        {
            pdata8bits  = NULL;
            pdata16bits = (uint16_t *) pData;
        }
        else
        {
            pdata8bits  = pData;
            pdata16bits = NULL;
        }

        /* Check the remain data to be received */
        while (huart->RxXferCount > 0U)
        {
            if (UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_RXNE, RESET, tickstart, Timeout) != HAL_OK)
            {
                huart->RxState = HAL_UART_STATE_READY;
                return HAL_TIMEOUT;
            }

            if (pdata8bits == NULL)
            {
                *pdata16bits = (uint16_t)(READ_REG(huart->Instance->DATAR) & 0x01FF);
                pdata16bits++;
            }
            else
            {
                if ((huart->Init.UART_WordLength == UART_WORDLENGTH_9B) ||
                    ((huart->Init.UART_WordLength == UART_WORDLENGTH_8B) && (huart->Init.UART_Parity == UART_PARITY_NONE)))
                {
                    *pdata8bits = (uint8_t)(READ_REG(huart->Instance->DATAR) & (uint8_t)0x00FF);
                }
                else
                {
                    *pdata8bits = (uint8_t)(READ_REG(huart->Instance->DATAR) & (uint8_t)0x007F);
                }
                pdata8bits++;
            }
            huart->RxXferCount--;
        }

        /* At end of Rx process, restore huart->RxState to Ready */
        huart->RxState = HAL_UART_STATE_READY;

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Sends an amount of data in non blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the sent data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 provided through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size)
{
    /* Check that a Tx process is not already ongoing */
    if (huart->gState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
        return HAL_ERROR;
        }

        huart->pTxBuffPtr = pData;
        huart->TxXferSize = Size;
        huart->TxXferCount = Size;

        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->gState = HAL_UART_STATE_BUSY_TX;

        /* Enable the UART Transmit data register empty Interrupt */
        __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Receives an amount of data in non blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the received data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 available through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be received.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    /* Check that a Rx process is not already ongoing */
    if (huart->RxState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return HAL_ERROR;
        }

        return (UART_Start_Receive_IT(huart, pData, Size));
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  This function handles UART interrupt request.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    uint32_t isrflags   = READ_REG(huart->Instance->STATR);
    uint32_t cr1its     = READ_REG(huart->Instance->CTLR1);
    uint32_t cr3its     = READ_REG(huart->Instance->CTLR3);
    uint32_t errorflags = 0x00U;
    uint32_t dmarequest = 0x00U;

    /* If no error occurs */
    errorflags = (isrflags & (uint32_t)(USART_STATR_PE | USART_STATR_FE | USART_STATR_ORE | USART_STATR_NE));
    if (errorflags == RESET)
    {
        /* UART in mode Receiver -------------------------------------------------*/
        if (((isrflags & USART_STATR_RXNE) != RESET) && ((cr1its & USART_CTLR1_RXNEIE) != RESET))
        {
            UART_Receive_IT(huart);
            return;
        }
    }

    /* If some errors occur */
    if ((errorflags != RESET) && (((cr3its & USART_CTLR3_EIE) != RESET)
                                  || ((cr1its & (USART_CTLR1_RXNEIE | USART_CTLR1_PEIE)) != RESET)))
    {
        /* UART parity error interrupt occurred ----------------------------------*/
        if (((isrflags & USART_STATR_PE) != RESET) && ((cr1its & USART_CTLR1_PEIE) != RESET))
        {
            huart->ErrorCode |= HAL_UART_ERROR_PE;
        }

        /* UART noise error interrupt occurred -----------------------------------*/
        if (((isrflags & USART_STATR_NE) != RESET) && ((cr3its & USART_CTLR3_EIE) != RESET))
        {
            huart->ErrorCode |= HAL_UART_ERROR_NE;
        }

        /* UART frame error interrupt occurred -----------------------------------*/
        if (((isrflags & USART_STATR_FE) != RESET) && ((cr3its & USART_CTLR3_EIE) != RESET))
        {
            huart->ErrorCode |= HAL_UART_ERROR_FE;
        }

        /* UART Over-Run interrupt occurred --------------------------------------*/
        if (((isrflags & USART_STATR_ORE) != RESET) && (((cr1its & USART_CTLR1_RXNEIE) != RESET)
                                                     || ((cr3its & USART_CTLR3_EIE) != RESET)))
        {
            huart->ErrorCode |= HAL_UART_ERROR_ORE;
        }

        /* Call UART Error Call back function if need be --------------------------*/
        if (huart->ErrorCode != HAL_UART_ERROR_NONE)
        {
            /* UART in mode Receiver -----------------------------------------------*/
            if (((isrflags & USART_STATR_RXNE) != RESET) && ((cr1its & USART_CTLR1_RXNEIE) != RESET))
            {
                UART_Receive_IT(huart);
            }

            if ((huart->ErrorCode & HAL_UART_ERROR_ORE) != RESET)
            {
                /* Blocking error : transfer is aborted
                   Set the UART state ready to be able to start again the process,
                   Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
                UART_EndRxTransfer(huart);
                HAL_UART_ErrorCallback(huart);
            }
            else
            {
                HAL_UART_ErrorCallback(huart);
                huart->ErrorCode = HAL_UART_ERROR_NONE;
            }
        }
        return;
    } /* End if some error occurs */

    /* UART in mode Transmitter ------------------------------------------------*/
    if (((isrflags & USART_STATR_TXE) != RESET) && ((cr1its & USART_CTLR1_TXEIE) != RESET))
    {
        UART_Transmit_IT(huart);
        return;
    }

    /* UART in mode Transmitter end --------------------------------------------*/
    if (((isrflags & USART_STATR_TC) != RESET) && ((cr1its & USART_CTLR1_TCIE) != RESET))
    {
        UART_EndTransmit_IT(huart);
        return;
    }
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed,
            the HAL_UART_TxCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed,
            the HAL_UART_RxCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  UART error callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__weak void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed,
            the HAL_UART_ErrorCallback could be implemented in the user file
    */
}

/* Privated functions ---------------------------------------------------------*/
/**
  * @brief  This function handles UART Communication Timeout. It waits
  *         until a flag is no longer in the specified status.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @param  Flag specifies the UART flag to check.
  * @param  Status The actual Flag status (SET or RESET).
  * @param  Tickstart Tick start value
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_WaitOnFlagUntilTimeout(UART_HandleTypeDef *huart, uint32_t Flag, FlagStatus Status,
                                                     uint32_t Tickstart, uint32_t Timeout)
{
    /* Wait until flag is set */
    while ((__HAL_UART_GET_FLAG(huart, Flag) ? SET : RESET) == Status)
    {
        /* Check for the Timeout */
        if ((Timeout != HAL_MAX_DELAY) && HAL_TickExpired(Tickstart, Timeout) || (Timeout == 0U))
        {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

/**
  * @brief  Start Receive operation in interrupt mode.
  * @note   This function could be called by all HAL UART API providing reception in Interrupt mode.
  * @note   When calling this function, parameters validity is considered as already checked,
  *         i.e. Rx State, buffer address, ...
  *         UART Handle is assumed as Locked.
  * @param  huart UART handle.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be received.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_Start_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    huart->pRxBuffPtr = pData;
    huart->RxXferSize = Size;
    huart->RxXferCount = Size;

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->RxState = HAL_UART_STATE_BUSY_RX;

    if (huart->Init.UART_Parity != UART_PARITY_NONE)
    {
        /* Enable the UART Parity Error Interrupt */
        __HAL_UART_ENABLE_IT(huart, UART_IT_PE);
    }

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

    /* Enable the UART Data Register not empty Interrupt */
    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

    return HAL_OK;
}

/**
  * @brief  End ongoing Rx transfer on UART peripheral (following error detection or Reception completion).
  * @param  huart UART handle.
  * @retval None
  */
static void UART_EndRxTransfer(UART_HandleTypeDef *huart)
{
    /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
    ATOMIC32_CLEAR_BIT(huart->Instance->CTLR1, (USART_CTLR1_RXNEIE | USART_CTLR1_PEIE));
    ATOMIC32_CLEAR_BIT(huart->Instance->CTLR3, USART_CTLR3_EIE);

    /* At end of Rx process, restore huart->RxState to Ready */
    huart->RxState = HAL_UART_STATE_READY;
}

/**
  * @brief  Sends an amount of data in non blocking mode.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart)
{
    const uint16_t *tmp;

    /* Check that a Tx process is ongoing */
    if (huart->gState == HAL_UART_STATE_BUSY_TX)
    {
        if ((huart->Init.UART_WordLength == UART_WORDLENGTH_9B) && (huart->Init.UART_Parity == UART_PARITY_NONE))
        {
            tmp = (const uint16_t *) huart->pTxBuffPtr;
            WRITE_REG(huart->Instance->DATAR, (uint16_t)(*tmp & (uint16_t)0x01FF));
            huart->pTxBuffPtr += 2U;
        }
        else
        {
            WRITE_REG(huart->Instance->DATAR, (uint8_t)(*huart->pTxBuffPtr++ & (uint8_t)0x00FF));
        }

        if (--huart->TxXferCount == 0U)
        {
            /* Disable the UART Transmit Data Register Empty Interrupt */
            __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

            /* Enable the UART Transmit Complete Interrupt */
            __HAL_UART_ENABLE_IT(huart, UART_IT_TC);
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Wraps up transmission in non blocking mode.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef *huart)
{
    /* Disable the UART Transmit Complete Interrupt */
    __HAL_UART_DISABLE_IT(huart, UART_IT_TC);

    /* Tx process is ended, restore huart->gState to Ready */
    huart->gState = HAL_UART_STATE_READY;

    /*Call legacy weak Tx complete callback*/
    HAL_UART_TxCpltCallback(huart);

    return HAL_OK;
}

/**
  * @brief  Wraps up transmission in non blocking mode.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart)
{
    uint8_t  *pdata8bits = NULL;
    uint16_t *pdata16bits = NULL;

    /* Check that a Rx process is ongoing */
    if (huart->RxState == HAL_UART_STATE_BUSY_RX)
    {
        if ((huart->Init.UART_WordLength == UART_WORDLENGTH_9B) && (huart->Init.UART_Parity == UART_PARITY_NONE))
        {
            /* Unused pdata8bits */
            UNUSED(pdata8bits);
            pdata16bits = (uint16_t *) huart->pRxBuffPtr;
            *pdata16bits = (uint16_t)(READ_REG(huart->Instance->DATAR) & (uint16_t)0x01FF);
            huart->pRxBuffPtr += 2U;
        }
        else
        {
            pdata8bits = (uint8_t *) huart->pRxBuffPtr;
            /* Unused pdata16bits */
            UNUSED(pdata16bits);

            if ((huart->Init.UART_WordLength == UART_WORDLENGTH_9B) ||
                ((huart->Init.UART_WordLength == UART_WORDLENGTH_8B) && (huart->Init.UART_Parity == UART_PARITY_NONE)))
            {
                *pdata8bits = (uint8_t)(READ_REG(huart->Instance->DATAR) & (uint8_t)0x00FF);
            }
            else
            {
                *pdata8bits = (uint8_t)(READ_REG(huart->Instance->DATAR) & (uint8_t)0x007F);
            }
            huart->pRxBuffPtr += 1U;
        }

        if (--huart->RxXferCount == 0U)
        {
            /* Disable the UART Data Register not empty Interrupt */
            __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);

            /* Disable the UART Parity Error Interrupt */
            __HAL_UART_DISABLE_IT(huart, UART_IT_PE);

            /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
            __HAL_UART_DISABLE_IT(huart, UART_IT_ERR);

            /* Rx process is completed, restore huart->RxState to Ready */
            huart->RxState = HAL_UART_STATE_READY;

            /*Call legacy weak Rx Event callback*/
            HAL_UART_RxCpltCallback(huart);

            return HAL_OK;
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}
