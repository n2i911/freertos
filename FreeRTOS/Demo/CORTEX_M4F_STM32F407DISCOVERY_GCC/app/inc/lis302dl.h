#ifndef LIS302DL_H
#define LIS302DL_H

void vLis302dlInit( void );
void vLis302dlWrite( SPI_TypeDef* SPIx, uint8_t uAddress, uint8_t uData );
uint8_t uLis302dlRead( SPI_TypeDef* SPIx, uint8_t uAddress );

#endif
