#ifndef AT24C04_H
#define AT24C04_H

void vAt24c04Write( I2C_TypeDef* I2Cx, uint8_t uAddress, uint8_t uData );
uint8_t uAt24c04Read( I2C_TypeDef* I2Cx, uint8_t uAddress );

#endif
