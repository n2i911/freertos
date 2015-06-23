/* Kernel includes. */
#include "FreeRTOS.h"

/* Hardware and starter kit includes. */
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

void vUserBtnInit( void )
{

GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

    /*
     * User Button is connected to PA0,
     * when push down will become 1 from 0
     */

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );

    GPIO_StructInit( &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;

    GPIO_Init( GPIOA, &GPIO_InitStructure );

    /* Set EXTI0 trigger from PA0 */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE );
    SYSCFG_EXTILineConfig( EXTI_PortSourceGPIOA,EXTI_PinSource0 );

    /* Enable EXTI0 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );
}

void EXTI0_IRQHandler( void )
{
    if( EXTI_GetITStatus( EXTI_Line0 ) != RESET )
    {
        /* Do whatever you want here when push USER button */
        EXTI_ClearITPendingBit( EXTI_Line0 );
    }
}
