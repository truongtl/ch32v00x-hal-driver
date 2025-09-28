/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_flash.h
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : Header file of FLASH HAL module
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
#ifndef __CH32V00X_HAL_FLASH_H
#define __CH32V00X_HAL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* FLASH Status */
typedef enum
{
    HAL_FLASH_ERROR_NONE         = 0x00000000U,  // không lỗi
    HAL_FLASH_ERROR_WRP          = 0x00000001U,  // write-protect error
    HAL_FLASH_ERROR_PROG         = 0x00000002U,  // programming error
    HAL_FLASH_ERROR_TIMEOUT      = 0x00000004U,  // timeout
    HAL_FLASH_ERROR_BUSY         = 0x00000008U,  // busy, in other operation
    HAL_FLASH_ERROR_ALIGN        = 0x00000010U,  // wrong alignment
    HAL_FLASH_ERROR_UNLOCK       = 0x00000020U,  // unlock failed
    HAL_FLASH_ERROR_FAST_UNLOCK  = 0x00000040U,  // fast unlock failed
    HAL_FLASH_ERROR_LOCK         = 0x00000080U   // lock failed
} HAL_FLASH_Error;

/* Type of program/erase (default or fast) */
typedef enum
{
    FLASH_PROG_METHOD_DEFAULT,
    FLASH_PROG_METHOD_FAST
} FLASH_ProgramMethod;

/* FLASH handle Structure definition */
typedef struct
{
    FLASH_ProgramMethod Flash_ProgramMethod;
    uint8_t Flash_ProgramType;
    uint8_t Flash_EraseType;
    uint32_t Flash_ProgramAdress;
    uint32_t Flash_EraseAddress;
    HAL_FLASH_Error ErrorCode;

} FLASH_HandleTypeDef;
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* FLASH Keys */
#define RDP_Key                    ((uint16_t)0x00A5)
#define FLASH_KEY1                 ((uint32_t)0x45670123)
#define FLASH_KEY2                 ((uint32_t)0xCDEF89AB)

/* FLASH BANK address */
#define FLASH_BANK1_END_ADDRESS    ((uint32_t)0x807FFFF)

/* Delay definition */
#define FLASH_ERASE_TIMEOUT               ((uint32_t)0x000B0000)
#define FLASH_PROGRAM_TIMEOUT             ((uint32_t)0x00002000)

/* Flash Program Valid Address */
#define VALID_ADDR_START             (FLASH_BASE)
#define VALID_ADDR_END               (FLASH_BASE + 0x3FFF)
#define FLASH_PAGE_SIZE              1024

/* FLASH Size */
#define Size_64B                   0x40
#define Size_1KB                   0x400

/* FLASH Type Program */
#define FLASH_TYPE_PROGRAM_HALFWORD    ((uint32_t)0x00000000)  /*!< Program a half-word (16-bit) at a specified address   */
#define FLASH_TYPE_PROGRAM_WORD        ((uint32_t)0x00000001)  /*!< Program a word (32-bit) at a specified address        */
#define FLASH_TYPE_PROGRAM_PAGE        ((uint32_t)0x00000002)  /*!< Program a page (64 bytes) in fast mode        */

/* FLASH Type Erase */
#define FLASH_TYPE_ERASE_PAGE            ((uint32_t)0x00000000)  /*!< Page erase only          */
#define FLASH_TYPE_ERASE_SECTOR          ((uint32_t)0x00000001)  /*!< Sector erase          */
#define FLASH_TYPE_ERASE_MASS            ((uint32_t)0x00000002)  /*!< Flash Mass erase activation */

/* Flash_Latency */
#define FLASH_Latency_0                  ((uint32_t)0x00000000) /* FLASH Zero Latency cycle */
#define FLASH_Latency_1                  ((uint32_t)0x00000001) /* FLASH One Latency cycle */
#define FLASH_Latency_2                  ((uint32_t)0x00000002) /* FLASH Two Latency cycles */

