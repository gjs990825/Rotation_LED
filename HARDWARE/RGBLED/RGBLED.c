#include "RGBLED.h"
#include "my_lib.h"

uint32_t RGBLEDCurrentColor = 0;

// RGBLED初始化
// TIM2 PWM输出，引脚 PA0/1/2
void RGBLED_Init(uint32_t initColor)
{
    // 一个毫秒需要的周期
    uint32_t total = SystemCoreClock / 1000;

    uint16_t RGBLEDArr = 0xFF;
    uint16_t RGBLEDPsc = total / 0xFF;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // T3C1 PA6
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = RGBLEDArr - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = RGBLEDPsc - 1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    RGBLED_Set(initColor);

    TIM_Cmd(TIM2, ENABLE);
}

void RGBLED_SetRGB(uint8_t R, uint8_t G, uint8_t B)
{
    TIM_SetCompare1(TIM2, R);
    TIM_SetCompare2(TIM2, G);
    TIM_SetCompare3(TIM2, B);
}

void RGBLED_Set(uint32_t clolor)
{
    // 限制颜色范围
    clolor = constrain_int(clolor, 0, 0xFFFFFF);
    RGBLEDCurrentColor = clolor;

    RGBLED_SetRGB((clolor >> 16) & 0xFF,
                  (clolor >> 8) & 0xFF,
                  clolor & 0xFF);
}

float RGBLED_GetCurrent(void)
{
    return RGBLEDCurrentColor;
}
