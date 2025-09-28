/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v00x_hal_gpio.c
 * Author             : truongtl
 * Version            : V1.0
 * Date               : 2025/09/28
 * Description        : GPIO HAL module driver.
 *                      This file provides hardware abstract interface to manage all GPIO functions
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
/* Exported functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct.
  * @param  GPIOx where x can be (A..D).
  * @param  GPIO_InitStruct pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct)
{
    /* Check the parameters */
    HAL_PARAM_CHECK(IS_GPIO_INSTANCE(GPIOx));
    HAL_PARAM_CHECK(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));
    HAL_PARAM_CHECK(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));

    uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
    uint32_t tmpreg = 0x00, pinmask = 0x00;

    currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);

    if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
    {
        /* Check the parameters */
        HAL_PARAM_CHECK(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
        currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
    }

    if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
    {
        tmpreg = READ_REG(GPIOx->CFGLR);

        for (pinpos = 0x00; pinpos < 0x08; pinpos++)
        {
            pos = ((uint32_t)0x01) << pinpos;
            currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

            if (currentpin == pos)
            {
                pos = pinpos << 2;
                pinmask = ((uint32_t)0x0F) << pos;
                tmpreg &= ~pinmask;
                tmpreg |= (currentmode << pos);

                if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
                {
                    WRITE_REG(GPIOx->BCR, (((uint32_t)0x01) << pinpos));
                }
                else
                {
                    if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
                    {
                        WRITE_REG(GPIOx->BSHR, (((uint32_t)0x01) << pinpos));
                    }
                }
            }
        }

        WRITE_REG(GPIOx->CFGLR, tmpreg);
    }
}

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx where x can be (A..D).
  * @param  GPIO_Pin specifies the port bit to read.
  *         This parameter can be GPIO_Pin_x where x can be (0..7).
  * @retval The input port pin value.
  */
GPIO_PinState HAL_GPIO_ReadInput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    HAL_PARAM_CHECK(IS_GPIO_INSTANCE(GPIOx));
    HAL_PARAM_CHECK(IS_GPIO_PIN(GPIO_Pin));

    uint8_t pin_state = 0x00;

    if (IS_BIT_SET(GPIOx->INDR, GPIO_Pin))
    {
        pin_state = (uint8_t)GPIO_PIN_SET;
    }
    else
    {
        pin_state = (uint8_t)GPIO_PIN_RESET;
    }

    return pin_state;
}

/**
  * @brief  Reads the specified output port pin.
  * @param  GPIOx where x can be (A..D).
  * @param  GPIO_Pin specifies the port bit to read.
  *         This parameter can be GPIO_Pin_x where x can be (0..7).
  * @retval The output port pin value.
  */
GPIO_PinState HAL_GPIO_ReadOutput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    HAL_PARAM_CHECK(IS_GPIO_INSTANCE(GPIOx));
    HAL_PARAM_CHECK(IS_GPIO_PIN(GPIO_Pin));

    uint8_t pin_state = 0x00;

    if (IS_BIT_SET(GPIOx->OUTDR, GPIO_Pin))
    {
        pin_state = (uint8_t)GPIO_PIN_SET;
    }
    else
    {
        pin_state = (uint8_t)GPIO_PIN_RESET;
    }

    return pin_state;
}

/**
  * @brief  Sets or clears the selected data port bit.
  * @note   This function uses GPIOx_BSHR/GPIOx_BCR register to allow atomic read/modify
  *         accesses. In this way, there is no risk of an IRQ occurring between
  *         the read and the modify access.
  * @param  GPIOx where x can be (A..D).
  * @param  GPIO_Pin specifies the port bit to be written.
  *          This parameter can be one of GPIO_Pin_x where x can be (0..7).
  * @param  PinState specifies the value to be written to the selected bit.
  *          This parameter can be one of the GPIO_PinState enum values:
  *            @arg GPIO_PIN_RESET: to clear the port pin
  *            @arg GPIO_PIN_SET: to set the port pin
  * @retval None
  */
