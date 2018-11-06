/*
 * The Clear BSD License
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
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

/**
 * @file fxls8471q_spi_interrupt.c
 * @brief The fxls8471q_interrupt.c file implements the ISSDK FXLS8471Q sensor
 * driver example demonstration with interrupt mode.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "fxls8471q_drv.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_SPI.h"

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! Prepare the register write list to configure FXLS8471Q in non-FIFO mode. */
const registerwritelist_t cFxls8471q_Config_Isr[] = {
    /*! Configure CTRL_REG1 register to put FXLS8471Q to 12.5Hz sampling rate. */
    {FXLS8471Q_CTRL_REG1, FXLS8471Q_CTRL_REG1_DR_12DOT5HZ, FXLS8471Q_CTRL_REG1_DR_MASK},
    /*! Configure settings for interrupt notification. */
    /*! Active High, Push-Pull */
    {FXLS8471Q_CTRL_REG3, FXLS8471Q_CTRL_REG3_IPOL_HIGH | FXLS8471Q_CTRL_REG3_PP_OD_PUSHPULL,
     FXLS8471Q_CTRL_REG3_IPOL_MASK | FXLS8471Q_CTRL_REG3_PP_OD_MASK},
    {FXLS8471Q_CTRL_REG4, FXLS8471Q_CTRL_REG4_INT_EN_DRDY_ENABLED,
     FXLS8471Q_CTRL_REG4_INT_EN_DRDY_MASK}, /*! Data Ready Event. */
    {FXLS8471Q_CTRL_REG5, FXLS8471Q_CTRL_REG5_INT_CFG_DRDY_INT1, FXLS8471Q_CTRL_REG5_INT_CFG_DRDY_MASK}, /*! INT1 Pin */
    __END_WRITE_DATA__};

/*! Prepare the register read list to read the raw accel data from the FXLS8471Q. */
const registerreadlist_t cFxls8471q_Output_Values[] = {
    {.readFrom = FXLS8471Q_OUT_X_MSB, .numBytes = FXLS8471Q_ACCEL_DATA_SIZE}, __END_READ_DATA__};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
volatile bool gFxls8471qDataReady = false;

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/*! -----------------------------------------------------------------------
 *  @brief       This is the Sensor Data Ready ISR implementation.
 *  @details     This function sets the flag which indicates if a new sample(s) is available for reading.
 *  @param[in]   pUserData This is a void pointer to the instance of the user specific data structure for the ISR.
 *  @return      void  There is no return value.
 *  @constraints None
 *  @reeentrant  Yes
 *  -----------------------------------------------------------------------*/
void fxls8471q_isr(void *pUserData)
{ /*! @brief Set flag to indicate Sensor has signalled data ready. */
    gFxls8471qDataReady = true;
}

/*! -----------------------------------------------------------------------
 *  @brief       This is the The main function implementation.
 *  @details     This function invokes board initializes routines, then then brings up the sensor and
 *               finally enters an endless loop to continuously read available samples.
 *  @param[in]   void This is no input parameter.
 *  @return      void  There is no return value.
 *  @constraints None
 *  @reeentrant  No
 *  -----------------------------------------------------------------------*/
int main(void)
{
    int32_t status;
    uint8_t data[FXLS8471Q_ACCEL_DATA_SIZE];
    fxls8471q_acceldata_t rawData;

    ARM_DRIVER_SPI *pSPIdriver = &SPI_S_DRIVER;
    fxls8471q_spi_sensorhandle_t fxls8471qDriver;
    GENERIC_DRIVER_GPIO *gpioDriver = &Driver_GPIO_KSDK;

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK FXLS8471Q sensor driver example demonstration with interrupt mode.\r\n");

    /*! Initialize INT1_FXLS8471Q pin used by board */
    gpioDriver->pin_init(&FXLS8471_INT1, GPIO_DIRECTION_IN, NULL, &fxls8471q_isr, NULL);
    /*! Initialize RGB LED pin used by board */
    gpioDriver->pin_init(&GREEN_LED, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

    /*! Initialize the SPI driver. */
    status = pSPIdriver->Initialize(SPI_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n SPI Initialization Failed\r\n");
        return -1;
    }

    /*! Set the SPI Power mode. */
    status = pSPIdriver->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n SPI Power Mode setting Failed\r\n");
        return -1;
    }

    /*! Set the SPI Slave speed. */
    status = pSPIdriver->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0, SPI_S_BAUDRATE);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n SPI Control Mode setting Failed\r\n");
        return -1;
    }

    /*! Initialize the FXLS8471Q sensor driver. */
    status = FXLS8471Q_SPI_Initialize(&fxls8471qDriver, &SPI_S_DRIVER, SPI_S_DEVICE_INDEX, &FXLS8471_SPI_CS,
                                      FXLS8471Q_WHO_AM_I_WHOAMI_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXLS8471Q Sensor Initialization Failed\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Initiliazed FXLS8471Q Sensor\r\n");

    /*!  Set the task to be executed while waiting for SPI transactions to complete. */
    FXLS8471Q_SPI_SetIdleTask(&fxls8471qDriver, (registeridlefunction_t)SMC_SetPowerModeVlpr, SMC);

    /*! Configure the FXLS8471Q sensor driver. */
    status = FXLS8471Q_SPI_Configure(&fxls8471qDriver, cFxls8471q_Config_Isr);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXLS8471Q Sensor Configuration Failed, Err = %d\r\n", status);
        return -1;
    }
    PRINTF("\r\n Successfully Applied FXLS8471Q Sensor Configuration\r\n");

    for (;;) /* Forever loop */
    {        /* In ISR Mode we do not need to check Data Ready Register.
              * The receipt of interrupt will indicate data is ready. */
        if (false == gFxls8471qDataReady)
        { /* Loop, if new sample is not available. */
            SMC_SetPowerModeWait(SMC);
            continue;
        }
        else
        { /*! Clear the data ready flag, it will be set again by the ISR. */
            gFxls8471qDataReady = false;
            gpioDriver->toggle_pin(&GREEN_LED);
        }

        /*! Read the raw sensor data from the FXLS8471Q. */
        status = FXLS8471Q_SPI_ReadData(&fxls8471qDriver, cFxls8471q_Output_Values, data);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }

        /*! Convert the raw sensor data to signed 16-bit container for display to the debug port. */
        rawData.accel[0] = ((int16_t)data[0] << 8) | (int16_t)data[1];
        rawData.accel[0] /= 4;
        rawData.accel[1] = ((int16_t)data[2] << 8) | (int16_t)data[3];
        rawData.accel[1] /= 4;
        rawData.accel[2] = ((int16_t)data[4] << 8) | (int16_t)data[5];
        rawData.accel[2] /= 4;

        /* NOTE: PRINTF is relatively expensive in terms of CPU time, specially when used with-in execution loop. */
        PRINTF("\r\n Accel X = %d  Y = %d  Z = %d\r\n", rawData.accel[0], rawData.accel[1], rawData.accel[2]);
        ASK_USER_TO_RESUME(100); /* Ask for user input after processing 100 samples. */
    }
}
