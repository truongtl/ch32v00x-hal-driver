/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v00x_hal_flash.c
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : FLASH HAL module driver.
 *                      This file provides hardware abstract interface to manage all FLASH functions
 *********************************************************************************
 * Copyright (c) 2025 Developed by truongtl (Truong Tran)
 *******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <ch32v00x_hal.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static HAL_StatusTypeDef FLASH_Program_Word(FLASH_HandleTypeDef* hflash, uint32_t Data);
static HAL_StatusTypeDef FLASH_Program_HalfWord(FLASH_HandleTypeDef* hflash, uint16_t Data);
static HAL_StatusTypeDef FLASH_Program_Page(FLASH_HandleTypeDef* hflash, uint32_t Data);
static HAL_StatusTypeDef FLASH_Erase_Page(FLASH_HandleTypeDef* hflash);
static HAL_StatusTypeDef FLASH_Erase_Sector(FLASH_HandleTypeDef* hflash);
static HAL_StatusTypeDef FLASH_Erase_Mass(FLASH_HandleTypeDef* hflash);
static HAL_StatusTypeDef FLASH_WaitForLastOperation(FLASH_HandleTypeDef* hflash, uint32_t Timeout);
static HAL_FLASH_Error FLASH_GetError(FLASH_HandleTypeDef* hflash);
static void FLASH_ClearError(FLASH_HandleTypeDef* hflash);
/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  Program halfword, word or page at a specified address.
  * @param  hflash  Flash handle instance.
  * @param  Data specifies the data to be programmed.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Program(FLASH_HandleTypeDef* hflash, uint64_t Data)
{
    if (hflash == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameters */
    HAL_PARAM_CHECK(IS_FLASH_PROGRAM_METHOD(hflash->Flash_ProgramMethod));
    HAL_PARAM_CHECK(IS_FLASH_TYPE_PROGRAM(hflash->Flash_ProgramType));
    HAL_PARAM_CHECK(IS_FLASH_VALID_ADDRESS(hflash->Flash_ProgramAdress));

    HAL_StatusTypeDef status;

    switch (hflash->Flash_ProgramMethod)
    {
        case FLASH_PROG_METHOD_DEFAULT:
            if (hflash->Flash_ProgramType == FLASH_TYPE_PROGRAM_HALFWORD)
            {
                /* Program halfword (16-bit) at a specified address. */
                status = FLASH_Program_HalfWord(hflash, (uint16_t)Data);
            }
            else if (hflash->Flash_ProgramType == FLASH_TYPE_PROGRAM_WORD)
            {
                /* Program word (32-bit) at a specified address. */
                status = FLASH_Program_Word(hflash, (uint32_t)Data);
            }
            else
            {
                ;
            }

            break;
        case FLASH_PROG_METHOD_FAST:
            if (hflash->Flash_ProgramType == FLASH_TYPE_PROGRAM_PAGE)
            {
                /* Fast program 64 bytes at a 64-byte aligned page address. */
                status = FLASH_Program_Page(hflash, (uint32_t)Data);
            }
            else
            {
                ;
            }

            break;
        default:
            break;
    }

    return status;
}

/**
  * @brief  Erase sector, page at a specified address or mass memory.
  * @param  hflash  Flash handle instance.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Erase(FLASH_HandleTypeDef* hflash)
{
    if (hflash == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameters */
    HAL_PARAM_CHECK(IS_FLASH_PROGRAM_METHOD(hflash->Flash_ProgramMethod));
    HAL_PARAM_CHECK(IS_FLASH_TYPE_ERASE(hflash->Flash_EraseType));
    HAL_PARAM_CHECK(IS_FLASH_VALID_ADDRESS(hflash->Flash_EraseAddress));

    HAL_StatusTypeDef status;

    switch (hflash->Flash_ProgramMethod)
    {
        case FLASH_PROG_METHOD_DEFAULT:
            if (hflash->Flash_EraseType == FLASH_TYPE_ERASE_SECTOR)
            {
                /* Erase a sector (1KB). */
                status = FLASH_Erase_Sector(hflash);
            }
            else if (hflash->Flash_EraseType == FLASH_TYPE_ERASE_MASS)
            {
                /* Erase all pages (16KB). */
                status = FLASH_Erase_Mass(hflash);
            }
            else
            {
                ;
            }

            break;
        case FLASH_PROG_METHOD_FAST:
            if (hflash->Flash_EraseType == FLASH_TYPE_ERASE_PAGE)
            {
                /* Erase a page (64 bytes). */
                status = FLASH_Erase_Page(hflash);
            }
            else if (hflash->Flash_EraseType == FLASH_TYPE_ERASE_MASS)
            {
                /* Erase all pages (16KB). */
                status = FLASH_Erase_Mass(hflash);
            }
            else
            {
                ;
            }
        default:
            break;
    }

    return status;
}

