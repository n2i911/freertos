/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <string.h>

/* Demo application includes. */
#include "serial.h"

/* Hardware and starter kit includes. */
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

#define shellSTACK_SIZE     256

#define comRX_BLOCK_TIME    ( ( TickType_t ) 0xffff )
#define comNO_BLOCK         ( ( TickType_t ) 0 )

#define shellPROMPT         "#>"

signed char console_buffer[ configCONSOLE_BUFFER_SIZE + 1 ];
signed char len = 0;
signed char *p = console_buffer;

static portTASK_FUNCTION_PROTO( vShellTask, pvParameters );
static xComPortHandle xPort = NULL;

void vStartShellTasks( UBaseType_t uxPriority )
{
    /* Spawn the task. */
    xTaskCreate( vShellTask, "Simple-Shell", shellSTACK_SIZE, NULL, uxPriority, ( TaskHandle_t * ) NULL );
}

static void prvShellBackspace( void )
{
    if( len == 0 )
        return;

    p--;
    *p = '\0';
    len--;

    xSerialPutChar( xPort, '\r', comNO_BLOCK );
    vSerialPutString( xPort, shellPROMPT, strlen( shellPROMPT ) );
    vSerialPutString( xPort, console_buffer, strlen( console_buffer ) );
    xSerialPutChar( xPort, ' ', comNO_BLOCK );
    xSerialPutChar( xPort, '\b', comNO_BLOCK );
}

static void prvShellRuncmd( void )
{
    /* TODO */
}

static signed portBASE_TYPE prvShellReadline( void )
{
signed char cByteRxed;

    if( xSerialGetChar( xPort, &cByteRxed, comRX_BLOCK_TIME ) )
    {
        switch( cByteRxed )
        {
        /* Enter */
        case '\r':
        case '\n':
            *p = '\0';
            vSerialPutString( xPort, "\r\n", 2 );

            /* prompt */
            vSerialPutString( xPort, shellPROMPT, strlen( shellPROMPT ) );

            return len;

        /* NULL */
        case '\0':
            return -1;

        /* ^C, break */
        case 0x3:
            return -1;

        /* ^U, erase line */
        case 0x15:
            return -1;

        /* ^W, erase word */
        case 0x17:
            return -1;

        /* ^H and DEL, backspace */
        case 0x08:
        case 0x7f:
            prvShellBackspace();
            return -1;

        default:
            xSerialPutChar( xPort, cByteRxed, comNO_BLOCK );
            *p++ = cByteRxed;
            *p = '\0';
            len++;

            return -1;
        }
    }

    return -1;
}

static portTASK_FUNCTION( vShellTask, pvParameters )
{
TickType_t xLastWakeTime;
const TickType_t xFrequency = 30;

    /* Initialise the xLastWakeTime variable with the current time. */
    xLastWakeTime = xTaskGetTickCount();

    /* prompt */
    vSerialPutString( xPort, "\r\n", 2 );
    vSerialPutString( xPort, shellPROMPT, strlen( shellPROMPT ) );

    console_buffer[0] = '\0';

    for( ;; )
    {
        if( prvShellReadline() > 0 )
        {
            prvShellRuncmd();

            console_buffer[0] = '\0';
            len = 0;
            p = console_buffer;
        }
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
}