void HAL_GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    /* Check the parameters */
    HAL_PARAM_CHECK(IS_GPIO_INSTANCE(GPIOx));
    HAL_PARAM_CHECK(IS_GPIO_PIN(GPIO_Pin));
    HAL_PARAM_CHECK(IS_GPIO_PIN_STATE(PinState));

    if (PinState != GPIO_PIN_RESET)
    {
        WRITE_REG(GPIOx->BSHR, GPIO_Pin);
    }
    else
    {
        WRITE_REG(GPIOx->BCR, GPIO_Pin);
    }
}

/**
  * @brief  Toggles the specified GPIO pins.
  * @param  GPIOx where x can be (A..D).
  * @param  GPIO_Pin specifies the pins to be toggled.
  *          This parameter can be one of GPIO_Pin_x where x can be (0..7).
  * @retval None
  */
void HAL_GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    HAL_PARAM_CHECK(IS_GPIO_INSTANCE(GPIOx));
    HAL_PARAM_CHECK(IS_GPIO_PIN(GPIO_Pin));

    if (IS_BIT_CLR(GPIOx->OUTDR, GPIO_Pin))
    {
        WRITE_REG(GPIOx->BSHR, GPIO_Pin);
    }
    else
    {
        WRITE_REG(GPIOx->BCR, GPIO_Pin);
    }
}

/**
  * @brief  Locks GPIO Pins configuration registers.
  * @note   The configuration of the locked GPIO pins can no longer be modified
  *         until the next reset.
  * @param  GPIOx where x can be (A..D).
  * @param  GPIO_Pin specifies the port bit to be locked.
  *          This parameter can be one of GPIO_Pin_x where x can be (0..7).
  * @retval None
  */
void HAL_GPIO_PinLockConfig(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    HAL_PARAM_CHECK(IS_GPIO_INSTANCE(GPIOx));
    HAL_PARAM_CHECK(IS_GPIO_PIN(GPIO_Pin));

    uint32_t tmp = 0x00010000;

    tmp |= GPIO_Pin;
    WRITE_REG(GPIOx->LCKR, tmp);
    WRITE_REG(GPIOx->LCKR, GPIO_Pin);
    WRITE_REG(GPIOx->LCKR, tmp);
    tmp = READ_REG(GPIOx->LCKR);
    tmp = READ_REG(GPIOx->LCKR);
}

/**
  * @brief  Selects the GPIO pin used as EXTI Line.
  * @param  GPIO_PortSource selects the GPIO port to be used as source for EXTI lines.
  *          This parameter can be GPIO_PortSourceGPIOx where x can be (A..D).
  * @param  GPIO_PinSource specifies the EXTI line to be configured.
  *          This parameter can be GPIO_PinSourcex where x can be (0..7).
  * @retval None
  */
void HAL_GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
    /* Check the parameters */
    HAL_PARAM_CHECK(IS_GPIO_PORT_SOURCE(GPIO_PortSource));
    HAL_PARAM_CHECK(IS_GPIO_PIN_SOURCE(GPIO_PinSource));

    uint32_t tmp = 0x00;

    tmp = ((uint32_t)(3 << (GPIO_PinSource << 1)));
    AFIO->EXTICR &= ~tmp;
    AFIO->EXTICR |= ((uint32_t)(GPIO_PortSource << (GPIO_PinSource << 1)));
}

/**
  * @brief  This function handles EXTI interrupt request.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  *          This parameter can be one of GPIO_Pin_x where x can be (0..7).
  * @retval None
  */
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
{
    /* Check the parameters */
    HAL_PARAM_CHECK(IS_GPIO_PIN(GPIO_Pin));

    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
        HAL_GPIO_EXTI_Callback(GPIO_Pin);
    }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  *          This parameter can be one of GPIO_Pin_x where x can be (0..7).
  * @retval None
  */
__weak void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}