/**
  * @brief  Unlock FLASH memory.
  * @param  hflash  Flash handle instance.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Unlock(FLASH_HandleTypeDef* hflash)
{
    if (hflash == NULL)
    {
        return HAL_ERROR;
    }

    if (READ_BIT(FLASH->CTLR, FLASH_CTLR_LOCK) != RESET)
    {
        /* Authorize the FPEC of Bank1 Access */
        WRITE_REG(FLASH->KEYR, FLASH_KEY1);
        WRITE_REG(FLASH->KEYR, FLASH_KEY2);

        /* Verify Flash is unlocked */
        if (READ_BIT(FLASH->CTLR, FLASH_CTLR_LOCK) != RESET)
        {
            hflash->ErrorCode |= HAL_FLASH_ERROR_UNLOCK;
            return HAL_ERROR;
        }
    }

    /* Fast program mode unlock */
    if (hflash->Flash_ProgramMethod == FLASH_PROG_METHOD_FAST)
    {
        WRITE_REG(FLASH->MODEKEYR, FLASH_KEY1);
        WRITE_REG(FLASH->MODEKEYR, FLASH_KEY2);

        if (READ_BIT(FLASH->CTLR, FLASH_CTLR_FLOCK) != RESET)
        {
            hflash->ErrorCode |= HAL_FLASH_ERROR_FAST_UNLOCK;
            return HAL_ERROR;
        }
    }

    return HAL_OK;
}

