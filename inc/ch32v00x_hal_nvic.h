/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_nvic.h
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : Header file of RCC HAL module
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/   
#ifndef __CH32V00X_HAL_NVIC_H
#define __CH32V00X_HAL_NVIC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* NVIC Init Structure definition
 *   interrupt nesting enable(CSR-0x804 bit1 = 1)
 *     NVIC_IRQChannelPreemptionPriority - range from 0 to 1.
 *     NVIC_IRQChannelSubPriority - range from 0 to 1.
 *
 *   interrupt nesting disable(CSR-0x804 bit1 = 0)
 *     NVIC_IRQChannelPreemptionPriority - range is 0.
 *     NVIC_IRQChannelSubPriority - range from 0 to 3.
 *
 */
typedef struct
{
    uint8_t NVIC_IRQChannel;
    uint8_t NVIC_IRQChannelPreemptionPriority;
    uint8_t NVIC_IRQChannelSubPriority;
    FunctionalState NVIC_IRQChannelCmd;
} NVIC_InitTypeDef;
/* Exported constants --------------------------------------------------------*/
/* CSR_INTSYSCR_INEST_definition */
#define INTSYSCR_INEST_NoEN   0x00   /* interrupt nesting disable(CSR-0x804 bit1 = 0) */
#define INTSYSCR_INEST_EN     0x01   /* interrupt nesting enable(CSR-0x804 bit1 = 1) */

/* Check the configuration of CSR(0x804) in the startup file(.S)
*   interrupt nesting enable(CSR-0x804 bit1 = 1)
*     priority - bit[7] - Preemption Priority
*                bit[6] - Sub priority
*                bit[5:0] - Reserve
*   interrupt nesting disable(CSR-0x804 bit1 = 0)
*     priority - bit[7:6] - Sub priority
*                bit[5:0] - Reserve
*/

#ifndef INTSYSCR_INEST
#define INTSYSCR_INEST   INTSYSCR_INEST_EN
#endif

/* Preemption_Priority_Group */
#define NVIC_PriorityGroup_0           ((uint32_t)0x00) /* interrupt nesting enable(CSR-0x804 bit1 = 1) */
#define NVIC_PriorityGroup_1           ((uint32_t)0x01) /* interrupt nesting disable(CSR-0x804 bit1 = 0) */
/* Exported functions --------------------------------------------------------*/
void HAL_NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);
void HAL_NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct);
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
void HAL_NVIC_DisableIRQ(IRQn_Type IRQn);
void HAL_NVIC_SystemReset(void);
/* Private macros ------------------------------------------------------------*/
/* NVIC check priority group */
#define IS_NVIC_PRI_GROUP(PRI_GROUP) ( \
    ((PRI_GROUP) == NVIC_PriorityGroup_0)    || \
    ((PRI_GROUP) == NVIC_PriorityGroup_1) )

/* NVIC check valid IRQ channel */
#define IS_NVIC_IRQ_CHANNEL(IRQ)                ((IRQ) >= (IRQn_Type)0x00U)

/* NVIC check pre-empt priority */
#define IS_NVIC_PREEMPTION_PRIORITY(PRIO)   \
    ( ((INTSYSCR_INEST == INTSYSCR_INEST_NoEN) ? ((PRIO) == 0U) : ((PRIO) <= 1U)) )

/* NVIC check sub priority */
#define IS_NVIC_SUB_PRIORITY(PRIO)          \
    ( ((INTSYSCR_INEST == INTSYSCR_INEST_NoEN) ? ((PRIO) <= 3U) : ((PRIO) <= 1U)) )

/* NVIC check channel command */
#define IS_NVIC_CHANNEL_CMD(CMD) ( \
    ((CMD) == ENABLE)    || \
    ((CMD) == DISABLE) )

#ifdef __cplusplus
}
#endif

#endif /* __CH32V00X_HAL_NVIC_H */
