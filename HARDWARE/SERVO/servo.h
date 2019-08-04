#if !defined(_SERVO_H_)
#define _SERVO_H_

#include "sys.h"

void Servo_Init(float initAngle, float upperLimit);
void Servo_To(float target);
float Servo_GetCurrent(void);


#endif // _SERVO_H_
