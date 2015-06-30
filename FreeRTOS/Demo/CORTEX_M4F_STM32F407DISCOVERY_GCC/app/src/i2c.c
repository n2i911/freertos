/* Kernel includes. */
#include "FreeRTOS.h"

/* Hardware and starter kit includes. */
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

void vI2cInit( void )
{
GPIO_InitTypeDef GPIO_InitStructure;
I2C_InitTypeDef I2C_InitStructure;
#if 0
NVIC_InitTypeDef NVIC_InitStructure;
#endif

    /*
     * Enable GPIOB clock
     * GPIO AF configuration
     * I2C2_SCL on PB10, I2C2_SDA on PB11
     */
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_Init( GPIOB, &GPIO_InitStructure );

    GPIO_PinAFConfig( GPIOB, GPIO_PinSource10, GPIO_AF_I2C2 );
    GPIO_PinAFConfig( GPIOB, GPIO_PinSource11, GPIO_AF_I2C2 );

    /* Setup I2C2 */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C2, ENABLE );

    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0xf0;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_ClockSpeed = 30000;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init( I2C2, &I2C_InitStructure );

    I2C_Cmd( I2C2, ENABLE );

#if 0
    /* I2C2 interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );
#endif
}

void vI2cStart( I2C_TypeDef* I2Cx, uint8_t uAddress, uint8_t uDirection )
{
    /* Wait until I2C is not busy */
    while( I2C_GetFlagStatus( I2Cx, I2C_FLAG_BUSY ) );

    /* Send START condition */
    I2C_GenerateSTART( I2Cx, ENABLE );

    /* Wait for EV5 (I2C_EVENT_MASTER_MODE_SELECT) */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_MODE_SELECT ) );

    /*
     * Send slave Address for write or read
     * The format of address parameter is |A|D|D|R|E|S|S|0|
     */
    I2C_Send7bitAddress( I2Cx, uAddress, uDirection );

    /* Wait for EV6 */
    if( uDirection == I2C_Direction_Transmitter )
    {
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );
    }
    else if( uDirection == I2C_Direction_Receiver )
    {
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );
    }
}

void vI2cStop( I2C_TypeDef* I2Cx )
{
    /* Send STOP Condition */
    I2C_GenerateSTOP( I2Cx, ENABLE );
}

void vI2cWrite( I2C_TypeDef* I2Cx, uint8_t uData )
{
    I2C_SendData( I2Cx, uData );

    /* Wait for EV8_2 (I2C_EVENT_MASTER_BYTE_TRANSMITTED) */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
}

uint8_t uI2cReadAck( I2C_TypeDef* I2Cx )
{
uint8_t uData;

    /* Enable acknowledge of recieved data */
    I2C_AcknowledgeConfig( I2Cx, ENABLE );

    /* Wait until one byte has been received */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED ) );

    /* Read data from I2C data register */
    uData = I2C_ReceiveData( I2Cx );

    return uData;
}

uint8_t uI2cReadNack( I2C_TypeDef* I2Cx )
{
uint8_t uData;

    /*
     * Disable acknowledge of received data
     * Nack also generates stop condition after last byte received
     */
    I2C_AcknowledgeConfig( I2Cx, DISABLE );
    I2C_GenerateSTOP( I2Cx, ENABLE );

    /* Wait until one byte has been received */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED ) );

    /* Read data from I2C data register */
    uData = I2C_ReceiveData( I2Cx );
    return uData;
}

void I2C2_EV_IRQHandler( void )
{
    /* TODO */
}