/* Values to be used with CH32V00x devices (1page = 64Byte) */
#define FLASH_WRProt_Pages0to15          ((uint32_t)0x00000001) /* CH32 Low and Medium density devices: Write protection of page 0 to 15 */
#define FLASH_WRProt_Pages16to31         ((uint32_t)0x00000002) /* CH32 Low and Medium density devices: Write protection of page 16 to 31 */
#define FLASH_WRProt_Pages32to47         ((uint32_t)0x00000004) /* CH32 Low and Medium density devices: Write protection of page 32 to 47 */
#define FLASH_WRProt_Pages48to63         ((uint32_t)0x00000008) /* CH32 Low and Medium density devices: Write protection of page 48 to 63 */
#define FLASH_WRProt_Pages64to79         ((uint32_t)0x00000010) /* CH32 Low and Medium density devices: Write protection of page 64 to 79 */
#define FLASH_WRProt_Pages80to95         ((uint32_t)0x00000020) /* CH32 Low and Medium density devices: Write protection of page 80 to 95 */
#define FLASH_WRProt_Pages96to111        ((uint32_t)0x00000040) /* CH32 Low and Medium density devices: Write protection of page 96 to 111 */
#define FLASH_WRProt_Pages112to127       ((uint32_t)0x00000080) /* CH32 Low and Medium density devices: Write protection of page 112 to 127 */
#define FLASH_WRProt_Pages128to143       ((uint32_t)0x00000100) /* CH32 Medium-density devices: Write protection of page 128 to 143 */
#define FLASH_WRProt_Pages144to159       ((uint32_t)0x00000200) /* CH32 Medium-density devices: Write protection of page 144 to 159 */
#define FLASH_WRProt_Pages160to175       ((uint32_t)0x00000400) /* CH32 Medium-density devices: Write protection of page 160 to 175 */
#define FLASH_WRProt_Pages176to191       ((uint32_t)0x00000800) /* CH32 Medium-density devices: Write protection of page 176 to 191 */
#define FLASH_WRProt_Pages192to207       ((uint32_t)0x00001000) /* CH32 Medium-density devices: Write protection of page 192 to 207 */
#define FLASH_WRProt_Pages208to223       ((uint32_t)0x00002000) /* CH32 Medium-density devices: Write protection of page 208 to 223 */
#define FLASH_WRProt_Pages224to239       ((uint32_t)0x00004000) /* CH32 Medium-density devices: Write protection of page 224 to 239 */
#define FLASH_WRProt_Pages240to255       ((uint32_t)0x00008000) /* CH32 Medium-density devices: Write protection of page 240 to 255 */

#define FLASH_WRProt_AllPages            ((uint32_t)0x0000FFFF) /* Write protection of all Pages */

/* Option_Bytes_IWatchdog */
#define OB_IWDG_SW                       ((uint16_t)0x0001) /* Software IWDG selected */
#define OB_IWDG_HW                       ((uint16_t)0x0000) /* Hardware IWDG selected */

/* Option_Bytes_nRST_STOP */
#define OB_STOP_NoRST                    ((uint16_t)0x0002) /* No reset generated when entering in STOP */
#define OB_STOP_RST                      ((uint16_t)0x0000) /* Reset generated when entering in STOP */

/* Option_Bytes_nRST_STDBY */
#define OB_STDBY_NoRST                   ((uint16_t)0x0004) /* No reset generated when entering in STANDBY */
#define OB_STDBY_RST                     ((uint16_t)0x0000) /* Reset generated when entering in STANDBY */

/* Option_Bytes_RST_ENandDT */
#define OB_RST_NoEN                      ((uint16_t)0x0018) /* Reset IO disable (PD7)*/
#define OB_RST_EN_DT12ms                 ((uint16_t)0x0010) /* Reset IO enable (PD7) and  Ignore delay time 12ms */
#define OB_RST_EN_DT1ms                  ((uint16_t)0x0008) /* Reset IO enable (PD7) and  Ignore delay time 1ms */
#define OB_RST_EN_DT128us                ((uint16_t)0x0000) /* Reset IO enable (PD7) and  Ignore delay time 128us */

/* Option_Bytes_Power_ON_Start_Mode */
#define OB_PowerON_Start_Mode_BOOT       ((uint16_t)0x0020) /* from Boot after power on */
#define OB_PowerON_Start_Mode_USER       ((uint16_t)0x0000) /* from User after power on */

/* FLASH_Interrupts */
#define FLASH_IT_ERROR                   ((uint32_t)0x00000400) /* FPEC error interrupt source */
#define FLASH_IT_EOP                     ((uint32_t)0x00001000) /* End of FLASH Operation Interrupt source */
#define FLASH_IT_BANK1_ERROR             FLASH_IT_ERROR         /* FPEC BANK1 error interrupt source */
#define FLASH_IT_BANK1_EOP               FLASH_IT_EOP           /* End of FLASH BANK1 Operation Interrupt source */

