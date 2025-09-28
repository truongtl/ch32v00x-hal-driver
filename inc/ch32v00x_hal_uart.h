/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_uart.h
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : Header file of UART HAL module
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
#ifndef __CH32V00X_HAL_UART_H
#define __CH32V00X_HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* UART Init Structure definition */
typedef struct
{
    uint32_t UART_BaudRate; /* This member configures the UART communication baud rate.
                                The baud rate is computed using the following formula:
                                 - IntegerDivider = ((PCLKx) / (16 * (UART_InitStruct->UART_BaudRate)))
                                 - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */

    uint16_t UART_WordLength; /* Specifies the number of data bits transmitted or received in a frame.
                                  This parameter can be a value of @ref UART_Word_Length */

    uint16_t UART_StopBits; /* Specifies the number of stop bits transmitted.
                                This parameter can be a value of @ref UART_Stop_Bits */

    uint16_t UART_Parity; /* Specifies the parity mode.
                              This parameter can be a value of @ref UART_Parity
                              @note When parity is enabled, the computed parity is inserted
                                    at the MSB position of the transmitted data (9th bit when
                                    the word length is set to 9 data bits; 8th bit when the
                                    word length is set to 8 data bits). */

    uint16_t UART_Mode; /* Specifies wether the Receive or Transmit mode is enabled or disabled.
                            This parameter can be a value of @ref UART_Mode */

    uint16_t UART_HardwareFlowControl; /* Specifies wether the hardware flow control mode is enabled
                                           or disabled.
                                           This parameter can be a value of @ref UART_Hardware_Flow_Control */
} UART_InitTypeDef;

/* HAL UART State structures definition */
typedef enum
{
    HAL_UART_STATE_RESET             = 0x00U,    /*!< Peripheral is not yet Initialized
                                                    Value is allowed for gState and RxState */
    HAL_UART_STATE_READY             = 0x20U,    /*!< Peripheral Initialized and ready for use
                                                    Value is allowed for gState and RxState */
    HAL_UART_STATE_BUSY              = 0x24U,    /*!< an internal process is ongoing
                                                    Value is allowed for gState only */
    HAL_UART_STATE_BUSY_TX           = 0x21U,    /*!< Data Transmission process is ongoing
                                                    Value is allowed for gState only */
    HAL_UART_STATE_BUSY_RX           = 0x22U,    /*!< Data Reception process is ongoing
                                                    Value is allowed for RxState only */
    HAL_UART_STATE_BUSY_TX_RX        = 0x23U,    /*!< Data Transmission and Reception process is ongoing
                                                    Not to be used for neither gState nor RxState.
                                                    Value is result of combination (Or) between gState and RxState values */
    HAL_UART_STATE_TIMEOUT           = 0xA0U,    /*!< Timeout state
                                                    Value is allowed for gState only */
    HAL_UART_STATE_ERROR             = 0xE0U     /*!< Error
                                                    Value is allowed for gState only */
} HAL_UART_StateTypeDef;

/* UART handle Structure definition */
typedef struct __UART_HandleTypeDef
{
    USART_TypeDef                 *Instance;        /*!< USART registers base address        */

    UART_InitTypeDef             Init;             /*!< UART communication parameters      */

    const uint8_t                 *pTxBuffPtr;      /*!< Pointer to UART Tx transfer Buffer */

    uint16_t                      TxXferSize;       /*!< UART Tx Transfer size              */

    __IO uint16_t                 TxXferCount;      /*!< UART Tx Transfer Counter           */

    uint8_t                       *pRxBuffPtr;      /*!< Pointer to UART Rx transfer Buffer */

    uint16_t                      RxXferSize;       /*!< UART Rx Transfer size              */

    __IO uint16_t                 RxXferCount;      /*!< UART Rx Transfer Counter           */

    __IO HAL_UART_StateTypeDef    gState;           /*!< UART state information related to global Handle management
                                                        and also related to Tx operations.
                                                        This parameter can be a value of @ref HAL_UART_StateTypeDef */

    __IO HAL_UART_StateTypeDef    RxState;          /*!< UART state information related to Rx operations.
                                                        This parameter can be a value of @ref HAL_UART_StateTypeDef */

    __IO uint32_t                 ErrorCode;        /*!< UART Error code                    */
} UART_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/
/* UART_Word_Length */
#define UART_WORDLENGTH_8B                  ((uint16_t)0x0000)
#define UART_WORDLENGTH_9B                  ((uint16_t)0x1000)

