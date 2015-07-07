/* Kernel includes. */
#include "FreeRTOS.h"

/* Hardware and starter kit includes. */
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"

/* Application includes */
#include "pwm.h"

void vServoRotate( TIM_TypeDef* TIMx, uint8_t uChannel, uint8_t uDegree )
{
    if( uDegree == 0)
        vPwmSetDutyCycle( TIMx, uChannel, ( ( PWM_PERIOD + 1 ) / 20 ) );
    else if( uDegree == 90 )
        vPwmSetDutyCycle( TIMx, uChannel, ( ( 1.5 ) * ( PWM_PERIOD + 1 ) / 20 ) );
    else if( uDegree == 180 )
        vPwmSetDutyCycle( TIMx, uChannel, ( ( 2 ) * ( PWM_PERIOD + 1 ) / 20 ) );

}
