/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief FRDM-TOUCH board defines */
/* Push buttons - mutual electrodes */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_1  TF_TSI_MUTUAL_CAP_TX_CHANNEL_3      /* PTE11 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_1  TF_TSI_MUTUAL_CAP_RX_CHANNEL_6      /* PTD5  */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_2  TF_TSI_MUTUAL_CAP_TX_CHANNEL_3      /* PTE11 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_2  TF_TSI_MUTUAL_CAP_RX_CHANNEL_7      /* PTD6  */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_3  TF_TSI_MUTUAL_CAP_TX_CHANNEL_2      /* PTE10 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_3  TF_TSI_MUTUAL_CAP_RX_CHANNEL_6      /* PTD5  */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_4  TF_TSI_MUTUAL_CAP_TX_CHANNEL_2      /* PTE10 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_4  TF_TSI_MUTUAL_CAP_RX_CHANNEL_7      /* PTD6  */

/* Slider - self electrodes */
#define FRDM_TOUCH_BOARD_TSI_SLIDER_ELECTRODE_1  TF_TSI_SELF_CAP_CHANNEL_10     /* PTD1 */
#define FRDM_TOUCH_BOARD_TSI_SLIDER_ELECTRODE_2  TF_TSI_SELF_CAP_CHANNEL_5      /* PTD7 */
                                                                                
/* Rotary - self electrodes */                                                  
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_1  TF_TSI_SELF_CAP_CHANNEL_0      /* PTE5 */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_2  TF_TSI_SELF_CAP_CHANNEL_1      /* PTE4 */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_3  TF_TSI_SELF_CAP_CHANNEL_9      /* PTB5 */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_4  TF_TSI_SELF_CAP_CHANNEL_8      /* PTB4 */

/*! @brief The board name */
#define BOARD_NAME "FRDM-KE15Z"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_LPUART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART1
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_IpSrcSysOscAsync
#define BOARD_UART_IRQ LPUART1_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART1_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C0

/*! @brief The CMP instance/channel used for board. */
#define BOARD_CMP_BASEADDR CMP0
#define BOARD_CMP_CHANNEL 7U

/*! @brief The rtc instance used for board. */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! @brief Indexes of the TSI channels for on-board electrodes */
#define BOARD_TSI_ELECTRODE_1 22U
#define BOARD_TSI_ELECTRODE_2 23U

/*! @brief Indexes of the TSI mutual channels for FRDM-TOUCH board */
#define BOARD_TSI_MUTUAL_TX_ELECTRODE_1  2U
#define BOARD_TSI_MUTUAL_RX_ELECTRODE_1  6U

/*! @brief Define the port interrupt number for the board switches */
#define BOARD_SW2_GPIO GPIOB
#define BOARD_SW2_PORT PORTB
#define BOARD_SW2_GPIO_PIN 11U
#define BOARD_SW2_IRQ PORTBCD_IRQn
#define BOARD_SW2_IRQ_HANDLER PORTBCD_IRQHandler
#define BOARD_SW2_NAME "SW2"

/* Board RGB LED color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_RED_GPIO GPIOD
#define BOARD_LED_RED_GPIO_PORT PORTD
#define BOARD_LED_RED_GPIO_PIN 0U
#define BOARD_LED_GREEN_GPIO GPIOD
#define BOARD_LED_GREEN_GPIO_PORT PORTD
#define BOARD_LED_GREEN_GPIO_PIN 16U
#define BOARD_LED_BLUE_GPIO GPIOD
#define BOARD_LED_BLUE_GPIO_PORT PORTD
#define BOARD_LED_BLUE_GPIO_PIN 15U

#define LED_RED1_INIT(output)                                  \
    GPIO_WritePinOutput(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output);\
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN)  /*!< Enable target LED_RED1 */
#define LED_RED1_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED1 */
#define LED_RED1_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn off target LED_RED1 */
#define LED_RED1_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED1 */

#define LED_GREEN1_INIT(output)                                    \
    GPIO_WritePinOutput(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output);\
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN)  /*!< Enable target LED_GREEN1 */
#define LED_GREEN1_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN1 */
#define LED_GREEN1_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN1 */
#define LED_GREEN1_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN1 */

#define LED_BLUE_INIT(output)                                  \
    GPIO_WritePinOutput(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, output);\
    BOARD_LED_BLUE_GPIO->PDDR |= (1U << BOARD_LED_BLUE_GPIO_PIN)  /*!< Enable target LED_BLUE */
#define LED_BLUE_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
