#include "servo.h"
#include "my_lib.h"

// 目标角度值
float servoTargetAngle = 0.0;

// PWM上下限
uint16_t servoPWMHigh = 1000;
uint16_t servoPWMLow = 100;

// 角度值上下限
float angleUpperLimit = 0;
float angleLowerLimit = 0;

// 舵机初始化
// TIM3 PWM输出，引脚 PA6
void Servo_Init(float initAngle, float upperLimit)
{
    angleUpperLimit = upperLimit;
    angleLowerLimit = 0; // 角度下限默认0

    // 一个毫秒需要的周期
    uint32_t total = SystemCoreClock / 50;

    uint16_t servoArr = total / 1000;
    uint16_t servoPsc = 1000;

    servoPWMLow = servoArr * (0.5 / 20.0);
    servoPWMHigh = servoArr * (2.5 / 20.0);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // T3C1 PA6
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = servoArr - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = servoPsc - 1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    Servo_To(initAngle);

    TIM_Cmd(TIM3, ENABLE);
}

void Servo_SetPWM(uint16_t pwm)
{
    TIM_SetCompare1(TIM3, pwm);
}

void Servo_To(float target)
{
    // 限制角度范围
    target = constrain_float(target, angleLowerLimit, angleUpperLimit);
    servoTargetAngle = target;

    // 映射PWM值，四舍五入输出
    Servo_SetPWM((uint16_t)(map(target, angleLowerLimit, angleUpperLimit, servoPWMLow, servoPWMHigh) + 0.5));
}

float Servo_GetCurrent(void)
{
    return servoTargetAngle;
}
