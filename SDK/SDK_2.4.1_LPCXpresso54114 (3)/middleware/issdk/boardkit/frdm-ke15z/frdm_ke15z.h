/*
 * The Clear BSD License
 * Copyright (c) 2017, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
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

/**
 * @file frdm_ke15z.h
 * @brief The frdm_ke15z.h file defines GPIO pin mappings for FRDM-KE15Z board
*/

#ifndef FRDM_KE15Z_H_
#define FRDM_KE15Z_H_

#include "pin_mux.h"
#include "fsl_smc.h"
#include "MKE15Z7.h"
#include "RTE_Device.h"
#include "gpio_driver.h"

// I2C1 Pin Handles
extern gpioHandleKSDK_t D15;
extern gpioHandleKSDK_t D14;

// I2C0 Handle
extern gpioHandleKSDK_t A5;
extern gpioHandleKSDK_t A4;

// SPI0 Handle
extern gpioHandleKSDK_t D13;
extern gpioHandleKSDK_t D11;
extern gpioHandleKSDK_t D12;

// UART0 Handle
extern gpioHandleKSDK_t D0;
extern gpioHandleKSDK_t D1;

// FRDM-KE15Z Arduino Connector Pin Defintion
extern gpioHandleKSDK_t A0;
extern gpioHandleKSDK_t A1;
extern gpioHandleKSDK_t A2;
extern gpioHandleKSDK_t A3;
extern gpioHandleKSDK_t D2;
extern gpioHandleKSDK_t D3;
extern gpioHandleKSDK_t D4;
extern gpioHandleKSDK_t D5;
extern gpioHandleKSDK_t D6;
extern gpioHandleKSDK_t D7;
extern gpioHandleKSDK_t D8;
extern gpioHandleKSDK_t D9;
extern gpioHandleKSDK_t D10;

// FRDM-KE15Z Internal Peripheral Pin Definitions
extern gpioHandleKSDK_t RED_LED;
extern gpioHandleKSDK_t GREEN_LED;
extern gpioHandleKSDK_t BLUE_LED;
extern gpioHandleKSDK_t INT1;
extern gpioHandleKSDK_t RST_A;

// I2C_S1: Pin mapping and driver information for default I2C brought to shield
#define I2C_S1_SCL_PIN A5
#define I2C_S1_SDA_PIN A4
#define I2C_S1_DRIVER Driver_I2C0
#define I2C_S1_DEVICE_INDEX I2C0_INDEX
#define I2C_S1_SIGNAL_EVENT I2C0_SignalEvent_t

// I2C_S2: Pin mapping and driver information for alternate I2C bus on shield
#define I2C_S2_SCL_PIN D15
#define I2C_S2_SDA_PIN D14
#define I2C_S2_DRIVER Driver_I2C1
#define I2C_S2_DEVICE_INDEX I2C1_INDEX
#define I2C_S2_SIGNAL_EVENT I2C1_SignalEvent_t

// I2C_B: Pin mapping and driver information for I2C routed on KE15Z base board
#define I2C_BB_SCL_PIN A5
#define I2C_BB_SDA_PIN A4
#define I2C_BB_DRIVER Driver_I2C0
#define I2C_BB_DEVICE_INDEX I2C0_INDEX
#define I2C_BB_SIGNAL_EVENT I2C0_SignalEvent_t

// SPIS: Pin mapping and driver information default SPI brought to shield
#define SPI_S_SCLK D13
#define SPI_S_MOSI D11
#define SPI_S_MISO D12
#define SPI_S_DRIVER Driver_SPI0
#define SPI_S_BAUDRATE 500000U ///< Transfer baudrate - 500k
#define SPI_S_DEVICE_INDEX SPI0_INDEX
#define SPI_S_SIGNAL_EVENT SPI0_SignalEvent_t

// UART: Driver information for default UART to communicate with HOST PC.
#define HOST_S_DRIVER Driver_USART1
#define HOST_S_SIGNAL_EVENT HOST_SignalEvent_t

// Bluetooth: Driver information for default UART to communicate with HOST PC.
#define HOST_B_DRIVER Driver_USART1
#define HOST_B_SIGNAL_EVENT HOST_SignalEvent_t

// On-Board FXOS8700 Sensor Information
#define FXOS8700_BB_I2C_ADDR 0x1C
#define FXOS8700_BB_INT1 INT1
#define FXOS8700_BB_RST RST_A

/* @brief  Ask use input to resume after specified samples have been processed. */
#define ASK_USER_TO_RESUME(x)                                                          \
    static volatile bool askResume = true;                                             \
    static uint16_t samplesToProcess = x - 1;                                          \
    if (askResume && !samplesToProcess--)                                              \
    {                                                                                  \
        PRINTF("\r\n Specified samples processed, press any key to continue... \r\n"); \
        GETCHAR();                                                                     \
        askResume = false;                                                             \
    }

// Miscellaneous Hardware Configuration Parameters
#define ADS_NVM_ADDR 0x0003F000 ///< start of the next to last 2K (sector size) of the 256K flash
#define FLASH_SECTOR_SIZE_PROPERTY kFLASH_PropertyPflashSectorSize

// Set GPIO pin for handling the gpio interrupt using PORTB_PORTC_PORTD_PORTE_IRQHandler
// By default, GPIO Pin set to GPIOB
#define GPIO_TYPE GPIOC
#define GPIO_PORT_NUM PORTC_NUM

/* Compatability definitions for KE15Z */
#define I2C0 LPI2C0
#define I2C1 LPI2C1
#define SPI0 LPSPI0
#define SPI1 LPSPI1
#define I2C_Type LPI2C_Type
#define SPI_Type LPSPI_Type
#define I2C_BASE_PTRS LPI2C_BASE_PTRS
#define SPI_BASE_PTRS LPSPI_BASE_PTRS

/* Reset the onboard FXOS8700 */
void BOARD_ACCEL_Reset(void);

#endif /* FRDM_KE15Z_H_ */
