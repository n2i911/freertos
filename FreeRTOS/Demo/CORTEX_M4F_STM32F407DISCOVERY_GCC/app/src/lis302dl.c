/* Kernel includes. */
#include "FreeRTOS.h"

/* Hardware and starter kit includes. */
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

/* Application includes */
#include "spi.h"

/* Register mapping */
#define LIS302DL_WHO_AM_I           0x0f
#define LIS302DL_CTRL_REG1          0x20
#define LIS302DL_CTRL_REG2          0x21
#define LIS302DL_CTRL_REG3          0x22
#define LIS302DL_HP_FILTER_RESET    0x23
#define LIS302DL_STATUS_REG         0x27
#define LIS302DL_OUTX               0x29
#define LIS302DL_OUTY               0x2b
#define LIS302DL_OUTZ               0x2d
#define LIS302DL_FF_WU_CFG_1        0x30
#define LIS302DL_FF_WU_SRC_1        0x31
#define LIS302DL_FF_WU_THS_1        0x32
#define LIS302DL_FF_WU_DURATION_1   0x33
#define LIS302DL_FF_WU_CFG_2        0x34
#define LIS302DL_FF_WU_SRC_2        0x35
#define LIS302DL_FF_WU_THS_2        0x36
#define LIS302DL_FF_WU_DURATION_2   0x37
#define LIS302DL_CLICK_CFG          0x38
#define LIS302DL_CLICK_SRC          0x39
#define LIS302DL_CLICK_THSY_X       0x3b
#define LIS302DL_CLICK_THSZ         0x3c
#define LIS302DL_CLICK_TIMELIMIt    0x3d
#define LIS302DL_CLICK_LATENCY      0x3e
#define LIS302DL_CLICK_WINDOW       0x3f

/* WHO_AM_I */
#define LIS302DL_ID                 0x3b
#define LIS3DSH_ID                  0x3f

void vLis302dlInit( void )
{
GPIO_InitTypeDef GPIO_InitStructure;

    /* PE3 for SPI CS */
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    GPIO_Init( GPIOE, &GPIO_InitStructure );

    GPIO_SetBits( GPIOE, GPIO_Pin_3 );
}

void vLis302dlWrite( SPI_TypeDef* SPIx, uint8_t uAddress, uint8_t uData )
{
    /* CS pull-low */
    GPIO_WriteBit( GPIOE, GPIO_Pin_3, Bit_RESET );

    uSpiWrite( SPI1, uAddress );
    uSpiWrite( SPI1, uData );

    /* CS pull-high */
    GPIO_WriteBit( GPIOE, GPIO_Pin_3, Bit_SET );
}

uint8_t uLis302dlRead( SPI_TypeDef* SPIx, uint8_t uAddress )
{
uint8_t uData;

    /* CS pull-low */
    GPIO_WriteBit( GPIOE, GPIO_Pin_3, Bit_RESET );

    uAddress = 0x80 | uAddress;

    uSpiWrite( SPI1, uAddress );

    /* Send dumy byte, for clock generation only */
    uData = uSpiWrite( SPI1, 0x00 );

    /* CS pull-high */
    GPIO_WriteBit( GPIOE, GPIO_Pin_3, Bit_SET );

    return uData;
}
