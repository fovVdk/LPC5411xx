/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_mailbox.h"

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Address of RAM, where the image for core1 should be copied */
#define CORE1_BOOT_ADDRESS (void *)0x20010000

#if defined(__CC_ARM)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
extern unsigned char core1_image_start[];
#define CORE1_IMAGE_START core1_image_start
#elif defined(__GNUC__)
extern const char m0_image_start[];
extern const char* m0_image_end;
extern int m0_image_size;
#define CORE1_IMAGE_START ((void *)m0_image_start)
#define CORE1_IMAGE_SIZE ((void *)m0_image_size)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Shared variable by both cores, before changing of this variable the cores must
   first take mailbox mutex, after changing the shared variable must retrun mutex */
volatile uint32_t g_shared = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size()
{
    uint32_t core1_image_size;
#if defined(__CC_ARM)
    core1_image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
#pragma section = "__sec_core"
    core1_image_size = (uint32_t)__section_end("__sec_core") - (uint32_t)&core1_image_start;
#elif defined(__GNUC__)
    core1_image_size = (uint32_t)m0_image_size;
#endif
    return core1_image_size;
}
#endif

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware.*/
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);

    BOARD_InitPins_Core0();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();

    PRINTF("Mailbox mutex example\r\n");

    /* Init Mailbox */
    MAILBOX_Init(MAILBOX);

#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* Calculate size of the image */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    PRINTF("Copy CORE1 image to address: 0x%x, size: %d\r\n", CORE1_BOOT_ADDRESS, core1_image_size);

    /* Copy application from FLASH to RAM */
    memcpy(CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);
#endif

    /* Boot source for Core 1 from flash */
    SYSCON->CPBOOT = SYSCON_CPBOOT_BOOTADDR(*(uint32_t *)((uint8_t *)CORE1_BOOT_ADDRESS + 0x4));
    SYSCON->CPSTACK = SYSCON_CPSTACK_STACKADDR(*(uint32_t *)CORE1_BOOT_ADDRESS);

    int32_t temp = SYSCON->CPUCTRL;
    temp |= 0xc0c48000;
    SYSCON->CPUCTRL = (temp | SYSCON_CPUCTRL_CM0RSTEN_MASK);
    SYSCON->CPUCTRL = (temp);

    /* Wait for start and initialization of second core */
    int i = 0;
    for (i = 0; i < 2000; i++)
    { 
       __NOP();
    }

    /* Send address of shared variable to CM0+ by Mailbox*/
    MAILBOX_SetValue(MAILBOX, kMAILBOX_CM0Plus, (uint32_t)&g_shared);

    while (1)
    {
        /* Get Mailbox mutex */
        while (MAILBOX_GetMutex(MAILBOX) == 0)
            ;

        /* The core0 has mutex, can change shared variable g_shared */
        g_shared++;

        PRINTF("Core0 has mailbox mutex, update shared variable to: %d\r\n", g_shared);

        /* Set mutex to allow access other core to shared variable */
        MAILBOX_SetMutex(MAILBOX);
    }
}

