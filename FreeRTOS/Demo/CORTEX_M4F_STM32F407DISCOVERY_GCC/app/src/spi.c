/* Kernel includes. */
#include "FreeRTOS.h"

/* Hardware and starter kit includes. */
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

/* Application includes */
#include "spi.h"

void vSpiInit( void )
{
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef SPI_InitStructure;

    /* GPIOA Configuration:
     * PA5 for SPI1_SCK
     * PA6 for SPI1_MISO
     * PA7 for SPI1_MOSI
     */
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig( GPIOA, GPIO_PinSource5, GPIO_AF_SPI1 );
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource6, GPIO_AF_SPI1 );
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource7, GPIO_AF_SPI1 );

    /*
     * Setup SPI1
     * According to the real hardware design and follow the lis302dl spec
     */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE );

    SPI_I2S_DeInit(SPI1);

    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init( SPI1, &SPI_InitStructure );

    SPI_Cmd( SPI1, ENABLE );
}

uint8_t uSpiWrite( SPI_TypeDef* SPIx, uint8_t uData )
{
    /* Wait until transmission complete */
    while( !SPI_I2S_GetFlagStatus( SPIx, SPI_I2S_FLAG_TXE ) );
    SPI_I2S_SendData( SPIx, uData );

    return uSpiRead( SPIx );
}

uint8_t uSpiRead( SPI_TypeDef* SPIx )
{
    /* Wait until transmission complete */
    while( !SPI_I2S_GetFlagStatus( SPIx, SPI_I2S_FLAG_RXNE ) );

    return SPI_I2S_ReceiveData( SPIx );
}