/* UART_Stop_Bits */
#define UART_STOPBITS_1                     ((uint16_t)0x0000)
#define UART_STOPBITS_0_5                   ((uint16_t)0x1000)
#define UART_STOPBITS_2                     ((uint16_t)0x2000)
#define UART_STOPBITS_1_5                   ((uint16_t)0x3000)

/* UART_Parity */
#define UART_PARITY_NONE                    ((uint16_t)0x0000)
#define UART_PARITY_EVEN                    ((uint16_t)0x0400)
#define UART_PARITY_ODD                     ((uint16_t)0x0600)

/* UART_Mode */
#define UART_MODE_RX                        ((uint16_t)0x0004)
#define UART_MODE_TX                        ((uint16_t)0x0008)
#define UART_MODE_TX_RX                     ((uint16_t)(UART_MODE_RX | UART_MODE_TX))

/* UART_Hardware_Flow_Control */
#define UART_HWCONTROL_NONE                 ((uint16_t)0x0000)
#define UART_HWCONTROL_RTS                  ((uint16_t)0x0100)
#define UART_HWCONTROL_CTS                  ((uint16_t)0x0200)
#define UART_HWCONTROL_RTS_CTS              ((uint16_t)0x0300)

/* UART_Interrupt_definition */
#define UART_IT_PE                          ((uint16_t)0x0028)
#define UART_IT_TXE                         ((uint16_t)0x0727)
#define UART_IT_TC                          ((uint16_t)0x0626)
#define UART_IT_RXNE                        ((uint16_t)0x0525)
#define UART_IT_ORE_RX                      ((uint16_t)0x0325)
#define UART_IT_IDLE                        ((uint16_t)0x0424)
#define UART_IT_LBD                         ((uint16_t)0x0846)
#define UART_IT_CTS                         ((uint16_t)0x096A)
#define UART_IT_ERR                         ((uint16_t)0x0060)
#define UART_IT_ORE_ER                      ((uint16_t)0x0360)
#define UART_IT_NE                          ((uint16_t)0x0260)
#define UART_IT_FE                          ((uint16_t)0x0160)
#define UART_IT_ORE                         UART_IT_ORE_ER
#define UART_IT_MASK                        ((uint16_t)0x001FU)

/* UART_Flags */
#define UART_FLAG_CTS                       ((uint16_t)0x0200)
#define UART_FLAG_LBD                       ((uint16_t)0x0100)
#define UART_FLAG_TXE                       ((uint16_t)0x0080)
#define UART_FLAG_TC                        ((uint16_t)0x0040)
#define UART_FLAG_RXNE                      ((uint16_t)0x0020)
#define UART_FLAG_IDLE                      ((uint16_t)0x0010)
#define UART_FLAG_ORE                       ((uint16_t)0x0008)
#define UART_FLAG_NE                        ((uint16_t)0x0004)
#define UART_FLAG_FE                        ((uint16_t)0x0002)
#define UART_FLAG_PE                        ((uint16_t)0x0001)

/* HAL UART Error Code */
#define HAL_UART_ERROR_NONE              0x00000000U   /*!< No error            */
#define HAL_UART_ERROR_PE                0x00000001U   /*!< Parity error        */
#define HAL_UART_ERROR_NE                0x00000002U   /*!< Noise error         */
#define HAL_UART_ERROR_FE                0x00000004U   /*!< Frame error         */
#define HAL_UART_ERROR_ORE               0x00000008U   /*!< Overrun error       */
/* Exported macro ------------------------------------------------------------*/
/*
   1 = CTLR1, 2 = CTLR2, 3 = CTLR3
*/
#define __HAL_UART_IT_REGIDX(__IT__) ((uint32_t)((uint8_t)(__IT__) >> 5U))
#define __HAL_UART_IT_POS(__IT__)    ((uint32_t)((__IT__) & UART_IT_MASK))
#define __HAL_UART_IT_MASK(__IT__)   (1U << __HAL_UART_IT_POS(__IT__))

