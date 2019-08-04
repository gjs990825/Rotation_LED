#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "sys.h"
#include "usart.h"

#define MAX_SPEED 700
#define MIN_SPEED 200

#define Set_LMotor(val) TIM_SetCompare4(TIM1, val)
#define Set_RMotor(val) TIM_SetCompare1(TIM1, val)

void TIM1_PWMInit(uint16_t arr, uint16_t psc);
void Motor_SpeedControl(int16_t speed);
void Motor_ShutDown(void);

#endif
