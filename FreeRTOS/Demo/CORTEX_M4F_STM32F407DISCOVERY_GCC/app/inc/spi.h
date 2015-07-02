#ifndef SPI_H
#define SPI_H

void vSpiInit( void );

uint8_t uSpiWrite( SPI_TypeDef* SPIx, uint8_t uData );
uint8_t uSpiRead( SPI_TypeDef* SPIx );

#endif