#define __HAL_UART_ENABLE(__HANDLE__)               ((__HANDLE__)->Instance->CTLR1 |=  USART_CTLR1_UE)
#define __HAL_UART_DISABLE(__HANDLE__)              ((__HANDLE__)->Instance->CTLR1 &=  ~USART_CTLR1_UE)
#define __HAL_UART_GET_FLAG(__HANDLE__, __FLAG__)   (((__HANDLE__)->Instance->STATR & (__FLAG__)) == (__FLAG__))
#define __HAL_UART_CLEAR_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->STATR = ~(__FLAG__))
#define __HAL_UART_ENABLE_IT(__HANDLE__, __IT__) \
do { \
    uint32_t __reg = __HAL_UART_IT_REGIDX(__IT__);  /* 1/2/3 => CTLRx */ \
    uint32_t __msk = __HAL_UART_IT_MASK(__IT__);    /* 1 << pos */ \
    if (__reg == 1U) { (__HANDLE__)->Instance->CTLR1 |= __msk; } \
    else if (__reg == 2U) { (__HANDLE__)->Instance->CTLR2 |= __msk; } \
    else { (__HANDLE__)->Instance->CTLR3 |= __msk; } \
} while (0)

#define __HAL_UART_DISABLE_IT(__HANDLE__, __IT__) \
do { \
    uint32_t __reg = __HAL_UART_IT_REGIDX(__IT__); \
    uint32_t __msk = __HAL_UART_IT_MASK(__IT__); \
    if (__reg == 1U) { (__HANDLE__)->Instance->CTLR1 &= ~__msk; } \
    else if (__reg == 2U) { (__HANDLE__)->Instance->CTLR2 &= ~__msk; } \
    else { (__HANDLE__)->Instance->CTLR3 &= ~__msk; } \
} while (0)

/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
void HAL_UART_MspInit(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
void HAL_UART_IRQHandler(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
/* Private macros ------------------------------------------------------------*/
/* UART check instance (only USART1 available) */
#define IS_UART_INSTANCE(INSTANCE) ((INSTANCE) == USART1)

/* UART check word length */
#define IS_UART_WORD_LENGTH(LEN)    ( \
    ((LEN) == UART_WORDLENGTH_8B) || \
    ((LEN) == UART_WORDLENGTH_9B) )

/* UART check stop bit */
#define IS_UART_STOPBITS(N)    ( \
    ((N) == UART_STOPBITS_1)   || \
    ((N) == UART_STOPBITS_0_5) || \
    ((N) == UART_STOPBITS_2)   || \
    ((N) == UART_STOPBITS_1_5) )

/* UART check parity bit */
#define IS_UART_PARITY(P)    ( \
    ((P) == UART_PARITY_NONE) || \
    ((P) == UART_PARITY_EVEN) || \
    ((P) == UART_PARITY_ODD) )

/* UART check mode TX/RX/Both */
#define IS_UART_MODE(M)    ( \
    ((M) == UART_MODE_RX) || \
    ((M) == UART_MODE_TX) || \
    ((M) == UART_MODE_TX_RX) )

/* UART check hardware flow control */
#define IS_UART_HWCONTROL(HW)    ( \
    ((HW) == UART_HWCONTROL_NONE)    || \
    ((HW) == UART_HWCONTROL_RTS)     || \
    ((HW) == UART_HWCONTROL_CTS)     || \
    ((HW) == UART_HWCONTROL_RTS_CTS) )

#ifdef __cplusplus
}
#endif

#endif /* __CH32V00X_HAL_UART_H */