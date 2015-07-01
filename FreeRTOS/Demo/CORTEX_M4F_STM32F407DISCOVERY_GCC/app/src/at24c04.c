/* Kernel includes. */
#include "FreeRTOS.h"

/* Hardware and starter kit includes. */
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

/* Application includes */
#include "i2c.h"

#define AT24C04_SLAVE_ADDRESS   0xa0

void vAt24c04Write( I2C_TypeDef* I2Cx, uint8_t uAddress, uint8_t uData )
{
    /* Write @uData at address @uAddress */
    vI2cStart( I2Cx, AT24C04_SLAVE_ADDRESS, I2C_Direction_Transmitter );
    vI2cWrite( I2Cx, uAddress );
    vI2cWrite( I2Cx, uData );
    vI2cStop( I2Cx );
}

uint8_t uAt24c04Read( I2C_TypeDef* I2Cx, uint8_t uAddress )
{
    /* Read from address @uAddress */
    vI2cStart( I2Cx, AT24C04_SLAVE_ADDRESS, I2C_Direction_Transmitter );
    vI2cWrite( I2Cx, uAddress );
    vI2cStop( I2Cx );

    /* Read data */
    vI2cStart( I2Cx, AT24C04_SLAVE_ADDRESS, I2C_Direction_Receiver );

    return uI2cReadNack( I2C2 );
}
