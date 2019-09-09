#include "ledarray.h"
#include "display.h"
#include "debug.h"
#include "stdio.h"

#if defined(_LED_IN_DIFFERENT_PORT_)

typedef struct ledPin
{
    GPIO_TypeDef *GPIOx;
    uint16_t pin;
} ledPin_t;

ledPin_t ledArray[16] = {
    {GPIOB, GPIO_Pin_0},
    {GPIOB, GPIO_Pin_1},
    {GPIOA, GPIO_Pin_7},
    {GPIOA, GPIO_Pin_6},
    {GPIOA, GPIO_Pin_5},
    {GPIOA, GPIO_Pin_4},
    {GPIOA, GPIO_Pin_0},
    {GPIOA, GPIO_Pin_11},
    {GPIOA, GPIO_Pin_12},
    {GPIOA, GPIO_Pin_15},
    {GPIOB, GPIO_Pin_3},
    {GPIOB, GPIO_Pin_8},
    {GPIOB, GPIO_Pin_7},
    {GPIOB, GPIO_Pin_6},
    {GPIOB, GPIO_Pin_5},
    {GPIOB, GPIO_Pin_4},
};

#endif // _LED_IN_DIFFERENT_PORT_

// LED: See Comments Above
// TIMER: TIM2,
// PWMPin: PA0\1
// EXTI: PC13
void LEDArray_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // LED输出控制
#if defined(_LED_IN_DIFFERENT_PORT_)

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    for (uint8_t i = 0; i < 16; i++)
    {
        GPIO_InitStructure.GPIO_Pin = ledArray[i].pin;
        GPIO_Init(ledArray[i].GPIOx, &GPIO_InitStructure);
    }

#else

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_All);

#endif // _LED_IN_DIFFERENT_PORT_

    // PWM 控制 A1 A2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 10K, 0xFF
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 0xFF - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_Cmd(TIM2, ENABLE);
    LEDArray_Color(0xFF);

    // 红外输入

#if defined(_LED_IN_DIFFERENT_PORT_)

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);

    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

#else

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);

    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

#endif
}

// 输出一列数据
void LEDArray_OutHex(uint16_t ledStatus)
{
    ledStatus = ~ledStatus;

#if defined(_LED_IN_DIFFERENT_PORT_)

    for (uint16_t i = 0; i < 16; i++)
    {
        if (ledStatus & 0x1)
        {
            GPIO_SetBits(ledArray[i].GPIOx, ledArray[i].pin);
        }
        else
        {
            GPIO_ResetBits(ledArray[i].GPIOx, ledArray[i].pin);
        }
        ledStatus >>= 1;
    }

#else

    GPIO_Write(GPIOB, ledStatus);

#endif // _LED_IN_DIFFERENT_PORT_
}

void LEDArray_OutArray(bool array[16])
{
#if defined(_LED_IN_DIFFERENT_PORT_)

    for (uint16_t i = 0; i < 16; i++)
    {
        if (array[i])
        {
            GPIO_ResetBits(ledArray[i].GPIOx, ledArray[i].pin);
        }
        else
        {
            GPIO_SetBits(ledArray[i].GPIOx, ledArray[i].pin);
        }
    }

#else

    GPIO_Write(GPIOB, ~Display_ArrayToHex(array));

#endif // _LED_IN_DIFFERENT_PORT_
}

// 全灭
void LEDArray_ALLOFF(void)
{
    LEDArray_OutHex(0);
}

// 全亮
void LEDArray_ALLON(void)
{
    LEDArray_OutHex(0xFFFF);
}

// 数值越高越红，0-0xFF
void LEDArray_Color(uint8_t clolor)
{
    TIM_SetCompare2(TIM2, 0xFF - clolor);
    TIM_SetCompare3(TIM2, clolor);
}

void EXTI15_10_IRQHandler(void)
{
    Display_InterruptHandle();

#if defined(_LED_IN_DIFFERENT_PORT_)

    EXTI_ClearITPendingBit(EXTI_Line12);

#else

    EXTI_ClearITPendingBit(EXTI_Line13);

#endif
}
