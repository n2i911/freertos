/* Kernel includes. */
#include "FreeRTOS.h"
#include "queue.h"

/* Hardware and starter kit includes. */
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

/* Demo application includes. */
#include "serial.h"

/* The queue used to hold received characters. */
static QueueHandle_t xRxedChars;
static QueueHandle_t xCharsForTx;

xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
xComPortHandle xReturn;
USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef USART_ClockInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

    /* Create the queues used to hold Rx/Tx characters. */
    xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
    xCharsForTx = xQueueCreate( uxQueueLength + 1, ( unsigned portBASE_TYPE ) sizeof( signed char ) );

    /* If the queue/semaphore was created correctly then setup the serial port
     * hardware. */
    if( ( xRxedChars != 0) && ( xCharsForTx != 0) )
    {
        /* Enable peripheral and GPIOA clock for USART2 */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_AHB1Periph_GPIOA, ENABLE);

        /* GPIOA Configuration:  USART2 TX on PA2 */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

        GPIO_Init( GPIOA, &GPIO_InitStructure );

        /*
         * Connect USART2 pins to AF2
         * TX = PA2
         * RX = PA3
         */
        GPIO_PinAFConfig( GPIOA, GPIO_PinSource2, GPIO_AF_USART2 );
        GPIO_PinAFConfig( GPIOA, GPIO_PinSource3, GPIO_AF_USART2 );

        USART_InitStructure.USART_BaudRate = ulWantedBaud;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

        USART_Init( USART2, &USART_InitStructure );

        USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
        USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
        USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
        USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

        USART_ClockInit( USART2, &USART_ClockInitStructure );

        USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );

        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

        NVIC_Init( &NVIC_InitStructure );

        USART_Cmd( USART2, ENABLE );
    }
    else
    {
        xReturn = ( xComPortHandle ) 0;
    }

    /* This demo file only supports a single port but we have to return
       something to comply with the standard demo header file. */
    return xReturn;
}

void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength )
{
signed char *pxNext;

    /* A couple of parameters that this port does not use. */
    ( void ) usStringLength;
    ( void ) pxPort;

    /* NOTE: This implementation does not handle the queue being full as no
       block time is used! */

    /* The port handle is not required as this driver only supports UART2. */
    ( void ) pxPort;

    /* Send each character in the string, one at a time. */
    pxNext = ( signed char * ) pcString;
    while( *pxNext )
    {
        xSerialPutChar( pxPort, *pxNext, 0 );
        pxNext++;
    }
}

signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
    /* The port handle is not required as this driver only supports one port. */
    ( void ) pxPort;

    /* Get the next character from the buffer.  Return false if no characters
       are available, or arrive before xBlockTime expires. */
    if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) )
    {
        return pdTRUE;
    }
    else
    {
        return pdFALSE;
    }
}

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
signed portBASE_TYPE xReturn;

    if( xQueueSend( xCharsForTx, ( void * ) &cOutChar, xBlockTime ) == pdPASS )
    {
        xReturn = pdPASS;
        USART_ITConfig( USART2, USART_IT_TXE, ENABLE );
    }
    else
    {
        xReturn = pdFAIL;
    }

    return xReturn;
}

void vSerialClose( xComPortHandle xPort )
{
}

void USART2_IRQHandler(void)
{
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
char cChar;

    /* Check if the USART2 TXE was set */
    if( USART_GetITStatus( USART2, USART_IT_TXE ) == SET )
    {
        /* The interrupt was caused by the THR becoming empty.  Are there any
           more characters to transmit? */
        if( xQueueReceiveFromISR( xCharsForTx, &cChar, &xHigherPriorityTaskWoken ) == pdTRUE )
        {
            /* A character was retrieved from the queue so can be sent to the
               THR now. */
            USART_SendData( USART2, cChar );
        }
        else
        {
            USART_ITConfig( USART2, USART_IT_TXE, DISABLE );
        }
    }

    /* Check if the USART2 RXNE flag was set */
    if( USART_GetITStatus( USART2, USART_IT_RXNE ) == SET )
    {
        cChar = USART_ReceiveData( USART2 );
        xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );
    }

    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