/* FLASH_Flags */
#define FLASH_FLAG_BSY                   ((uint32_t)0x00000001) /* FLASH Busy flag */
#define FLASH_FLAG_EOP                   ((uint32_t)0x00000020) /* FLASH End of Operation flag */
#define FLASH_FLAG_WRPRTERR              ((uint32_t)0x00000010) /* FLASH Write protected error flag */
#define FLASH_FLAG_OPTERR                ((uint32_t)0x00000001) /* FLASH Option Byte error flag */

#define FLASH_FLAG_BANK1_BSY             FLASH_FLAG_BSY       /* FLASH BANK1 Busy flag*/
#define FLASH_FLAG_BANK1_EOP             FLASH_FLAG_EOP       /* FLASH BANK1 End of Operation flag */
#define FLASH_FLAG_BANK1_WRPRTERR        FLASH_FLAG_WRPRTERR  /* FLASH BANK1 Write protected error flag */

/* System_Reset_Start_Mode */
#define Start_Mode_USER                  ((uint32_t)0x00000000)
#define Start_Mode_BOOT                  ((uint32_t)0x00004000)
/* Exported macro ------------------------------------------------------------*/
/**
  * @brief  Get the specified FLASH flag status.
  * @param  __FLAG__ specifies the FLASH flags to check.
  *          This parameter can be any combination of the following values:
  *            @arg FLASH_FLAG_BSY      : FLASH Busy flag
  *            @arg FLASH_FLAG_EOP      : FLASH End of Operation flag
  *            @arg FLASH_FLAG_WRPRTERR : FLASH Write protected error flag
  *            @arg FLASH_FLAG_OPTERR   : FLASH option byte error flag
  * @retval The new state of __FLAG__ (SET or RESET).
  */
#define __HAL_FLASH_GET_FLAG(__FLAG__) \
  (((__FLAG__) == FLASH_FLAG_OPTERR) ? ((FLASH->OBR   & (FLASH_FLAG_OPTERR)) != 0U) \
                                     : ((FLASH->STATR & (__FLAG__))          != 0U))

/**
  * @brief  Clear the specified FLASH flags.
  * @param  __FLAG__ specifies the FLASH flags to clear.
  *          This parameter can be any combination of the following values:
  *            @arg FLASH_FLAG_EOP      : FLASH End of Operation flag
  *            @arg FLASH_FLAG_WRPRTERR : FLASH Write protected error flag
  * @retval none
  */
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)                   \
  do {                                                     \
    uint32_t __f = (__FLAG__) & (FLASH_FLAG_EOP | FLASH_FLAG_WRPRTERR); \
    if (__f) { FLASH->STATR = __f; }                       \
  } while (0)
/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef HAL_FLASH_Program(FLASH_HandleTypeDef* hflash, uint64_t Data);
HAL_StatusTypeDef HAL_FLASH_Erase(FLASH_HandleTypeDef* hflash);
HAL_StatusTypeDef HAL_FLASH_Unlock(FLASH_HandleTypeDef* hflash);
HAL_StatusTypeDef HAL_FLASH_Lock(FLASH_HandleTypeDef* hflash);
HAL_StatusTypeDef HAL_FLASH_SystemReset(FLASH_HandleTypeDef* hflash, uint32_t Mode);
/* Private macros ------------------------------------------------------------*/
/* Flash check program method (fast of default) */
#define IS_FLASH_PROGRAM_METHOD(VALUE)   (((VALUE) == FLASH_PROG_METHOD_DEFAULT) || \
                                          ((VALUE) == FLASH_PROG_METHOD_FAST))

/* Flash check program type (DEFAULT: halfword, word; FAST: page (64B)) */
#define IS_FLASH_TYPE_PROGRAM(VALUE) (((VALUE) == FLASH_TYPE_PROGRAM_HALFWORD) || \
                                      ((VALUE) == FLASH_TYPE_PROGRAM_WORD) || \
                                      ((VALUE) == FLASH_TYPE_PROGRAM_PAGE))

/* Flash check erase type (DEFAULT: sector (1KB), mass (16KB); FAST: page (64B), mass (16KB)) */
#define IS_FLASH_TYPE_ERASE(VALUE)   (((VALUE) == FLASH_TYPE_ERASE_PAGE) || \
                                      ((VALUE) == FLASH_TYPE_ERASE_SECTOR) || \
                                      ((VALUE) == FLASH_TYPE_ERASE_MASS))

/* Flash check valid address (0x08000000 - 0x08004000) */
#define IS_FLASH_VALID_ADDRESS(ADD)   (((ADD) >= VALID_ADDR_START) && ((ADD) <= VALID_ADDR_END))

#ifdef __cplusplus
}
#endif

#endif /* __CH32V00X_HAL_FLASH_H */
