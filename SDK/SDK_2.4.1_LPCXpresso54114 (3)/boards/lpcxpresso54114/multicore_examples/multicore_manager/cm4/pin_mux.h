/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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
 
 #ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/  

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif



/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void); /* Function assigned for the Cortex-M4F */

/* FC0_RXD_SDA_MOSI (number 31), U18[4]/TO_MUX_P0_0-ISP_RX */
#define BOARD_INITPINS_CORE0_DEBUG_UART_RX_PERIPHERAL                  FLEXCOMM0   /*!< Device name: FLEXCOMM0 */
#define BOARD_INITPINS_CORE0_DEBUG_UART_RX_SIGNAL                   RXD_SDA_MOSI   /*!< FLEXCOMM0 signal: RXD_SDA_MOSI */
#define BOARD_INITPINS_CORE0_DEBUG_UART_RX_PIN_NAME             FC0_RXD_SDA_MOSI   /*!< Pin name */
#define BOARD_INITPINS_CORE0_DEBUG_UART_RX_LABEL     "U18[4]/TO_MUX_P0_0-ISP_RX"   /*!< Label */
#define BOARD_INITPINS_CORE0_DEBUG_UART_RX_NAME                  "DEBUG_UART_RX"   /*!< Identifier name */

/* FC0_TXD_SCL_MISO (number 32), U6[4]/U22[3]/P0_1-ISP_TX */
#define BOARD_INITPINS_CORE0_DEBUG_UART_TX_PERIPHERAL                  FLEXCOMM0   /*!< Device name: FLEXCOMM0 */
#define BOARD_INITPINS_CORE0_DEBUG_UART_TX_SIGNAL                   TXD_SCL_MISO   /*!< FLEXCOMM0 signal: TXD_SCL_MISO */
#define BOARD_INITPINS_CORE0_DEBUG_UART_TX_PIN_NAME             FC0_TXD_SCL_MISO   /*!< Pin name */
#define BOARD_INITPINS_CORE0_DEBUG_UART_TX_LABEL      "U6[4]/U22[3]/P0_1-ISP_TX"   /*!< Label */
#define BOARD_INITPINS_CORE0_DEBUG_UART_TX_NAME                  "DEBUG_UART_TX"   /*!< Identifier name */

/*!
 *
 */
void BOARD_InitPins_Core0(void); /* Function assigned for the Cortex-M4F */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