/**
  * @brief  Lock FLASH memory.
  * @param  hflash  Flash handle instance.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Lock(FLASH_HandleTypeDef* hflash)
{
    if (hflash == NULL)
    {
        return HAL_ERROR;
    }

    SET_BIT(FLASH->CTLR, FLASH_CTLR_LOCK);

    /* Verify Flash is locked */
    if (!IS_BIT_SET(FLASH->CTLR, FLASH_CTLR_LOCK))
    {
        hflash->ErrorCode |= HAL_FLASH_ERROR_LOCK;
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
  * @brief  Start mode after system reset.
  * @param  hflash  Flash handle instance.
  * @param  Mode Start mode:
  *            @arg Start_Mode_USER: USER start after system reset
  *            @arg Start_Mode_BOOT: Boot start after system reset
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_SystemReset(FLASH_HandleTypeDef* hflash, uint32_t Mode)
{
    if (hflash == NULL)
    {
        return HAL_ERROR;
    }

    HAL_FLASH_Unlock(hflash);

    WRITE_REG(FLASH->BOOT_MODEKEYR, FLASH_KEY1);
    WRITE_REG(FLASH->BOOT_MODEKEYR, FLASH_KEY2);
    CLEAR_BIT(FLASH->STATR, FLASH_STATR_MODE);

    if (Mode == Start_Mode_BOOT)
    {
        SET_BIT(FLASH->STATR, FLASH_STATR_MODE);
    }

    HAL_FLASH_Lock(hflash);

    return HAL_OK;
}

/* Privated functions ---------------------------------------------------------*/
/**
  * @brief  Program word or page at a specified address.
  * @param  hflash  Flash handle instance.
  * @param  Data specifies the data to be programmed.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
static HAL_StatusTypeDef FLASH_Program_Word(FLASH_HandleTypeDef* hflash, uint32_t Data)
{
    HAL_StatusTypeDef status;

    if ((hflash->Flash_ProgramAdress & 0x3U) != 0U)
    {
        hflash->ErrorCode |= HAL_FLASH_ERROR_ALIGN;
        return HAL_ERROR;
    }

    status = FLASH_WaitForLastOperation(hflash, FLASH_PROGRAM_TIMEOUT);

    if (status == HAL_OK)
    {
        SET_BIT(FLASH->CTLR, FLASH_CTLR_PG);

        *(__IO uint16_t *)hflash->Flash_ProgramAdress = (uint16_t)Data;
        status = FLASH_WaitForLastOperation(hflash, FLASH_PROGRAM_TIMEOUT);

        if (status == HAL_OK)
        {
            *(__IO uint16_t *)(hflash->Flash_ProgramAdress + 2U) = (uint16_t)(Data >> 16);
            status = FLASH_WaitForLastOperation(hflash, FLASH_PROGRAM_TIMEOUT);
            CLEAR_BIT(FLASH->CTLR, FLASH_CTLR_PG);
        }
        else
        {
            CLEAR_BIT(FLASH->CTLR, FLASH_CTLR_PG);
        }

        if (status == HAL_OK)
        {
            if (*(__IO uint32_t *)hflash->Flash_ProgramAdress != Data)
            {
                hflash->ErrorCode |= HAL_FLASH_ERROR_PROG;
                status = HAL_ERROR;
            }
        }
    }

    return status;
}

/**
  * @brief  Program halfword at a specified address.
  * @param  hflash  Flash handle instance.
  * @param  Data specifies the data to be programmed.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
static HAL_StatusTypeDef FLASH_Program_HalfWord(FLASH_HandleTypeDef* hflash, uint16_t Data)
{
    HAL_StatusTypeDef status;

    if ((hflash->Flash_ProgramAdress & 0x1U) != 0U)
    {
        hflash->ErrorCode |= HAL_FLASH_ERROR_ALIGN;
        return HAL_ERROR;
    }

    status = FLASH_WaitForLastOperation(hflash, FLASH_PROGRAM_TIMEOUT);

    if (status == HAL_OK)
    {
        SET_BIT(FLASH->CTLR, FLASH_CTLR_PG);
        *(__IO uint16_t *)hflash->Flash_ProgramAdress = Data;
        status = FLASH_WaitForLastOperation(hflash, FLASH_PROGRAM_TIMEOUT);
        CLEAR_BIT(FLASH->CTLR, FLASH_CTLR_PG);

        if (status == HAL_OK)
        {
            if (*(__IO uint16_t *)hflash->Flash_ProgramAdress != Data)
            {
                hflash->ErrorCode |= HAL_FLASH_ERROR_PROG;
                status = HAL_ERROR;
            }
        }
    }

    return status;
}

/**
  * @brief  Program page at a specified address.
  * @param  hflash  Flash handle instance.
  * @param  Data specifies the data to be programmed.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
static HAL_StatusTypeDef FLASH_Program_Page(FLASH_HandleTypeDef* hflash, uint32_t Data)
{
    HAL_StatusTypeDef status;
    const uint32_t *w = (const uint32_t *)(uintptr_t)Data;
    uint32_t i;

    if ((hflash->Flash_ProgramAdress & 0x3FU) != 0U)
    {
        hflash->ErrorCode |= HAL_FLASH_ERROR_ALIGN;
        return HAL_ERROR;
    }

    if (READ_BIT(FLASH->CTLR, FLASH_CTLR_FLOCK) != 0U)
    {
        hflash->ErrorCode |= HAL_FLASH_ERROR_FAST_UNLOCK;
        return HAL_ERROR;
    }

    status = FLASH_WaitForLastOperation(hflash, FLASH_PROGRAM_TIMEOUT);
    if (status != HAL_OK)
    {
        return status;
    }

    SET_BIT(FLASH->CTLR, FLASH_CTLR_PAGE_PG);
    SET_BIT(FLASH->CTLR, FLASH_CTLR_BUF_RST);
    status = FLASH_WaitForLastOperation(hflash, FLASH_PROGRAM_TIMEOUT);
    CLEAR_BIT(FLASH->CTLR, FLASH_CTLR_PAGE_PG);

    if (status != HAL_OK)
    {
        return status;
    }

    for (i = 0U; i < 16U; i++)
    {
        SET_BIT(FLASH->CTLR, FLASH_CTLR_PAGE_PG);
        *(__IO uint32_t *)(hflash->Flash_ProgramAdress + (i * 4U)) = w[i];
        SET_BIT(FLASH->CTLR, FLASH_CTLR_BUF_LOAD);
        status = FLASH_WaitForLastOperation(hflash, FLASH_PROGRAM_TIMEOUT);
        CLEAR_BIT(FLASH->CTLR, FLASH_CTLR_PAGE_PG);

        if (status != HAL_OK)
        {
            return status;
        }
    }

    SET_BIT(FLASH->CTLR, FLASH_CTLR_PAGE_PG);
    WRITE_REG(FLASH->ADDR, hflash->Flash_ProgramAdress);
    SET_BIT(FLASH->CTLR, FLASH_CTLR_STRT);
    status = FLASH_WaitForLastOperation(hflash, FLASH_PROGRAM_TIMEOUT);
    CLEAR_BIT(FLASH->CTLR, FLASH_CTLR_PAGE_PG);

    if (status == HAL_OK)
    {
        for (i = 0U; i < 16U; i++)
        {
            if (*(__IO uint32_t *)(hflash->Flash_ProgramAdress + (i * 4U)) != w[i])
            {
                hflash->ErrorCode |= HAL_FLASH_ERROR_PROG;
                status = HAL_ERROR;
                break;
            }
        }
    }

    return status;
}

/**
  * @brief  Erase page at a specified address.
  * @param  hflash  Flash handle instance.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
static HAL_StatusTypeDef FLASH_Erase_Page(FLASH_HandleTypeDef* hflash)
{
    if ((hflash->Flash_EraseAddress & 0x3FU) != 0U)
    {
        hflash->ErrorCode |= HAL_FLASH_ERROR_ALIGN;
        return HAL_ERROR;
    }

    uint32_t start, end;

    SET_BIT(FLASH->CTLR, FLASH_CTLR_PAGE_ER);
    WRITE_REG(FLASH->ADDR, hflash->Flash_EraseAddress);
    SET_BIT(FLASH->CTLR, FLASH_CTLR_STRT);
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)) {}
    CLEAR_BIT(FLASH->CTLR, FLASH_CTLR_PAGE_ER);

    start = hflash->Flash_EraseAddress;
    end = start + 64U;
    while (start < end)
    {
        if (*(__IO uint16_t *)start != 0xFFFFU)
        {
            hflash->ErrorCode |= HAL_FLASH_ERROR_PROG;
            return HAL_ERROR;
        }
        start += 2U;
    }

    return HAL_OK;
}

/**
  * @brief  Erase sector at a specified address.
  * @param  hflash  Flash handle instance.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
static HAL_StatusTypeDef FLASH_Erase_Sector(FLASH_HandleTypeDef* hflash)
{
    HAL_StatusTypeDef status;
    uint32_t start, end;

    if ((hflash->Flash_EraseAddress & (FLASH_PAGE_SIZE - 1U)) != 0U)
    {
        hflash->ErrorCode |= HAL_FLASH_ERROR_ALIGN;
        return HAL_ERROR;
    }

    status = FLASH_WaitForLastOperation(hflash, FLASH_ERASE_TIMEOUT);

    if (status == HAL_OK)
    {
        SET_BIT(FLASH->CTLR,FLASH_CTLR_PER);
        WRITE_REG(FLASH->ADDR, hflash->Flash_EraseAddress);
        SET_BIT(FLASH->CTLR,FLASH_CTLR_STRT);
        status = FLASH_WaitForLastOperation(hflash, FLASH_ERASE_TIMEOUT);
        CLEAR_BIT(FLASH->CTLR, FLASH_CTLR_PER);
    }

    if (status != HAL_OK)
    {
        return status;
    }

    start = hflash->Flash_EraseAddress;
    end = start + FLASH_PAGE_SIZE;
    while (start < end)
    {
        if (*(__IO uint16_t *)start != 0xFFFFU)
        {
            hflash->ErrorCode |= HAL_FLASH_ERROR_PROG;
            return HAL_ERROR;
        }
        start += 2U;
    }

    return HAL_OK;
}

/**
  * @brief  Erase mass memory.
  * @param  hflash  Flash handle instance.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
static HAL_StatusTypeDef FLASH_Erase_Mass(FLASH_HandleTypeDef* hflash)
{
    HAL_StatusTypeDef status;
    uint32_t start;

    status = FLASH_WaitForLastOperation(hflash, FLASH_ERASE_TIMEOUT);

    if (status == HAL_OK)
    {
        SET_BIT(FLASH->CTLR,FLASH_CTLR_MER);
        SET_BIT(FLASH->CTLR,FLASH_CTLR_STRT);
        status = FLASH_WaitForLastOperation(hflash, FLASH_ERASE_TIMEOUT);
        CLEAR_BIT(FLASH->CTLR, FLASH_CTLR_MER);
    }

    if (status != HAL_OK)
    {
        return status;
    }

    start = VALID_ADDR_START;
    while (start < VALID_ADDR_END)
    {
        if (*(__IO uint16_t *)start != 0xFFFFU)
        {
            hflash->ErrorCode |= HAL_FLASH_ERROR_PROG;
            return HAL_ERROR;
        }
        start += 2U;
    }

    return HAL_OK;
}

/**
  * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * @param  hflash  Flash handle instance.
  * @param  Timeout FLASH programming Timeout.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
static HAL_StatusTypeDef FLASH_WaitForLastOperation(FLASH_HandleTypeDef* hflash, uint32_t Timeout)
{
    uint32_t tickstart;

    tickstart = HAL_GetTick();

    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0U)
    {
        if (Timeout != HAL_MAX_DELAY)
        {
            if ((HAL_GetTick() - tickstart) > Timeout)
            {
                hflash->ErrorCode |= HAL_FLASH_ERROR_TIMEOUT;
                return HAL_TIMEOUT;
            }
        }
    }

    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP) != 0U)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
    }

    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPRTERR) != 0U)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPRTERR);
        hflash->ErrorCode |= HAL_FLASH_ERROR_WRP;
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
  * @brief  Get FLASH error code.
  * @param  hflash  Flash handle instance.
  *
  * @retval HAL_FLASH_Error Error Code
  */
static HAL_FLASH_Error FLASH_GetError(FLASH_HandleTypeDef* hflash)
{
    return hflash->ErrorCode;
}

/**
  * @brief  Clear FLASH error code.
  * @param  hflash  Flash handle instance.
  *
  * @retval None
  */
static void FLASH_ClearError(FLASH_HandleTypeDef* hflash)
{
    hflash->ErrorCode = HAL_FLASH_ERROR_NONE;
}
