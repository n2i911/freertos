/*
    FreeRTOS V8.2.2 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef BASIC_WEB_SERVER_H
#define BASIC_WEB_SERVER_H

#include <91x_type.h>

/*------------------------------------------------------------------------------*/
/*                            MACROS                                             */
/*------------------------------------------------------------------------------*/
#define basicwebWEBSERVER_PRIORITY      ( tskIDLE_PRIORITY + 2 )

/* The port on which we listen. */
#define webHTTP_PORT		( 80 )

/* Delay on close error. */
#define webSHORT_DELAY		( 10 / portTICK_PERIOD_MS )

/* The IP address being used. */
#define emacIPADDR0			172
#define emacIPADDR1 		25
#define emacIPADDR2   		218
#define emacIPADDR3   		17

/* The gateway address being used. */
#define emacGATEWAY_ADDR0   10
#define emacGATEWAY_ADDR1   52
#define emacGATEWAY_ADDR2   156
#define emacGATEWAY_ADDR3   254

/* The network mask being used. */
#define emacNET_MASK0 		255
#define emacNET_MASK1 		255
#define emacNET_MASK2 		255
#define emacNET_MASK3 		0

#define STATIC_IP   1
#define DHCP_IP     2

#define lwipBASIC_SERVER_STACK_SIZE	250

/*------------------------------------------------------------------------------*/
/*                            PROTOTYPES                                        */
/*------------------------------------------------------------------------------*/
/* The function that implements the WEB server task. */
void vBasicWEBServer( void *pvParameters );

/* Initialisation required by lwIP. */
void vlwIPInit( void );

void  PrintIPOnLCD(unsigned int ipAddr);

void ToDoAfterGettingIP(bool dhcpStaticFlag);

void InitializeStaticIP(void);

void DelayForDHCPToCome(void);

#endif

