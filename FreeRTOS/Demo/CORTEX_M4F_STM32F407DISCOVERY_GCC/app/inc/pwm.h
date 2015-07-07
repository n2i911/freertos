#ifndef PWM_H
#define PWM_H

#define PWM_PERIOD      ( 20000 - 1 )
#define PWM_PRESCALER   ( 84 - 1 )

void vPwmInit( uint16_t uPeriod, uint16_t uPrescalerValue );
void vPwmSetDutyCycle( TIM_TypeDef* TIMx, uint8_t uChannel, uint16_t uDuty );

#endif
