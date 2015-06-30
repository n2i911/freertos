#ifndef I2C_H
#define I2C_H

void vI2cInit( void );

void vI2cStart( I2C_TypeDef* I2Cx, uint8_t uAddress, uint8_t uDirection );
void vI2cStop( I2C_TypeDef* I2Cx );
void vI2cWrite( I2C_TypeDef* I2Cx, uint8_t uData );
uint8_t uI2cReadAck( I2C_TypeDef* I2Cx );
uint8_t uI2cReadNack( I2C_TypeDef* I2Cx );

#endif
