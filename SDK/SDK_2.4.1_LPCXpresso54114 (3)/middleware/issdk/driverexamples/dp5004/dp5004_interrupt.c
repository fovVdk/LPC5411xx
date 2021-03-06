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
 * @file dp5004_interrupt.c
 * @brief The dp5004_interrupt.c file implements the ISSDK MPXVDP5004 analog sensor
 *        driver example demonstration with interrupt mode.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "board.h"
#include "pin_mux.h"
#include "fsl_lptmr.h"
#include "fsl_adc12.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
/* Timer timeout Callback. */
#define dp5004_odr_callback PWT_LPTMR0_IRQHandler
/* ADC completion Callback. */
#define adc12_irq_callback ADC0_IRQHandler

/* The desired ODR in milli seconds for the Pressure output */
#define MPXV5004DR_ODR_ms 1000U

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
volatile uint32_t gAdcConversionValue;
volatile bool bAdcCompletedFlag = false;
adc12_channel_config_t gAdcChannelConfigStruct;

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/* LPTMR based ODR control Timer Callback function. */
void dp5004_odr_callback(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    /* Trigger ADC Conversion */
    ADC12_SetChannelConfig(ADC0, MPXVDP5004_ADC12_CHANNEL_GROUP, &gAdcChannelConfigStruct);
}

/* ADC12 based voltage conversion completion IRQ Callback function. */
void adc12_irq_callback(void)
{
    /* Read conversion result to clear the conversion completed flag. */
    gAdcConversionValue = ADC12_GetChannelConversionValue(ADC0, MPXVDP5004_ADC12_CHANNEL_GROUP);
    bAdcCompletedFlag = true;
}

void KE15Z_LPTMR0_ADC0_Initialize(void)
{
    lptmr_config_t lptmrConfig;
    adc12_config_t adc12ConfigStruct;

    /* Configure ODR Timer. */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    LPTMR_Init(LPTMR0, &lptmrConfig);
    LPTMR_SetTimerPeriod(LPTMR0, MSEC_TO_COUNT(MPXV5004DR_ODR_ms, CLOCK_GetFreq(kCLOCK_LpoClk)));
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    EnableIRQ(PWT_LPTMR0_IRQn);
    LPTMR_StartTimer(LPTMR0);

    /* Configure ADC. */
    /* Set ADC12's clock source to be Slow IRC async clock. */
    CLOCK_SetIpSrc(kCLOCK_Adc0, kCLOCK_IpSrcSircAsync);
    EnableIRQ(ADC0_IRQn);
    ADC12_GetDefaultConfig(&adc12ConfigStruct);
    adc12ConfigStruct.clockSource = kADC12_ClockSourceAlt0;
    adc12ConfigStruct.resolution = kADC12_Resolution12Bit;
    ADC12_Init(ADC0, &adc12ConfigStruct);
    /* Set to software trigger mode. */
    ADC12_EnableHardwareTrigger(ADC0, false);
    /* Calibrate ADC. */
    if (kStatus_Success != ADC12_DoAutoCalibration(ADC0))
    {
        PRINTF("ADC calibration failed!\r\n");
    }
    gAdcChannelConfigStruct.channelNumber = MPXVDP5004_ADC12_CHANNEL;
    /* Enable the interrupt. */
    gAdcChannelConfigStruct.enableInterruptOnConversionCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint16_t pressurePascals;
    GENERIC_DRIVER_GPIO *gpioDriver = &Driver_GPIO_KSDK;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n MPXV5004DP ADC based Interrupt Example\r\n");

    /*! Initialize GREEN LED pin used by FRDM board */
    gpioDriver->pin_init(&GREEN_LED, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

    /* Initialize LPTMR and ADC framework */
    KE15Z_LPTMR0_ADC0_Initialize();

    /* Forever loop */
    while (true)
    {
        if (false == bAdcCompletedFlag)
        {
            SMC_SetPowerModeWait(SMC);
            continue;
        }
        else
        {
            bAdcCompletedFlag = false;
            gpioDriver->toggle_pin(&GREEN_LED);
        }

        pressurePascals = (uint16_t)MPXV5004DP_PRESSURE_FROM_ADC_VALUE(gAdcConversionValue);
        PRINTF("\r\n Differential Pressure Value: %dPa\r\n", pressurePascals);
        ASK_USER_TO_RESUME(10); /* Ask for user input after processing 10 samples. */
    }
}
