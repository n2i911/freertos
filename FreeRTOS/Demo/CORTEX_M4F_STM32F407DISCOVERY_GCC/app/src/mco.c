/* Kernel includes. */
#include "FreeRTOS.h"

/* Hardware and starter kit includes. */
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

void vMcoInit( void )
{
GPIO_InitTypeDef GPIO_InitStructure;

    /*
     * PA8 for MCO1
     * Enable the GPIOA Clock
     */
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;

    GPIO_Init( GPIOA, &GPIO_InitStructure );

    GPIO_PinAFConfig( GPIOA, GPIO_PinSource8, GPIO_AF_MCO );

    /*
     * MCO1 is sourced from PLLCLK and divided by 4
     */
    RCC_MCO1Config( RCC_MCO1Source_PLLCLK, RCC_MCO1Div_4 );

    /*
     * PC9 for MCO2
     * Enable GPIOC clock
     */
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_PinAFConfig( GPIOC, GPIO_PinSource9, GPIO_AF_MCO );

    /* MCO2 is sourced from SYSCLK and divided by 4*/
    RCC_MCO2Config( RCC_MCO2Source_SYSCLK, RCC_MCO2Div_4 );
}
