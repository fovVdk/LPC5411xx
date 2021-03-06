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

/*! \file status.c
    \brief Application-specific status subsystem

    Applications may change how they choose to display status information.
    The default implementation here uses LEDs on NXP Freedom boards.
    You may swap out implementations as long as the "Required" methods and states
    are retained.
*/

#include "board.h"              // KSDK HAL
#ifndef CPU_LPC54114J256BD64_cm4
#include "fsl_port.h"           // KSDK Port Module Interface
#endif
#include "sensor_fusion.h"      // Sensor fusion structures and functions
#include "drivers.h"            // Common driver header file
#include "status.h"             // Header for this .c file

// bit-field definitions
#define N   0x00                // No color
#define R   0x04                // Red LED
#define G   0x02                // Green LED
#define B   0x01                // Blue LED

// set RGB LED as a function of bit-field values
void ssSetLeds(int8_t RGB)
{
    if (RGB & R)
        LED_RED_ON();
    else
        LED_RED_OFF();
    if (RGB & G)
        LED_GREEN_ON();
    else
        LED_GREEN_OFF();
    if (RGB & B)
        LED_BLUE_ON();
    else
        LED_BLUE_OFF();
}

// Do an immediate status update
void ssSetStatusNow(StatusSubsystem *pStatus, fusion_status_t status)
{
    pStatus->status = status;
    pStatus->next = status;

    uint8_t blink = false;
    uint8_t RGB = N;

    // This is where we actually change the visual indicator
    // We are not using the blue LED because it is not available on some
    // board combinations.
    switch (status)
    {
        case INITIALIZING:      // solid GREEN
            RGB = G;
            break;
        case NORMAL:            // blinking GREEN
            RGB = G;
            blink = true;
            break;
        case LOWPOWER:          // blinking YELLOW
            RGB = R|G;
            blink = true;
            break;
        case SOFT_FAULT:        // solid RED (usually momentary)
        case HARD_FAULT:        // solid RED
            RGB = R;
            break;
        default:                // default = off;
            RGB = N;
    }

    if ((!blink) | (status != pStatus->previous))
    {
        ssSetLeds(RGB);
        pStatus->toggle = true;
    }
    else
    {
        if (pStatus->toggle)
        {
            ssSetLeds(N);
        }
        else
        {
            ssSetLeds(RGB);
        }

        pStatus->toggle = !pStatus->toggle;
    }
    while (status == HARD_FAULT) ; // Never return on hard fault
    // while (status == SOFT_FAULT) ; // DEBUG ONLY Never return on soft fault
}

// Unit test for status sub-system
void ssTest(StatusSubsystem *pStatus)
{
    switch (pStatus->status)
    {
        case OFF:
            ssSetStatusNow(pStatus, INITIALIZING);
            break;
        case INITIALIZING:
            ssSetStatusNow(pStatus, LOWPOWER);
            break;
        case LOWPOWER:
            ssSetStatusNow(pStatus, NORMAL);
            break;
        case NORMAL:
            ssSetStatusNow(pStatus, RECEIVING_WIRED);
            break;
        case RECEIVING_WIRED:
            ssSetStatusNow(pStatus, RECEIVING_WIRELESS);
            break;
        case RECEIVING_WIRELESS:
            ssSetStatusNow(pStatus, SOFT_FAULT);
            break;
        case SOFT_FAULT:
            ssSetStatusNow(pStatus, HARD_FAULT);
            break;
        case HARD_FAULT:
            ssSetStatusNow(pStatus, OFF);
            break;
    }
}

// undefine these just in case some other library needs them
#undef N
#undef R
#undef G
#undef B


// queue up a status change (which will take place at the next updateStatus)
void ssQueueStatus(StatusSubsystem *pStatus, fusion_status_t status)
{
    pStatus->next = status;
}

// promote any previously queued status update
void ssUpdateStatus(StatusSubsystem *pStatus)
{
    pStatus->previous = pStatus->status;
    ssSetStatusNow(pStatus, pStatus->next);
}

// make an immediate update to the system status
void ssSetStatus(StatusSubsystem *pStatus, fusion_status_t status)
{
    pStatus->next = status;
    ssUpdateStatus(pStatus);
}

/// initializeStatusSubsystem() should be called once at startup to initialize the
/// data structure and to put hardware into the proper state for communicating status.
void initializeStatusSubsystem(StatusSubsystem *pStatus)
{
    pStatus->set = ssSetStatus;
    pStatus->queue = ssQueueStatus;
    pStatus->update = ssUpdateStatus;
    pStatus->test = ssTest;
    pStatus->previous = OFF;
    pStatus->set(pStatus, OFF);
    pStatus->queue(pStatus, OFF);
    pStatus->toggle = false;

    /* Un-gate the port clocks */
    CLOCK_EnableClock(RED_LED.clockName);
    CLOCK_EnableClock(GREEN_LED.clockName);
    //CLOCK_EnableClock(BLUE_LED.clockName);
#ifndef CPU_LPC54114J256BD64_cm4
    // Not needed for the LPC54114 (done elsewhere)
    // Led pin mux Configuration
    PORT_SetPinMux(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN,
                   kPORT_MuxAsGpio);
    PORT_SetPinMux(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN,
                   kPORT_MuxAsGpio);
#endif
    /* set initial values */
    LED_RED_INIT(LOGIC_LED_OFF);
    LED_GREEN_INIT(LOGIC_LED_OFF);
}
